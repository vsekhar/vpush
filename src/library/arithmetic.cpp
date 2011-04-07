#include <cmath>

#include <boost/numeric/conversion/converter.hpp>

#include <vpush/library/arithmetic.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace library {
namespace arithmetic {

template <typename T>
double add(Protein& p) { push<T>(p, pop<T>(p) + pop<T>(p)); return 1; }

template <typename T>
double subtract(Protein& p) { push<T>(p, pop<T>(p) - pop<T>(p)); return 1; }

template <typename T>
double multiply(Protein& p) { push<T>(p, pop<T>(p) * pop<T>(p)); return 1; }

template <typename T>
double divide(Protein& p) {
	if(top<T>(p) == static_cast<T>(0))
		return 0;
	T divisor = pop<T>(p);
	push<T>(p, pop<T>(p) / divisor);
	return 1;
}

double pow(Protein& p) {
	push<double>(p, std::pow(pop<double>(p), pop<double>(p)));
	return 1;
}

double mod(Protein& p) {
	push<int>(p, pop<int>(p) % pop<int>(p));
	return 1;
}

double to_int(Protein& p) {
	using namespace ::boost::numeric;
	typedef converter<int, double,
						conversion_traits<int, double>,
						def_overflow_handler,
						RoundEven<double> >
		my_converter;
	push<int>(p, my_converter::convert(pop<double>(p)));
	return 1;
}

double to_double(Protein &p) { push<double>(p, pop<int>(p)); return 1; }

void initialize() {
	using vpush::functions;
	using vpush::type;
	functions.add("ADD.INT", add<int>, type<int>() * 2);
	functions.add("SUBTRACT.INT", subtract<int>, type<int>() * 2);
	functions.add("MULTIPLY.INT", multiply<int>, type<int>() * 2);
	functions.add("DIVIDE.INT", divide<int>, type<int>() * 2);

	functions.add("ADD.DBL", add<double>, type<double>() * 2);
	functions.add("SUBTRACT.DBL", subtract<double>, type<double>() * 2);
	functions.add("MULTIPLY.DBL", multiply<double>, type<double>() * 2);
	functions.add("DIVIDE.DBL", divide<double>, type<double>() * 2);
	
	functions.add("POW.DBL", pow, type<double>() * 2);
	functions.add("MOD.INT", mod, type<int>() * 2);
	functions.add("TO_INT.DBL", to_int, type<double>());
	functions.add("TO_DBL.INT", to_double, type<int>());
}

} // namespace arithmetic
} // namespace library
} // namespace vpush

