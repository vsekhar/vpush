/***********************************************************************
vpush::detail::literal<> is a template for instantiating literals

Instantiate a literal with a state type and an initial state. For simple
types, use:
	literal<int>(4);

For more complex types, define a struct that stores the state, provides
init and mutate methods, and is convertible to the type to be pushed onto
stacks:

	struct my_int_state_type {
		void init() { // do something }
		void mutate() { // do something }
		operator int() { // convert to type to be put on stacks }
		typedef int result_type;	// tells vpush how to convert it
	private:
		int _internal_state_storage;
	};

Then tell vpush both the state structure and the type it should be
converted to when pushing onto the stack:

	literal<my_int_state_type>
***********************************************************************/

#ifndef __VPUSH_LITERAL_HPP__
#define __VPUSH_LITERAL_HPP__

#include <vpush/code.hpp>
#include <vpush/stack.hpp>

namespace vpush {
namespace detail {

template <typename STATE, typename RET>
void literal_func(const STATE& s) { push<RET>(s); }

template <typename STATE, unsigned int has_mutate>
struct conditional_mutate {
	static void mutate(STATE& s) {s.mutate();}
};

template <typename STATE>
struct conditional_mutate<STATE, 0> {
	static void mutate(STATE&) {}
};

template <typename STATE>
struct literal : element_base {
	// TODO: if STATE is a class, get its result_type, else result_type=STATE
	// typedef mpl::if_<mpl::is_class<STATE>, STATE::result_type, STATE>::type result_type;
	literal(const STATE& s) :
		code_base(NULL, type_checker()),
		_state(s)
	{}
	
	void init() {
		// TODO: conditional compilation to call STATE.mutate() if it exists,
		//		 no-op if not
		
		//_state.init();
	}
	void mutate() {
		// TODO: conditional compilation to call STATE.mutate() if it exists,
		//		 no-op if not
		//_state.mutate();
	}
	
	virtual void exec() const {
		literal_func<STATE, result_type>(_state);
	}
	
	// TODO: Add serialization handling (STATE must be serializable)

private:
	STATE _state;
};

} // namespace detail
} // namespace vpush

#endif

