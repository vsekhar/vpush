#ifndef __VPUSH_STACKOPS_HPP__
#define __VPUSH_STACKOPS_HPP__

#include <ostream>

#include <boost/foreach.hpp>
#include <boost/fusion/container/map.hpp>

#include <vpush/detail/stack.hpp>
#include <vpush/detail/codestack.hpp>
#include <vpush/protein.hpp>

namespace vpush {

namespace fus = ::boost::fusion;

/* stack-wide operations queries & operations */
template <typename T> inline std::size_t size(Protein& p) { return stack<T>(p).size(); }
template <typename T> inline bool empty(Protein& p) { return stack<T>(p).empty(); }
template <typename T> inline void clear(Protein& p) { stack<T>(p).clear(); }

/* non-modifying element access */
template <typename T> inline T top(Protein& p) { return stack<T>(p).back(); }
template <class T>    inline T first(Protein& p) { return top<T>(p); }
template <class T>    inline T second(Protein& p) { return *(++stack<T>(p).rbegin()); }

/* modifying element access (pushing and popping) */
template <typename T> inline void push(Protein& p, const T& v) { stack<T>(p).push_back(v); }

template <typename T>
inline T pop(Protein& p) {
	detail::stack<T>& s = stack<T>(p);
	if(s.empty()) throw detail::stack_underflow(typeid(T));
	T ret(s.back());
	s.pop_back();
	return ret;
}

/* non-standard stack operations (for combinatorics operators) */

template <typename T>
void inline push_i(Protein& p, const T& t, int i) {
	detail::stack<T>& s = stack<T>(p);
	if(i < 0)
		throw detail::stack_underflow(typeid(T));

	// if stack isn't as big as i, push at bottom
	i = (unsigned)i < s.size() ? i : s.size();
	typename detail::stack<T>::reverse_iterator itr = s.rbegin();
	itr += i;
	s.insert(itr.base(), t);
}

template <typename T>
inline T pop_i(Protein& p, int i) {
	detail::stack<T>& s = stack<T>(p);
	if(i < 0 || s.size() < ((unsigned)i)+1)
		throw detail::stack_underflow(typeid(T));

	// some funny iterator arithmetic for converting between
	// reverse and forward iterators
	typename detail::stack<T>::reverse_iterator itr
		= s.rbegin() + i;
	T ret = *itr;
	s.erase(--itr.base());
	return ret;
}

template <typename T>
void inline push_second(Protein& p, const T& t) { push_i<T>(p, t, 1); }

template <typename T>
inline T pop_second(Protein& p) { return pop_i<T>(p, 1); }

template <typename T>
inline void swap(Protein& p, std::size_t a, std::size_t b) {
	std::swap(*(stack<T>(p).rbegin() + a), *(stack<T>(p).rbegin() + b));
}

} // namespace vpush

#endif

