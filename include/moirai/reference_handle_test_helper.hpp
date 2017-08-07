#pragma once

#include "reference_handle.hpp"

namespace moirai
{
	namespace tests
	{
		/**
		 * \fn	template<typename TTo> TTo* unchecked_cast_to_raw_ptr(void* ref_hndl)
		 *
		 * \brief	For unit tests only. Retrieving the raw pointer out of a reference_handle
		 *
		 * \tparam	TTo	The type wrapped.
		 * \param [in]	ref_hndl	A pointer to a reference_handle<TTo>.
		 *
		 * \return	A pointer to a TTo.
		 */

		template<typename TTo>
		TTo* unchecked_cast_to_raw_ptr(void* ref_hndl) {
			return (static_cast<moirai::reference_handle<TTo>*>(ref_hndl))->get_ptr();
		}
	}
}
