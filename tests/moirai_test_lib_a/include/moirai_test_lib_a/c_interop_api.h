/** \file c_interop_api.h
* Definition of a test C API.
*/

#pragma once

#include "./setup_modifiers.h"
#define USING_MOIRAI
#include "moirai/setup_modifiers.h"

#if defined(USING_DOMAIN_A_API)
// this file is included by code that uses the C API
#define A_PTR void*
#define B_PTR void*
#define VOID_PTR_PROVIDER_PTR void*
#else

// this file is included by code that defines a C API
#include "moirai/reference_handle.hpp"
#include "./domain_a.h"
using namespace moirai;
using namespace moirai::tests::domain_a;
using namespace moirai::tests::domain_b;
#define A_PTR reference_handle<A>* 
#define B_PTR reference_handle<B>* 
#define VOID_PTR_PROVIDER_PTR moirai::opaque_ptr_provider*

#endif

#ifdef __cplusplus
extern "C" {
#endif

	DOMAIN_A_API char* get_name_a(A_PTR x);
	DOMAIN_A_API A_PTR create_a_obj();
	DOMAIN_A_API A_PTR clone_handle_a(A_PTR a);
	DOMAIN_A_API B_PTR clone_handle_b_in_domain_a(B_PTR b);
	DOMAIN_A_API B_PTR new_handle_b_in_domain_a(B_PTR b);

	DOMAIN_A_API void meet_b(A_PTR a, B_PTR b);

	DOMAIN_A_API B_PTR spawn_b(A_PTR a);

	// Some API functions returns char* that must not be freed by the caller code, but using free_string.
	DOMAIN_A_API void free_string(char* x);
	// Notify this library that an object managed by an opaque pointer
	// has one less reference count from the caller code. This can trigger 
	// the disposal of the inner object if it has reached a count of 0 references. 
	DOMAIN_A_API void release_handle(VOID_PTR_PROVIDER_PTR ptr);

	// Read the reference count of a handle. This function should not be necessary 
	// in a production API but added here for didactic and testing purposes.
	DOMAIN_A_API int reference_count(VOID_PTR_PROVIDER_PTR ptr);

#ifdef __cplusplus
}
#endif
