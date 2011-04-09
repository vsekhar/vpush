#ifndef __VPUSH_DETAIL_CODE_FWD_HPP__
#define __VPUSH_DETAIL_CODE_FWD_HPP__

#include <vpush/protein_fwd.hpp>

namespace vpush {
namespace detail {

typedef double(*op_func_t)(Protein&);

struct Code;
struct Exec;

}
}

#endif

