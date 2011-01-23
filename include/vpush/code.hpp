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

#include <boost/fusion/include/list.hpp>
#include <boost/fusion/include/transform.hpp>
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

template <typename T>
struct wrap {	typedef type_wrapper<T> type; };

struct fill {
	template <typename SIG> struct result;

	template <typename T>
	struct result<fill(const type_wrapper<T>&)> {
		typedef T type;
	};

	// Reference type needed for bf::transform_view
	template <typename T>
	struct result<fill(type_wrapper<T>&)> {
		typedef T type;
	};
	
	// Non-reference type needed for bf::vector
	template <typename T>
	struct result<fill(type_wrapper<T>)> {
		typedef T type;
	};

	template <typename T> T
	operator()(const type_wrapper<T>&) const {return stack<T>().pop();}

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

	virtual void exec() const {
		check();	// throws stack_underflow
		
		typedef typename mpl::transform<parameter_types, wrap<mpl::_1>,
			mpl::back_inserter<mpl::vector<> > >::type mpl_wrapped_params;
		typedef typename bf::result_of::as_vector<mpl_wrapped_params>::type wrapped_params;
		wrapped_params wp;
		typedef typename bf::result_of::transform<const wrapped_params, fill>::type params_view;
		typedef typename bf::result_of::as_vector<const params_view>::type params_t;
		
		// Something weird is going on here because transform_view stores references
		// Possibly have a look at http://www.martinecker.com/wiki/index.php?title=Don't_Let_'Em_Fool_You._Or:_Getting_Stored_Types_when_using_Boost.Fusion's_Transform_Algorithm
		// Or post a minimal problem to the boost list to see how to do it.
		
		//params_t params = bf::as_vector(bf::transform(wp, fill()));
		FPTR function = reinterpret_cast<FPTR>(_fptr);
		std::cout << "About to invoke function with " << params_t::size::value << "vars" << std::endl;
		result_type r = bf::invoke(function, bf::as_vector(bf::transform(wp, fill())));
		std::cout << "About to push result: " << r << std::endl;
		stack<result_type>().push(r);
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

typedef boost::reference_wrapper<code_base> code;

// TODO: Function *template* wrapper, with associated type lists for instantiation

} // namespace detail
} // namespace vpush

#endif // __VPUSH_CODE_HPP__

