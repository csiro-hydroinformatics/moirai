#pragma once
#include "reference_handle.h"


namespace moirai
{

	// known_conversions and as_type are in this file to keep it separated from reference_handle.hpp. 
	// Users should include this file, then their template specializations, and *last* reference_handle.hpp to avoid 
	// confusing behaviors in reference handle type casts

	/**
	* \struct	known_conversions
	*
	* \brief
	* 			A template declaration that can be specialized to safely handle
	* 			some type casts of objects received via opaque pointers.
	* 			This template is to handle some form of dynamic type cast capability despite the static/runtime boundary
	* 			across a C API with opaque pointers and the absence of software reflection capabilities in C++.
	* 			The approach inspired in parts by section 5.3.6. Boundary Crossing with Trampolines, in
	* 			"Davide Di Gennaro, Advanced Metaprogramming in Classic C++, Apress, 2015". ISBN-13 : 978-1-4842-1011-6   pp. 572
	*
	* \tparam	U	Generic type parameter.
	*/
	template<typename U> struct known_conversions
	{
		static U* dyn_cast(void* p, const typeinfo& tinfo)
		{
			if (typeinfo(typeid(U)) == tinfo)
				return static_cast<U*>(p);
			return nullptr;
		}
	};

	/**
	* \fn	template<typename T> T* as_type(void* p, const typeinfo& tinfo)
	*
	* \brief	Terminal as_type version: called when there is one type argument
	*
	* \tparam	T	Generic type parameter.
	* \param [in,out]	p	 	If non-null, the void to process.
	* \param 		  	tinfo	The tinfo.
	*
	* \return	Null if it fails, else a pointer to a T.
	*/

	template<typename T>
	T* as_type(void* p, const typeinfo& tinfo)
	{
		if (tinfo == typeinfo(typeid(T)))
			return static_cast<T*>(p);
		else
			return nullptr;
	}

	/**
	* \fn	template<typename T, typename R, typename... RArgs> T* as_type(void* p, const typeinfo& tinfo)
	*
	* \brief	Variadic as_type version: called when there is more than one type argument
	*
	* \tparam	T	 	Generic type parameter.
	* \tparam	R	 	Type of the r.
	* \tparam	RArgs	Type of the arguments.
	* \param [in,out]	p	 	If non-null, the void to process.
	* \param 		  	tinfo	The tinfo.
	*
	* \return	Null if it fails, else a pointer to a T.
	*/

	template<typename T, typename R, typename... RArgs>
	T* as_type(void* p, const typeinfo& tinfo)
	{
		if (tinfo == typeinfo(typeid(R)))
			return static_cast<T*>(static_cast<R*>(p));
		else
			return as_type<T, RArgs...>(p, tinfo);
	}

}

