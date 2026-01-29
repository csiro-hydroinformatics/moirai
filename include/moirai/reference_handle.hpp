#pragma once

#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <functional>
#include <stdexcept>

#include "reference_type_converters.hpp"

using std::string;

namespace moirai
{
	/**
	* \class	cast_ptr_provider
	*
	* \brief	A non-template parent class for reference handles.
	* 			This class sits at the interface between templated and non-templated
	* 			code to still perform reliable type conversions of pointers passed across a C API.
	*/
	class cast_ptr_provider : public opaque_ptr_provider
	{
	protected:
		cast_ptr_provider(const std::type_info& tinfo) : opaque_ptr_provider(tinfo)
		{
		}
	public:
		template<typename U>
		bool can_cast_to()
		{
			return (dynamic_cast_to<U>() != nullptr);
		}

		template<typename U>
		U* dynamic_cast_to()
		{
			return known_conversions<U>::dyn_cast(get_void_ptr(), this->wrapped_type_info);
			//return cast_via_exception<U>();
		}

		virtual void * get_void_ptr() = 0;
		virtual long count() const = 0;

		virtual ~cast_ptr_provider() {/*Nothing*/}

	private:

		//// The following uses a try/catch statement for some logic. This is usually a no-no, 
		//// but an option found in the following reference. 
		//// Unfortunately, this does not appear to work - an U* will always be returned even with it should not.
		//// 5.3.6. Boundary Crossing with Trampolines
		//// Advanced Metaprogramming in Classic C++
		//// By : Davide Di Gennaro
		//// Publisher : Apress
		//// Pub.Date : May 2, 2015
		//// Print ISBN - 13 : 978 - 1 - 4842 - 1011 - 6
		//// Pages in Print Edition : 572
		//template <typename U>
		//U* cast_via_exception()
		//{
		//	try
		//	{
		//		void* vp = get_void_ptr();
		//		throw static_cast<U*>(vp);;
		//	}
		//	catch (U* p)   // yes, it was indeed an U*
		//	{
		//		return p;
		//	}
		//	catch (...)    // no, it was something else
		//	{
		//		return 0;
		//	}
		//}
	};

	/**
	 * \brief	A shared pointer for reference counting objects when used via the API.
	 *
	 * \tparam	T	Generic type parameter.
	 *
	 * \remark	This template does not replace std::shared_ptr but wraps it. It is here to ensure stricter reference count to objects.
	 *			This class deliberately does not have the MOIRAI_API modifier, otherwise this would
	 *			force you to define all template implementations to be used hereafter.
	 *			This is why we have a separate reference_handle_map that is exported.
	 *			This template is intended for use for types exported via a C API, but may be of wider use later on.
	 */
	template<typename T>
	class reference_handle : public cast_ptr_provider
	{
	public:
		/**
		* \brief	reference_handle constructor
		*
		* \param	object	  	const reference to an object, that must have a deep copy constructor
		*/
		reference_handle(const T& object) : cast_ptr_provider(typeid(T))
		{
			T* p = new T(object);
			_shared_pointer = find_shared_ptr(p);
		}

		/**
		* \brief	reference_handle constructor
		*
		* \param	p	  	pointer to the object this reference_handle will wrap. It may already be wrapped by another reference_handle.
		*/
		reference_handle(T* p) : cast_ptr_provider(typeid(T))
		{
			if (p == nullptr)
				throw std::invalid_argument("Pointer must not be nullptr");
			_shared_pointer = find_shared_ptr(p);
		}

		/**
		* \brief	reference_handle copy constructor
		*
		* \param	ptr	  	reference_handle to copy.
		*/
		reference_handle(const reference_handle<T>& ptr) : cast_ptr_provider(typeid(T))
		{
			_shared_pointer = find_shared_ptr(ptr._shared_pointer.get());
		}

		~reference_handle()
		{
			reference_handle_map::instance().release(this->get_ptr());
		}

		/**
		* \brief	create a new reference_handle wrapping the same pointer as this reference_handle
		*/
		reference_handle<T>* new_reference_handle()
		{
			return new reference_handle<T>(get_ptr());
		}

		/** \brief casting operator (dynamic_cast) to a type U*. applies dynamic_cast to the inner wrapped pointer */
		template<typename U>
		explicit operator U*() const
		{
			return dynamic_cast<U*>(get_ptr());
		}

		/**
		 * \brief	Gets the number of reference_handle referencing the wrapped object.
		 *
		 * \return	Reference count.
		 */
		long count() const
		{
			return _shared_pointer.use_count() - 1;
		}

		/**
		 * \brief	Gets the "raw" pointer.
		 *
		 * \return	"raw" pointer to the object wrapped by this reference_handle
		 */
		T* get_ptr()
		{
			return _shared_pointer.get();
		}

		/**
		* \brief	Gets an opaque pointer to the object
		*
		* \return	"raw" pointer to the object wrapped by this reference_handle
		*/
		void * get_void_ptr()
		{
			return (void*)(get_ptr());
		}

	private:

