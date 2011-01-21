#ifndef __VPUSH_STACKS_HPP__
#define __VPUSH_STACKS_HPP__

#include <map>
#include <vpush/stack_fwd.hpp>
#include <vpush/util/typeinfo.hpp>
#include <vpush/util/singleton.hpp>

namespace vpush {

typedef std::map<util::TypeInfo, detail::stack_base*> stacks_t;
typedef util::singleton<stacks_t> stacks_singleton;

// This has to be a singleton what we put in it (stacks) are singletons
inline stacks_t& stacks() { return stacks_singleton::instance(); }

} // namespace vpush

#endif // __VPUSH_STACKS_HPP__

