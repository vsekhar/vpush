#include <iostream>
#include <boost/assert.hpp>

#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/gestation.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

inline double step(Protein& p, bool trace) {
	using detail::Exec;
	if(top<Exec>(p).type == Exec::OPEN) {
		detail::unwind(stack<Exec>(p));
		return 0;
	}
	else if(top<Exec>(p).type == Exec::CLOSE)
		throw detail::unmatched_brackets();
	else {
		Exec e = pop<Exec>(p);
		if(functions.get_types(e.fptr).check(p)) {
			try {
				double cost = e.fptr(p);
				BOOST_ASSERT_MSG(cost >= 0, ("Function returned negative cost: " + functions.get_name(e)).c_str());
				p.energy -= cost;
				if(cost > 0 && trace)
					std::cout << "Energy after " << e << ": " << p.energy << std::endl;
				return cost;
			}
			catch(const detail::stack_underflow &exc) {
				// Append op-code
				throw detail::stack_underflow(exc, std::string(" ")+functions.get_name(e));
			}
		}
		return 0;
	}
}

// NB: have to use double& to output cost because multi_index_container.modify()
// takes functor by value...
void engine(Protein& p, double& total_cost, double max_energy, bool trace) {
	using detail::Exec;
	p.running = true;
	double starting_cost = total_cost;
	while(!empty<Exec>(p)
			&& p.energy >= 1
			&& p.running
			&& (!max_energy || total_cost-starting_cost < max_energy))
		total_cost += step(p, trace);
	//fitness testing and energy rewards??
}

// for python testing
double run_protein(Protein &p, double max_energy, bool trace) {
	double ret = 0;
	engine(p, ret, max_energy, trace);
	return ret;
}

} // namespace vpush
