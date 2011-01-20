#include <vpush/detail/typechecker.hpp>

namespace vpush {
namespace detail {

type_checker::type_checker(util::TypeInfo t) {
	add(t);
}

void type_checker::add(util::TypeInfo t) {
	_types.push_back(t);
}

type_checker type_checker::merge(const type_checker& other) const {
	type_checker ret(*this);
	std::copy(other._types.begin(), other._types.end(), ret._types.end());
	return ret;
}

type_checker operator+(const type_checker& c1, const type_checker& c2) {
	return c1.merge(c2);
}

} // namespace detail
} // namespace vpush



