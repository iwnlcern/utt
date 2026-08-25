#include "core/naive_position.hpp"
#include "core/position.hpp"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using json = nlohmann::json;

namespace {

constexpr uint64_t kPliesPerRun = 2'000'000;
constexpr std::array<uint64_t, 4> kSeeds = {
    0x123456789ABCDEF0ULL,
    0x0F1E2D3C4B5A6978ULL,
    0xD1CEB00C5EED1234ULL,
    0xA5A5A5A5DEADBEEFULL,
};

uint64_t next_random(uint64_t& state) {
  state ^= state << 13;
  state ^= state >> 7;
  state ^= state << 17;
  return state;
}

std::string capture_command(const std::string& command) {
  std::array<char, 256> buffer{};
  std::string output;
  FILE* pipe = popen(command.c_str(), "r");
  if (pipe == nullptr) throw std::runtime_error("cannot execute identity command");
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
    output += buffer.data();
  }
  if (pclose(pipe) != 0) throw std::runtime_error("identity command failed: " + command);
  while (!output.empty() && (output.back() == '\n' || output.back() == '\r')) output.pop_back();
  return output;
}

std::string executable_sha256(const char* argv0) {
  const std::string path = std::filesystem::canonical(argv0).string();
  if (path.find('\'') != std::string::npos) throw std::runtime_error("unsupported quote in executable path");
  const std::string line = capture_command("/usr/bin/shasum -a 256 '" + path + "'");
  if (line.size() < 64) throw std::runtime_error("shasum produced no digest");
  return line.substr(0, 64);
}

std::string fresh_uuid() {
  std::random_device random;
  std::array<uint32_t, 4> words{};
  for (auto& word : words) word = random();
  std::ostringstream out;
  out << std::hex << std::setfill('0') << std::setw(8) << words[0] << '-'
      << std::setw(4) << (words[1] >> 16) << '-' << std::setw(4) << (words[1] & 0xFFFFu)
      << '-' << std::setw(4) << (words[2] >> 16) << '-' << std::setw(4)
      << (words[2] & 0xFFFFu) << std::setw(8) << words[3];
  return out.str();
}

json identity(const char* argv0, const std::string& session_uuid) {
  return {{"executable_sha256", executable_sha256(argv0)},
          {"compiler", __VERSION__},
          {"compile_flags", UTTT_COMPILE_FLAGS},
          {"cpu", capture_command("/usr/sbin/sysctl -n machdep.cpu.brand_string")},
          {"seeds", kSeeds},
          {"session_uuid", session_uuid}};
}

uint64_t reference_plies(uint64_t target, uint64_t seed) {
  uint64_t rng = seed;
  uint64_t sink = 0;
  uint64_t plies = 0;
  auto position = uttt::naive::RefPosition::initial();
  while (plies < target) {
    uttt::MoveList moves;
    position.legal_moves(moves);
    if (moves.n == 0) {
      position = uttt::naive::RefPosition::initial();
      continue;
    }
    const uttt::Move move = moves.m[next_random(rng) % moves.n];
    const uttt::Seat mover = (next_random(rng) & 1u) != 0 ? uttt::Seat::X : uttt::Seat::O;
    position = position.applied_unchecked(move, mover);
    sink ^= uint64_t(move.board) << 8 | move.cell;
    ++plies;
  }
  asm volatile("" : : "r"(sink) : "memory");
  return plies;
}

uint64_t candidate_plies(uint64_t target, uint64_t seed) {
  uint64_t rng = seed;
  uint64_t sink = 0;
  uint64_t plies = 0;
  auto position = uttt::Position::initial();
  while (plies < target) {
    uttt::MoveList moves;
    position.legal_moves(moves);
    if (moves.n == 0) {
      position = uttt::Position::initial();
      continue;
    }
    const uttt::Move move = moves.m[next_random(rng) % moves.n];
    const uttt::Seat mover = (next_random(rng) & 1u) != 0 ? uttt::Seat::X : uttt::Seat::O;
    position = position.applied(move, mover).value();
    sink ^= position.key;
    ++plies;
  }
  asm volatile("" : : "r"(sink) : "memory");
  return plies;
}

using Driver = uint64_t (*)(uint64_t, uint64_t);

std::vector<double> measure(Driver driver, int warmup_runs, int measured_runs) {
  for (int run = 0; run < warmup_runs; ++run) {
    driver(kPliesPerRun, kSeeds[run % kSeeds.size()]);
  }
  std::vector<double> results;
  for (int run = 0; run < measured_runs; ++run) {
    const auto start = std::chrono::steady_clock::now();
    const uint64_t plies = driver(kPliesPerRun, kSeeds[run % kSeeds.size()]);
    const auto stop = std::chrono::steady_clock::now();
    const double nanos = std::chrono::duration<double, std::nano>(stop - start).count();
    results.push_back(nanos / static_cast<double>(plies));
  }
  return results;
}

