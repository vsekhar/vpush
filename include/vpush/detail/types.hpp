#ifndef __VPUSH_TYPES_HPP__
#define __VPUSH_TYPES_HPP__

#include <list>

#include <vpush/env_fwd.hpp>
#include <vpush/detail/wrappers.hpp>
#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

struct types {
	types() : _types() {}
	types(const types& t) : _types(t._types) {}

	// MPL functor
	struct adder {
		adder(types& tc)	: _tc(tc) {}
		template <typename T>
		void operator()(type_wrapper<T>) { _tc.add(typeid(T)); }

		types& _tc;
	};

	void add(util::TypeInfo);
	void clear() { _types.clear(); }

	friend types& operator+=(types&, const types&);
	friend types& operator*=(types&, unsigned int);

	typedef std::list<util::TypeInfo> types_t;
	types_t _types;

};

types operator+(const types&, const types&);
types operator*(const types&, unsigned int);

template <typename T>
types type() { return types(typeid(T)); }

void check_stacks(const Env&, const types&);

} // namespace detail
} // namespace vpush

#endif // __VPUSH_TYPES_HPP__

