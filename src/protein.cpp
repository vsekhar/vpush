#include <vpush/protein.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/stackops.hpp>

namespace vpush {

struct clearer {
	template <typename T> void operator()(T& t) const { t.second.clear(); }
};

void Protein::reset() {
	fus::for_each(stacks, clearer());
}

struct size_accumulator {
	size_accumulator(const std::size_t& start, bool b) : value(start), byte_count(b) {}
	template <typename T>
	void operator()(const T& t) const { value+=t.second.size() * (byte_count ? sizeof(T) : 1); }
	mutable std::size_t value;
	const bool byte_count;
};

std::size_t Protein::size() const {
	size_accumulator s(0, true);
	fus::for_each(stacks, s);
	return s.value;
}

std::size_t Protein::count() const {
	size_accumulator s(0, false);
	fus::for_each(stacks, s);
	return s.value;
}

Protein random_protein(std::size_t s) {
	Protein ret;
	for(std::size_t i = 0; i < s; ++i)
		push<detail::Exec>(ret, functions.get_random());
	return ret;
}

void print_protein(Protein& p, std::ostream& o) {
	using std::endl;
	using vpush::detail::Exec;
	using vpush::detail::Code;
	
	o << "Exec: " << stack<Exec>(p) << endl;
	o << "Code: " << stack<Code>(p) << endl;
	o << size<bool>(p) << " bool: " << stack<bool>(p) << endl;
	o << size<int>(p) << " ints: " << stack<int>(p) << endl;
	o << size<double>(p) << " double: " << stack<double>(p) << endl;
	o << "Next op-code: " << top<Exec>(p);
	o << endl;
	o << "----" << endl;
}

} // namespace vpush

