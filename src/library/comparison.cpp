#include <vpush/library.hpp>

namespace vpush {
namespace library {
namespace comparison {

template <typename T>
double equals(Protein& p) {
	push<bool>(p, pop<T>(p) == pop<T>(p));
	return 1;
}

template <typename T>
double equals_code(Protein &p) {
	push<bool>(p, get_item(stack<T>(p)) == get_item(stack<T>(p)));
	return 1;
}

template <typename T>
double lessthan(Protein &p) {
	T first = pop<T>(p);
	push<bool>(p, pop<T>(p) < first);
	return 1;
}

template <typename T>
double greaterthan(Protein &p) {
	T first = pop<T>(p);
	push<bool>(p, pop<T>(p) > first);
	return 1;
}

template <typename T>
double max(Protein& p) {
	push<T>(p, std::max(pop<T>(p), pop<T>(p)));
	return 1;
}

template <typename T>
double min(Protein& p) {
	push<T>(p, std::min(pop<T>(p), pop<T>(p)));
	return 1;
}

void initialize() {
	using std::string;
	
	functions.add("==.INT", equals<int>, type<int>() * 2);
	functions.add("==.DBL", equals<double>, type<double>() * 2);
	functions.add("==.BOOL", equals<bool>, type<bool>() * 2);
	functions.add("==.STRING", equals<string>, type<string>() * 2);
	functions.add("==.CODE", equals_code<Code>, type<Code>() * 2);
	functions.add("==.EXEC", equals_code<Exec>, type<Exec>() * 2);

	functions.add("<.INT", lessthan<int>, type<int>() * 2);
	functions.add("<.DBL", lessthan<double>, type<double>() * 2);
	functions.add("<.STRING", lessthan<string>, type<string>() * 2);

	functions.add(">.INT", greaterthan<int>, type<int>() * 2);
	functions.add(">.DBL", greaterthan<double>, type<double>() * 2);
	functions.add(">.STRING", greaterthan<string>, type<string>() * 2);

	functions.add("MAX.INT", max<int>, type<int>() * 2);
	functions.add("MAX.DBL", max<double>, type<double>() * 2);
	functions.add("MAX.STRING", max<string>, type<string>() * 2);
	
	functions.add("MIN.INT", min<int>, type<int>() * 2);
	functions.add("MIN.DBL", min<double>, type<double>() * 2);
	functions.add("MIN.STRING", min<string>, type<string>() * 2);
}

}
}
}
