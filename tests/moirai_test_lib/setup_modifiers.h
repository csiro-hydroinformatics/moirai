#pragma once

#define USING_MOIRAI
#include "moirai/setup_modifiers.h"

// Defining the SPECIES_API macro following guidelines in e.g. https://msdn.microsoft.com/en-us/library/8fskxacy.aspx

// The declaration modifiers can be overriden by the user by defining SPECIES_LIB_MODIFIERS
#ifndef SPECIES_LIB_MODIFIERS

// We use __declspec on Windows. On other platforms, at least linux, no need to do so
// This is practically probably not necessary anymore to condition on _WIN32, but as __declspec remains flagged as Microsoft specific let's do so.
#ifdef _WIN32
#ifdef USING_SPECIES_API
#define SPECIES_API __declspec(dllimport)
#else
#define SPECIES_API __declspec(dllexport)
// To prevent warnings such as:
// Warning	C4251	'YourClass' : class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' needs to have dll - interface to be used by clients of class 'Blah'
#pragma warning (disable : 4251)
#endif
#else // #ifdef _WIN32
// define the macro as 'nothing'
#define SPECIES_API
#endif // #ifdef _WIN32

#else  //  SPECIES_LIB_MODIFIERS is defined
#define SPECIES_API SPECIES_LIB_MODIFIERS 
#endif 


