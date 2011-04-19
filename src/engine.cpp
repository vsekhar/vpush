#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/gestation.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

static void ProteinRunner::operator()(Protein& p) const {
	using detail::Exec;

	while(!empty<Exec>(p) && p.energy > 0) {
		if(trace) 
			print_trace(p);

		if(top<Exec>(p).type == Exec::OPEN)
			detail::unwind(stack<Exec>(p));
		else {
			Exec e = pop<Exec>(p);
			if(functions.get_types(e.fptr).check(p))
				p.energy -= e.fptr(p);
		}
	}
	
	detach_gestator();
	
	//fitness testing and energy rewards??
}

// for python testing
void run_protein(Protein &p, bool trace) {
	ProteinRunner()(p);
}

} // namespace vpush
