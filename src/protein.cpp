#include <vpush/protein.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/stackops.hpp>

namespace vpush {

Protein random_protein(std::size_t s) {
	Protein ret;
	for(std::size_t i = 0; i < s; ++i)
		push<detail::Exec>(ret, functions.get_random());
	return ret;
}

void print_trace(Protein& p, std::ostream& o) {
	using std::endl;
	using vpush::detail::Exec;
	using vpush::detail::Code;
	
	o << "Exec: " << stack<Exec>(p) << endl;
	o << "Code: " << stack<Code>(p) << endl;
	o << size<bool>(p) << " bool: " << stack<bool>(p) << endl;
	o << size<int>(p) << " ints: " << stack<int>(p) << endl;
	o << size<double>(p) << " double: " << stack<double>(p) << endl;
	o << "Next op-code: " << top<Exec>(p);
	o << endl;
	o << "----" << endl;
}

} // namespace vpush

