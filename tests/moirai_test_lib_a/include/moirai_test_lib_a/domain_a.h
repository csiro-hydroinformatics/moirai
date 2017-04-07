#pragma once

#include "../../../moirai_test_lib_b/include/moirai_test_lib_b/domain_b.h"

#define MOIRAI_TEST_A_CLASSES_ATTR __declspec(dllexport)

namespace moirai
{
	namespace tests
	{
		namespace domain_a
		{
			using moirai::tests::domain_b::B;

			using std::string;

			class MOIRAI_TEST_A_CLASSES_ATTR A
			{
			public:
				A();
				virtual ~A();
				virtual string get_name() const;
				virtual B* make_b() const;
				virtual void meet_b(const B& b) const;
			private:
			};
		}
	}
}