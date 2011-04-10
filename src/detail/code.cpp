#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace detail {

std::ostream& operator<<(std::ostream& o, const Code& c) {
	if(c.type == Code::OPEN)
		o << "(";
	else if(c.type == Code::CLOSE)
		o << ")";
	else
		o << vpush::functions.get_name(c.fptr);
	return o;
}

}
}

