#include <string>
#include <fstream>
#include <iterator>

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
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

Exec code_close() { return Exec::CLOSE; }
Exec code_open() { return Exec::OPEN; }

Exec byName(string n) { return functions.get_code(n); }
Exec random() { return functions.get_random(); }

struct VectorPickleSuite : py::pickle_suite {
	static py::tuple getinitargs(const util::vector& v) {
		return py::make_tuple(v[0], v[1], v[2]);
	}
};

struct ToroidalPickleSuite : py::pickle_suite {
	static py::tuple getinitargs(const util::toroidal_dimension& td) {
		return py::make_tuple(td.get());
	}
};

struct CodePickleSuite : py::pickle_suite {
	static py::tuple getinitargs(const Code& c) {
		std::string name;
		if(c.type == Code::CODE)
			name = vpush::functions.get_name(c);
		return py::make_tuple(c.type, name);
	}
};

struct ProteinPickleSuite : py::pickle_suite {
	struct StackExtractor {
		StackExtractor(py::list& l) : _list(l) {}
		template <typename S>
		void operator()(const S& s) const {
			py::list converted_stack;
			py::stl_input_iterator<typename S::second_type::value_type> begin(converted_stack);
			std::copy(s.second.begin(), s.second.end(), begin);
			_list.append(converted_stack);
		}
		py::list& _list;
	};

	struct StackInserter {
		StackInserter(py::list& l) : _list(l) {}
		template <typename S>
		void operator()(S& s) const {
			typename S::second_type new_stack;
			new_stack.reserve(py::len(_list));
			py::stl_input_iterator<typename S::second_type::value_type> begin(_list.pop(0)), end;
			for( ; begin != end; ++begin)
				new_stack.push_back(*begin);
			// doesn't work for some reason:
			// std::copy(begin, end, std::back_inserter(new_stack));
			s.second.swap(new_stack);
		}
		mutable py::list& _list;
	};

	static py::list getstate(const Protein& p) {
		py::list ret;
		ret.append(p.energy);
		ret.append(p.x);
		ret.append(p.y);
		ret.append(p.z);
		ret.append(p.facing);
		StackExtractor se(ret);
		p.for_each(ret);
		return ret;
	}
	
	static void setstate(Protein& p, py::list l) {
		p.energy = py::extract<double>(l.pop(0));
		p.x = py::extract<vpush::util::toroidal_dimension>(l.pop(0));
		p.y = py::extract<vpush::util::toroidal_dimension>(l.pop(0));
		p.z = py::extract<vpush::util::toroidal_dimension>(l.pop(0));
		p.facing = py::extract<vpush::util::vector>(l.pop(0));
		StackInserter si(l);
		p.for_each(si);
	}
};

struct SoupPickleSuite : py::pickle_suite {
	struct SoupExtractor {
		SoupExtractor(py::list& l) : _list(l) {}
		void operator()(const Protein& p) {
			_list.append(p);
		}
		py::list& _list;
	};
	
	static py::list getstate(const soup_t& s) {
		py::list ret;
		SoupExtractor se(ret);
		s.for_each(se);
		return ret;
	}
	
	struct SoupInserter {
		SoupInserter(soup_t& s) : _soup(s) {}
		void operator()(const Protein &p) {
			_soup.push_back(p);
		}
		soup_t _soup;
	};
	
	static void setstate(soup_t& s, py::list l) {
		soup_t new_soup;
		py::stl_input_iterator<py::list> begin(l), end;
		for( ; begin != end; ++begin) {
			const Protein& p = extract<const Protein&>(*begin);
			new_soup.push_back(p);
		}
		s.swap(new_soup);
	}
};

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
void set_soup(const vpush::soup_t& s) { vpush::soup = s; }
const vpush::detail::functions_t& get_functions() { return vpush::functions; }

BOOST_PYTHON_MODULE(vpush) {
	// on import
	vpush::initialize();
	
	// utils
	class_<util::vector>("Vector")
		.def_pickle(VectorPickleSuite())
		;
	class_<util::toroidal_dimension>("ToroidalDimension")
		.def_pickle(ToroidalPickleSuite())
		;
	
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
		.def_pickle(CodePickleSuite())
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
		.def_pickle(ProteinPickleSuite())
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
		.def_pickle(SoupPickleSuite())
		;
	def("get_soup", get_soup, return_value_policy<reference_existing_object>());
	def("set_soup", set_soup);
	
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

