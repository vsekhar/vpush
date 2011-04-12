#ifndef __VPUSH_DETAIL_TYPECHECKER_HPP__
#define __VPUSH_DETAIL_TYPECHECKER_HPP__

#include <vpush/detail/types.hpp>
#include <vpush/protein_fwd.hpp>
#include <vpush/detail/stack.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/codelist.hpp>
#include <vpush/util/typeinfo.hpp>

namespace vpush {
namespace detail {

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

template <>
struct type_checker<Code> : type_checker_base {
	type_checker() : type_checker_base() {}
	type_checker(std::size_t s) : type_checker_base(s) {}
	type_checker(const type_checker& t) : type_checker_base(t) {}
	virtual ~type_checker() {}
	virtual bool check(Protein &p) const {
		return has_n_items<Code>(::vpush::stack<Code>(p), count);
	}
	virtual type_checker_base* do_clone() const { return new type_checker<Code>(*this); }
};

template <>
struct type_checker<Exec> : type_checker_base {
	type_checker() : type_checker_base() {}
	type_checker(std::size_t s) : type_checker_base(s) {}
	type_checker(const type_checker& t) : type_checker_base(t) {}
	virtual ~type_checker() {}
	virtual bool check(Protein &p) const {
		return has_n_items<Exec>(::vpush::stack<Exec>(p), count);
	}
	virtual type_checker_base* do_clone() const { return new type_checker<Exec>(*this); }
};

} // namespace detail

template <typename T>
detail::type_container type() {
	detail::type_container ret;
	util::TypeInfo ti(typeid(T));
	ret._types.insert(ti, new detail::type_checker<T>(1));
	return ret;
}

} // namespace vpush

#endif

