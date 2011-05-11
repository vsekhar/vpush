#include <vpush/protein_fwd.hpp>

namespace vpush {

void engine(Protein& p, double&, double max_energy, bool trace = false);

// for python testing
double run_protein(Protein& p, double max_energy, bool trace = false);

} // namespace vpush

