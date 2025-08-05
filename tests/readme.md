
## Debugging issue number 1

Assumption: I have installed a `pyenv` for windows for the related project `py-cffi-callback-repro`. Let us create a virtual environment for `moirai` to try to diabnose and address the issue. 

```bat
set user_name=per202
pyenv --version
python --version
cd C:\Users\%user_name%\
:: mkdir .venv
cd .venv
python -m venv moirai

.\moirai\Scripts\activate
pip install pytest wheel twine
:: to be able to attach to the process, perhaps
pip install jupyterlab 
python -m ipykernel install --user --name moirai --display-name "moirai"

cd c:\src\moirai\tests\python_test_bindings
pip install -e .
```

```sh
pip install xarray pandas refcount cinterop
```

Associate the new environment with the project in vscode.

```bat
set PATH=C:\src\moirai\tests\x64\Debug;%PATH%
python
```

```python
import testmoirai.wrap.ffi_interop_stub as f
# Attach visuial studio cpp debugger
f.testmoirai_so.register_exception_callback_function(f._exception_callback_testmoirai)
```


## Claude 3.5 tells me

Based on the information provided, there are a few potential reasons why this access violation is occurring:

1. Callback function type mismatch:
   The C++ code defines the callback as `typedef void(*exception_callback)(const char * str);`
   But the Python code uses `@testmoirai_ffi.callback("void(char *)")`. 
   The `const` qualifier is missing in the Python definition. This could potentially cause issues.

2. Thread safety:
   While the `error_log` class uses a mutex, the `register_exception_callback_function` is not thread-safe. If multiple threads are trying to register callbacks simultaneously, it could lead to race conditions.

3. Memory management:
   The `register_exception_callback` function takes a `const void*`, but it's unclear how this is being stored and managed. If the Python callback object is garbage collected, it could lead to a dangling pointer.

4. FFI configuration:
   The FFI setup might not be correctly defining the function signatures, leading to misaligned memory access.

5. DLL loading issues:
   The lazy loading of the DLL (`testmoirai_ffi.dlopen(long_fname, 1)`) might be causing issues if the DLL is not fully initialized when the callback is registered.

6. Windows-specific issues:
   Since this only happens on some Windows platforms, it could be related to specific Windows versions, security settings, or memory management differences.

To diagnose and potentially fix the issue:

1. Ensure the callback function signature matches exactly between C++ and Python.

2. Add more error checking and logging in both C++ and Python code.

3. Try eager loading of the DLL instead of lazy loading.

4. Double-check that the Python callback object remains alive for the duration of the program.

5. Implement proper thread-safety in the `register_exception_callback_function`.

6. Use a debugger to step through the C++ code and see exactly where the access violation occurs.

7. Consider using a tool like AddressSanitizer to detect memory issues.

Without more detailed debugging information, it's difficult to pinpoint the exact cause. However, these suggestions should help narrow down the problem.

### Trying

To diagnose and potentially fix the issue:

> Ensure the callback function signature matches exactly between C++ and Python.

I don't think `const` is a C99 concept, this is C++. Still trying to set this up in the signature for the callbacks for `cffi`; seems to not make a difference.

> Add more error checking and logging in both C++ and Python code.

Bit generic. I can (if VS2022 behaves) catch the exception in C++ land, and the call stack seems to be quite clear where this bombs (also there is the `next statement to be executed` explanation in mouse hover that is a bit confusing)

> Try eager loading of the DLL instead of lazy loading.

This is one I have big hopes for. Normally lazy loading is about C functions, not the static implementation of global object/variables declaration, see [dlopen man page](https://man7.org/linux/man-pages/man3/dlopen.3.html)

> Double-check that the Python callback object remains alive for the duration of the program.

> Implement proper thread-safety in the `register_exception_callback_function`.

> Use a debugger to step through the C++ code and see exactly where the access violation occurs.

> Consider using a tool like AddressSanitizer to detect memory issues.

Without more detailed debugging information, it's difficult to pinpoint the exact cause. However, these suggestions should help narrow down the problem.