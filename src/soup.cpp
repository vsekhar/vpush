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
		soup_container::index<bySeq>::type& seq = container.get<bySeq>();
		soup_container::iterator i = seq.end();
		for(int j=0; j < -delta; ++j) --i;
		container.get<bySeq>().erase(i, seq.end());
	}
}

std::size_t soup_t::deep_size() const {
	const soup_container::index<bySeq>::type& c = container.get<bySeq>();
	std::size_t accum = 0;
	BOOST_FOREACH(const Protein& p, c)
		accum += p.size();
	return accum;
}

std::size_t soup_t::deep_count() const {
	const soup_container::index<bySeq>::type& c = container.get<bySeq>();
	std::size_t accum = 0;
	BOOST_FOREACH(const Protein& p, c)
		accum += p.count();
	return accum;
}

double soup_t::energy() const {
	double ret = 0;
	BOOST_FOREACH(const Protein& p, container)
		ret += p.energy;
	return ret;
}

double soup_t::run() {
	ProteinRunner runner = ProteinRunner();
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
