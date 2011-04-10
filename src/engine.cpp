#include <iostream>

#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

using detail::Exec;
using detail::Code;

double run_protein(Protein& p, bool trace) {
	using detail::Exec;

	while(!empty<Exec>(p) && p.energy > 0) {
		if(trace) {
			using std::cout;
			using std::endl;
			cout << "Exec: " << stack<Exec>(p) << endl;
			cout << "Code: " << stack<Code>(p) << endl;
			cout << size<bool>(p) << " bool: " << stack<bool>(p) << endl;
			cout << size<int>(p) << " ints: " << stack<int>(p) << endl;
			cout << size<double>(p) << " double: " << stack<double>(p) << endl;
			cout << "Next op-code: " << top<Exec>(p) << endl;
			cout << "----" << endl;
		}
	
		Exec e = pop<Exec>(p);
		
		switch(e.type) {
			case Exec::OPEN:	detail::unwind(stack<Exec>(p));
								break;
			case Exec::CLOSE:	throw detail::unmatched_brackets();
								break;
			default:			const detail::type_container& types
									= vpush::functions.get_types(e.fptr);
								if(types.check(p))
									p.energy -= e.fptr(p);
								break;
		}
	}
	
	//fitness testing?

	return 0; // fitness
}

} // namespace vpush
