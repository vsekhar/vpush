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
	SRC e = pop<SRC>(p);
	switch(e.type) {
		case SRC::OPEN:		return put_list(get_list(stack<SRC>(p)), stack<DEST>(p));
		case SRC::CLOSE:	throw detail::unmatched_brackets();
		default:			push<DEST>(p, e);
							return 1;
	}
}

double random_code(Protein &p) {
	push<Code>(p, functions.get_random());
	return 1;
}

template <typename T>
double make_list(Protein& p) {
	if(top<int>(p) > 0) {
		detail::stack<T>& s = stack<T>(p);
		int count = pop<int>(p);
		int index = s.size() - count;
		index = std::max(0, index);
		typename detail::stack<T>::iterator i = s.begin() + index;
		s.insert(i, T(T::CLOSE));
		s.push_back(T(T::OPEN));
		return count;
	}
	else
		return 0;
}

template <typename T>
double equals_code(Protein &p) {
	T t1 = pop<T>(p);
	if(t1.type == T::OPEN) {
		// first item is a list
		std::vector<T> v1 = detail::get_list(stack<T>(p));
		try {
			T t2 = pop<T>(p); // might stack_underflow
			
			if(t2.type != T::OPEN) {
				// second item is not a list
				push<bool>(p, false);
				return v1.size() + 1;
			}
			
			// compare lists
			std::vector<T> v2 = detail::get_list(stack<T>(p));
			push<bool>(p, v1 == v2);
			return v1.size() + v2.size();
		}
		catch(detail::stack_underflow) {
			// no second item, so no-op
			detail::put_list(v1, stack<T>(p)); // restores brackets
			return 0;
		}
	}
	else {
		// first item is op-code
		try {
			T t2 = pop<T>(p); // might stack_underflow

			if(t2.type == T::OPEN) {
				// second item is not op-code
				std::vector<T> v2 = detail::get_list(stack<T>(p)); // consume
				push<bool>(p, false);
				return v2.size() + 1;
			}

			// compare op-codes
			push<bool>(p, t2 == t1);
			return 2;
		}
		catch(detail::stack_underflow) {
			// no second item, so no-op
			push<T>(p, t1);
			return 0;
		}
	}
}

template <typename T>
double dup(Protein& p) {
	T t1 = pop<T>(p);
	switch(t1.type) {
		case T::OPEN:	{
							std::vector<T> v1 = detail::get_list(stack<T>(p));
							detail::put_list(v1, stack<T>(p));
							detail::put_list(v1, stack<T>(p));
						}
						break;
		case T::CLOSE:	throw detail::unmatched_brackets();
		case T::CODE:	push<T>(p, t1); push<T>(p, t1); break;
	}
	return 1;
}

template <typename T>
double do_range(Protein& p) {
	int end_index = pop<int>(p);
	int current_index = pop<int>(p);
	T t1 = pop<T>(p);
	
	// handle list
	if(t1.type == T::OPEN) {
		std::vector<T> v1 = detail::get_list(stack<T>(p));
		if(current_index==end_index) {
			push<int>(p, current_index);
			detail::put_list(v1, stack<T>(p));
		}
		else {
			int increment = end_index < current_index ? -1 : 1;
			push<int>(p, current_index + increment);
			push<int>(p, end_index);
			detail::put_list(v1, stack<T>(p));
			push<T>(p, do_range<T>);
			detail::put_list(v1, stack<T>(p));
		}
	}
	else if (t1.type == T::CLOSE)
		throw detail::unmatched_brackets();
	
	// handle single op-code
	else {
		if(current_index==end_index) {
			push<int>(p, current_index);
			push<T>(p, t1);
		}
		else {
			int increment = end_index < current_index ? -1 : 1;
			push<int>(p, current_index + increment);
			push<int>(p, end_index);
			push<T>(p, t1);
			push<T>(p, do_range<T>);
			push<T>(p, t1);
		}
	}
	return 1;
}

void initialize() {
	using vpush::functions;
	using vpush::type;
	functions.add("QUOTE.CODE", quote<Exec, Code>, type<Exec>());
	functions.add("QUOTE.EXEC", quote<Code, Exec>, type<Code>());
	functions.add("RANDOM.CODE", random_code);
	functions.add("MAKELIST.CODE", make_list<Code>, type<int>());
	functions.add("MAKELIST.EXEC", make_list<Exec>, type<int>());

	functions.add("EQUALS.CODE", equals_code<detail::Code>, type<detail::Code>() * 2);
	functions.add("EQUALS.EXEC", equals_code<detail::Exec>, type<detail::Exec>() * 2);
	
	functions.add("DUP.CODE", dup<detail::Code>, type<detail::Code>());
	functions.add("DUP.EXEC", dup<detail::Exec>, type<detail::Exec>());
	functions.add("DO_RANGE.CODE", do_range<detail::Code>, type<detail::Code>() + type<int>() * 2);
	functions.add("DO_RANGE.EXEC", do_range<detail::Exec>, type<detail::Exec>() + type<int>() * 2);
}

} // namespace code
} // namespace library
} // namespace vpush

