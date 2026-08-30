module checker;

import checker::report;
import checker::verify;
import libc;
import std::encoding::json;
import std::io;
import std::io::file;
import std::os::posix;
import std::time;
import std::time::clock;

fn int usage()
{
    io::eprintn("Usage: uttt-checker cert <file>");
    io::eprintn("       uttt-checker verdict <file> [--mode deliverable|subgame]");
    return 2;
}

fn int io_error(String path)
{
    io::eprintfn("uttt-checker: cannot read %s", path);
    return 2;
}

fn long cpu_now_ns()
{
    libc::TimeSpec stamp;
    if (posix::clock_gettime(posix::CLOCK_PROCESS_CPUTIME_ID, &stamp) != 0) return 0;
    return (long)stamp.s * 1_000_000_000 + (long)stamp.ns;
}

fn void print_report(report::Report result)
{
    io::printn(json::marshal(tmem, result));
}

fn int main(String[] args)
{
    if (args.len < 2) return usage();

    if (args[1] == "cert")
    {
        if (args.len != 3) return usage();
        char[]? loaded = file::load_temp(args[2]);
        if (catch loaded) return io_error(args[2]);
        char[] blob = loaded;
        Clock wall_start = clock::now();
        long cpu_start = cpu_now_ns();
        verify::VerifyResult result = verify::verify_certificate(blob);
        long wall_ms = wall_start.to_now().to_ms();
        long cpu_ms = (cpu_now_ns() - cpu_start) / 1_000_000;
        print_report(report::from_certificate(result, wall_ms, cpu_ms));
        return result.ok ? 0 : 1;
    }

    if (args[1] == "verdict")
    {
        if (args.len != 3 && args.len != 5) return usage();
        verify::Mode mode = verify::Mode.DELIVERABLE;
        if (args.len == 5)
        {
            if (args[3] != "--mode") return usage();
            if (args[4] == "deliverable")
            {
                mode = verify::Mode.DELIVERABLE;
            }
            else if (args[4] == "subgame")
            {
                mode = verify::Mode.SUBGAME;
            }
            else
            {
                return usage();
            }
        }
        if (!file::is_file(args[2])) return io_error(args[2]);
        Clock wall_start = clock::now();
        long cpu_start = cpu_now_ns();
        verify::VerdictResult result = verify::verify_verdict(args[2], mode);
        long wall_ms = wall_start.to_now().to_ms();
        long cpu_ms = (cpu_now_ns() - cpu_start) / 1_000_000;
        print_report(report::from_verdict(result, wall_ms, cpu_ms));
        return result.ok ? 0 : 1;
    }

    return usage();
}
