#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace detail {

Code::Code(std::string s) {
	Code c = vpush::functions.get_code(s);
	fptr = c.fptr;
	type = c.type;
}

std::ostream& operator<<(std::ostream& o, const Code& c) {
	o << vpush::functions.get_name(c);
	return o;
}

}
}

