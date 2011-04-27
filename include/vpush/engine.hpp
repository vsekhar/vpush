#include <vpush/protein_fwd.hpp>

namespace vpush {

void engine(Protein&, double&, bool trace = false);

// for python testing
double run_protein(Protein&, bool trace = false);

} // namespace vpush

