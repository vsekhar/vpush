/***********************************************************************
vpush::detail::literal<> is a template for instantiating literals
***********************************************************************/

#ifndef __VPUSH_LITERAL_HPP__
#define __VPUSH_LITERAL_HPP__

#include <vpush/code.hpp>

namespace vpush {
namespace detail {

template <typename RET, typename STATE>
RET get_literal(STATE& s) { return s; }

template <typename STATE, typename RET = STATE>
struct literal : stack_code {
	literal(const STATE& s) :
		stack_code(get_literal<RET,STATE>, type_checker()),
		_state(s)
	{}
	
	void init() {
		// TODO: conditional compilation to call STATE.mutate() if it exists,
		//		 no-op if not
		_state.init();
	}
	void mutate() {
		// TODO: conditional compilation to call STATE.mutate() if it exists,
		//		 no-op if not
		_state.mutate();
	}
	
	// TODO: Add serialization handling (STATE must be serializable)

private:
	STATE _state;
};

}
}


#endif
