#include <vpush/library.hpp>

namespace vpush {
namespace library {
namespace interaction {

// TODO: How to store neighbors once found? for multiple interactions?
// 		- global 'neighbor' variable, similar to gestator? (might need to start a state...)
//		- 'proteinref' stack... but how to handle invalidated pointers?
//			- every protein has a GUID? and multi_index on guid?

double get_nearest_neighbor() {
	// build sorted list of distances
	// use normalized distances as probability distribution to choose a neighbor
	// (closest neighbor has exponentially highest probability, tunable using
	// a parameter)
}

double clear_neighbor() {
}

double consume() {
}

void initialize() {
}

}}} // namespace vpush::library::interaction

