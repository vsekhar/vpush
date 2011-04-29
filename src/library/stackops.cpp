#include <vpush/library.hpp>
#include <vpush/detail/codelist.hpp>

namespace vpush {
namespace library {
namespace stackops {

template <typename T>
double eat(Protein &p) { pop<T>(p); return 1; }

template <typename T>
double eat_code(Protein &p) {get_item(stack<T>(p)); return 1;}

template <typename T>
double dup(Protein &p) {
	push<T>(p, top<T>(p));
	return 1;
}

template <typename T>
double dup_code(Protein& p) {
	item<T> i = get_item(stack<T>(p));
	put_item(i, stack<T>(p));
	put_item(i, stack<T>(p));
	return i.container.size() * 2;
}

template <typename T>
double swap(Protein& p) {
	std::iter_swap(stack<T>(p).rbegin(), ++stack<T>(p).rbegin());
	return 2;
}

template <typename T>
double swap_code(Protein &p) {
	item<T> a = get_item(stack<T>(p));
	item<T> b = get_item(stack<T>(p));
	put_item(a, stack<T>(p));
	put_item(b, stack<T>(p));
	return a.container.size() + b.container.size();
}

template <typename T, bool erase = true>
void do_yank(Protein &p, int index) {
	push<T>(p, pop_i<T>(p, index, erase));
}

template <typename T, bool erase = true>
double yank(Protein &p) {
	int index = pop<int>(p);
	if(index < 0 || (unsigned)(index+1) > size<T>(p)) return 0;
	do_yank<T>(p, index);
	return index+1;
}

template <typename T, bool erase = true>
double yank_code(Protein &p) {
	int index = pop<int>(p);
	std::size_t count;
	if(index < 0) return 0;
	try {
		item<T> i = get_nth_item(stack<T>(p), (unsigned)index, erase);
		put_item(i, stack<T>(p));
		count = i.container.size();
	}
	catch(const detail::stack_underflow&) {
		push<int>(p, index);
		return 0;
	}
	return count;
}

template <typename T>
double rotate(Protein &p) {
	do_yank<T>(p, 2);
	return 3;
}

template <typename T>
double rotate_code(Protein &p) {
	item<T> a = get_item(stack<T>(p));
	item<T> b = get_item(stack<T>(p));
	item<T> c = get_item(stack<T>(p));
	put_item(b, stack<T>(p));
	put_item(a, stack<T>(p));
	put_item(c, stack<T>(p));
	return a.container.size() + b.container.size() + c.container.size();
}

template <typename T>
double clear(Protein& p) { vpush::clear<T>(p); return 1; }

void initialize() {
	functions.add("EAT.INT", eat<int>, type<int>());
	functions.add("EAT.DBL", eat<double>, type<double>());
	functions.add("EAT.BOOL", eat<bool>, type<bool>());
	functions.add("EAT.STRING", eat<std::string>, type<std::string>());
	functions.add("EAT.CODE", eat_code<Code>, type<Code>());
	functions.add("EAT.EXEC", eat_code<Exec>, type<Exec>());

	functions.add("DUP.INT", dup<int>, type<int>());
	functions.add("DUP.DBL", dup<double>, type<double>());
	functions.add("DUP.BOOL", dup<bool>, type<bool>());
	functions.add("DUP.STRING", dup<std::string>, type<std::string>());
	functions.add("DUP.CODE", dup_code<Code>, type<Code>());
	functions.add("DUP.EXEC", dup_code<Exec>, type<Exec>());

	functions.add("SWAP.INT", swap<int>, type<int>() * 2);
	functions.add("SWAP.DBL", swap<double>, type<double>() * 2);
	functions.add("SWAP.BOOL", swap<bool>, type<bool>() * 2);
	functions.add("SWAP.STRING", swap<std::string>, type<std::string>() * 2);
	functions.add("SWAP.CODE", swap_code<Code>, type<Code>() * 2);
	functions.add("SWAP.EXEC", swap_code<Exec>, type<Exec>() * 2);

	functions.add("YANK.INT", yank<int>, type<int>());
	functions.add("YANK.DBL", yank<double>, type<int>());
	functions.add("YANK.BOOL", yank<bool>, type<int>());
	functions.add("YANK.STRING", yank<std::string>, type<int>());
	functions.add("YANK.CODE", yank_code<Code>, type<int>());
	functions.add("YANK.EXEC", yank_code<Exec>, type<int>());

	functions.add("YANKDUP.INT", yank<int, false>, type<int>());
	functions.add("YANKDUP.DBL", yank<double, false>, type<int>());
	functions.add("YANKDUP.BOOL", yank<bool, false>, type<int>());
	functions.add("YANKDUP.STRING", yank<std::string, false>, type<int>());
	functions.add("YANKDUP.CODE", yank_code<Code, false>, type<int>());
	functions.add("YANKDUP.EXEC", yank_code<Exec, false>, type<int>());

	functions.add("ROTATE.INT", rotate<int>, type<int>() * 3);
	functions.add("ROTATE.DBL", rotate<double>, type<double>() * 3);
	functions.add("ROTATE.BOOL", rotate<bool>, type<bool>() * 3);
	functions.add("ROTATE.STRING", rotate<std::string>, type<std::string>() * 3);
	functions.add("ROTATE.CODE", rotate_code<Code>, type<Code>() * 3);
	functions.add("ROTATE.EXEC", rotate_code<Exec>, type<Exec>() * 3);
	
	functions.add("CLEAR.INT", clear<int>, type<int>());
	functions.add("CLEAR.DBL", clear<double>, type<double>());
	functions.add("CLEAR.BOOL", clear<bool>, type<bool>());
	functions.add("CLEAR.STRING", clear<std::string>, type<std::string>());
	functions.add("CLEAR.CODE", clear<Code>, type<Code>());
	functions.add("CLEAR.Exec", clear<Exec>, type<Exec>());
}

}
}
}
