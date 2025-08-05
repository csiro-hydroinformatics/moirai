import os
import sys

import testmoirai.wrap.testmoirai_wrap_generated as twg

pkg_dir = os.path.join(os.path.dirname(__file__), "..")

sys.path.append(pkg_dir)

from testmoirai.classes import *

# Note that these tests are very basic from a python interop perspective.
# more elaborate ones are in the refcount package


def test_basic_obj_lifetime():
    import gc

    cat = Cat.new()
    dog = Dog.new()

    cn = twg.get_name_py(cat)
    dn = twg.get_name_py(dog)
    assert cn == "cat"
    assert dn == "dog"
    del cat
    del dog
    gc.collect()


def test_reference_count():
    import gc

    cat = Cat.new()
    assert twg.reference_count_py(cat) == 1
    mamal = twg.create_mamal_ptr_py(cat)
    assert twg.reference_count_py(cat) == 2
    cat = None
    del cat
    gc.collect()
    assert twg.reference_count_py(mamal) == 1
    # del(mamal)


def test_callback_error_handling():
    import pytest
    from testmoirai.wrap.ffi_interop import PyMoiraiError

    assert twg.has_callback_registered_py() > 0
    with pytest.raises(PyMoiraiError):
        twg.trigger_callback_py()
    try:
        twg.trigger_callback_py()
    except PyMoiraiError as e:
        assert str(e) == "Error message sent from C++"


if __name__ == "__main__":
    test_callback_error_handling()
