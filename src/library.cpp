#include <vpush/library.hpp>

namespace vpush {
namespace library {

void initialize() {
	basic::initialize();
	code::initialize();
	arithmetic::initialize();
	math::initialize();
	combinators::initialize();
	boolean::initialize();
	stackops::initialize();
}

} // namespace library
} // namespace vpush

