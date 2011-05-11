#include <iterator>

#include <boost/foreach.hpp>

#include <vpush/soup.hpp>
#include <vpush/engine.hpp>
#include <vpush/gestation.hpp>

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

struct sizer {
	sizer() : result(0) {}
	void operator()(const Protein& p) {result+=p.size();}
	std::size_t result;
};

std::size_t soup_t::deep_size() const {
	sizer s;
	this->for_each(s);
	return s.result;
}

struct counter {
	counter() : result(0) {}
	void operator()(const Protein& p) {result+=p.count();}
	std::size_t result;
};

std::size_t soup_t::deep_count() const {
	counter c;
	this->for_each(c);
	return c.result;
}

struct energy_summer {
	energy_summer() : result(0) {}
	void operator()(const Protein& p) {result+=p.energy;}
	double result;
};

double soup_t::energy() const {
	energy_summer es;
	this->for_each(es);
	return es.result;
}

double soup_t::run(bool trace) {
	typedef soup_container::index<bySeq>::type index;
	index& c = container.get<bySeq>();
	index::iterator i = c.begin();
	double cost = 0;
	for(; i != c.end(); ++i) {
		detach_gestator();
		c.modify(i, boost::bind(engine, _1, boost::ref(cost), trace));
	}
	return cost;
}

soup_t soup;

} // namespace vpush
