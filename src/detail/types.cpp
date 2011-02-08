#include <map>

#include <boost/foreach.hpp>

#include <vpush/detail/types.hpp>
#include <vpush/env.hpp>
#include <vpush/exception.hpp>

namespace vpush {
namespace detail {

void type_container::add(util::TypeInfo t) {
	_types.push_back(t);
}

type_container& type_container::operator+=(const type_container& other) {
	std::copy(other._types.begin(), other._types.end(), std::back_inserter(_types));
	return *this;
}

type_container& type_container::operator*=(unsigned int count) {
	if(count == 0)
		clear();
	else {
		type_container temp(*this);
		for(unsigned int i=1; i < count; ++i)
			*this += temp;
	}
	return *this;
}

type_container operator+(const type_container& c1, const type_container& c2) {
	type_container ret;
	ret += c1;
	ret += c2;
	return ret;
}

type_container operator*(const type_container& c1, unsigned int count) {
	type_container ret;
	ret *= count;
	return ret;
}

} // namespace detail
} // namespace vpush

