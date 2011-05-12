#include <vpush/library.hpp>

namespace vpush {
namespace library {

void initialize() {
	arithmetic::initialize();
	basic::initialize();
	boolean::initialize();
	code::initialize();
	combinators::initialize();
	comparison::initialize();
	conditional::initialize();
	gestation::initialize();
	interaction::initialize();
	math::initialize();
	movement::initialize();
	soup::initialize();
	stackops::initialize();
}

} // namespace library
} // namespace vpush

