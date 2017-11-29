#pragma once

#include "../../include/moirai/reference_type_converters.hpp"
#include "setup_modifiers.h"

#define MOIRAI_TEST_CLASSES_ATTR SPECIES_API

namespace moirai
{
	namespace tests
	{
		using std::string; 

		class MOIRAI_TEST_CLASSES_ATTR animal
		{
		public:
			animal();
			virtual ~animal();
			virtual string get_name() const = 0;
		private:
		};

		class MOIRAI_TEST_CLASSES_ATTR mamal : public animal
		{
		public:
			mamal();
			virtual ~mamal();
			string get_name() const;
		private:
		};

		class MOIRAI_TEST_CLASSES_ATTR dog : public mamal
		{
		public:
			dog();
			virtual ~dog();
			string get_name() const;
		private:
		};

		class MOIRAI_TEST_CLASSES_ATTR cat : public mamal
		{
		public:
			cat();
			virtual ~cat();
			string get_name() const;
		private:
		};

		class MOIRAI_TEST_CLASSES_ATTR homo_erectus : public mamal
		{
		public:
			homo_erectus();
			virtual ~homo_erectus();
			string get_name() const;
		private:
		};
	}

	using namespace tests;
	template<> struct known_conversions<mamal>
	{
		static mamal* dyn_cast(void* p, const typeinfo& tinfo)
		{
			return as_type<mamal, dog, cat, homo_erectus>(p, tinfo);
		}
	};

	template<> struct known_conversions<animal>
	{
		static animal* dyn_cast(void* p, const typeinfo& tinfo)
		{
			return as_type<animal, mamal, dog, cat, homo_erectus>(p, tinfo);
		}
	};

}