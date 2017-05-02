#pragma once

// USING_MOIRAI defines on what side of the API we are
#include "moirai/setup_modifiers.h"
#include "moirai/reference_handle.h"

#ifdef USING_MOIRAI
#define REFERENCE_HANDLE_PTR void*
#else
#define REFERENCE_HANDLE_PTR moirai::opaque_ptr_provider*
#endif

#ifdef __cplusplus
extern "C" {
#endif

	MOIRAI_API void dispose_reference_handle(REFERENCE_HANDLE_PTR ptr);
	MOIRAI_API int get_reference_count(REFERENCE_HANDLE_PTR ptr);

#ifdef __cplusplus
}
#endif