		/**
		 * \brief	Creates shared pointer.
		 *
		 * \param [in]	p	opaque pointer to an object of type T to manage via shared pointer
		 *
		 * \return	A shared pointer. The shared pointer is created such that the object descructor ~T is called.
		 */
		static std::shared_ptr<void> create_shared_ptr(void* p)
		{
			// We cannot create a shared pointer of type shared_ptr<void> directly, otherwise at 
			// destruction time the destructor ~T would not be called. 
			// We need to create a shared_ptr<T> then to do a static pointer cast; the resulting 
			// shared_ptr<void> then 'remembers' the 'true' type and will dispose of the object properly
			T* typed_p = (T*)p;
			std::shared_ptr<T> ptr(typed_p);
			return std::static_pointer_cast<void>(ptr);
		}

		std::shared_ptr<T> find_shared_ptr(T* p)
		{
			return std::static_pointer_cast<T>(reference_handle_map::instance().get(p, create_shared_ptr));
		}
		std::shared_ptr<T> _shared_pointer;
	};

	/**
	* \brief	Try to downcast the inner pointer of the wrapper. Throw an exception if impossible. 
	*
	* \tparam	T	type parameter to downcast the inner pointer to (i.e. can it be cast to a T*) 
	*
	* \param	ptr_wrapper	wrapper of the raw pointer to test for downcasting.
	* \return	Downcast pointer to the object wrapped by ptr_wrapper.
	*/
	template <typename T>
	T* checked_downcast(opaque_ptr_provider* ptr_wrapper)
	{
		if (ptr_wrapper == nullptr)
			throw std::invalid_argument("The pointer to a reference handle is nullptr");
		T* result = dynamic_cast<T*>(ptr_wrapper);
		if (result == nullptr)
		{
			string expected(typeid(T).name());
			string actual(ptr_wrapper->wrapped_type_name());
			string errorMsg = string("Expected type ") + expected + ", but got an opaque pointer to a type " + actual;
			throw std::invalid_argument(errorMsg);
		}
		return result;
	}

	/**
	* \brief	Try to downcast the inner pointer of the wrapper, and return a new down-typed reference_handle. Throw an exception if impossible.
	*
	* \tparam	T	type parameter to downcast the inner pointer to (i.e. can it be cast to a T*)
	*
	* \param	ptr_wrapper	wrapper of the raw pointer to test for downcasting.
	* \return	new reference_handle around a downcast pointer to the object wrapped by ptr_wrapper.
	*/
	template <typename T>
	reference_handle<T>* checked_reference_handle(opaque_ptr_provider* ptr_wrapper)
	{
		return checked_downcast<reference_handle<T>>(ptr_wrapper);
	}

	/**
	* \brief	Unwrap a pointer wrapper, casting to a specific type the inner pointer
	*
	* \tparam	T	type parameter to cast the inner, raw pointer to (i.e. can it be cast to a T* )
	*
	* \param	ptr_wrappers	wrapper of the raw pointer to test for casting.
	*
	* \return	raw pointer of type T* to the object wrapped
	*/
	template <typename T>
	T* as_raw_pointer(opaque_ptr_provider* ptr_wrapper)
	{
		if (ptr_wrapper == nullptr)
			throw std::invalid_argument("Pointer is nullptr - not accepted by the API");
		reference_handle<T>* sp = dynamic_cast<reference_handle<T>*>(ptr_wrapper);
		if (sp != nullptr)
		{
			return sp->get_ptr();
		}
		else
		{
			cast_ptr_provider* cpp = dynamic_cast<cast_ptr_provider*>(ptr_wrapper);
			if (cpp == nullptr)
				throw std::invalid_argument("Opaque pointer wrapper is not a cast_ptr_provider - not accepted by the API");
			T* result = cpp->dynamic_cast_to<T>();
			if (result == nullptr)
			{
				string expected(typeid(T).name());
				string actual(ptr_wrapper->wrapped_type_name());
				string errorMsg = string("Cannot cast pointer to ") + actual + " to a pointer to " + expected;
				throw std::invalid_argument(errorMsg);
			}
			return result;
		}
	}

	/**
	* \brief	Unwrap several pointer wrappers, casting to a specific type each inner pointers.
	*
	* \tparam	T	type parameter to cast the inner, raw pointers to (i.e. can it be cast to a T* )
	*
	* \param	ptr_wrappers	array of wrappers of the raw pointers to test for casting.
	*
	* \return	raw pointers of type T* to each object wrapped
	*/
	template <typename T>
	T** as_raw_pointers(opaque_ptr_provider** ptr_wrappers, int n)
	{
		T** result = new T*[n];
		for (size_t i = 0; i < n; i++)
			result[i] = as_raw_pointer<T>(ptr_wrappers[i]);
		return result;
	}

}

#ifndef FORCE_OPAQUE_PTR_TYPECAST
#define CHECKED_RETRIEVE_PTR(T, x)    (moirai::as_raw_pointer<T>(x))
#define RETRIEVE_POINTERS_FROM_SHPTR(T, ptr_wrappers, n) moirai::as_raw_pointers<T>(ptr_wrappers, n)
#else
#define CHECKED_RETRIEVE_PTR(T, x)    (moirai::as_raw_pointer<T>((moirai::opaque_ptr_provider*)x))
#endif

