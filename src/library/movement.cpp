#include <iostream>
#include <cmath>

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

#include <vpush/library.hpp>
#include <vpush/soup.hpp>
#include <vpush/util/vector.hpp>

#define MAX_MOVEMENT_CONSUMPTION (0.1)

namespace vpush {
namespace library {
namespace movement {

typedef std::vector<double> direction;

// It costs 'n' units of energy to move 1 unit of distance in space
// occupied by 'n' proteins

double move(Protein& p) {
	// This is a very costly operation since the doubles are often
	// ones or twos (which imply a movement clear across the toroidal space).
	// This high cost increases with larger populations...
	// TODO: Need a way to moderate this cost

	double density = vpush::soup.size();
	if(density == 0)
		return 0;

	double magnitude = pop<double>(p);
	const double max_energy = MAX_MOVEMENT_CONSUMPTION * p.energy / density;
#ifdef _DEBUG
	BOOST_ASSERT(max_energy > 0);
#endif
	if(std::abs(magnitude) > max_energy) {
		if(magnitude < 0)
			magnitude = -max_energy;
		else
			magnitude = max_energy;
	}

	util::vector v = p.facing * magnitude;
	p.x += v[0];
	p.y += v[1];
	p.z += v[2];
	return std::abs(magnitude * density);
}

double turn(Protein &p) {
	// get in reverse
	util::vector v;
	v[2] = pop<double>(p);
	v[1] = pop<double>(p);
	v[0] = pop<double>(p);
	p.facing = util::normalized(v);
	return 1;
}

void initialize() {
	functions.add("MOVE", move, type<double>());
	functions.add("TURN", turn, type<double>() * 3);
}

}}} // namespace vpush::library::movement
