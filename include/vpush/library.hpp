#ifndef __VPUSH_LIBRARY_HPP__
#define __VPUSH_LIBRARY_HPP__

namespace vpush {
namespace library {

// main initialization
void initialize();

///////////////////////////////
// sub-library initializations
///////////////////////////////

namespace arithmetic {
void initialize();
}

namespace basic {
void initialize();
}

} // namespace library
} // namespace vpush

#endif

