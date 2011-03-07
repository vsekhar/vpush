#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <string>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/for_each.hpp>

#include <vpush/detail/env_impl.hpp>

namespace vpush {

namespace mpl = ::boost::mpl;

template <typename types>
struct Env : public mpl::fold<types, detail::Env_null, detail::Env_cons<mpl::_1, mpl::_2> >::type {
	typedef typename mpl::fold<types, detail::Env_null, detail::Env_cons<mpl::_1, mpl::_2> >::type base_type;
	typedef Env<types> this_type;

	
	typedef unsigned int (*fptr_t)(this_type&);
	void register_(fptr_t fptr, std::string name) {
		return;
	}

	template <typename FPTR>
	void register_(FPTR fptr, std::string name) {
		return;
	}
	
	template <template <class> class ENV>
	void multi_reg() { ENV<this_type>::register_(*this); }

	
private:
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE& ar, const unsigned int) {
		detail::stack_serializer<ARCHIVE, Env<types> > ss(ar, *this);
		boost::mpl::for_each<types, typename detail::wrap<mpl::_1> >(ss);
	}
};

} // namespace vpush

#endif

