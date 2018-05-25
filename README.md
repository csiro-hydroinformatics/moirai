﻿MOIRAI: Manage C++ Objects's lifetime when exposed through a C API
=====================================================

Windows - master: [![Master build status](https://ci.appveyor.com/api/projects/status/8fobpshmwrmt3vmd/branch/master?svg=true)](https://ci.appveyor.com/project/jmp75/moirai/branch/master) devel: [![Devel build status](https://ci.appveyor.com/api/projects/status/8fobpshmwrmt3vmd/branch/devel?svg=true)](https://ci.appveyor.com/project/jmp75/moirai/branch/devel)


# Purpose

This C++ library is designed to help handling C++ objects from so called opaque pointers, via a C API. Prominent features are:

* Counting references via the C API to C++ domain objects (instead of having to do so in each high-level language wrapper)
* Handle C++ class inheritance even via opaque pointers
* Mechanism for resilience to incorrect type casts when unwrapping opaque pointers (i.e. get an exception, not a nasty crash...)
* thread-safe design

ANSI C code remains the most portable and least invasive way to access libraries in a binary compatible manner across compilers, mainly due to the incompatible name mangling schemes generated by C++ compilers. 

## License

This software is dual-licensed:
* [Affero GPLv3](https://www.gnu.org/licenses/agpl-3.0.en.html) for non-commercial purposes.
* Commercial license options can otherwise be arranged. The intent is to promote fair use and citations rather than expecting paid options, so please contact us (e-mail). 

Copyright 2017 CSIRO - Author: Jean-Michel Perraud

## Naming

In Greek mythology, the [Moirai](https://en.wikipedia.org/wiki/Moirai) or Moerae (Ancient Greek: Μοῖραι, "apportioners"), often known in English as the Fates (Latin: Fatae), were the white-robed incarnations of destiny. [...]. They controlled the mother thread of life of every mortal from birth to death.

# Background and rationale

Moirai has been used to expose C++ libraries via a C API so that they could be accessed by users more fluent in tools such as R, Python and Matlab(r). Moirai helps to track the lifetime of C++ objects handled from these languages. 

Moirai is not a wrapper generator. There are tools to generate wrappers around C/C++ code for use from these and other languages. The venerable SWIG is but one, and you should assess whether this is what you want, along with or instead of Moirai. They are quite different beasts and likely to be complementary. Moirai targets a specific need: object lifetime via opaque pointers in C API. But why would you design an "old style" C API to access your C++ objects?

As an example, R on Windows (using the Rcpp package) requires a version of the GNU g++ compiler that may not be suitable to the library. A "pure" C API is needed needed to be able to compile the R wrappers with GCC and still use another compiler such as Visual C++ for the core library.

_Placeholder_: Elicit product segmentation and where Moirai Fits. Basically few contenders. std::shared_ptr and co actually not enough.

# Requirements

This should be usable by any recent C++ compiler. This has been used by projects using gcc 4.8 and above, and Microsoft Visual C++ 2013 and above.

Unit tests use the ["Catch" unit testing framework](https://github.com/philsquared/Catch).

# Getting started

You may have a look at the unit tests under the 'tests' folder, in particular moirai_test_api and moirai_test_lib which are the closest to a typical use case. moirai_test is the next level you'll then want to look at.

You can follow [a walkthrough](./doc/Walkthrough.md) derived from the unit tests.

# Compiling

`moirai` is largely header-only for use in your projects, however there is a compiled shared library, and this is a deliberate design choice.

## Windows

You can use the solution file under the `tests` folder. In order to compile the unit tests, you will need to use the template file `moirai.props.in`, copy it to your `Documents` folder (a.k.a. 'My Documents') e.g. to `C:\Users\username\Documents\moirai.props`. You likely need to adapt the line `<Moirai_IncludePath>C:/local/include</Moirai_IncludePath>`. This should be the folder where `catch\catch.hpp` is located (see Requirements). Leave other lines in `moirai.props` unchanged.

## Linux

```sh
INSTALL_PREFIX=/usr/local # adapt
GITHUB_REPOS=~/src/github_jm # adapt

CLEAN_BUILD="rm -rf ../build/*"

CXX_COMP=-DCMAKE_CXX_COMPILER=g++
C_COMP=-DCMAKE_C_COMPILER=gcc
BUILD_CONFIG="-DCMAKE_BUILD_TYPE=Release"
# BUILD_CONFIG="-DCMAKE_BUILD_TYPE=Debug"

CM="cmake ${CXX_COMP} ${C_COMP} ${BUILD_CONFIG} -DBUILD_SHARED_LIBS=ON .."
SUDO_CMD="sudo"
cd ${GITHUB_REPOS}/moirai
mkdir -p build ; cd build
$CLEAN_BUILD
$CM
make
./moirai_test
./moirai_test_api

$SUDO_CMD make install
```

# Related work

I found out the [Loki](http://loki-lib.sourceforge.net/html/main.html) library. Nifty work, but a much larger and possibly different scope, more complicated. I have not thoroughly tasted (sic).

Moirai is not an API bindings generator. [SWIG](http://swig.org) is. That said I assessed for most C API I have designed that I was [better off with my own](https://github.com/jmp75/rcpp-wrapper-generation), though.

