#ifndef __VPUSH_DETAIL_CODESTACK_HPP__
#define __VPUSH_DETAIL_CODESTACK_HPP__

#include <string>
#include <vector>
#include <stdexcept>

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <vpush/detail/stack.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {
namespace detail {

template <typename T>
struct codestack : public stack<T> {
private:
	typedef std::vector<std::string> names_t;
	
	friend class ::boost::serialization::access;
	template <class A> void save(A& a, unsigned int) const {
		names_t codenames(std::vector<T>::size());
		for(typename std::vector<T>::const_iterator i = std::vector<T>::begin();
			i != std::vector<T>::end(); ++i) {
				if(i->type == T::OPEN)
					codenames.push_back("(");
				else if(i->type == T::CLOSE)
					codenames.push_back(")");
				else
					codenames.push_back(functions.get_name(i->fptr));
			}
		a & codenames;
	}
	
	template <class A> void load(A& a, unsigned int) {
		names_t codenames;
		a & codenames;
		for(typename names_t::const_iterator i = codenames.begin();
			i != codenames.end(); ++i) {
				if(*i == "(")
					std::vector<T>::push_back(T(T::OPEN));
				else if(*i == ")")
					std::vector<T>::push_back(T(T::CLOSE));
				else
					std::vector<T>::push_back(T(functions.get_fptr(*i)));
		}
	}
	
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};


} // namespace detail
} // namespace vpush

#endif

