#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace util {

bool operator<(const TypeInfo& t1, const std::type_info& t2) {
	return t1.get().before(t2);
}

bool operator==(const TypeInfo& t1, const std::type_info& t2) {
	return t1.get() == t2;
}

std::ostream& operator<<(std::ostream& os, const TypeInfo &t) {
	return os << t.get().name();
}

} // namespace util
} // namespace vpush

