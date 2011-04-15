#include <vpush/soup.hpp>

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

const Protein& soup_t::operator[](std::size_t n) const {
	soup_container::const_iterator itr = container.get<bySeq>().begin();
	for(std::size_t i=0; i < n; ++i) ++itr;
	return *itr;
}

soup_t soup;

} // namespace vpush
