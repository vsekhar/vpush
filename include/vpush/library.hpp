#ifndef __VPUSH_LIBRARY_HPP__
#define __VPUSH_LIBRARY_HPP__

#include <vpush/stackops.hpp>
#include <vpush/detail/typechecker.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace library {

using vpush::functions;
using vpush::type;
using detail::Code;
using detail::Exec;
using detail::item;
using detail::get_item;
using detail::get_nth_item;
using detail::put_item;

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

