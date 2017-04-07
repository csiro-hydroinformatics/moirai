#include "./include/moirai_test_lib_b/domain_b.h"

namespace moirai
{
	namespace tests
	{
		namespace domain_b
		{
			B::B() {}
			B::~B() {}
			string B::get_name() const { return "B"; }
		}
	}
}