#include "species.h"

namespace moirai
{
	namespace tests
	{
		animal::animal() {}
		animal::~animal() {}

		mamal::mamal() : animal() {}
		mamal::~mamal() {}
		string mamal::get_name() const { return string("mamal"); }

		dog::dog() : mamal() {}
		dog::~dog() {}
		string dog::get_name() const { return string("dog"); }

		cat::cat() : mamal() {}
		cat::~cat() {}
		string cat::get_name() const { return string("cat"); }

		homo_erectus::homo_erectus() : mamal() {}
		homo_erectus::~homo_erectus() {}
		string homo_erectus::get_name() const { return string("homo_erectus"); }

	}
}