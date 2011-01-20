#ifndef __VPUSH_STACKS_HPP__
#define __VPUSH_STACKS_HPP__

#include <map>
#include <boost/ref.hpp>
#include <vpush/util/singleton.hpp>
#include <vpush/stack_fwd.hpp>

namespace vpush {

typedef std::map<util::TypeInfo, detail::stack_base*> stacks_t;
typedef util::singleton<stacks_t> stacks_singleton;

inline stacks_t& stacks() { return stacks_singleton::instance(); }

} // namespace vpush

#endif // __VPUSH_STACKS_HPP__

