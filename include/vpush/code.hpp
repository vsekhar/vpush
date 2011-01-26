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

typedef void(*void_fptr_t)();

struct code_base : boost::noncopyable {
	code_base(void_fptr_t f, const type_checker& tc) : _fptr(f), _type_checker(tc) {}
	virtual inline void exec() const { check(); _fptr(); }
	inline void check() const { _type_checker.check(); }
protected:
	code_base(void_fptr_t f) : _fptr(f), _type_checker() {}
	void_fptr_t _fptr;
	type_checker _type_checker;
	std::string _name;
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

template <typename FPTR>
struct stack_code : public basic_code<FPTR> {
	stack_code(FPTR f, const type_checker& tc) : basic_code<FPTR>(f, tc) {}
	virtual void exec() const {
		basic_code<FPTR>::check();
		FPTR func = reinterpret_cast<FPTR>(basic_code<FPTR>::_fptr);
		func();
	}
};

} // namespace detail

typedef boost::reference_wrapper<detail::code_base> code;

} // namespace vpush

#endif // __VPUSH_CODE_HPP__

