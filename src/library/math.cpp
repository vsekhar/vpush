#include <cmath>

#include <vpush/library.hpp>

namespace vpush {
namespace library {
namespace math {

double pow(Protein& p) {
	push<double>(p, std::pow(pop<double>(p), pop<double>(p)));
	return 1;
}

double sin(Protein& p) {
	push<double>(p, std::sin(pop<double>(p)));
	return 1;
}

double cos(Protein &p) {
	push<double>(p, std::cos(pop<double>(p)));
	return 1;
}

double tan(Protein &p) {
	push<double>(p, std::tan(pop<double>(p)));
	return 1;
}

void initialize() {
	functions.add("POW.DBL", pow, type<double>() * 2);
	functions.add("SIN.DBL", sin, type<double>());
	functions.add("COS.DBL", cos, type<double>());
	functions.add("TAN.DBL", tan, type<double>());
}

}
}
}

