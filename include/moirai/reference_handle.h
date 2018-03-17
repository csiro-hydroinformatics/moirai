#pragma once

#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <functional>

#include "moirai/setup_modifiers.h"

using std::string;

namespace moirai
{

	/** \brief	A class that store a map of all the shared_ptr created/manipulated via the API. */
	class MOIRAI_API reference_handle_map
	{
		// "We" will look back at this and scratch "our" heads. 
		// The following references material that should explain why this class is as it is.
		// I battled several hours to get the behavior I wanted.
		// 
		// Getting static member initialization is obtusely difficult but for integral types. I found how to do this in the meta- FAQ at the address:
		// https://isocpp.org/wiki/faq/ctors#static-init-order
		// 
		// Note that this is critical to have MOIRAI_API on this class, otherwise you would have reference counting happening per compiled DLL
		// http://www.boost.org/doc/libs/1_58_0/libs/smart_ptr/sp_techniques.html#pvoid
		// 
		// Marginally:
		// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Counted_Body
		// 
	private:


		/** \brief	a map from raw memory addresses the shared_ptr used to count references to this address */
		std::map<size_t, std::shared_ptr<void>> pointers;

		/** \brief	A mutex to keep this class safe for multi-threaded use */
		std::mutex map_mutex;
		reference_handle_map();
	public:

		/**
		 * \brief	Gets the single instance of this class
		 *
		 * \return	reference to instance;
		 */
		static reference_handle_map& instance();

		/** \brief	Defines an alias for a function type to create shared_ptr */
		typedef std::function<std::shared_ptr<void>(void* p)> create_handle_func;

		/**
		 * \brief	Gets.
		 *
		 * \remark	This function is thread safe and atomic.
		 * \param [in]	p opaque pointer to the object to wrap with a shared_ptr
		 * \param	createPtr	The function to create the shared_ptr around the parameter 'p', if needed.
		 *
		 * \return	A std::shared_ptr&lt;void&gt;
		 */
		std::shared_ptr<void> get(void* p, const create_handle_func& createPtr);

		/**
		 * \brief	Query if a memory address has a reference counting handle.
		 *
		 * \param [in,out]	p	If non-null, the void to process.
		 *
		 * \return	True if handle, false if not.
		 */

		bool has_handle(void* p);

		/**
		 * \brief	Releases the given address (decrement reference counts and possibly destruction of the object)
		 *
		 * \param	address	The address.
		 */
		void release(size_t address);

		/**
		* \brief	Releases the given address (decrement reference counts and possibly destruction of the object)
		*
		* \param	p	pointer to the object.
		*/
		void release(void* p);
	};

	class MOIRAI_API typeinfo
	{
		const std::type_info* p_;

	public:
		typeinfo();

		typeinfo(const std::type_info& t);

		/*inline*/ const char* name() const;

		/*inline*/ const size_t hash_code() const;

		/*inline*/ bool operator<(const typeinfo& that) const;

		/*inline*/ bool operator==(const typeinfo& that) const;
	};


	/**
	* \brief	A parent class for shared pointer wrappers, returning the untyped part of the information about the object.
	*
	* \remarks
	* 			This non-template class is needed to provide function definitions to the API when compiling this DLL.
	* 			This shields the API from complexities of the reference_handle template, otherwise the API header would
	* 			need to import a lot of additional baggage.
	*/
	class MOIRAI_API opaque_ptr_provider
	{
	protected:
		typeinfo wrapped_type_info;
		opaque_ptr_provider(const std::type_info& tinfo);
		const size_t type_info_hash_code();
	public:
		virtual ~opaque_ptr_provider();
		virtual void * get_void_ptr() = 0;
		virtual long count() const = 0;
		virtual string get_status() = 0;
		const char* wrapped_type_name() const;
	};

}


