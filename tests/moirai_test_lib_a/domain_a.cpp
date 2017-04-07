#include "./include/moirai_test_lib_a/domain_a.h"

namespace moirai
{
	namespace tests
	{
		namespace domain_a
		{
			A::A() {}
			A::~A() {}
			string A::get_name() const { return "A"; }
			B* A::make_b() const { return new B(); }
			void A::meet_b(const B& b) const { /*nothing*/; }
		}
	}
}