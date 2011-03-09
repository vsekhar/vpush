#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include <vpush/detail/stack.hpp>
#include <vpush/exception.hpp>

namespace vpush {

namespace mpl = ::boost::mpl;

template <typename> struct wrap {};

struct Env {

	void reset() {
		bool_stack.clear();
		int_stack.clear();
		double_stack.clear();
		// code_stack.clear();
		// exec_stack.clear();
		// name_stack.clear();
	}

	// stacks
	detail::stack<bool> bool_stack;
	detail::stack<int> int_stack;
	detail::stack<double> double_stack;
	// detail::stack<Code> code_stack;
	// detail::stack<Exec> exec_stack;
	// detail::stack<Name> name_stack;
private:
	friend class ::boost::serialization::access;
	template <class A> void serialize(A& a, unsigned int) {
		a & bool_stack;
		a & int_stack;
		a & double_stack;
		// ar & code_stack;
		// ar & exec_stack;
		// ar & name_stack;
	}

}; // struct Env

// stack access
template <typename T> inline detail::stack<T>& get_stack(Env&) {
	throw detail::no_such_stack(typeid(T));
}

} // namespace vpush

#endif

