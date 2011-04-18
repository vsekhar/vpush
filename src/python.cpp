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

Protein random_protein(std::size_t s) {
	Protein ret;
	for(std::size_t i = 0; i < s; ++i)
		push<Exec>(ret, functions.get_random());
	return ret;
}

template <typename T>
void do_push_code(Protein &p, string name) { push<T>(p, functions.get_code(name)); }

template <typename T>
void do_push_code_close(Protein &p) { push<T>(p, T::CLOSE); }

template <typename T>
void do_push_code_open(Protein &p) { push<T>(p, T::OPEN); }

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

BOOST_PYTHON_MODULE(vpush) {
	// on import
	vpush::initialize();
	
	// Code, Exec, and functions
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
	
	class_<functions_t>("Functions", no_init)
		.def("get_code", &functions_t::get_code)
		.def("get_name", &functions_t::get_name)
		;
	scope().attr("functions") = functions;
	
	// Protein and Soup
	class_<Protein>("Protein")
		.def_readwrite("energy", &Protein::energy)
		.def("random", random_protein)
		.staticmethod("random")
		;

	def("push_code", do_push_code<Code>);
	def("push_code_open", do_push_code_open<Code>);
	def("push_code_close", do_push_code_close<Code>);
	def("push_exec", do_push_code<Exec>);
	def("push_exec_open", do_push_code_open<Exec>);
	def("push_exec_close", do_push_code_close<Exec>);
	// engine.cpp functions to run a protein

	class_<soup_t>("Soup")
		.def("__len__", &soup_t::size)
		.def("set_size", &soup_t::set_size)
		.def("push_back", &soup_t::push_back)
		;
	scope().attr("soup") = soup;
	def("load_soup", load_soup);
	def("save_soup", save_soup);
}

} // namespace python
} // namespace vpush
