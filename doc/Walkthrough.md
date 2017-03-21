MOIRAI - a walkthrough
========================

Let's say you have a c++ library that includes objects such as follows. The example is contrived for the sake of didactic goals.

```c++
	class animal
	{
	// member declarations
	};
	class mamal : public animal
	{		// member declarations
	};
	class dog : public mamal
	{		// member declarations
	};
```

You want to access it from .NET, and use the following objects to do so. Note that while the C# class structure closely mirrors the C++ one there is no intrinsic obligation for it to be so.

```c#
    public abstract class Animal
    {
        public abstract string GetName();
        public abstract string GetAction();
    }

    public abstract class Mamal : Animal
    {
    }

    public class Cat : Mamal
    {
        public Cat()
        {
            // create c++ cat
        }

        public override string GetAction()
        {
            throw new NotImplementedException();
        }

        public override string GetName()
        {
            throw new NotImplementedException();
        }
        // other stuff

    }
```

# Designing a C API

## Think about your concepts

Moirai is useful in cases where want to expose some key C++ objects via a C API. If you envisage you'll need a very long list of C API functions to reach the level of features you want, you may need to rethink whether a C++ API is not preferable (despite other negative tradeoff aspects previously mentioned). Conversely if you can get away with purely non-void ANSI C types in and out, you do not need Moirai, but chances are you had good reasons to look at this present document in the first place.

## Defining the use cases and test cases

TODO: proselytize

# Implementing the C API

Functions for the C API could look like follows when declared and seen within the C++ library:

```c++
	char* get_name(animal* x);
	cat* create_cat();
```

However things like 'animal' have no intrinsic reference counting. The calling code would need to do it all. Moirai does this via the C API by non-intrusively wrapping objects such as an 'animal':

```c++
	char* get_name( reference_handle<animal>* x);
	reference_handle<cat>* create_cat();
```

It is a C API in the sense that from the outside, the code using the library should see:

```c++
	char* get_name(void* x);
	void* create_cat();
```

While you could have two mirorring header files, this is obviously not recommended practice. Macros can be used in this context. Say you declare your API in a file `c_interop_api.h`. You may want to include Moirai's `setup_modifiers.h`. A possible pattern is as follows, so that users of your C API just declare `#define USING_SPECIES_API` in their code.

```c++
#ifdef USING_SPECIES_API
#define USING_MOIRAI
#endif
// USING_MOIRAI defines on what side of the API we are
#include "moirai/setup_modifiers.h"
// You may want to use a specific name for export attribute modifiers for your API
#define SPECIES_API MOIRAI_API
```

Now your API header file can declare macros to refer to the C++ classes as opaque pointers or reference_handle<>:

```c++
#if defined(USING_MOIRAI)
// this file is included by code that uses the C API
#define ANIMAL_PTR void*
// etc. 
#define VOID_PTR_PROVIDER_PTR void*
#else
// this file is included by code that defines a C API
#include "moirai/reference_handle.hpp"
#include "species.h"
using namespace moirai;
using namespace moirai::tests;
#define ANIMAL_PTR reference_handle<animal>* 
// etc. 
#endif
```

So that you can declare your API functions as:

```c++
#ifdef __cplusplus
extern "C" {
#endif
	SPECIES_API char* get_name(ANIMAL_PTR x);
	SPECIES_API CAT_PTR create_cat();
	// etc. 
#ifdef __cplusplus
}
#endif
```

# Creating the language bindings

TODO

## Manual / codegen


# Illustrate class hierarchy

TODO API functions that accept "abstract" types.

# Illustrate thread safety

TODO don't know. hard to illustrate simply.
