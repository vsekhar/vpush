#include <iostream>

#include <boost/foreach.hpp>

#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/protein_fwd.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

using detail::Exec;
using detail::Code;

double run_protein(Protein& p, bool trace) {
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
	
	//fitness testing?

	return 0; // fitness
}

} // namespace vpush
