#include <vpush/detail/stack.hpp>

namespace vpush {
namespace detail {

stack_base* new_clone(const stack_base& s) { return s.clone(); }
void delete_clone(const stack_base* s) { if(s) delete s; }


} // namespace detail
} // namespace vpush

