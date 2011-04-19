#include <vpush/protein_fwd.hpp>

namespace vpush {

struct ProteinRunner {
	ProteinRunner(bool t = false) : trace(t) {}
	static void operator()(Protein&) const;
	bool trace;
};

// for python testing
double run_protein(Protein&, bool trace = false);

} // namespace vpush

