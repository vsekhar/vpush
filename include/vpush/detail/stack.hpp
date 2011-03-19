#ifndef __VPUSH_DETAIL_STACK_HPP__
#define __VPUSH_DETAIL_STACK_HPP__

#include <vector>
#include <ostream>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>

namespace vpush {
namespace detail {

template <typename T>
struct stack : std::vector<T> {
private:
	friend class ::boost::serialization::access;
	template <typename A> void serialize(A& a, unsigned int) {
		a & ::boost::serialization::base_object<std::vector<T> >(*this);
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& o, const stack<T>& s) {
	bool first = true;
	for(typename stack<T>::const_reverse_iterator i = s.rbegin();
		i != s.rend(); i++) {
			if(first)
				first = false;
			else
				o << ", ";
			o << *i;
	}
	return o;
}

} // namespace detail
} // namespace vpush

#endif // __VPUSH_STACK_HPP__

