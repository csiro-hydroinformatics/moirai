#pragma once

#include "./setup_modifiers.h"
#include "../../../../include/moirai/reference_handle.hpp"

namespace moirai
{
	namespace tests
	{
		namespace domain_b
		{
			using std::string;

			class MOIRAI_TEST_B_CLASSES_ATTR B
			{
			public:
				B();
				virtual ~B();
				virtual string get_name() const;
			private:
			};
		}
	}
}