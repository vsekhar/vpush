#include <vpush/library.hpp>

namespace vpush {
namespace library {
namespace interaction {

// TODO: How to store neighbors once found? for multiple interactions?
// 		- global 'neighbor' variable, similar to gestator? (might need to start a state...)
//		- 'proteinref' stack... but how to handle invalidated pointers?
//			- every protein has a GUID? and multi_index on guid?


// TODO: Need a state... with gestator... or a simplifying choice...

double random_protein(Protein &p) {
	// find another protein? label it? hold it? GUID it?
	return 1;
}

double read(Protein &p) {
	// examine the other protein...?
	return 1;
}

double other_energy(Protein &p) {
	// push<double>(p, other_protein_energy);
	return 1;
}

double consume() {
	// enter combat, probabilistically winning or losing based on energy levels
	// loser ends up with 0 energy
	// winner ends up with some fraction of loser's energy (less combat costs)
	return 1;
}

void initialize() {
}

}}} // namespace vpush::library::interaction

