#ifndef __VPUSH_TYPECHECKER_HPP__
#define __VPUSH_TYPECHECKER_HPP__

#include <list>
#include <map>

#include <boost/foreach.hpp>

#include <vpush/util/typeinfo.hpp>
#include <vpush/stack.hpp>		// need both stack.hpp and stacks.hpp
#include <vpush/stacks.hpp>
#include <vpush/exception.hpp>

namespace vpush {
namespace detail {

struct type_checker {
	type_checker() : _types() {}
	type_checker(util::TypeInfo);
	type_checker(const type_checker& t) : _types(t._types) {}

	template <typename T>
	void add() { _types.push_back(typeid(T)); }
	void add(util::TypeInfo);
	inline void check() const {
		std::map<util::TypeInfo, std::size_t> type_count;
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
	std::list<util::TypeInfo> _types;

};

type_checker operator+(const type_checker&, const type_checker&);
type_checker operator*(const type_checker&, unsigned int);

template <typename T>
type_checker type() { return type_checker(typeid(T)); }

// MPL functors

struct add_typechecker_type {
	add_typechecker_type(type_checker& t) : _type_checker(t) {}
	template <typename T>
	inline void operator()(T) { _type_checker.add(typeid(T)); }
	type_checker& _type_checker;
};

} // namespace detail
} // namespace vpush

#endif // __VPUSH_TYPECHECKER_HPP__

