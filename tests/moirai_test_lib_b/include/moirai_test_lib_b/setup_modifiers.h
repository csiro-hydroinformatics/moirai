#pragma once

// Defining the DOMAIN_B_API macro following guidelines in e.g. https://msdn.microsoft.com/en-us/library/8fskxacy.aspx

// The declaration modifiers can be overriden by the user by defining DOMAIN_B_LIB_MODIFIERS
#ifndef DOMAIN_B_LIB_MODIFIERS

// We use __declspec on Windows. On other platforms, at least linux, no need to do so
// This is practically probably not necessary anymore to condition on _WIN32, but as __declspec remains flagged as Microsoft specific let's do so.
#ifdef _WIN32
#ifdef USING_DOMAIN_B_API
#define DOMAIN_B_API __declspec(dllimport)
#define MOIRAI_TEST_B_CLASSES_ATTR __declspec(dllimport)
#else
#define DOMAIN_B_API __declspec(dllexport)
#define MOIRAI_TEST_B_CLASSES_ATTR __declspec(dllexport)
// To prevent warnings such as:
// Warning	C4251	'YourClass' : class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' needs to have dll - interface to be used by clients of class 'Blah'
#pragma warning (disable : 4251)
#endif
#else // #ifdef _WIN32
// define the macro as 'nothing'
#define DOMAIN_B_API
#endif // #ifdef _WIN32

#else  //  DOMAIN_B_LIB_MODIFIERS is defined
#define DOMAIN_B_API DOMAIN_B_LIB_MODIFIERS 
#endif 


