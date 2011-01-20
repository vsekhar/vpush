#include <vpush/detail/typechecker.hpp>

namespace vpush {
namespace detail {

type_checker::type_checker(util::TypeInfo t) {
	add(t);
}

void type_checker::add(util::TypeInfo t) {
	_types.push_back(t);
}

type_checker& operator+=(type_checker& me, const type_checker& other) {
	std::copy(other._types.begin(), other._types.end(), me._types.end());
	return me;
}

type_checker& operator*=(type_checker& me, unsigned int count) {
	if(count == 0)
		me.clear();
	else {
		type_checker temp(me);
		for(unsigned int i=1; i < count; ++i)
			me += temp;
	}
	return me;
}

type_checker operator+(const type_checker& c1, const type_checker& c2) {
	type_checker ret;
	ret += c1;
	ret += c2;
	return ret;
}

type_checker operator*(const type_checker& c1, unsigned int count) {
	type_checker ret;
	ret *= count;
	return ret;
}

} // namespace detail
} // namespace vpush



