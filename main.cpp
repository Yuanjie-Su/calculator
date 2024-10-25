#include <sstream>
#include "parser.h"
#include "driver.h"
#include "table.h"

using Table::table;

int main(int argc, char* argv[]) {
	table["pi"] = 3.1415926535897932385;
	table["e"] = 2.7182818284590452354;

	Token_stream ts{ std::cin };
	Driver::calculate(ts);

	std::cout << Error::no_of_errors << '\n';

	return Error::no_of_errors;
}