double median(std::vector<double> values) {
  std::sort(values.begin(), values.end());
  const std::size_t mid = values.size() / 2;
  return values.size() % 2 == 0 ? (values[mid - 1] + values[mid]) / 2.0 : values[mid];
}

void write_json(const std::filesystem::path& path, const json& value) {
  std::ofstream output(path);
  if (!output) throw std::runtime_error("cannot open output: " + path.string());
  output << std::setw(2) << value << '\n';
}

struct Options {
  bool reference = false;
  bool candidate = false;
  int warmup_runs = 2;
  int measured_runs = 10;
  std::filesystem::path out;
  std::filesystem::path baseline;
  std::filesystem::path verdict;
};

Options parse_options(int argc, char** argv) {
  Options options;
  for (int i = 1; i < argc; ++i) {
    const std::string_view arg = argv[i];
    auto next = [&]() -> std::string {
      if (++i >= argc) throw std::runtime_error("missing option value");
      return argv[i];
    };
    if (arg == "--reference") options.reference = true;
    else if (arg == "--candidate") options.candidate = true;
    else if (arg == "--warmup-runs") options.warmup_runs = std::stoi(next());
    else if (arg == "--measured-runs") options.measured_runs = std::stoi(next());
    else if (arg == "--out") options.out = next();
    else if (arg == "--baseline") options.baseline = next();
    else if (arg == "--out-verdict") options.verdict = next();
    else throw std::runtime_error("unknown option: " + std::string(arg));
  }
  if (options.reference == options.candidate) throw std::runtime_error("choose one benchmark mode");
  if (options.warmup_runs < 0 || options.measured_runs < 10) {
    throw std::runtime_error("requires nonnegative warmup and at least 10 measured runs");
  }
  if (options.reference && options.out.empty()) throw std::runtime_error("reference needs --out");
  if (options.candidate && (options.baseline.empty() || options.verdict.empty())) {
    throw std::runtime_error("candidate needs --baseline and --out-verdict");
  }
  return options;
}

}  // namespace

int main(int argc, char** argv) {
  try {
    const Options options = parse_options(argc, argv);
    if (options.reference) {
      const std::string session_uuid = fresh_uuid();
      const json own_identity = identity(argv[0], session_uuid);
      const auto runs = measure(reference_plies, options.warmup_runs, options.measured_runs);
      const double value = median(runs);
      write_json(options.out,
                 {{"mode", "reference"},
                  {"plies_per_run", kPliesPerRun},
                  {"warmup_runs", options.warmup_runs},
                  {"measured_runs", options.measured_runs},
                  {"per_run_ns_per_ply", runs},
                  {"median_ref_ns_per_ply", value},
                  {"identity", own_identity}});
      std::cout << "median_ref_ns_per_ply=" << value << '\n';
      std::cout << "session_uuid=" << session_uuid << '\n';
      return 0;
    }

    std::ifstream baseline_input(options.baseline);
    if (!baseline_input) throw std::runtime_error("cannot open baseline read-only");
    const json baseline = json::parse(baseline_input);
    const std::string session_uuid = baseline.at("identity").at("session_uuid").get<std::string>();
    const json own_identity = identity(argv[0], session_uuid);
    if (own_identity != baseline.at("identity")) {
      throw std::runtime_error("identity mismatch; refusing candidate measurement");
    }
    const auto runs = measure(candidate_plies, options.warmup_runs, options.measured_runs);
    const double candidate = median(runs);
    const double reference = baseline.at("median_ref_ns_per_ply").get<double>();
    const bool ratio_pass = candidate <= reference;
    const bool absolute_pass = candidate <= 100.0;
    const bool pass = ratio_pass && absolute_pass;
    const json verdict = {{"verdict", pass ? "PASS" : "FAIL"},
                          {"median_ref_ns_per_ply", reference},
                          {"median_cand_ns_per_ply", candidate},
                          {"ratio", candidate / reference},
                          {"ratio_predicate", ratio_pass},
                          {"absolute_100ns_predicate", absolute_pass},
                          {"per_run_ns_per_ply", runs},
                          {"baseline_session_uuid", session_uuid},
                          {"identity", own_identity}};
    write_json(options.verdict, verdict);
    std::cout << "median_ref_ns_per_ply=" << reference << '\n';
    std::cout << "median_cand_ns_per_ply=" << candidate << '\n';
    std::cout << "ratio_pass=" << std::boolalpha << ratio_pass << '\n';
    std::cout << "absolute_100ns_pass=" << std::boolalpha << absolute_pass << '\n';
    std::cout << "verdict=" << (pass ? "PASS" : "FAIL") << '\n';
    std::cout << "session_uuid=" << session_uuid << '\n';
    return pass ? 0 : 1;
  } catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 2;
  }
}
