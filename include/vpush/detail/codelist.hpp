#ifndef __VPUSH_DETAIL_CODELIST_HPP__
#define __VPUSH_DETAIL_CODELIST_HPP__

#include <vector>
#include <algorithm>

#include <vpush/detail/stack_fwd.hpp>
#include <vpush/protein_fwd.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace detail {

template <typename T>
std::vector<T> get_list(stack<T>& s) {
	std::vector<T> ret;
	unsigned int bracket_level = 1;
	
	typename std::vector<T>::reverse_iterator start = s.rbegin();
	while(start != s.rend()) {
		switch(start->type) {
			case T::OPEN:	++bracket_level; break;
			case T::CLOSE:	--bracket_level; break;
			case T::CODE:	break; // eliminate warning
		}
		if(!bracket_level) break;
		++start;
	}

	if(bracket_level)
		throw detail::unmatched_brackets();

	// start points to the closing bracket (which is not copied)
	typename std::vector<T>::iterator itr = start.base() - 1;
	ret.insert(ret.end(), itr+1, s.end());
	s.erase(itr, s.end());
	return ret;	
}

template <typename T, typename U = T>
std::size_t put_list(std::vector<T> v, stack<U>& s, bool unwind = false) {
	if(!unwind)
		s.push_back(T(T::CLOSE));
	s.insert(s.end(), v.begin(), v.end());
	if(!unwind)
		s.push_back(T(T::OPEN));
	return v.size() + (unwind ? 0 : 2);
}

template <typename SRC, typename DEST>
std::size_t unwind(stack<SRC>& src, stack<DEST>& dest) {
	return put_list(get_list(src), dest, true);
}

template <typename T> std::size_t unwind(stack<T>& s) { return unwind(s,s); }

} // namespace detail
} // namespace vpush

#endif

