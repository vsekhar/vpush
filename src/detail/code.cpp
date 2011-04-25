#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace detail {

Code::Code(codetype t, const std::string& name) {
	type = t;
	switch(type) {
		case Code::OPEN:
		case CODE::CLOSE:	fptr = NULL; break;
		default;			fptr = vpush::functions.get_fptr(name);
							break;
	}
}

std::ostream& operator<<(std::ostream& o, const Code& c) {
	switch(c.type) {
		case Code::OPEN:	o << "("; break;
		case Code::CLOSE:	o << ")"; break;
		default:			o << vpush::functions.get_name(c.fptr); break;
	}
	return o;
}

}
}

