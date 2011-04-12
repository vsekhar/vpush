#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <vpush/library.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>
#include <vpush/exception.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace library {
namespace code {

using detail::Exec;
using detail::Code;

template <typename SRC, typename DEST>
double quote(Protein &p) {
	return put_item(get_item(stack<SRC>(p)), stack<DEST>(p));
}

double random_code(Protein &p) {
	push<Code>(p, functions.get_random());
	return 1;
}

template <typename T>
double make_list(Protein& p) {
	detail::stack<T>& s = stack<T>(p);
	int count = top<int>(p);
	if(count > 0 && has_n_items(s, count)) {
		typename detail::stack<T>::iterator itr = detail::advance_n_items(s, count);
		s.insert(itr, T::CLOSE);
		s.push_back(T::OPEN);
		return s.end() - itr;
	}
	else
		return 0;
}

template <typename T>
double equals_code(Protein &p) {
	return get_item(stack<T>(p)) == get_item(stack<T>(p));
}

template <typename T>
double dup(Protein& p) {
	detail::item<T> i = detail::get_item(stack<T>(p));
	detail::put_item(i, stack<T>(p));
	detail::put_item(i, stack<T>(p));
	return i.container.size() * 2;
}

template <typename T>
double do_range(Protein& p) {
	int end_index = pop<int>(p);
	int current_index = pop<int>(p);
	int increment = end_index < current_index ? -1 : 1;
	detail::item<T> i = detail::get_item(stack<T>(p));
	push<int>(p, current_index + increment);
	push<int>(p, end_index);
	detail::put_item(i, stack<T>(p));
	push<T>(p, do_range<T>);
	detail::put_item(i, stack<T>(p));
	return 1;
}

void initialize() {
	using vpush::functions;
	using vpush::type;
	using vpush::detail::Code;
	using vpush::detail::Exec;

	functions.add("QUOTE.CODE", quote<Exec, Code>, type<Exec>());
	functions.add("QUOTE.EXEC", quote<Code, Exec>, type<Code>());
	functions.add("RANDOM.CODE", random_code);
	functions.add("MAKELIST.CODE", make_list<Code>, type<int>());
	functions.add("MAKELIST.EXEC", make_list<Exec>, type<int>());

	functions.add("EQUALS.CODE", equals_code<Code>, type<Code>() * 2);
	functions.add("EQUALS.EXEC", equals_code<Exec>, type<Exec>() * 2);
	
	functions.add("DUP.CODE", dup<Code>, type<Code>());
	functions.add("DUP.EXEC", dup<Exec>, type<Exec>());
	functions.add("DO_RANGE.CODE", do_range<Code>, type<Code>() + type<int>() * 2);
	functions.add("DO_RANGE.EXEC", do_range<Exec>, type<Exec>() + type<int>() * 2);
}

} // namespace code
} // namespace library
} // namespace vpush

