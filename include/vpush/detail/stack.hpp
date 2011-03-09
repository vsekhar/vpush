#ifndef __VPUSH_DETAIL_STACK_HPP__
#define __VPUSH_DETAIL_STACK_HPP__

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>

namespace vpush {
namespace detail {

template <typename T>
struct stack : public std::vector<T> {
private:
	friend class ::boost::serialization::access;
	template <typename A> void serialize(A& a, unsigned int) {
		a & ::boost::serialization::base_object<std::vector<T> >(*this);
	}
};

} // namespace detail
} // namespace vpush

#endif // __VPUSH_STACK_HPP__

