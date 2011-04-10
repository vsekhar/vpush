#include <string>

#include <vpush/library.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/stackops.hpp>

namespace vpush {
namespace library {
namespace basic {

template <typename T>
double eat(Protein &p) { pop<T>(p); return 1; }

void initialize() {
	using vpush::functions;
	using vpush::type;
	functions.add("EAT.INT", eat<int>, type<int>());
	functions.add("EAT.DBL", eat<double>, type<double>());
	functions.add("EAT.BOOL", eat<bool>, type<bool>());
	functions.add("EAT.STRING", eat<std::string>, type<std::string>());
}

} // namespace basic
} // namespace library
} // namespace vpush

