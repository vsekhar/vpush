#include <string>

#include <boost/foreach.hpp>

#include <vpush/library.hpp>
#include <vpush/gestation.hpp>

namespace vpush {
namespace library {
namespace gestation {

using std::string;

double detach_protein(Protein &p) { return detach_gestator() ? 1 : 0; }

double transfer_energy(Protein &p) {
	if(top<double>(p) < 0 || p.energy < top<double>(p))
		return 0;

	ensure_gestator();
	double amount = pop<double>(p);
	gestator->energy += amount;
	p.energy -= amount;
	return 1;
}

template <typename T>
double move_data(Protein &p) {
	if(top<int>(p) < 0)
		return 0;
	
	ensure_gestator();
	std::list<T> buffer;
	unsigned count = (unsigned)pop<int>(p);
	for(unsigned i = 0; i < count; ++i)
		buffer.push_front(pop<T>(p));
	BOOST_FOREACH(const T& t, buffer)
		push<T>(*gestator, t);
	return count;
}

template <typename T>
double move_code(Protein &p) {
	if(top<int>(p) < 0)
		return 0;

	ensure_gestator();
	std::list<item<T> > items;
	unsigned count = (unsigned) pop<int>(p);
	for(unsigned i = 0; i < count; ++i)
		items.push_front(get_item<T>(stack<T>(p)));
	BOOST_FOREACH(const item<T>& i, items)
		put_item(i, stack<T>(*gestator));
	return 1;
}

void initialize() {
	functions.add("DETACH_PROTEIN", detach_protein);
	functions.add("TRANSFER_ENERGY", transfer_energy);
	functions.add("MOVE.BOOL", move_data<bool>, type<int>());
	functions.add("MOVE.INT", move_data<int>, type<int>());
	functions.add("MOVE.DBL", move_data<double>, type<int>());
	functions.add("MOVE.STRING", move_data<string>, type<int>());
	functions.add("MOVE.CODE", move_code<Code>);
	functions.add("MOVE.EXEC", move_code<Exec>);
}

} // namespace gestation
} // namespace library
} // namespace vpush

