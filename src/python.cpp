#include <string>
#include <fstream>
#include <iterator>

#include <boost/python.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/vpush.hpp>
#include <vpush/util/toroidal.hpp>

namespace vpush {
namespace python {

using std::string;
using namespace ::boost::python;
using namespace ::vpush;
using namespace ::vpush::detail;

namespace py = ::boost::python;

// Code helpers

Exec code_close() { return Exec::CLOSE; }
Exec code_open() { return Exec::OPEN; }
Exec byName(string n) { return functions.get_code(n); }
Exec random() { return functions.get_random(); }

// Stack access

template <typename T>
void push_wrap(Protein *p, T t) { push<T>(*p, t); }

template <typename T>
T pop_wrap(Protein *p) { return pop<T>(*p); }

// Soup and function access

const vpush::soup_t& get_soup() { return vpush::soup; }
void set_soup(const vpush::soup_t& s) { vpush::soup = s; }
const vpush::detail::functions_t& get_functions() { return vpush::functions; }

// Protein and soup serialization

void load_protein(Protein* p, string filename) {
	std::ifstream i(filename);
	boost::archive::text_iarchive ar(i);
	ar >> *p;
}

void save_protein(const Protein* p, string filename) {
	std::ofstream o(filename);
	boost::archive::text_oarchive ar(o);
	ar << *p;
}

void load_soup(soup_t* s, string filename) {
	std::ifstream i(filename);
	boost::archive::text_iarchive ar(i);
	soup_t new_soup;
	ar >> new_soup;
	s->swap(new_soup);
}

void save_soup(const soup_t* s, string filename) {
	std::ofstream o(filename);
	boost::archive::text_oarchive ar(o);
	ar << *s;
}

// Gestation and incubator
list copy_incubator() {
	py::list ret;
	BOOST_FOREACH(const Protein& p, incubator)
		ret.append(Protein(p));
	return ret;
}

object copy_gestator() {
	if(gestator)
		return object(Protein(*gestator));
	else
		return object();
}

double gestator_energy() {
	if(gestator)
		return gestator->energy;
	else
		return 0;
}

double incubator_energy() {
	double ret = 0;
	BOOST_FOREACH(const Protein&p, incubator)
		ret += p.energy;
	return ret;
}

BOOST_PYTHON_MODULE(vpush) {
	// on import
	vpush::initialize();
	
	// Code and Exec types
	enum_<Code::codetype>("CodeType")
		.value("OPEN", Code::OPEN)
		.value("CLOSE", Code::CLOSE)
		.export_values()
		;

	class_<Code>("Code", no_init)
		.def_readonly("type", &Code::type)
		.def("byName", byName)
		.staticmethod("byName")
		.def("random", random)
		.staticmethod("random")
		;

	class_<Exec, bases<Code> >("Exec", no_init);
	
	// Functions
	class_<functions_t>("Functions", no_init)
		.def("get_code", &functions_t::get_code)
		.def("get_name", &functions_t::get_name)
		.def("close", code_close)
		.staticmethod("close")
		.def("open", code_open)
		.staticmethod("open")
		;
	def("functions", get_functions, return_value_policy<reference_existing_object>());
	
	// Protein
	class_<Protein>("Protein")
		.def("__len__", &Protein::size)
		.def("count", &Protein::count)
		.def_readwrite("energy", &Protein::energy)
		.def("random", random_protein)
		.staticmethod("random")
		.def("load", load_protein)
		.def("save", save_protein)
		.def("push_int", push_wrap<int>)
		.def("pop_int", pop_wrap<int>)
		.def("push_exec", push_wrap<Exec>)
		.def("pop_exec", pop_wrap<Exec>)
		.def("push_code", push_wrap<Code>)
		.def("pop_code", pop_wrap<Code>)
		;

	// Engine (for running proteins)
	def("run_protein", run_protein,
		(arg("protein"), arg("max_energy")=0, arg("trace") = false));

	// Soup
	class_<soup_t>("Soup")
		.def(init<const soup_t&>())
		.def("__len__", &soup_t::size)
//		.def("__iter__", py::range<copy_const_reference>(&soup_t::begin, &soup_t::end))
		.def("set_size", &soup_t::set_size,
			(arg("soup_size"), "protein_size", "initial_energy"))
		.def("deep_size", &soup_t::deep_size)
		.def("deep_count", &soup_t::deep_count)
		.def("energy", &soup_t::energy)
		.def("load", load_soup)
		.def("save", save_soup)
		.def("push_back", &soup_t::push_back)
		.def("clear", &soup_t::clear)
		.def("run", &soup_t::run,
			(arg("max_energy")=0, arg("trace")=false))
		;
	def("get_soup", get_soup, return_value_policy<reference_existing_object>());
	def("set_soup", set_soup);
	
	// Gestation and incubation management
	def("clear_gestator", clear_gestator);
	def("detach_gestator", detach_gestator);
	def("clear_incubator", clear_incubator);
	def("flush_incubator", flush_incubator);

	// Gestation and incubation python access
	def("copy_gestator", copy_gestator);
	def("gestator_energy", gestator_energy);
	def("copy_incubator", copy_incubator);
	def("incubator_energy", incubator_energy);
	
	// Parameters
	// TODO: Parameter setters/getters (they have to be globals)
	// TODO: add run parameters like:
	// carrying_cost (p.energy -= p.size() * COST;)
	// movement_cost (factor to adjust movement cost)
	// decay_rate (0.5 * carrying_cost?)
	// decay_threshold (-30?)
}

} // namespace python
} // namespace vpush

