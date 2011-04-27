#include <iostream>
#ifdef _DEBUG
#include <boost/assert.hpp>
#endif

#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/gestation.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

void engine(Protein& p, double& total_cost, bool trace) {
	using detail::Exec;
	p.running = true;
	while(!empty<Exec>(p) && p.energy > 0 && p.running) {
		if(top<Exec>(p).type == Exec::OPEN)
			detail::unwind(stack<Exec>(p));
		else if(top<Exec>(p).type == Exec::CLOSE)
			throw detail::unmatched_brackets();
		else {
			Exec e = pop<Exec>(p);
			if(!e.fptr)
				throw std::runtime_error("Null fptr");
			if(functions.get_types(e.fptr).check(p)) {
				try {
					double cost = e.fptr(p);
#ifdef _DEBUG
					BOOST_ASSERT(cost >= 0);
#endif
					p.energy -= cost;
					total_cost += cost;
					if(cost > 0 && trace)
						std::cout << "Energy after " << e << ": " << p.energy << std::endl;
				}
				catch(const detail::stack_underflow &exc) {
					// Append op-code
					throw detail::stack_underflow(exc, std::string(" ")+functions.get_name(e));
				}
			}
		}
	}
	detach_gestator();
	//fitness testing and energy rewards??
}

// for python testing
double run_protein(Protein &p, bool trace) {
	double ret = 0;
	engine(p, ret, trace);
	return ret;
}

} // namespace vpush
