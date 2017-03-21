#pragma once

#include <string>
#include <mutex>
#include "moirai/setup_modifiers.h"

class MOIRAI_API error_log
{
public:
	typedef void(*exception_callback)(const char * str);
	//typedef void(__stdcall * exception_callback)(const char * str);
	template<typename TException>
	static void handle_std_exception(const TException& e);
	static std::string last_exception_message;
	static exception_callback callback;

private:
	// We will make the error_log within the API single-threaded to prevent concurrent messages
	// R and probably Python would bomb if more than one error is reported concurrently.
	// The mutex cannot be used in the R package itself (too old a G++ in RTools), so we may as well 
	// implement it here where we can use VCPP.
	static std::recursive_mutex mutex;

};

template<typename TException>
void error_log::handle_std_exception(const TException& e)
{
	std::lock_guard<std::recursive_mutex> guard(mutex);
	last_exception_message = string(e.what());
	if (callback != nullptr)
		(*callback)(last_exception_message.c_str());
	else
		throw e;
}
