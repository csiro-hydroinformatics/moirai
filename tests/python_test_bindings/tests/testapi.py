import os
import numpy as np
import datetime as dt
import sys
from datetime import datetime

pkg_dir = os.path.join(os.path.dirname(__file__),'..')

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
    assert(cn == "cat")
    assert(dn == "dog")
    del(cat)
    del(dog)
    gc.collect()


def test_reference_count():
    import gc
    cat = Cat.new()
    assert(twg.reference_count_py(cat) == 1)
    mamal = twg.create_mamal_ptr_py(cat)
    assert(twg.reference_count_py(cat) == 2)
    del(cat)
    gc.collect()
    assert(twg.reference_count_py(cat) == 1)
    # del(mamal)



