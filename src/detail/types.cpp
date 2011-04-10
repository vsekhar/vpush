#include <map>

#include <boost/foreach.hpp>
#include <boost/assign/ptr_map_inserter.hpp>

#include <vpush/detail/types.hpp>
#include <vpush/exception.hpp>

namespace vpush {
namespace detail {

bool type_container::check(Protein& p) const {
	for(types_t::const_iterator i = _types.begin();
		i != _types.end(); ++i)
		if(!(i->second->check(p)))
			return false;
	return true;
}

type_container& type_container::operator+=(const type_container& other) {

	for(types_t::const_iterator i = other._types.begin();
		i != other._types.end(); ++i) {

		try {
			_types.at(i->first).count += i->second->count;
		}
		catch(boost::bad_ptr_container_operation) {
			_types.insert(i, ++types_t::const_iterator(i));
		}
	}

	return *this;
}

type_container& type_container::operator*=(unsigned int count) {
	if(count == 0)
		clear();
	else if(count > 1) {
		type_container temp(*this);
		for(unsigned int i=1; i < count; ++i)
			*this += temp;
	}
	return *this;
}

type_container operator+(const type_container& c1, const type_container& c2) {
	type_container ret(c1);
	ret += c2;
	return ret;
}

type_container operator*(const type_container& c1, unsigned int count) {
	type_container ret(c1);
	ret *= count;
	return ret;
}

} // namespace detail
} // namespace vpush

