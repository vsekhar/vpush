#include <string>

#include <vpush/library.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace library {
namespace basic {

template <typename T>
double eat(Protein &p) { pop<T>(p); return 1; }

template <typename T>
double equals(Protein& p) {
	push<bool>(p, pop<T>(p) == pop<T>(p));
	return 1;
}

template <typename T>
double equals_code(Protein &p) {
	return get_item(stack<T>(p)) == get_item(stack<T>(p));
}

void initialize() {
	functions.add("EAT.INT", eat<int>, type<int>());
	functions.add("EAT.DBL", eat<double>, type<double>());
	functions.add("EAT.BOOL", eat<bool>, type<bool>());
	functions.add("EAT.STRING", eat<std::string>, type<std::string>());
	
	functions.add("EQUALS.INT", equals<int>, type<int>() * 2);
	functions.add("EQUALS.DBL", equals<double>, type<double>() * 2);
	functions.add("EQUALS.BOOL", equals<bool>, type<bool>() * 2);
	functions.add("EQUALS.STRING", equals<std::string>, type<std::string>() * 2);
	functions.add("EQUALS.CODE", equals_code<Code>, type<Code>() * 2);
	functions.add("EQUALS.EXEC", equals_code<Exec>, type<Exec>() * 2);
}

} // namespace basic
} // namespace library
} // namespace vpush

