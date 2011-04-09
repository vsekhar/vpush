#ifndef __VPUSH_TYPES_HPP__
#define __VPUSH_TYPES_HPP__

#include <list>
#include <map>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <vpush/util/typeinfo.hpp>
#include <vpush/detail/stack_fwd.hpp>

namespace vpush {
namespace detail {

struct type_checker_base {
	type_checker_base() : count(0) {}
	type_checker_base(std::size_t s) : count(s) {}
	type_checker_base(const type_checker_base& t) : count(t.count) {}
	virtual ~type_checker_base() {}
	virtual bool check(Protein &) const = 0;
	type_checker_base* clone() const { return do_clone(); }
	virtual type_checker_base* do_clone() const = 0;
	std::size_t count;
};

template <typename T>
struct type_checker : type_checker_base {
	type_checker() : type_checker_base() {}
	type_checker(std::size_t s) : type_checker_base(s) {}
	type_checker(const type_checker& t) : type_checker_base(t) {}
	virtual ~type_checker() {}
	virtual bool check(Protein &p) const {
		return ::vpush::stack<T>(p).size() >= count;
	}
	virtual type_checker_base* do_clone() const { return new type_checker<T>(*this); }
};

inline type_checker_base* new_clone(const type_checker_base& t) {
	return t.clone();
}

struct type_container {

	// For MPL::for_each
	template <typename T>
	struct wrap {};
	
	template <typename C>
	struct adder {
		adder(C& c) : _c(c) {}

		template <typename T>
		void operator()(wrap<T>) {
			if(_c.find(typeid(T)) != _c.end())
				++_c[typeid(T)].count;
			else
				_c.insert(typeid(T), new type_checker<T>());
		}

		C& _c;
	};

	type_container() : _types() {}
	template <typename LIST>
	type_container(const LIST&) {
		namespace mpl = ::boost::mpl;
		mpl::for_each<LIST, wrap<mpl::_1> >(adder<types_t>(_types));
	}
	type_container(const type_container& t) : _types(t._types) {}

	template <typename T>
	void add() {
		if(_types.find(typeid(T)) != _types.end())
			++_types.at(typeid(T)).count;
		else {
			util::TypeInfo t(typeid(T));
			_types.insert(t, new type_checker<T>(1));
		}
	}

	void clear() { _types.clear(); }
	bool check(Protein& p) const;

	type_container& operator+=(const type_container&);
	type_container& operator*=(unsigned int);

	typedef boost::ptr_map<util::TypeInfo, type_checker_base> types_t;
	types_t _types;

};

type_container operator+(const type_container&, const type_container&);
type_container operator*(const type_container&, unsigned int);


} // namespace detail

template <typename T>
detail::type_container type() {
	detail::type_container t;
	t.add<T>();
	return t;
}

} // namespace vpush

#endif // __VPUSH_TYPES_HPP__

