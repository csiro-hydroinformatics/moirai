#pragma once

#include "moirai/reference_handle_map_export.h"

namespace moirai
{
	template<typename T=void*>
	class pointer_handle
	{
	public:
		pointer_handle(T p) 
		{
			if (p == nullptr)
				throw std::invalid_argument("pointer_handle argument must not be nullptr");
			else
				ptr = p;
		}
		~pointer_handle()
		{
			if (ptr != nullptr)
				::dispose_reference_handle(ptr);
		}

		T get() { return ptr; }
		int get_reference_count() 
		{ 
			return ::get_reference_count(ptr); 
		}

	private:
		T ptr = nullptr;
	};


	/** \brief	A class you may consider using if you are calling from C++ a C API offering moirai pointers, e.g. using Rcpp::XPtr<opaque_pointer_handle> if using R and Rcpp*/
	using opaque_pointer_handle = pointer_handle<void*>;
}

