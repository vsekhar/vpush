#include <boost/tuple/tuple.hpp>

#include <vpush/library.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {
namespace library {
namespace combinators {

template <typename T>
double k(Protein& p) {
	// Kxy = x
	typename detail::item<T> x = detail::get_item(stack<T>(p));
	typename detail::item<T> y = detail::get_item(stack<T>(p));
	detail::put_item(x, stack<T>(p));
	// toss y
	return x.container.size() + y.container.size();
}

template <typename T>
double s(Protein& p) {
	
	std::vector<typename detail::item<T> > items;
	typename detail::item<T> x = detail::get_item(stack<T>(p));
	typename detail::item<T> y = detail::get_item(stack<T>(p));
	typename detail::item<T> z = detail::get_item(stack<T>(p));
	
	push<T>(p, T::CLOSE);
	detail::put_item(z, stack<T>(p));
	detail::put_item(y, stack<T>(p));
	push<T>(p, T::OPEN);
	detail::put_item(z, stack<T>(p));
	detail::put_item(x, stack<T>(p));

	return x.container.size() + y.container.size() + z.container.size();
}

void initialize() {
	functions.add("S.CODE", s<Code>, type<Code>() * 3);
	functions.add("S.EXEC", s<Exec>, type<Exec>() * 3);
	functions.add("K.CODE", k<Code>, type<Code>() * 2);
	functions.add("K.EXEC", k<Exec>, type<Exec>() * 2);
}

} // namespace combinators
} // namespace library
} // namespace vpush


