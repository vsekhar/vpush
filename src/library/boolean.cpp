#include <vpush/library.hpp>
#include <vpush/util/random.hpp>

namespace vpush {
namespace library {
namespace boolean {

double and_(Protein& p) {
	push<bool>(p, pop<bool>(p) && pop<bool>(p));
	return 1;
}

double or_(Protein &p) {
	push<bool>(p, pop<bool>(p) || pop<bool>(p));
	return 1;
}

double not_(Protein &p) {
	push<bool>(p, !pop<bool>(p));
	return 1;
}

double random(Protein &p) {
	push<bool>(p, util::RandomBool()());
	return 1;
}

template <typename T>
double test(Protein &p) {
	push<bool>(p, pop<T>(p) != static_cast<T>(0));
	return 1;
}

void initialize() {
	functions.add("AND.BOOL", and_, type<bool>() * 2);
	functions.add("OR.BOOL", or_, type<bool>() * 2);
	functions.add("NOT.BOOL", not_, type<bool>());
	functions.add("TESTINT.BOOL", test<int>, type<int>());
	functions.add("TESTDBL.BOOL", test<double>, type<double>());
}

}
}
}
