#include <string>
#include <vector>
#include <algorithm>

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

double quote_code(Protein &p) {
	Exec e = pop<Exec>(p);
	switch(e.type) {
		case Exec::OPEN:	return put_list(get_list(stack<Exec>(p)), stack<Code>(p));
		case Exec::CLOSE:	throw detail::unmatched_brackets();
		default:			push<Code>(p, e);
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
		std::vector<T> v1 = detail::get_list(stack<T>(p));
		try {
			T t2 = pop<T>(p); // might stack_underflow
			if(t2.type != T::OPEN) {
				push<T>(p, t2);
				double ret = detail::put_list(v1, stack<T>(p));
				push<bool>(p, false);
				return ret;
			}
			std::vector<T> v2 = detail::get_list(stack<T>(p));
			push<bool>(p, v1 == v2);
			return v1.size() + v2.size();
		}
		catch(detail::stack_underflow) {
			double ret = detail::put_list(v1, stack<T>(p));
			push<bool>(p, false);
			return ret;
		}
	}
	else {
		push<bool>(p, t1 == pop<T>(p));
		return 1;
	}
}

void initialize() {
	using vpush::functions;
	using vpush::type;
	functions.add("QUOTE.CODE", quote_code, type<Exec>());
	functions.add("RANDOM.CODE", random_code);
	functions.add("MAKELIST.CODE", make_list<Code>, type<int>());
	functions.add("MAKELIST.EXEC", make_list<Exec>, type<int>());

	functions.add("EQUALS.CODE", equals_code<detail::Code>, type<detail::Code>() * 2);
	functions.add("EQUALS.EXEC", equals_code<detail::Exec>, type<detail::Exec>() * 2);
	
}

} // namespace code
} // namespace library
} // namespace vpush

