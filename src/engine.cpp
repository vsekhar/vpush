#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {

using detail::Exec;
using detail::Code;

double run_protein(Protein& p) {
	using detail::Exec;

	while(!empty<Exec>(p) && p.energy > 0) {
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
