#ifndef __VPUSH_DETAIL_CODELIST_HPP__
#define __VPUSH_DETAIL_CODELIST_HPP__

#include <vector>
#include <algorithm>
#include <utility>

#include <boost/assert.hpp>

#include <vpush/detail/stack_fwd.hpp>

namespace vpush {
namespace detail {

// an "item": a container of T's, with a bool for each container indicating
// if the container genuinely contains a list (wrapping brackets are always omitted)
template <typename T>
struct item {
	item() : container(), is_atom(false) {}
	item(const std::vector<T>& c, bool b) : container(c), is_atom(b) {}
	std::vector<T> container;
	bool is_atom;
};

template <typename T>
inline bool operator==(const item<T>& i1, const item<T>& i2) {
	return i1.container == i2.container && i1.is_atom == i2.is_atom;
}

template <typename T>
struct itr_count_pair {
	typedef std::pair<
		typename stack<T>::iterator,
		std::size_t
	> type;
};

template <typename T>
typename itr_count_pair<T>::type advance_n_items_impl(stack<T>& s, std::size_t n) {
	std::size_t count = 0;
	std::size_t bracket_level = 0;
	typename std::vector<T>::reverse_iterator itr = s.rbegin();
	while(itr != s.rend()) {
		if(count >= n)
			break;
		switch(itr->type) {
			case T::OPEN:	++bracket_level; break;
			case T::CLOSE:	--bracket_level; break;
			case T::CODE:	break; // eliminate warning
		}
		if(!bracket_level)
			count++;
		++itr;
	}

#ifdef _DEBUG
	BOOST_ASSERT(bracket_level == 0);
#endif

	return std::make_pair(itr.base(), count);
}

// returns a forward iterator pointing to the n'th item from the end
// E.g. for stack: rbegin/pop -> [a, b, c, d, e, f, g, h]
//  advance_n_items(s, 4) returns iterator pointing to 'e'
//  advance_n_items(s, 0) returns iterator pointing to top ('a')
//
// Running insert(advance_n_items(4), 'x') yields:
//	rbegin/pop -> [a, b, c, d, e, x, f, g, h]

template <typename T>
typename stack<T>::iterator advance_n_items(stack<T>& s, std::size_t n) {
	typename itr_count_pair<T>::type ret = advance_n_items_impl(s, n);
	if((ret.second) < n) // brackets needed, otherwise g++ thinks i'm starting a template instantiation...
		throw detail::stack_underflow(typeid(T));
	return ret.first;
}

// checks if the CODE/EXEC stack has 'n' items (op-codes OR lists)
template <typename T>
bool has_n_items(stack<T> &s, std::size_t n) {
	typename itr_count_pair<T>::type ret = advance_n_items_impl(s, n);
	if(ret.second == n)
		return true;
	else
		return false;
}

// get an item
template <typename T>
item<T> get_item(stack<T>& s, bool erase = true) {
	item<T> ret;
	typename std::vector<T>::iterator itr = advance_n_items(s, 1);
	if(itr->type == T::CLOSE) {
		// list (itr points to the closing bracket)
		ret.container.insert(ret.container.end(), itr+1, s.end()-1); // brackets not copied
		ret.is_atom = false;
	}
	else {
		// atom
		ret.container.insert(ret.container.end(), itr, s.end());
		ret.is_atom = true;
#ifdef _DEBUG
		BOOST_ASSERT(ret.container.size() == 1);
#endif
	}
	if(erase)
		s.erase(itr, s.end());
	return ret;	
}

// get n'th item (top item is at n==0)
template <typename T>
item<T> get_nth_item(stack<T>& s, std::size_t n, bool erase = true) {
	if(n==0)
		return get_item(s, erase);
	typename std::vector<T>::iterator start = advance_n_items(s, n);
	typename std::vector<T>::iterator end = advance_n_items(s, n-1);
	item<T> ret;
	ret.container.insert(ret.container.end(), start, end);
	if(erase)
		s.erase(start, end);
	ret.is_atom = (end - start == 1);
	return ret;
}

template <typename T, typename U = T>
std::size_t put_item(item<T> i, stack<U>& s, bool preserve_list = true) {
#ifdef _DEBUG
	BOOST_ASSERT(!i.is_atom || i.container.size() == 1);
#endif
	preserve_list &= !i.is_atom;
	if(preserve_list) s.push_back(T(T::CLOSE));
	s.insert(s.end(), i.container.begin(), i.container.end());
	if(preserve_list) s.push_back(T(T::OPEN));
	return i.container.size() + (preserve_list ? 2 : 0);
}

template <typename SRC, typename DEST>
std::size_t unwind(stack<SRC>& src, stack<DEST>& dest) {
	return put_item(get_item(src), dest, false);
}
template <typename T> std::size_t unwind(stack<T>& s) { return unwind(s,s); }

} // namespace detail
} // namespace vpush

#endif

