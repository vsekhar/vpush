#ifndef __VPUSH_CODE_HPP__
#define __VPUSH_CODE_HPP__

#include <iostream>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/ref.hpp>

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>

#include <vpush/code_fwd.hpp>
#include <vpush/codes.hpp>
#include <vpush/util/typeinfo.hpp>
#include <vpush/detail/typechecker.hpp>
#include <vpush/stack.hpp>

namespace vpush {
namespace detail {

namespace ft = ::boost::function_types;
namespace bf = ::boost::fusion;
namespace mpl = ::boost::mpl;

enum element_type {CODE, LITERAL, LIST};

struct element_base : boost::noncopyable {
	element_base(element_type t) : _type(t) {}
	element_type type() const { return _type; }
	virtual inline void exec() const == 0;
private:
	element_type _type;
};

struct code_base : element_base {
	code_base(void_fptr_t f, const type_checker& tc) : element_base(CODE), _fptr(f), _type_checker(tc) {}
	virtual inline void exec() const { check(); _fptr(); }
	inline void check() const { _type_checker.check(); }
	inline void_fptr_t get_fptr() const { return _fptr; }
protected:
	code_base(void_fptr_t f) : element_base(CODE), _fptr(f), _type_checker() {}
	void_fptr_t _fptr;
	type_checker _type_checker;
};

template <typename FPTR>
struct basic_code : public code_base {
	typedef typename ft::result_type<FPTR>::type result_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;

	basic_code(FPTR f) : code_base(reinterpret_cast<void_fptr_t>(f)) {
		typedef typename mpl::transform<parameter_types, type_wrapper<mpl::_1>,
			mpl::back_inserter<mpl::vector<> > >::type wrapped_types;
		boost::mpl::for_each<wrapped_types>(type_checker::adder(_type_checker));
	}
	
	//Function object for popping the stack
	template <typename T>
	struct popper {
		operator T() {return pop<T>();}
	};

	virtual void exec() const {
		check();	// throws stack_underflow
		
		typedef typename mpl::transform<parameter_types, popper<mpl::_1>,
			mpl::back_inserter<mpl::vector<> > >::type poppers_t;
		FPTR function = reinterpret_cast<FPTR>(_fptr);
		push<result_type>(bf::invoke(function, poppers_t()));
	}

protected:
	basic_code(FPTR f, const type_checker& tc) : code_base(reinterpret_cast<void_fptr_t>(f), tc) {
		boost::mpl::for_each<parameter_types>(type_checker::adder(_type_checker));
	}
};

} // namespace detail

struct code {
	typedef boost::reference_wrapper<const detail::code_base> code_ref_t;

	code(const detail::code_base& c) : _code(c) {}
	code(const code& c) : _code(c._code) {}
	code(const code_ref_t& c) : _code(c.get()) {}
	void exec() const { _code.get().exec(); }
	detail::void_fptr_t get_fptr() const { return _code.get().get_fptr(); }
private:
	code_ref_t _code;
};

} // namespace vpush

#endif // __VPUSH_CODE_HPP__

