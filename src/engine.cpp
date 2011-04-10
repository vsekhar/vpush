#include <vector>
#include <algorithm>
#include <iterator>

#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {

void unwind(Protein& p) {
	using detail::Exec;
	
	std::vector<Exec> temp_vec;
	unsigned int bracket_level = 0;
	
	while(!empty<Exec>(p)) {
		Exec e = pop<Exec>(p);
		if(e.type == Exec::OPEN)
			++bracket_level;
		else if(e.type == Exec::CLOSE) {
			if(bracket_level)
				--bracket_level;
			else
				break; // done (eat the CLOSE op-code)
		}
		temp_vec.push_back(e);
	}
	
	if(bracket_level)
		throw detail::unmatched_brackets();
	
	std::reverse_copy(temp_vec.begin(), temp_vec.end(), std::back_inserter(stack<Exec>(p)));
}

double run_protein(Protein& p) {
	using detail::Exec;

	while(!empty<Exec>(p) && p.energy > 0) {
		Exec e = pop<Exec>(p);
		
		switch(e.type) {
			case Exec::OPEN:	unwind(p);
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
