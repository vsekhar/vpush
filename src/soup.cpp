#include <iterator>

#include <boost/foreach.hpp>

#include <vpush/soup.hpp>
#include <vpush/engine.hpp>

namespace vpush {

void soup_t::add(std::size_t n, std::size_t protein_size, double starting_energy) {
	for(std::size_t i = 0; i < n; ++i) {
		Protein p = random_protein(protein_size);
		p.energy = starting_energy;
		container.push_back(p);
	}
}

void soup_t::set_size(std::size_t soup_size, std::size_t protein_size, double starting_energy) {
	int delta = soup_size - container.size();
	if(delta > 0)
		add(delta, protein_size, starting_energy);
	else {
		typedef soup_container::index<byEnergy>::type index_t;
		index_t& c = container.get<byEnergy>();
		index_t::iterator i = c.end();
		std::advance(i, -delta);
		c.erase(i, c.end());
	}
}

std::size_t soup_t::deep_size() const {
	std::size_t accum = 0;
	BOOST_FOREACH(const Protein& p, container)
		accum += p.size();
	return accum;
}

std::size_t soup_t::deep_count() const {
	std::size_t accum = 0;
	BOOST_FOREACH(const Protein& p, container)
		accum += p.count();
	return accum;
}

double soup_t::energy() const {
	double ret = 0;
	BOOST_FOREACH(const Protein& p, container)
		ret += p.energy;
	return ret;
}

double soup_t::run(bool trace) {
	ProteinRunner runner = ProteinRunner(trace);
	double energy_used = 0;
	typedef soup_container::index<bySeq>::type index;
	index& c = container.get<bySeq>();
	index::iterator i = c.begin();
	for(; i != c.end(); ++i) {
		double initial_energy = i->energy;
		c.modify(i, runner);
		energy_used += initial_energy - i->energy;
	}
	return energy_used;
}

soup_t soup;

} // namespace vpush
