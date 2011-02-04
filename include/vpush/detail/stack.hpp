#ifndef __VPUSH_STACK_HPP__
#define __VPUSH_STACK_HPP__

#include <boost/ptr_container/ptr_map.hpp>

#include <vpush/util/typeinfo.hpp>
#include <vpush/env_fwd.hpp>

namespace vpush {
namespace detail {

struct stack_base {
	virtual std::size_t size() const = 0;
	virtual bool empty() const = 0;
	virtual stack_base* clone() const = 0;
	virtual ~stack_base() {}
};

// for ptr_containers
stack_base* new_clone(const stack_base&);
void delete_clone(const stack_base*);

template <typename T>
struct stack : stack_base, std::list<T> {
	virtual std::size_t size() const { return std::list<T>::size(); }
	virtual bool empty() const { return std::list<T>::empty(); }
	virtual stack_base* clone() const { return new stack<T>(*this); }
};

typedef boost::ptr_map<util::TypeInfo, stack_base> stacks_t;

typedef int(*op_func_t)(Env&);
typedef std::map<std::string, op_func_t> functions_t;

} // namespace detail
} // namespace vpush

#endif // __VPUSH_STACK_HPP__

