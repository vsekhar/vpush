#include <cmath>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>

#include <vpush/library.hpp>

namespace vpush {
namespace library {
namespace movement {

typedef std::vector<double> direction;

// It costs 'n' units of energy to move 1 unit of distance in space
// occupied by 'n' proteins

double move(Protein& p) {
	double desired_magnitude = pop<double>(p);
	double density = soup.size();
	double actual_magnitude
		= std::max(desired_magnitude*density, p.energy) / density;

	p.x += p.facing_x * actual_magnitude;
	p.y += p.facing_y * actual_magnitude;
	p.z += p.facing_z * actual_magnitude;
	return actual_magnitude * density;
}

double turn(Protein &p) {
	using boost::assign;
	
	// get and reverse
	std::vector<double> v = pop<double>(p), pop<double>(p), pop<double>(p);
	v.reverse();

	// normalize
	double mag = 0;
	BOOST_FOREACH(const double& d, v) mag += d;
	mag = std::sqrt(mag);
	BOOST_FOREACH(double& d, v) d /= mag;
	
	// turn
	p.facing_x = v[0];
	p.facing_y = v[1];
	p.facing_z = v[2];
	return 1;
}

void initialize() {
	functions.add("MOVE", move, type<double>() * 3);
	functions.add("TURN", turn, type<double>() * 3);
}

}}} // namespace vpush::library::movement
