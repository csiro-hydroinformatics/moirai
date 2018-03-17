#pragma once

#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <functional>

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
		reference_handle(const T& object) : cast_ptr_provider(typeid(T))
		{
			T* p = new T(object);
			sharedPtr = find_shared_ptr(p);
		}

		reference_handle(T* p) : cast_ptr_provider(typeid(T))
		{
			if (p == nullptr)
				throw std::invalid_argument("Pointer must not be nullptr");
			sharedPtr = find_shared_ptr(p);
		}

		reference_handle(const reference_handle<T>& ptr) : cast_ptr_provider(typeid(T))
		{
			sharedPtr = find_shared_ptr(ptr.sharedPtr.get());
		}

		~reference_handle()
		{
			reference_handle_map::instance().release(this->get_ptr());
		}

		reference_handle<T>* new_reference_handle()
		{
			return new reference_handle<T>(get_ptr());
		}

		/** \brief	T* casting operator. */
		//explicit operator T*() 
		//{ 
		//	return get_ptr(); 
		//}

		template<typename U>
		explicit operator U*() const
		{
			return dynamic_cast<U*>(get_ptr());
		}

		//template<typename U>
		//bool can_cast_to()
		//{
		//	return std::is_convertible<T, U>::value;
		//}

		/**
		 * \brief	Gets the number of reference_handle referencing the wrapped object.
		 *
		 * \return	Reference count.
		 */
		long count() const
		{
			return sharedPtr.use_count() - 1;
		}

		/**
		 * \brief	Gets the "raw" pointer.
		 *
		 * \return	"raw" pointer to the object wrapped by this reference_handle
		 */
		T* get_ptr()
		{
			return sharedPtr.get();
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

		string get_status()
		{
			string tname(this->wrapped_type_name());
			long c = this->count();
			string msg("Reference handle for ");
			msg += tname;
			msg += string(". Reference count is ") + std::to_string(c);

			string tryRetrieve = ". Raw pointer appears valid.";

			try
			{
				T* ptr = this->get_ptr();
				T blah = *ptr;
			}
			catch (std::exception& e)
			{
				tryRetrieve = string("Error trying to retrieve 'raw' pointer: ");
				tryRetrieve += string(e.what());
			}
			return msg + tryRetrieve;
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

		//template <typename U>
		//struct DefaultDeleter {
		//	void operator()(T* t) { delete static_cast<U*>(t); }
		//};
		std::shared_ptr<T> find_shared_ptr(T* p)
		{
			return std::static_pointer_cast<T>(reference_handle_map::instance().get(p, create_shared_ptr));
		}
		std::shared_ptr<T> sharedPtr;
	};

	template <typename T>
	T* checked_downcast(opaque_ptr_provider* sharedPtr)
	{
		if (sharedPtr == nullptr)
			throw std::invalid_argument("The pointer to a reference handle is nullptr");
		T* result = dynamic_cast<T*>(sharedPtr);
		if (result == nullptr)
		{
			string expected(typeid(T).name());
			string actual(sharedPtr->wrapped_type_name());
			string errorMsg = string("Expected type ") + expected + ", but got an opaque pointer to a type " + actual;
			throw std::invalid_argument(errorMsg);
		}
		return result;
	}

	template <typename T>
	reference_handle<T>* checked_reference_handle(opaque_ptr_provider* sharedPtr)
	{
		return checked_downcast<reference_handle<T>>(sharedPtr);
	}

	template <typename T>
	T** as_raw_pointers(opaque_ptr_provider** sharedPtrs, int n)
	{
		T** result = new T*[n];
		for (size_t i = 0; i < n; i++)
			result[i] = (T*)sharedPtrs[i]->get_void_ptr(); // TODO: replace this cast with something more resilient. 
		return result;
	}

	/**
	 * \fn	template <typename T> T* as_raw_pointer(opaque_ptr_provider* sharedPtr)
	 *
	 * \brief	Gets the raw pointer that is wrapped by the opaque pointer
	 *
	 * \exception	std::invalid_argument	Thrown when an invalid argument error condition occurs.
	 *
	 * \tparam	T	Generic type parameter, the type of the inner C++ object wrapped.
	 * \param [in,out]	sharedPtr	the opaque pointer, wrapper around a (pointer to) a C++ object
	 *
	 * \return	a pointer to a T object wrapped by the opaque pointer.
	 */

	template <typename T>
	T* as_raw_pointer(opaque_ptr_provider* sharedPtr)
	{
		if (sharedPtr == nullptr)
			throw std::invalid_argument("Pointer is nullptr - not accepted by the API");
		reference_handle<T>* sp = dynamic_cast<reference_handle<T>*>(sharedPtr);
		if (sp != nullptr)
		{
			try
			{
				return sp->get_ptr();
			}
			catch (std::exception& e) 
			{ 
				string tname(sp->wrapped_type_name());
				string msg("Error trying to retrieve typed 'raw' pointer to object of type ");
				msg = msg + tname + string(". ");
				msg = msg + string(e.what());
				throw std::invalid_argument(msg);
			}
		}
		else
		{
			cast_ptr_provider* cpp = dynamic_cast<cast_ptr_provider*>(sharedPtr);
			if (cpp == nullptr)
				throw std::invalid_argument("Opaque pointer wrapper is not a cast_ptr_provider - not accepted by the API");
			T* result = cpp->dynamic_cast_to<T>();
			if (result == nullptr)
			{
				string expected(typeid(T).name());
				string actual(sharedPtr->wrapped_type_name());
				string errorMsg = string("Cannot cast pointer to ") + actual + " to a pointer to " + expected;
				throw std::invalid_argument(errorMsg);
			}
			return result;
		}
	}

	char* inspect_reference_state(opaque_ptr_provider* sharedPtr)
	{
		if (sharedPtr == nullptr)
			throw std::invalid_argument("Pointer is nullptr - not accepted by the API");
		string tname(sharedPtr->wrapped_type_name());
		void* p = sharedPtr->get_void_ptr();
		long c = sharedPtr->count();
	}
}

#ifndef FORCE_OPAQUE_PTR_TYPECAST
#define CHECKED_RETRIEVE_PTR(T, x)    (moirai::as_raw_pointer<T>(x))
#define RETRIEVE_POINTERS_FROM_SHPTR(T, sharedPtrs, n) moirai::as_raw_pointers<T>(sharedPtrs, n)
#else
#define CHECKED_RETRIEVE_PTR(T, x)    (moirai::as_raw_pointer<T>((moirai::opaque_ptr_provider*)x))
#endif

