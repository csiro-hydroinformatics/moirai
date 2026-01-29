#include <stdexcept>

#include "moirai/reference_handle_map_export.h"

#include "moirai/error_reporting.h"

using std::string;

namespace moirai {
	namespace error_handling {


		error_log::error_log()
		{
			last_exception_message = string("");
			callback = nullptr;
		}

		string error_log::get_last_exception_message()
		{
			return last_exception_message;
		}

		void error_log::register_exception_callback(const void* callback, bool allow_override)
		{
			if (callback == nullptr)
				this->callback = nullptr;
			else
			{
				if (this->callback != nullptr && !allow_override)
					throw std::logic_error("moirai::error_log already has an error handler set up!");
				else
					this->callback = (error_log::exception_callback) callback;
			}
		}

		bool error_log::has_callback_registered()
		{
			return (this->callback != nullptr);
		}

	}
}

void dispose_reference_handle(REFERENCE_HANDLE_PTR ptr)
{
	// Can't throw an exception in an external C function...
	//if (ptr == nullptr)
	//	throw std::invalid_argument("Moirai dispose_reference_handle called with a nullptr as an argument");
	if (ptr != nullptr)
		delete ptr;
}

int get_reference_count(REFERENCE_HANDLE_PTR ptr)
{
	return ptr->count();
}

// bool has_error_handling_callback_registered()
// {
// 	return error_log::has_callback_registered();
// }

// void register_error_handling_callback(const void* callback)
// {
// 	error_log::register_exception_callback(callback);
// }
