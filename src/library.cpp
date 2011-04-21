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
	gestation::initialize();
	math::initialize();
	movement::initialize();
	soup::initialize();
	stackops::initialize();
}

} // namespace library
} // namespace vpush

