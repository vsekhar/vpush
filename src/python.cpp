#include <string>
#include <fstream>

#include <boost/python.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/vpush.hpp>

namespace vpush {
namespace python {

using std::string;
using namespace ::boost::python;
using namespace ::vpush;
using namespace ::vpush::detail;

namespace py = ::boost::python;

Exec code_close() { return Exec::CLOSE; }
Exec code_open() { return Exec::OPEN; }

Exec byName(string n) { return functions.get_code(n); }
Exec random() { return functions.get_random(); }

void load_soup(string filename) {
	std::ifstream i(filename);
	boost::archive::text_iarchive ar(i);
	soup_t new_soup;
	ar >> new_soup;
	soup.swap(new_soup);
}

void save_soup(string filename) {
	std::ofstream o(filename);
	boost::archive::text_oarchive ar(o);
	ar << soup;
}

template <typename T>
void push_wrap(Protein *p, T t) { push<T>(*p, t); }

template <typename T>
T pop_wrap(Protein *p) { return pop<T>(*p); }

vpush::soup_t& get_soup() { return vpush::soup; }
const vpush::detail::functions_t& get_functions() { return vpush::functions; }

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
		.def(init<Code::codetype>(args("type")))
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
	def("functions", get_functions, return_value_policy<copy_const_reference>());
	
	// Protein
	class_<Protein>("Protein")
		.def("__len__", &Protein::size)
		.def("count", &Protein::count)
		.def_readwrite("energy", &Protein::energy)
		.def("random", random_protein)
		.staticmethod("random")
		.def("push_int", push_wrap<int>)
		.def("pop_int", pop_wrap<int>)
		.def("push_exec", push_wrap<Exec>)
		.def("pop_exec", pop_wrap<Exec>)
		.def("push_code", push_wrap<Code>)
		.def("pop_code", pop_wrap<Code>)
		;

	// Engine (for running proteins)
	class_<ProteinRunner>("ProteinRunner")
		.def("__call__", &ProteinRunner::operator())
		.def_readwrite("trace", &ProteinRunner::trace)
		.def_readonly("result", &ProteinRunner::result)
		;
	def("run_protein", run_protein,
		(arg("protein"), arg("trace") = false));

	// Soup
	class_<soup_t>("Soup")
		.def("__len__", &soup_t::size)
		.def("set_size", &soup_t::set_size,
			(arg("soup_size"), "protein_size", "initial_energy"))
		.def("deep_size", &soup_t::deep_size)
		.def("deep_count", &soup_t::deep_count)
		.def("energy", &soup_t::energy)
		.def("push_back", &soup_t::push_back)
		.def("clear", &soup_t::clear)
		.def("run", &soup_t::run,
			(arg("trace")=false))
		;
	def("get_soup", get_soup, return_value_policy<reference_existing_object>());
	
	def("load_soup", load_soup, arg("filename"));
	def("save_soup", save_soup, arg("filename"));
	
	// Gestation and incubation
	def("release_incubator", release_incubator);
	// TODO: incubator access for testing?
	
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

