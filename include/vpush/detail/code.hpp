#ifndef __VPUSH_DETAIL_CODE_HPP__
#define __VPUSH_DETAIL_CODE_HPP__

#include <vpush/protein_fwd.hpp>

namespace vpush {
namespace detail {

typedef double(*op_func_t)(Protein&);

struct Code {
	enum codetype {CODE, OPEN, CLOSE};

	Code(codetype t) : fptr(0), type(t) {}
	Code(op_func_t f) : fptr(f), type(CODE) {}
	Code(const Code& c) : fptr(c.fptr), type(c.type) {}

	op_func_t fptr;
	codetype type;
};

struct Exec : public Code {
	Exec(codetype t) : Code(t) {}
	Exec(op_func_t f) : Code(f) {}
	Exec(const Code& c) : Code(c) {}
	Exec(const Exec& e) : Code(e) {}
};

} // namespace detail
} // namespace vpush

#endif

