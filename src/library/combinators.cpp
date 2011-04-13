#include <boost/tuple/tuple.hpp>

#include <vpush/library.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {
namespace library {
namespace combinators {

template <typename T>
double k(Protein& p) {
	double count = 0;
	typename detail::item<T> i1 = detail::get_item(stack<T>(p));
	count += i1.container.size();
	count += detail::get_item(stack<T>(p)).container.size(); // toss
	detail::put_item(i1, stack<T>(p));
	return count;
}

template <typename T>
double s(Protein& p) {
	std::vector<typename detail::item<T>> items;
	items.reserve(3);
	for(unsigned i = 0; i < 3; ++i)
		items.push_back(detail::get_item(stack<T>(p)));
	
	push<T>(p, T::CLOSE);
	detail::put_item(items[2], stack<T>(p));
	detail::put_item(items[1], stack<T>(p));
	push<T>(p, T::OPEN);
	detail::put_item(items[2], stack<T>(p));
	detail::put_item(items[0], stack<T>(p));

	return items[0].container.size()
		+ items[1].container.size()
		+ items[2].container.size();
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


