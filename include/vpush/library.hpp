#ifndef __VPUSH_LIBRARY_HPP__
#define __VPUSH_LIBRARY_HPP__

#include <vpush/stackops.hpp>
#include <vpush/detail/typechecker.hpp>

namespace vpush {
namespace library {

// main initialization
void initialize();

///////////////////////////////
// sub-library initializations
///////////////////////////////

namespace basic {
void initialize();
}

namespace code {
void initialize();
}

namespace arithmetic {
void initialize();
}

namespace combinators {
void initialize();
}

} // namespace library
} // namespace vpush

#endif

