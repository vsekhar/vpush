#include <string>

#include <boost/foreach.hpp>
#include <boost/numeric/conversion/converter.hpp>

#include <vpush/library.hpp>
#include <vpush/gestation.hpp>

namespace vpush {
namespace library {
namespace gestation {

using std::string;

typedef typename ::boost::numeric::converter<std::size_t, double> Double2UInt;

double detach_protein(Protein &p) { return detach_gestator() ? 1 : 0; }

double transfer_energy(Protein &p) {
	static const double transfer_cost = 1.0;
	if(top<double>(p) < 0 || p.energy < top<double>(p))
		return 0;

	ensure_gestator(p);
	double amount = pop<double>(p); // get an amount
	if(amount > 0)
		// can't transfer more than we have (less transfer_cost)
		amount = std::min(amount, p.energy-transfer_cost);
	else if(amount < 0)
		// can't leave gestatee with negative energy
		amount = std::max(amount, -gestator->energy);
	gestator->energy += amount;
	p.energy -= amount;
	return transfer_cost;
}

template <typename T>
double move_data(Protein &p) {
	static const double move_cost = 1.0;

	if(top<int>(p) < 0)
		return 0;
	
	ensure_gestator(p);
	std::list<T> buffer;
	std::size_t count = std::abs(pop<int>(p));
	count = std::min(count, size<T>(p)); // can't move more than we have
	std::size_t max_count = Double2UInt::convert(p.energy/move_cost);
	count = std::min(count, max_count); // can't move more than we have energy for
	for(std::size_t i = 0; i < count; ++i)
		buffer.push_front(pop<T>(p));
	BOOST_FOREACH(const T& t, buffer)
		push<T>(*gestator, t);
	return count * move_cost;
}

template <typename T>
double move_code(Protein &p) {
	static const double move_cost = 1.0;
	if(top<int>(p) < 0)
		return 0;

	ensure_gestator(p);
	std::list<item<T> > items;
	std::size_t count = std::abs(pop<int>(p));
	std::size_t max_count = Double2UInt::convert(p.energy/move_cost);
	count = std::min(count, max_count);
	std::size_t actual_count = 0;
	try {
		for(std::size_t i = 0; i < count; ++i) {
			items.push_front(get_item<T>(stack<T>(p)));
			++actual_count;
		}
	}
	catch(const detail::stack_underflow&) {}
	BOOST_FOREACH(const item<T>& i, items)
		put_item(i, stack<T>(*gestator));
	return actual_count * move_cost;
}

void initialize() {
	functions.add("DETACH_PROTEIN", detach_protein);
	functions.add("TRANSFER_ENERGY", transfer_energy, type<double>());
	functions.add("MOVE.BOOL", move_data<bool>, type<int>());
	functions.add("MOVE.INT", move_data<int>, type<int>());
	functions.add("MOVE.DBL", move_data<double>, type<int>());
	functions.add("MOVE.STRING", move_data<string>, type<int>());
	functions.add("MOVE.CODE", move_code<Code>, type<int>());
	functions.add("MOVE.EXEC", move_code<Exec>, type<int>());
}

} // namespace gestation
} // namespace library
} // namespace vpush

