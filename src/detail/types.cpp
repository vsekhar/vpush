#include <map>

#include <boost/foreach.hpp>

#include <vpush/detail/types.hpp>
#include <vpush/env.hpp>
#include <vpush/exception.hpp>

namespace vpush {
namespace detail {

void types::add(util::TypeInfo t) {
	_types.push_back(t);
}

types& operator+=(types& me, const types& other) {
	std::copy(other._types.begin(), other._types.end(), me._types.end());
	return me;
}

types& operator*=(types& me, unsigned int count) {
	if(count == 0)
		me.clear();
	else {
		types temp(me);
		for(unsigned int i=1; i < count; ++i)
			me += temp;
	}
	return me;
}

types operator+(const types& c1, const types& c2) {
	types ret;
	ret += c1;
	ret += c2;
	return ret;
}

types operator*(const types& c1, unsigned int count) {
	types ret;
	ret *= count;
	return ret;
}

} // namespace detail
} // namespace vpush



