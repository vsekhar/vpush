#ifndef __VPUSH_DETAIL_CODESTACK_HPP__
#define __VPUSH_DETAIL_CODESTACK_HPP__

#include <string>
#include <vector>

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <vpush/detail/stack.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace detail {

template <typename T>
struct codestack : public stack<T> {
private:
	friend class ::boost::serialization::access;
	template <class A> void save(A& a, unsigned int) const {
		std::size_t count = std::vector<T>::size();
		a << count;
		for(std::size_t i = 0; i < count; ++i) {
				std::string name;
				const T& code = (*this)[i];
				if(code.type == T::OPEN)
					name = "(";
				else if(code.type == T::CLOSE)
					name = ")";
				else {
					name = functions.get_name(code.fptr);
				}
				a << name;
			}
	}
	
	template <class A> void load(A& a, unsigned int) {
		std::vector<T>::clear();
		std::size_t count = 0;
		a >> count;

		for(std::size_t i = 0; i < count; ++i) {
			std::string name;
			a >> name;
			if(name == "(")
				std::vector<T>::push_back(T(T::OPEN));
			else if(name == ")")
				std::vector<T>::push_back(T(T::CLOSE));
			else
				std::vector<T>::push_back(T(functions.get_fptr(name)));
		}
	}
	
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};


} // namespace detail
} // namespace vpush

#endif

