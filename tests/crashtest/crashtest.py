
import sys
import os
print(os.environ["PATH"])

"""
Wrapper around the testmoirai C API functions using CFFI.
"""

import os
import sys
from functools import wraps
from pathlib import Path

from cffi import FFI
from cinterop.cffi.marshal import CffiMarshal
from refcount.putils import augment_path_env, build_new_path_env, library_short_filename

testmoirai_ffi = FFI()
# here = os.path.abspath(os.path.dirname(__file__))
here = r"C:\src\moirai\tests\python_test_bindings\testmoirai\wrap"
testmoirai_pkg_dir = os.path.join(here, "..")
cdefs_dir = os.path.join(testmoirai_pkg_dir, "data")
assert os.path.exists(cdefs_dir)

with open(os.path.join(cdefs_dir, "structs_cdef.h")) as f_headers:
    testmoirai_ffi.cdef(f_headers.read())

with open(os.path.join(cdefs_dir, "funcs_cdef.h")) as f_headers:
    testmoirai_ffi.cdef(f_headers.read())

short_fname = library_short_filename("moirai_test_lib")

t_pkg_dir = Path(testmoirai_pkg_dir)


tmp_env_var = "LIBRARY_PATH_TMP"
if sys.platform == "linux":
    to_env = "LIBRARY_PATH"
    build_dir = t_pkg_dir / "../../../build/"
    os.environ[tmp_env_var] = str(build_dir)
    os.environ[to_env] = build_new_path_env(tmp_env_var, to_env, sys.platform)
    long_fname = str(build_dir / short_fname)
elif sys.platform == "win32":
    build_dir = t_pkg_dir / ".." / ".." / "x64" / "Debug"
    print(build_dir)
    assert build_dir.exists()
    new_paths = augment_path_env(str(build_dir), None, "PATH", prepend=True)
    os.environ["PATH"] = new_paths
    long_fname = short_fname

print(f"testmoirai_ffi.RTLD_NOW: {testmoirai_ffi.RTLD_NOW}")
testmoirai_so = testmoirai_ffi.dlopen(long_fname, 1)  # Lazy loading

marshal = CffiMarshal(testmoirai_ffi)


class PyMoiraiError(Exception):
    """Exception when calling a testmoirai function."""

    def __init__(self, message):
        super(PyMoiraiError, self).__init__(message)


# This will store the exception message raised by testmoirai
_exception_txt_raised_testmoirai = None


def last_error_message_from_c():
    global _exception_callback_testmoirai
    return _exception_callback_testmoirai


class NativeException(Exception):
    """Raised when a call to a native library raised an error via a python callback function"""

    def __init__(self, message):
        super(NativeException, self).__init__(message)


@testmoirai_ffi.callback("void(const char *)")
def _exception_callback_testmoirai(exception_string):
    """
    This function is called when testmoirai raises an exception.
    It sets the global variable ``_exception_txt_raised_testmoirai``

    :param cdata exception_string: Exception string.
    """
    global _exception_txt_raised_testmoirai
    _exception_txt_raised_testmoirai = testmoirai_ffi.string(exception_string)


def check_exceptions(func):
    """
    Returns a wrapper that raises a Python exception if a testmoirai exception
    occured.
    """

    @wraps(func)
    def wrapper(*args, **kwargs):
        """
        This decorator will first call the function ``func``
        After that it will raise a Python PyMoiraiError exception if the
        global variable ``_exception_txt_raised_testmoirai`` is set.

        :param func func: Python function wrapping a testmoirai function.
        """
        # log_func_call(func, *args, **kwargs)
        # Call the function
        return_value = func(*args, **kwargs)
        # Check if an exception was raised
        global _exception_txt_raised_testmoirai
        if _exception_txt_raised_testmoirai is not None:
            temp_exception = _exception_txt_raised_testmoirai
            _exception_txt_raised_testmoirai = None
            if isinstance(temp_exception, bytes):
                temp_exception = temp_exception.decode("utf-8")
            raise PyMoiraiError(temp_exception)
        return return_value

    return wrapper


# testmoirai_so.register_exception_callback_function(_exception_callback_testmoirai)
