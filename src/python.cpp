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

struct CodePickleSuite : ::boost::python::pickle_suite {
	using ::boost::python::tuple;
	using ::boost::python::make_tuple;
	
	static tuple getinitargs(const Code& c) {
		std::string name;
		if(c.type == Code::CODE)
			name = vpush::detail::functions.get_name(c);
		return make_tuple(c.type, name);
	}
};

struct ProteinPickleSuite : ::boost::python::pickle_suite {
	using ::boost::python::list;
	using ::boost::python::extract;
	
	struct StackExtractor {
		StackExtractor(list& l) : _list(l) {}
		template <typename S>
		void operator()(const S& s) const {
			list converted_stack;
			_list.append(std::copy(s.begin(), s.end(), std::begin(converted_stack)));
		}	
		list& _list;
	};

	struct StackInserter {
		StackInserter(const list& l) : _list(l) {}
		template <typename S>
		void operator()(S& s) const {
			const list& converted_stack = extract<list>(*std::begin(_list));
			std::copy(converted_stack.begin(), converted_stack.end(), std::back_inserter(s));
		}
		const list& _list;
	};

	static list getstate(const Protein& p) {
		list ret;
		ret.append(p.energy);
		ret.append(p.x);
		ret.append(p.y);
		ret.append(p.z);
		ret.append(p.facing);
		p.for_each(StackExtractor(ret));
		return ret;
	}
	
	static void setstate(Protein& p, list l) {
		p.energy = extract<double>(list.pop(0));
		p.x = extract<vpush::detail::toroidal_dimension>(list.pop(0));
		p.y = extract<vpush::detail::toroidal_dimension>(list.pop(0));
		p.z = extract<vpush::detail::toroidal_dimension>(list.pop(0));
		p.facing = extract<vpush::util::vector>(list.pop(0));
		p.for_each(StackInserter(list));
	}
};

struct SoupPickleSuite : ::boost::python::pickle_suite {
	static list getstate(const Soup_t& s) {
		list ret;
		BOOST_FOREACH(const Protein& p, s)
			ret.append(p);
		return ret;
	}
	
	struct SoupInserter {
		SoupInserter(Soup_t& s) : _soup(s) {}
		static void operator()(const Protein &p) const {
			_soup.insert(p);
		}
		Soup_t _soup;
	}
	
	static void setstate(Soup_t& s, list l) {
		using ::boost::python::object;
		Soup_t new_soup;
		BOOST_FOREACH(const object& o, l) {
			const Protein &p = extract<const Protein&>(o);
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
		.def(init<Code::codetype, std::string>((arg("type"), "name")))
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

