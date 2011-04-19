#include <cmath>

#include <boost/foreach.hpp>

#include <vpush/library.hpp>
#include <vpush/soup.hpp>
#include <vpush/util/vector.hpp>

namespace vpush {
namespace library {
namespace movement {

typedef std::vector<double> direction;

// It costs 'n' units of energy to move 1 unit of distance in space
// occupied by 'n' proteins

double move(Protein& p) {
	double desired_magnitude = pop<double>(p);
	double density = vpush::soup.size();
	double actual_magnitude
		= std::max(desired_magnitude*density, p.energy) / density;

	util::vector v = p.facing * actual_magnitude;
	p.x += v[0];
	p.y += v[1];
	p.z += v[2];
	return actual_magnitude * density;
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
	functions.add("MOVE", move, type<double>() * 3);
	functions.add("TURN", turn, type<double>() * 3);
}

}}} // namespace vpush::library::movement
