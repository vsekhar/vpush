#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace detail {

std::ostream& operator<<(std::ostream& o, const Code& c) {
	o << vpush::functions.get_name(c);
	return o;
}

}
}

