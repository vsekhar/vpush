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

	ensure_gestator(p);
	double amount = pop<double>(p);
	amount = std::min(amount, p.energy-1);
	if(amount > 0) {
		gestator->energy += amount;
		p.energy -= amount;
	}
	return 1;
}

template <typename T>
double move_data(Protein &p) {
	if(top<int>(p) < 0)
		return 0;
	
	ensure_gestator(p);
	std::list<T> buffer;
	unsigned count = (unsigned)pop<int>(p);
	count = std::min(count, size<T>(p));
	count = std::min(count, (unsigned)p.energy);
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

	ensure_gestator(p);
	std::list<item<T> > items;
	unsigned count = (unsigned) pop<int>(p);
	count = std::min(count, (unsigned)p.energy);
	unsigned actual_count = 0;
	try {
		for(unsigned i = 0; i < count; ++i) {
			items.push_front(get_item<T>(stack<T>(p)));
			++actual_count;
		}
	}
	catch(const detail::stack_underflow&) {}
	BOOST_FOREACH(const item<T>& i, items)
		put_item(i, stack<T>(*gestator));
	return actual_count;
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

