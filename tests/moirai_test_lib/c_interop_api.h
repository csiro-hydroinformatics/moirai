/** \file c_interop_api.h
* Definition of a test C API.
*/

#pragma once


#ifdef USING_SPECIES_API
#define USING_MOIRAI
#endif
// USING_MOIRAI defines on what side of the API we are
#include "moirai/setup_modifiers.h"
// You may want to use a specific name for export attribute modifiers for your API
#define SPECIES_API MOIRAI_API

#if defined(USING_MOIRAI)
// this file is included by code that uses the C API
#define ANIMAL_PTR void*
#define MAMAL_PTR void*
#define DOG_PTR  void*
#define CAT_PTR  void*
#define HOMO_ERECTUS_PTR void*
#define VOID_PTR_PROVIDER_PTR void*
#else

// this file is included by code that defines a C API
#include "moirai/reference_handle.hpp"
#include "species.h"
using namespace moirai;
using namespace moirai::tests;
#define ANIMAL_PTR reference_handle<animal>* 
#define MAMAL_PTR reference_handle<mamal>* 
#define DOG_PTR reference_handle<dog>* 
#define CAT_PTR reference_handle<cat>* 
#define HOMO_ERECTUS_PTR reference_handle<homo_erectus>* 
#define VOID_PTR_PROVIDER_PTR moirai::opaque_ptr_provider*

#endif

#ifdef __cplusplus
extern "C" {
#endif

	SPECIES_API char* get_name(ANIMAL_PTR x);
	SPECIES_API CAT_PTR create_cat();
	SPECIES_API DOG_PTR create_dog();
	SPECIES_API int put_in_dog_kenel(DOG_PTR d);
	SPECIES_API HOMO_ERECTUS_PTR create_homo_erectus();

	// Some API functions returns char* that must not be freed by the caller code, but using free_string.
	SPECIES_API void free_string(char* x);
	// Notify this library that an object managed by an opaque pointer
	// has one less reference count from the caller code. This can trigger 
	// the disposal of the inner object if it has reached a count of 0 references. 
	SPECIES_API void release_handle(VOID_PTR_PROVIDER_PTR ptr);

	// Read the reference count of a handle. This function should not be necessary 
	// in a production API but added here for didactic and testing purposes.
	SPECIES_API int reference_count(VOID_PTR_PROVIDER_PTR ptr);

	SPECIES_API MAMAL_PTR create_mamal_ptr(CAT_PTR cat);

#ifdef __cplusplus
}
#endif
