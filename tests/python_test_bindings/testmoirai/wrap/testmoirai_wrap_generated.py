##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

import xarray as xr
import pandas as pd
import numpy as np
from typing import TYPE_CHECKING, Dict, List, Tuple, Any, Optional
from datetime import datetime
from refcount.interop import CffiData, OwningCffiNativeHandle, DeletableCffiNativeHandle, wrap_as_pointer_handle
from cinterop.cffi.marshal import as_bytes, TimeSeriesGeometryNative
from testmoirai.wrap.ffi_interop import marshal, testmoirai_so
# phase out importing from testmoirai.classes, to prevent cyclic imports
# from testmoirai.classes import wrap_cffi_native_handle
# Additional specific imports for this package

import testmoirai.wrap.ffi_interop as _m_wrap


if TYPE_CHECKING:
    from testmoirai.classes import (
        Animal,
        Mamal,
        Dog,
        Cat,
        HomoErectus,
    )

    from refcount.interop import WrapperCreationFunction

__wrap_cffi_native_handle:Optional['WrapperCreationFunction']=None

def set_wrap_cffi_native_handle(wrapper_function:'WrapperCreationFunction'):
    global __wrap_cffi_native_handle
    __wrap_cffi_native_handle = wrapper_function

def custom_wrap_cffi_native_handle(obj, type_id='', release_native = None):
    '''Create a wrapper around a cffi pointer (if this is one), 
    with the suitable native release function call specific to this external pointer. 
    '''
    if __wrap_cffi_native_handle is None:
        raise RuntimeError('The function creating custom wrappers around native objects is None: you must use set_wrap_cffi_native_handle to initialise it')
    if release_native is None:
        release_native = dispose_shared_pointer_py
    return __wrap_cffi_native_handle(obj, type_id, release_native)

def char_array_to_py(values:CffiData, dispose:bool=True) -> str:
    pystring = marshal.c_string_as_py_string(values)
    if dispose:
        testmoirai_so.free_string(values)
    return pystring

def dispose_shared_pointer_py(ptr:Any) -> None:
    # Upon a process terminating, somehow wrap_as_pointer_handle can end up being None,
    # leading to a TypeError: 'NoneType' object is not callable.
    # This is a nuisance, and hard to fully diagnose.
    # So, we will use the following workaround to guard against it. See WIRADA-659.
    if wrap_as_pointer_handle is None:
        return
    ptr_xptr = wrap_as_pointer_handle(ptr)
    # Upon a process terminating, somehow 'testmoirai_so' can end up being None,
    # leading to a TypeError: 'NoneType' object is not callable.
    # This is a nuisance, and hard to fully diagnose.
    # So, we will use the following workaround to guard against it. See WIRADA-659.
    if testmoirai_so is not None: #  and testmoirai_so.release_handle is not None:
        testmoirai_so.release_handle(ptr_xptr.ptr)

@_m_wrap.check_exceptions
def _get_name_native(x):
    result = testmoirai_so.get_name(x)
    return result

def get_name_py(x:'Animal') -> str:
    """get_name_py
    
    get_name_py: generated wrapper function for API function get_name
    
    Args:
        x ('Animal'): x
    
    Returns:
        (str): returned result
    
    """
    x_xptr = wrap_as_pointer_handle(x)
    result = _get_name_native(x_xptr.ptr)
    return char_array_to_py(result, dispose=True)


@_m_wrap.check_exceptions
def _create_cat_native():
    result = testmoirai_so.create_cat()
    return result

def create_cat_py() -> 'Cat':
    """create_cat_py
    
    create_cat_py: generated wrapper function for API function create_cat
    
    Args:
    
    Returns:
        ('Cat'): returned result
    
    """
    result = _create_cat_native()
    return custom_wrap_cffi_native_handle(result, 'CAT_PTR')


@_m_wrap.check_exceptions
def _create_dog_native():
    result = testmoirai_so.create_dog()
    return result

def create_dog_py() -> 'Dog':
    """create_dog_py
    
    create_dog_py: generated wrapper function for API function create_dog
    
    Args:
    
    Returns:
        ('Dog'): returned result
    
    """
    result = _create_dog_native()
    return custom_wrap_cffi_native_handle(result, 'DOG_PTR')


@_m_wrap.check_exceptions
def _put_in_dog_kenel_native(d):
    result = testmoirai_so.put_in_dog_kenel(d)
    return result

