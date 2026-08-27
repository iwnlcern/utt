#include "search/search.hpp"

namespace uttt {

// The generic implementation remains header-visible for test-only models;
// this explicit production instantiation gives the planned translation unit
// ownership of UtttModel search code.
template struct Search<UtttModel>;

} // namespace uttt
