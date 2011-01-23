#ifndef __VPUSH_TYPECHECKER_HPP__
#define __VPUSH_TYPECHECKER_HPP__

#include <list>
#include <map>

#include <boost/foreach.hpp>

#include <vpush/util/typeinfo.hpp>
#include <vpush/stack.hpp>		// need both stack.hpp and stacks.hpp
#include <vpush/stacks.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/wrappers.hpp>

namespace vpush {
namespace detail {

struct type_checker {
	type_checker() : _types() {}
	type_checker(const type_checker& t) : _types(t._types) {}

	// MPL functor
	struct adder {
		adder(type_checker& tc)	: _tc(tc) {}
		template <typename T>
		void operator()(type_wrapper<T>) { _tc.add(typeid(T)); }

		type_checker& _tc;
	};

	void add(util::TypeInfo);
	inline void check() const {
		std::map<util::TypeInfo, unsigned int> type_count;
		BOOST_FOREACH(util::TypeInfo type, _types)
			++type_count[type];

		typedef std::pair<util::TypeInfo, std::size_t> map_element;
		BOOST_FOREACH(const map_element& e, type_count)
			if(stacks()[e.first]->size() < e.second)
				throw stack_underflow(e.first);
	}
	void clear() { _types.clear(); }

	friend type_checker& operator+=(type_checker&, const type_checker&);
	friend type_checker& operator*=(type_checker&, unsigned int);

private:
	typedef std::list<util::TypeInfo> types_t;
	types_t _types;

};

type_checker operator+(const type_checker&, const type_checker&);
type_checker operator*(const type_checker&, unsigned int);

template <typename T>
type_checker type() { return type_checker(typeid(T)); }

} // namespace detail
} // namespace vpush

#endif // __VPUSH_TYPECHECKER_HPP__

