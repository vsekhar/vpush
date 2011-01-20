#ifndef __VPUSH_CODE_HPP__
#define __VPUSH_CODE_HPP__

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/fusion/functional.hpp>

#include <boost/fusion/container/list.hpp>
#include <boost/fusion/include/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>

// These includes are all causing errors, Boost bug 5097
// https://svn.boost.org/trac/boost/ticket/5097
//#include <boost/fusion/algorithm/transformation/transform.hpp>
//#include <boost/fusion/include/transform.hpp>
//#include <boost/fusion/adapted/mpl.hpp>
//#include <boost/fusion/include/mpl.hpp>

#include <vpush/util/typeinfo.hpp>
#include <vpush/detail/typechecker.hpp>
#include <vpush/stack.hpp>

namespace vpush {
namespace detail {

namespace ft = ::boost::function_types;
namespace bf = ::boost::fusion;
namespace mpl = ::boost::mpl;

typedef void(*void_fptr_t)();

struct code_base {
	code_base(void_fptr_t f, const type_checker& tc) : _fptr(f), _type_checker(tc) {}
	virtual inline void exec() const { check(); _fptr(); }
	inline void check() const { _type_checker.check(); }
private:
	code_base(void_fptr_t f) : _fptr(f), _type_checker() {}
	void_fptr_t _fptr;
	type_checker _type_checker;
};

template <typename T> struct type_wrapper {type_wrapper() {}};

template <typename T>
struct wrap_types {	typedef type_wrapper<T> type; };

struct get_params {
	template <typename SIG> struct result;
	template <typename THIS, typename T>
	struct result<THIS(type_wrapper<T>)> {
		typedef T type;
	};
	template <typename T> typename result<get_params(type_wrapper<T>)>::type
	operator()(type_wrapper<T>) const {return stack<T>().pop();}
};

template <typename FPTR>
struct basic_code : code_base {
	typedef typename ft::result_type<FPTR>::type return_type;
	typedef typename ft::parameter_types<FPTR>::type parameter_types;
	basic_code(FPTR f) : code_base(reinterpret_cast<void_fptr_t>(f)) {
		boost::mpl::for_each<parameter_types>(add_typechecker_type(_type_checker));
	}
	virtual void exec() const {
		check();	// throws stack_underflow
		
		// TODO: This section awaiting a Boost Fusion bug fix #5097
		// https://svn.boost.org/trac/boost/ticket/5097
		// In the meantime, build stack functions only
		
		//typedef typename mpl::transform<parameter_types, wrap_types<mpl::_1>,
		//	mpl::back_inserter<mpl::vector<> > >::type mpl_wrapped_params;
		//typedef typename bf::result_of::as_vector<mpl_wrapped_params>::type wrapped_params;
		//wrapped_params wp;
		//typename typename bf::result_of::transform<wrapped_params, get_params>::type params;
		//typedef bf::list<> empty_list;
		//params p = bf::transform(wp, empty_list, get_params());
		//FPTR function = reinterpret_cast<FPTR>(_fptr);
		//bf::invoke(function, p);
	}
};


struct code {
	code(code_base* p) : _code_ptr(p) {}
	code(const code_base& c) : _code_ptr(&c) {}
	code(const code& c) : _code_ptr(c._code_ptr) {}
	operator const code_base&() const { return *_code_ptr; }
	inline void exec() const { _code_ptr->exec(); }
private:
	code();
	const code_base * _code_ptr;
};

// TODO: Function *template* wrapper, with associated type lists for instantiation

} // namespace detail
} // namespace vpush

#endif // __VPUSH_CODE_HPP__

