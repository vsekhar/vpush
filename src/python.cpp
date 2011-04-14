#include <string>

#include <boost/python.hpp>

#include <vpush/protein.hpp>
#include <vpush/soup.hpp>
#include <vpush/library.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>

using namespace ::boost::python;
using namespace ::vpush;
using namespace ::vpush::detail;

BOOST_PYTHON_MODULE(vpush) {
	// on import
	library::initialize();
	
	// Code, Exec, and functions
	class_<Code>("Code", no_init)
		.def_readonly("type", &Code::type)
		;
	class_<Exec, bases<Code> >("Exec", no_init);
	
	class_<functions_t>("Functions", no_init)
		.def("get_code", &functions_t::get_code)
		.def("get_name", &functions_t::get_name)
		;
	scope().attr("functions") = functions;
	
	// Protein and Soup
	class_<Protein>("Protein");
	def("print_protein", print_trace);
	class_<soup_t>("Soup")
		.def("__len__", &soup_t::size)
		.def("set_size", &soup_t::set_size)
		.def("push_back", &soup_t::push_back)
		;
}

