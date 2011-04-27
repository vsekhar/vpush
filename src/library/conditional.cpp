#include <vpush/library.hpp>

namespace vpush {
namespace library {
namespace conditional {

double if_(Protein& p) {
	if(pop<bool>(p)) {
		item<Exec> i = get_item(stack<Exec>(p));
		get_item(stack<Exec>(p));
		put_item(i, stack<Exec>(p));
	}
	else
		get_item(stack<Exec>(p));
	return 1;
}

void initialize() {
	functions.add("IF", if_, type<bool>() + (type<Exec>()*2));
}

}}} // namespace vpush::library::conditional

