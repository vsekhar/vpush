#ifndef __VPUSH_TYPES_HPP__
#define __VPUSH_TYPES_HPP__

#include <list>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/identity.hpp>

#include <vpush/env_fwd.hpp>
#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

struct type_container {

	// For MPL::for_each
	template <typename T>
	struct wrap {};
	
	template <typename C>
	struct adder {
		adder(C& c) : _c(c) {}

		template <typename T>
		void operator()(wrap<T>) { _c.push_back(typeid(T)); }

		C& _c;
	};

	type_container() : _types() {}
	template <typename LIST>
	type_container(const LIST&) {
		namespace mpl = ::boost::mpl;
		mpl::for_each<LIST, wrap<mpl::_1> >(adder<types_t>(_types));
	}
	type_container(const type_container& t) : _types(t._types) {}


	void add(util::TypeInfo);
	void clear() { _types.clear(); }

	type_container& operator+=(const type_container&);
	type_container& operator*=(unsigned int);

	typedef std::list<util::TypeInfo> types_t;
	types_t _types;

};

type_container operator+(const type_container&, const type_container&);
type_container operator*(const type_container&, unsigned int);


} // namespace detail

template <typename T>
detail::type_container type() {
	detail::type_container t;
	t.add(typeid(T));
	return t;
}

} // namespace vpush

#endif // __VPUSH_TYPES_HPP__

