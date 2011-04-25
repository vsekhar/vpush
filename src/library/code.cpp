#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/tokenizer.hpp>

#include <vpush/library.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>
#include <vpush/exception.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace library {
namespace code {


template <typename T>
double is_atom(Protein &p) {
	push<bool>(p, top<T>(p).type == T::CODE);
	return 1;
}

template <typename SRC, typename DEST>
double quote(Protein &p) {
	return put_item(get_item(stack<SRC>(p)), stack<DEST>(p));
}

template <typename T>
double to_string(Protein &p) {
	item<T> i = get_item(stack<T>(p));
	std::stringstream ss;
	bool first = true;
	BOOST_FOREACH(const T& t, i) {
		if(first) first = false;
		else ss << " ";
		ss << t;
	}
	push<string>(p, ss.str());
	return 0;
}

template <typename T>
double from_string(Protein &p) {
	string str = pop<string>(p);
	boost::char_separator<char> sep(" ");
	boost::tokenizer tok(str, sep);
	BOOST_FOREACH(const string& s, tok)
		push<T>(p, functions.get_code(s));
	return 0;
}

double random_code(Protein &p) {
	push<Code>(p, functions.get_random());
	return 1;
}

template <typename T>
double cons(Protein &p) {
	item<T> a = get_item(stack<T>(p));
	item<T> b = get_item(stack<T>(p));
	b.is_atom = false;
	b.container.insert(b.container.end(), a.container.begin(), a.container.end());
	put_item(b, stack<T>(p));
	return b.container.size();
}

template <typename T>
double make_list(Protein& p) {
	using detail::itr_count_pair;
	using detail::advance_n_items_impl;

	detail::stack<T>& s = stack<T>(p);
	if(top<int>(p) < 0) return 0;
	int count = pop<int>(p);
	count = std::min(count, (int)p.energy);
	typename itr_count_pair<T>::type insertpoint;
	try {
		insertpoint	= advance_n_items_impl(s, count);
	}
	catch(const detail::stack_underflow&) {
		insertpoint = std::make_pair(s.begin(), s.size());
	}
	s.insert(insertpoint.first, T::CLOSE);
	s.push_back(T::OPEN);
	return insertpoint.second;
}

template <typename T>
typename std::vector<T>::const_iterator find_substring(const std::vector<T>& vec, const std::vector<T>& substring) {
	typename std::vector<T>::const_iterator i = vec.begin();
	for(; i < vec.end(); ++i) {
		bool match = true;
		for(std::size_t j = 0; j < substring.size(); ++j)
			if(*(i+j) != substring[j]) {
				match = false;
				break;
			}
		if(match) return i;
	}
	return i;
}

template <typename T>
double contains(Protein &p) {
	item<T> a = get_nth_item(stack<T>(p), 0, false);
	item<T> b = get_nth_item(stack<T>(p), 1, false);
	push<bool>(p, find_substring(b.container, a.container) != b.container.end());
	return a.container.size() + b.container.size();
}

template <typename T>
double do_range(Protein& p) {
	int end_index = pop<int>(p);
	int current_index = pop<int>(p);
	int increment = end_index < current_index ? -1 : 1;
	item<T> i = get_item(stack<T>(p));
	push<int>(p, current_index + increment);
	push<int>(p, end_index);
	put_item(i, stack<T>(p));
	push<T>(p, do_range<T>);
	put_item(i, stack<T>(p));
	return 1;
}

void initialize() {
	functions.add("ISATOM.CODE", is_atom<Code>, type<Code>());
	functions.add("ISATOM.EXEC", is_atom<Exec>, type<Exec>());

	functions.add("QUOTE.CODE", quote<Exec, Code>, type<Exec>());
	functions.add("QUOTE.EXEC", quote<Code, Exec>, type<Code>());
	functions.add("TO_STRING.CODE", to_string<Code>, type<Code>());
	functions.add("TO_STRING.EXEC", to_string<Exec>, type<Exec>());
	functions.add("FROM_STRING.CODE", from_string<Code>, type<string>());
	functions.add("FROM_STRING.EXEC", from_string<Exec>, type<string>());

	functions.add("RANDOM.CODE", random_code);

	functions.add("CONS.CODE", cons<Code>, type<Code>() * 2);
	functions.add("CONS.EXEC", cons<Exec>, type<Exec>() * 2);
	functions.add("MAKELIST.CODE", make_list<Code>, type<int>());
	functions.add("MAKELIST.EXEC", make_list<Exec>, type<int>());
	functions.add("CONTAINS.CODE", contains<Code>, type<Code>() * 2);
	functions.add("CONTAINS.EXEC", contains<Exec>, type<Exec>() * 2);

	functions.add("DO_RANGE.CODE", do_range<Code>, type<Code>() + type<int>() * 2);
	functions.add("DO_RANGE.EXEC", do_range<Exec>, type<Exec>() + type<int>() * 2);
}

} // namespace code
} // namespace library
} // namespace vpush

