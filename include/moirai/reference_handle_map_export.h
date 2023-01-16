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

	/**
	 * \fn	MOIRAI_API void dispose_reference_handle(REFERENCE_HANDLE_PTR ptr);
	 *
	 * \brief	Destroy the reference handle pointed to by the argument.
	 *
	 * \param	ptr	The pointer to the reference handle.
	 */

	MOIRAI_API void dispose_reference_handle(REFERENCE_HANDLE_PTR ptr);

	/**
	 * \fn	MOIRAI_API int get_reference_count(REFERENCE_HANDLE_PTR ptr);
	 *
	 * \brief	Gets the number of references to the underlying object handled by the reference handle.
	 *
	 * \param	ptr	The pointer to the reference handle.
	 *
	 * \return	The reference count.
	 */

	MOIRAI_API int get_reference_count(REFERENCE_HANDLE_PTR ptr);

	// MOIRAI_API bool has_error_handling_callback_registered();

	// MOIRAI_API void register_error_handling_callback(const void* callback);

#ifdef __cplusplus
}
#endif