def put_in_dog_kenel_py(d:'Dog') -> int:
    """put_in_dog_kenel_py
    
    put_in_dog_kenel_py: generated wrapper function for API function put_in_dog_kenel
    
    Args:
        d ('Dog'): d
    
    Returns:
        (int): returned result
    
    """
    d_xptr = wrap_as_pointer_handle(d)
    result = _put_in_dog_kenel_native(d_xptr.ptr)
    return result


@_m_wrap.check_exceptions
def _create_homo_erectus_native():
    result = testmoirai_so.create_homo_erectus()
    return result

def create_homo_erectus_py() -> 'HomoErectus':
    """create_homo_erectus_py
    
    create_homo_erectus_py: generated wrapper function for API function create_homo_erectus
    
    Args:
    
    Returns:
        ('HomoErectus'): returned result
    
    """
    result = _create_homo_erectus_native()
    return custom_wrap_cffi_native_handle(result, 'HOMO_ERECTUS_PTR')


@_m_wrap.check_exceptions
def _release_handle_native(ptr):
    testmoirai_so.release_handle(ptr)

def release_handle_py(ptr:Any) -> None:
    """release_handle_py
    
    release_handle_py: generated wrapper function for API function release_handle
    
    Args:
        ptr (Any): ptr
    
    """
    ptr_xptr = wrap_as_pointer_handle(ptr)
    _release_handle_native(ptr_xptr.ptr)


@_m_wrap.check_exceptions
def _release_handle_species_via_moirai_native(ptr):
    testmoirai_so.release_handle_species_via_moirai(ptr)

def release_handle_species_via_moirai_py(ptr:Any) -> None:
    """release_handle_species_via_moirai_py
    
    release_handle_species_via_moirai_py: generated wrapper function for API function release_handle_species_via_moirai
    
    Args:
        ptr (Any): ptr
    
    """
    ptr_xptr = wrap_as_pointer_handle(ptr)
    _release_handle_species_via_moirai_native(ptr_xptr.ptr)


@_m_wrap.check_exceptions
def _reference_count_native(ptr):
    result = testmoirai_so.reference_count(ptr)
    return result

def reference_count_py(ptr:Any) -> int:
    """reference_count_py
    
    reference_count_py: generated wrapper function for API function reference_count
    
    Args:
        ptr (Any): ptr
    
    Returns:
        (int): returned result
    
    """
    ptr_xptr = wrap_as_pointer_handle(ptr)
    result = _reference_count_native(ptr_xptr.ptr)
    return result


@_m_wrap.check_exceptions
def _create_mamal_ptr_native(cat):
    result = testmoirai_so.create_mamal_ptr(cat)
    return result

def create_mamal_ptr_py(cat:'Cat') -> 'Mamal':
    """create_mamal_ptr_py
    
    create_mamal_ptr_py: generated wrapper function for API function create_mamal_ptr
    
    Args:
        cat ('Cat'): cat
    
    Returns:
        ('Mamal'): returned result
    
    """
    cat_xptr = wrap_as_pointer_handle(cat)
    result = _create_mamal_ptr_native(cat_xptr.ptr)
    return custom_wrap_cffi_native_handle(result, 'MAMAL_PTR')


@_m_wrap.check_exceptions
def _register_exception_callback_function_native(callback):
    testmoirai_so.register_exception_callback_function(callback)

def register_exception_callback_function_py(callback:Any) -> None:
    """register_exception_callback_function_py
    
    register_exception_callback_function_py: generated wrapper function for API function register_exception_callback_function
    
    Args:
        callback (Any): callback
    
    """
    _register_exception_callback_function_native(callback)


@_m_wrap.check_exceptions
def _has_callback_registered_native():
    result = testmoirai_so.has_callback_registered()
    return result

def has_callback_registered_py() -> int:
    """has_callback_registered_py
    
    has_callback_registered_py: generated wrapper function for API function has_callback_registered
    
    Args:
    
    Returns:
        (int): returned result
    
    """
    result = _has_callback_registered_native()
    return result


@_m_wrap.check_exceptions
def _trigger_callback_native():
    testmoirai_so.trigger_callback()

def trigger_callback_py() -> None:
    """trigger_callback_py
    
    trigger_callback_py: generated wrapper function for API function trigger_callback
    
    Args:
    
    """
    _trigger_callback_native()


