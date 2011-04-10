#include <vpush/library.hpp>

namespace vpush {
namespace library {

void initialize() {
	basic::initialize();
	code::initialize();
	arithmetic::initialize();
}

} // namespace library
} // namespace vpush

