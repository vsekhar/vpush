#include <iostream>

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

	Exec last = Exec(Exec::OPEN);
	while(!empty<Exec>(p) && p.energy > 0) {
		if(trace) 
			print_trace(p);

		Exec e = pop<Exec>(p);
		
		switch(e.type) {
			case Exec::OPEN:	detail::unwind(stack<Exec>(p));
								break;
			case Exec::CLOSE:	throw detail::unmatched_brackets();
								break;
			case Exec::CODE:	if(functions.get_types(e.fptr).check(p))
								try {
									p.energy -= e.fptr(p);
								}
								catch(std::exception&) {
									using std::cerr;
									using std::endl;
									cerr << "ERROR: Exception occured (running " << functions.get_name(e.fptr) << ")" << endl;
									cerr << "(last op-code: " << functions.get_name(last.fptr) << ")" << endl;
									print_trace(p, cerr);
									throw;
								}
								break;
		}
		last = e;
	}
	
	//fitness testing?

	return 0; // fitness
}

} // namespace vpush
