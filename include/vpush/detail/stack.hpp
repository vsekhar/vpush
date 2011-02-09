#ifndef __VPUSH_DETAIL_STACK_HPP__
#define __VPUSH_DETAIL_STACK_HPP__

#include <vector>

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
struct stack : stack_base, std::vector<T> {
	typedef std::vector<T> base_t;
	virtual std::size_t size() const { return base_t::size(); }
	virtual bool empty() const { return base_t::empty(); }
	virtual stack_base* clone() const { return new stack<T>(*this); }
};

} // namespace detail
} // namespace vpush

#endif // __VPUSH_STACK_HPP__

