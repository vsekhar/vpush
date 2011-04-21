#include <vpush/protein_fwd.hpp>

namespace vpush {

struct ProteinRunner {
	ProteinRunner() : trace(false), result(0) {}
	ProteinRunner(bool t) : trace(t), result(0) {}
	void operator()(Protein&);
	bool trace;
	double result;
};

// for python testing
double run_protein(Protein&, bool trace = false);

} // namespace vpush

