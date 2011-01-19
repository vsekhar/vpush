#include <ostream>
#include <list>
#include <vector>
#include <string>

#include "vpush/register.hpp"

struct badtype {
	badtype(int) {}
private:
	badtype();
};

int my_func(int i, int j) { return i+j; }
badtype badfunc() { return badtype(1); }

int main(int argc, char** argv) {
	vpush::register_(my_func, "my_func");
	vpush::register_(badfunc, "bad_func");
	return 0;
}

