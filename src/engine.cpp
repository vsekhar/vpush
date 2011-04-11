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

inline void print_trace(Protein& p, std::ostream& o = std::cout) {
	using std::endl;
	o << "Exec: " << stack<Exec>(p) << endl;
	o << "Code: " << stack<Code>(p) << endl;
	o << size<bool>(p) << " bool: " << stack<bool>(p) << endl;
	o << size<int>(p) << " ints: " << stack<int>(p) << endl;
	o << size<double>(p) << " double: " << stack<double>(p) << endl;
	o << "Next op-code: " << top<Exec>(p);
	switch(top<Exec>(p).type) {
		case Exec::OPEN:	o << "(OPEN)"; break;
		case Exec::CLOSE:	o << "(CLOSE)"; break;
		case Exec::CODE:	o << "(CODE)"; break;
		default:			throw detail::no_such_function(functions.get_name(top<Exec>(p).fptr));
	}
	o << endl;
	o << "----" << endl;
}

double run_protein(Protein& p, bool trace) {
	using detail::Exec;

	while(!empty<Exec>(p) && p.energy > 0) {
		if(trace) 
			print_trace(p);

		Exec e = pop<Exec>(p);
		
		switch(e.type) {
			case Exec::OPEN:	detail::unwind(stack<Exec>(p));
								break;
			case Exec::CLOSE:	throw detail::unmatched_brackets();
								break;
			default:			if(functions.get_types(e.fptr).check(p)) try {
									p.energy -= e.fptr(p);
								}
								catch(std::exception ex) {
									using std::cerr;
									using std::endl;
									cerr << "ERROR: Exception occured (running " << functions.get_name(e.fptr) << ")" << endl;
									print_trace(p, cerr);
									throw;
								}
								break;
		}
	}
	
	//fitness testing?

	return 0; // fitness
}

} // namespace vpush
