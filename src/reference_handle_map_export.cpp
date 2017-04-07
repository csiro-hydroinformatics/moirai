
#include "moirai/reference_handle_map_export.h"

#include "moirai/error_reporting.h"

using std::string;
using moirai::error_handling::error_log;

string error_log::last_exception_message = string("");
std::recursive_mutex error_log::mutex;
error_log::exception_callback error_log::callback = nullptr;

string error_log::get_last_exception_message()
{
	return last_exception_message;
}

void error_log::register_exception_callback(const void* callback, bool allow_override)
{
	if (callback == nullptr)
		error_log::callback = nullptr;
	else
	{
		if (error_log::callback != nullptr && !allow_override)
			throw std::logic_error("moirai::error_log already has an error handler set up!");
		else
			error_log::callback = (error_log::exception_callback) callback;
	}
}

