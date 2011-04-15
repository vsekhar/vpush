#include <string>

#include <vpush/library.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace library {
namespace basic {

template <typename T>
double zero(Protein& p) { push<T>(p, 0); return 1; }

template <typename T>
double one(Protein& p) { push<T>(p, 1); return 1; }

void initialize() {
	using std::string;

	functions.add("ZERO.INT", zero<int>);
	functions.add("ZERO.DBL", zero<double>);
	functions.add("ONE.INT", one<int>);
	functions.add("ONE.DBL", one<double>);
}

} // namespace basic
} // namespace library
} // namespace vpush

