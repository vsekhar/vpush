#include <boost/foreach.hpp>

#include <vpush/soup.hpp>
#include <vpush/engine.hpp>

namespace vpush {

void soup_t::add(std::size_t n) {
	for(std::size_t i = 0; i < n; ++i)
		container.push_back(Protein());
}

void soup_t::set_size(std::size_t n) {
	int delta = n - container.size();
	if(delta > 0)
		add(delta);
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

double soup_t::energy() const {
	double ret = 0;
	BOOST_FOREACH(const Protein& p, container)
		ret += p.energy;
	return ret;
}

void soup_t::run() {
	ProteinRunner runner;
	typedef soup_container::index<byEnergy>::type index;
	index& c = container.get<byEnergy>();
	index::iterator i = c.begin();
	for(; i != c.end(); ++i)
		c.modify(i, runner);
}

soup_t soup;

} // namespace vpush
