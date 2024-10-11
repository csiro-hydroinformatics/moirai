"""
    Wrapper around the testmoirai C API functions using CFFI.
"""
from functools import wraps
from cffi import FFI
import os

from typing import List, Dict, Any
from refcount.putils import library_short_filename, update_path_windows, build_new_path_env
import pandas as pd

from refcount.interop import OwningCffiNativeHandle, CffiNativeHandle
from cinterop.cffi.marshal import CffiMarshal


testmoirai_ffi = FFI()
here = os.path.abspath(os.path.dirname(__file__))
testmoirai_pkg_dir = os.path.join(here,'..')
cdefs_dir = os.path.join(testmoirai_pkg_dir, 'data')
assert os.path.exists(cdefs_dir)

with open(os.path.join(cdefs_dir, 'structs_cdef.h')) as f_headers:
    testmoirai_ffi.cdef(f_headers.read())

with open(os.path.join(cdefs_dir, 'funcs_cdef.h')) as f_headers:
    testmoirai_ffi.cdef(f_headers.read())

short_fname = library_short_filename('moirai_test_lib')
from pathlib import Path
t_pkg_dir=Path(testmoirai_pkg_dir)


import sys

tmp_env_var='LIBRARY_PATH_TMP'
if(sys.platform == 'linux'):
    to_env = 'LIBRARY_PATH'
    os.environ[tmp_env_var]=str(t_pkg_dir / "../../../build/" )
    os.environ[to_env] = build_new_path_env(tmp_env_var, to_env, sys.platform)
    long_fname = short_fname
elif(sys.platform == 'win32'):
    to_env = 'PATH'
    os.environ[to_env] = prepend_path_env([libs_path], to_env)
    update_path_windows(from_env='LIBRARY_PATH', to_env='PATH')
    long_fname = short_fname
testmoirai_so = testmoirai_ffi.dlopen(long_fname, 1) # Lazy loading

marshal = CffiMarshal(testmoirai_ffi)
class PyMoiraiError(Exception):
    """ Exception when calling a testmoirai function. """
    def __init__(self, message):
        super(PyMoiraiError, self).__init__(message)


# This will store the exception message raised by testmoirai
_exception_txt_raised_testmoirai = None

class NativeException(Exception):
    """ Raised when a call to a native library raised an error via a python callback function """
    def __init__(self, message):
        super(NativeException, self).__init__(message)

@testmoirai_ffi.callback("void(char *)")
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
            raise PyMoiraiError(temp_exception)
        return return_value
    return wrapper


testmoirai_so.RegisterExceptionCallback(_exception_callback_testmoirai)


@check_exceptions
def testmoirai_dispose_multi_time_series_data(data):
    """
        :param ptr data: Pointer to a MultiTimeSeriesData.
    """
    testmoirai_so.DisposeMultiTimeSeriesData(data)

