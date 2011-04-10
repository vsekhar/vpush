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

void initialize() {
	using vpush::functions;
	using vpush::type;
	functions.add("QUOTE.CODE", quote_code, type<Exec>());
	functions.add("RANDOM.CODE", random_code);
	functions.add("MAKELIST.CODE", make_list<Code>, type<int>());
	functions.add("MAKELIST.EXEC", make_list<Exec>, type<int>());
}

} // namespace code
} // namespace library
} // namespace vpush

