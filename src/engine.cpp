#ifdef _DEBUG
#include <boost/assert.hpp>
#endif

#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/gestation.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

void ProteinRunner::operator()(Protein& p) {
	using detail::Exec;
	double init_energy = p.energy;

	while(!empty<Exec>(p) && p.energy > 0) {
		if(trace) 
			print_trace(p);

		if(top<Exec>(p).type == Exec::OPEN)
			detail::unwind(stack<Exec>(p));
		else {
			Exec e = pop<Exec>(p);
			if(functions.get_types(e.fptr).check(p)) {
				double cost = e.fptr(p);
				p.energy -= cost;
#ifdef _DEBUG
				BOOST_ASSERT(cost >= 0);
#endif
			}
		}
	}
	
	detach_gestator();
	
	//fitness testing and energy rewards??
	
	// Energy consumed
	result += init_energy - p.energy;
}

// for python testing
double run_protein(Protein &p, bool trace) {
	ProteinRunner runner = ProteinRunner();
	runner(p);
	return runner.result;
}

} // namespace vpush
