#ifndef __VPUSH_PROTEIN_HPP__
#define __VPUSH_PROTEIN_HPP__

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ref.hpp>

#include <vpush/protein_fwd.hpp>
#include <vpush/detail/stack.hpp>
#include <vpush/detail/toroidal.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/exception.hpp>

namespace vpush {

struct CodeProtein {
	void reset() {
		code_stack.clear();
		exec_stack.clear();
	}
	
	detail::stack<detail::Code> code_stack;
	detail::stack<detail::Exec> exec_stack;

private:
	friend class ::boost::serialization::access;
	template <class A> void serialize(A& a, unsigned int) {
		// TODO: do conversions using vpush::functions;
		// TODO: separate load/save
		// a & code_stack;
		// a & exec_stack;
	}
};

struct Protein : CodeProtein {
	typedef CodeProtein base_type;
	void reset() {
		base_type::reset();
		bool_stack.clear();
		int_stack.clear();
		double_stack.clear();
		unbound_name_stack.clear();
	}

	// data stacks
	detail::stack<bool> bool_stack;
	detail::stack<int> int_stack;
	detail::stack<double> double_stack;
	detail::stack<std::string> unbound_name_stack;
	
	// co-ordinates
	detail::toroidal_vector location;

private:
	friend class ::boost::serialization::access;
	template <class A> void serialize(A& a, unsigned int) {
		a & ::boost::serialization::base_object<base_type>(*this);
		a & bool_stack;
		a & int_stack;
		a & double_stack;
		a & unbound_name_stack;
	}

}; // struct Protein

// stack access
template <typename T> inline detail::stack<T>& get_stack(Protein&) {
	throw detail::no_such_stack(typeid(T));
}

} // namespace vpush

#endif

