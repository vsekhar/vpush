#include <vpush/library.hpp>
#include <vpush/soup.hpp>

namespace vpush {
namespace library {
namespace soup {

double proteincount(Protein &p) {
	push<double>(p, vpush::soup.size());
	return 1;
}

void initialize() {
	functions.add("PROTEINCOUNT", proteincount);
}

}}} // namespace vpush::library::soup

