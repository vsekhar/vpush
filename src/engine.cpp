#include <vpush/engine.hpp>
#include <vpush/stackops.hpp>
#include <vpush/exception.hpp>
#include <vpush/detail/functions.hpp>

namespace vpush {

double run_protein(Protein& p) {
	using detail::Exec;
	
	// unsigned int bracket_level = 0;
	
	while(!empty<Exec>(p)) {
		Exec e = pop<Exec>(p);
		const detail::type_container& types
			= vpush::functions.get_types(e.fptr);
		if(types.check(p))
			e.fptr(p);
	}
	return 0;
}

} // namespace vpush
