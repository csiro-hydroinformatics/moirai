﻿# MOIRAI: Manage C++ Objects's lifetime when exposed through a C API

<a href="https://scan.coverity.com/projects/csiro-hydroinformatics-moirai">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/27474/badge.svg"/>
</a>

Windows - master: [![Master build status](https://ci.appveyor.com/api/projects/status/8fobpshmwrmt3vmd/branch/master?svg=true)](https://ci.appveyor.com/project/jmp75/moirai/branch/master) testing: [![testing build status](https://ci.appveyor.com/api/projects/status/8fobpshmwrmt3vmd/branch/testing?svg=true)](https://ci.appveyor.com/project/jmp75/moirai/branch/testing)

Linux - master: [![Linux master Build Status](https://travis-ci.org/csiro-hydroinformatics/moirai.svg?branch=master "Linux master Build Status")](https://travis-ci.org/csiro-hydroinformatics/moirai/builds) testing: [![Linux testing Build Status](https://travis-ci.org/csiro-hydroinformatics/moirai.svg?branch=testing "Linux testing Build Status")](https://travis-ci.org/csiro-hydroinformatics/moirai/builds)

## Purpose

This C++ library is designed to help handling C++ objects from so-called opaque pointers, via a C API. Prominent features are:

* counting references via the C API to C++ domain objects (instead of having to do so in each high-level language wrapper)
* handle C++ class inheritance even via opaque pointers
* mechanism for resilience to incorrect type casts when unwrapping opaque pointers (i.e. get an exception, not a nasty crash...)
* thread-safe design

ANSI C code remains the most portable and least invasive way to access libraries in a binary compatible manner across compilers, mainly due to the incompatible name mangling schemes generated by C++ compilers.

## Related resources and use cases

`moirai` is agnostic to the technology binding to the C++/C API. The repository [c-interop](https://github.com/csiro-hydroinformatics/c-interop) features more advanced interoperability material with bingings in python, matlab and R. The python package [`refcount`](https://github.com/csiro-hydroinformatics/pyrefcount) also features in the python bindings.

`moirai` features in the library [uchronia](https://github.com/csiro-hydroinformatics/uchronia-time-series/) for time series handling for ensemble simulations and forecasts in C++.

## License

MIT-derived CSIRO license (see [License.txt](./LICENSE.txt))

## Naming

In Greek mythology, the [Moirai](https://en.wikipedia.org/wiki/Moirai) or Moerae (Ancient Greek: Μοῖραι, "apportioners"), often known in English as the Fates (Latin: Fatae), were the white-robed incarnations of destiny. [...]. They controlled the mother thread of life of every mortal from birth to death.

## Requirements

This should be usable by any recent C++ compiler. This has been used by projects using gcc 4.8 and above, and Microsoft Visual C++ 2013 and above.

Unit tests use the ["Catch" unit testing framework](https://github.com/philsquared/Catch).

## Getting started

You may have a look at the unit tests under the 'tests' folder, in particular `moirai_test_api` and `moirai_test_lib` which are the closest to a typical use case. moirai_test is the next level you'll then want to look at.

You can follow [a walkthrough](./doc/Walkthrough.md) derived from the unit tests.

## Compiling

`moirai` is largely header-only for use in your projects, however there is a compiled shared library, and this is a deliberate design choice.

### Windows

You can use the solution file under the `tests` folder. In order to compile the unit tests, you will need to use the template file `moirai.props.in`, copy it to your `Documents` folder (a.k.a. 'My Documents') e.g. to `C:\Users\username\Documents\moirai.props`. You likely need to adapt the line `<Moirai_IncludePath>C:/local/include</Moirai_IncludePath>`. This should be the folder where `catch\catch.hpp` is located (see Requirements). Leave other lines in `moirai.props` unchanged.

### Linux

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

It is possible to uninstall using:

```sh
$SUDO_CMD make uninstall
```

## Background and rationale

Moirai has been used to expose C++ libraries via a C API so that they could be accessed by users more fluent in tools such as R, Python and Matlab(r). Moirai helps to track the lifetime of C++ objects handled from these languages.

Moirai is not a wrapper generator. There are tools to generate wrappers around C/C++ code for use from these and other languages. The venerable SWIG is but one, and you should assess whether this is what you want, along with or instead of Moirai. They are quite different beasts and likely to be complementary. Moirai targets a specific need: object lifetime via opaque pointers in C API. But why would you design an "old style" C API to access your C++ objects?

As an example, R on Windows (using the Rcpp package) requires a version of the GNU g++ compiler that may not be suitable to the library. A "pure" C API is needed needed to be able to compile the R wrappers with GCC and still use another compiler such as Visual C++ for the core library.

### Related third party work

I found out the [Loki](http://loki-lib.sourceforge.net/html/main.html) library. Nifty work, but a much larger and possibly different scope, more complicated. I have not thoroughly tasted (sic).

Moirai is not an API bindings generator. [SWIG](http://swig.org) is. That said I assessed for most C API I have designed that I was [better off with my own](https://github.com/csiro-hydroinformatics/c-api-wrapper-generation), though.

