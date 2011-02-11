#ifndef __VPUSH_DETAIL_STACK_HPP__
#define __VPUSH_DETAIL_STACK_HPP__

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

struct stack_base {
	virtual std::size_t size() const = 0;
	virtual bool empty() const = 0;
	virtual util::TypeInfo get_type() const = 0;
	virtual stack_base* clone() const = 0;
	virtual ~stack_base() {}
private:
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE&, const unsigned int) {}
};

// for ptr_containers
stack_base* new_clone(const stack_base&);
void delete_clone(const stack_base*);

template <typename T>
struct stack : stack_base, std::vector<T> {
	typedef std::vector<T> base_t;
	virtual std::size_t size() const { return base_t::size(); }
	virtual bool empty() const { return base_t::empty(); }
	virtual util::TypeInfo get_type() const { return typeid(T); }
	virtual stack_base* clone() const { return new stack<T>(*this); }
private:
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {
		ar & ::boost::serialization::base_object<stack_base>(*this);
		ar & ::boost::serialization::base_object<std::vector<T> >(*this);
	}
};

} // namespace detail
} // namespace vpush

#endif // __VPUSH_STACK_HPP__

