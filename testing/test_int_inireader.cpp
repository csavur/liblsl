#include "../src/inireader.h"
#include "catch.hpp"
#include <sstream>

void try_load(INI &pt, const char *contents) {
	std::istringstream stream{std::string(contents)};
	pt.load(stream);
}

TEST_CASE("ini files are parsed correctly", "[ini][basic]") {
	INI pt;
	try_load(pt, "x=5\n"
				 "y=2\n"
				 "[foo]\n"
				 "foo=bar\n"
				 "; foo=commented out\n"
				 "double=equals=sign\n"
				 "[white space]\n"
				 "\tfoo  =\t bar\r\n");
	CHECK(pt.get("doesntexist", 0) == 0);
	CHECK(pt.get<int>("defaultval") == 0);
	CHECK(pt.get<int>("x") == 5);
	CHECK(pt.get<std::string>("foo.foo") == "bar");
	CHECK(pt.get<std::string>("white space.foo") == "bar");
}

TEST_CASE("bad ini files are rejected", "[ini][basic]") {
	INI pt;
	CHECK_THROWS(try_load(pt, "[badsection"));
	CHECK_THROWS(try_load(pt, "duplicate=1\nduplicate=2"));
	CHECK_THROWS(try_load(pt, "missingval"));
	CHECK_THROWS(try_load(pt, "missingval= "));
	CHECK_THROWS(try_load(pt, " = missingkey"));
}