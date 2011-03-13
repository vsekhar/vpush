#ifndef __VPUSH_DETAIL_CODE_HPP__
#define __VPUSH_DETAIL_CODE_HPP__

#include <vpush/protein_fwd.hpp>

namespace vpush {
namespace detail {

typedef double(*op_func_t)(Protein&);

struct Code {
	Code(op_func_t f) : fptr(f) {}
	operator op_func_t() const { return fptr; }
	op_func_t fptr;
};

struct Exec : public Code {};

} // namespace detail
} // namespace vpush

#endif

