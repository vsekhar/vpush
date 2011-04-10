#ifndef __VPUSH_DETAIL_CODELIST_HPP__
#define __VPUSH_DETAIL_CODELIST_HPP__

#include <vector>
#include <algorithm>

#include <vpush/detail/stack_fwd.hpp>
//#include <vpush/detail/code_fwd.hpp>
#include <vpush/protein_fwd.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace detail {

template <typename T>
std::vector<T> get_list(stack<T>& s) {
	std::vector<T> ret;
	unsigned int bracket_level = 0;
	
	typename std::vector<T>::iterator start = s.end() - 1;
	while(start != s.begin()) {
		if(start->type == T::OPEN)
			++bracket_level;
		else if(start->type == T::CLOSE) {
			if(bracket_level)
				--bracket_level;
			else
				break;
		}
		else
			--start;
	}
	if(bracket_level)
		throw detail::unmatched_brackets();

	// start points to the close bracket
	ret.reserve((s.end() - start) - 1);
	std::copy(start+1, s.end(), std::back_inserter(ret));
	s.erase(start, s.end());
	return ret;	
}

template <typename T, typename U = T>
std::size_t put_list(std::vector<T> v, stack<U>& s, bool unwind = false) {
	if(unwind)
		s.reserve(s.size() + v.size());
	else {
		s.reserve(s.size() + v.size() + 2);
		s.push_back(T(T::CLOSE));
	}
	std::copy(v.begin(), v.end(), std::back_inserter(s));
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

