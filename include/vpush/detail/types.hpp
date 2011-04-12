#ifndef __VPUSH_DETAIL_TYPES_HPP__
#define __VPUSH_DETAIL_TYPES_HPP__

#include <list>
#include <map>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <vpush/util/typeinfo.hpp>
#include <vpush/protein_fwd.hpp>

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

inline type_checker_base* new_clone(const type_checker_base& t) {
	return t.clone();
}

struct type_container {

	type_container() : _types() {}
	type_container(const type_container& t) : _types(t._types) {}

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
} // namespace vpush

#endif // __VPUSH_TYPES_HPP__

