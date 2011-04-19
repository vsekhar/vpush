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
	scope().attr("functions") = functions;
	
	// Protein
	class_<Protein>("Protein")
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
	def("run_protein", run_protein,
		(arg("protein"), arg("trace") = false));

	// Soup
	class_<soup_t>("Soup")
		.def("__len__", &soup_t::size)
		.def("set_size", &soup_t::set_size)
		.def("push_back", &soup_t::push_back)
		.def("clear", &soup_t::clear)
		;
	scope().attr("soup") = soup;
	def("load_soup", load_soup, arg("filename"));
	def("save_soup", save_soup, arg("filename"));
}

} // namespace python
} // namespace vpush

