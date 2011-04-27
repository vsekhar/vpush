#include <string>

#include <vpush/library.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>
#include <vpush/util/random.hpp>
#include <vpush/stackops.hpp>
#include <vpush/exception.hpp>

#define VPUSH_RANDOM_STRING_LEN (6)

namespace vpush {
namespace library {
namespace basic {

double stop(Protein& p) { p.running = false; return 1; }

template <typename T>
double zero(Protein& p) { push<T>(p, 0); return 1; }

template <typename T>
double one(Protein& p) { push<T>(p, 1); return 1; }

double random_string(Protein &p) {
	util::RandomString rstr;
	push<std::string>(p, rstr(VPUSH_RANDOM_STRING_LEN));
	return 1;
}

void initialize() {
	functions.add("STOP", stop);
	functions.add("ZERO.INT", zero<int>);
	functions.add("ZERO.DBL", zero<double>);
	functions.add("ONE.INT", one<int>);
	functions.add("ONE.DBL", one<double>);
	
	functions.add("RANDOM.STRING", random_string);
}

} // namespace basic
} // namespace library
} // namespace vpush

