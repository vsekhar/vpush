#ifndef __VPUSH_DETAIL_CODE_HPP__
#define __VPUSH_DETAIL_CODE_HPP__

#include <ostream>
#include <stdexcept>

#include <vpush/detail/code_fwd.hpp>

namespace vpush {
namespace detail {

struct Code {
	enum codetype {CODE, OPEN, CLOSE};

	Code(codetype t) : fptr(0), type(t) {
#ifdef _DEBUG
		if(t == CODE)
			throw std::invalid_argument("Code(codetype) constructor can only be used for brackets");
#endif
	}
	explicit Code(op_func_t f) : fptr(f), type(CODE) {}
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

inline bool operator==(const Code& c1, const Code& c2) { return c1.fptr==c2.fptr && c1.type==c2.type; }
inline bool operator!=(const Code& c1, const Code& c2) { return !(c1==c2); }

std::ostream& operator<<(std::ostream&, const Code&);

} // namespace detail
} // namespace vpush

#endif

