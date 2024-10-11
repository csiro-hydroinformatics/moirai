"""Pythonic classes accessing underlying C++ objects functionalities
"""

from typing import Any, Callable, List, TYPE_CHECKING, Union
from cffi import FFI
from refcount.interop import DeletableCffiNativeHandle, CffiData, CffiWrapperFactory

import testmoirai.wrap.testmoirai_wrap_generated as twg

from datetime import datetime, timedelta
import pandas as pd

class Animal(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(Animal, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )

class Mamal(Animal):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(Mamal, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )


class Dog(Mamal):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(Dog, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )
    @staticmethod
    def new():
        return twg.create_dog_py()

class Cat(Mamal):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(Cat, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )
    @staticmethod
    def new():
        return twg.create_cat_py()

class HomoErectus(Mamal):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(HomoErectus, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )
    @staticmethod
    def new():
        return twg.create_homo_erectus_py()



_api_type_wrapper = {
    "ANIMAL_PTR": Animal,
    "MAMAL_PTR": Mamal,
    "DOG_PTR": Dog,
    "CAT_PTR": Cat,
    "HOMO_ERECTUS_PTR": HomoErectus,
}

_STRICT_WRAPPING = False

_wrapper_factory = CffiWrapperFactory(_api_type_wrapper, _STRICT_WRAPPING)


def _create_wrapper(obj: Any, type_id: str, release_native: Callable):
    return _wrapper_factory.create_wrapper(obj, type_id, release_native)


def wrap_cffi_native_handle(obj: Any, type_id: str, release_native: Callable):
    if not isinstance(obj, FFI.CData):
        return obj
    return _create_wrapper(obj, type_id, release_native)


twg.set_wrap_cffi_native_handle(wrapper_function=wrap_cffi_native_handle)
