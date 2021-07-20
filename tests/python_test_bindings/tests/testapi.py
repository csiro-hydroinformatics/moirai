import os
import numpy as np
import datetime as dt
import sys
from datetime import datetime

pkg_dir = os.path.join(os.path.dirname(__file__),'..')

sys.path.append(pkg_dir)

from testmoirai.something import *

def test_basic_obj_lifetime():
    cat = create_cat()
    dog = create_dog()

    auto cn = get_name(cat)
    auto dn = get_name(dog)
    assert(string(cn) == "cat")
    assert(string(dn) == "dog")

    free_string(cn)
    free_string(dn)

    release_handle(cat)
    release_handle(dog)


def test_reference_count():

	cat = create_cat()
	assert(reference_count(cat) == 1)
	MAMAL_PTR mamal = create_mamal_ptr(cat)
	assert(reference_count(cat) == 2)
	release_handle(cat)
	assert(reference_count(mamal) == 1)
	release_handle(mamal)


def test_multiple_apis_with_opaque_ptrs():

	A_PTR a = create_a_obj()
	assert(reference_count(a) == 1)
	A_PTR a2 = clone_handle_a(a)
	assert(reference_count(a) == 2)
	release_handle(a2)
	assert(reference_count(a) == 1)
	release_handle(a)


	B_PTR b = create_b_obj()
	assert(reference_count(b) == 1)
	B_PTR bb3 = clone_handle_b_in_domain_b(b)
	assert(reference_count(b) == 2)
	release_handle(bb3)
	assert(reference_count(b) == 1)
	bb3 = new_handle_b_in_domain_b(b)
	assert(reference_count(b) == 2)
	release_handle(bb3)
	assert(reference_count(b) == 1)


	B_PTR b2 = clone_handle_b_in_domain_a(b)
	assert(reference_count(b) == 2)
	B_PTR b3 = new_handle_b_in_domain_a(b)
	assert(reference_count(b) == 3)
	release_handle(b3)
	assert(reference_count(b) == 2)
	release_handle(b2)
	assert(reference_count(b) == 1)
	release_handle(b)




def test_multiple_apis_with_opaque_ptrs_released_via_Moirai_C_API():

	#see whether we can centralize API calls for disposal of objects,
	#	to limit duplications or errors in using's libraries
	A_PTR a = create_a_obj()
	assert(reference_count(a) == 1)
	assert(get_reference_count(a) == 1)
	A_PTR a2 = clone_handle_a(a)
	assert(reference_count(a) == 2)
	assert(get_reference_count(a) == 2)
	release_handle_domain_a_via_moirai(a2)
	assert(reference_count(a) == 1)
	assert(get_reference_count(a) == 1)
	release_handle_domain_a_via_moirai(a)


	B_PTR b = create_b_obj()
	assert(reference_count(b) == 1)
	assert(get_reference_count(b) == 1)
	B_PTR bb3 = clone_handle_b_in_domain_b(b)
	assert(reference_count(b) == 2)
	assert(get_reference_count(b) == 2)
	release_handle_domain_b_via_moirai(bb3)
	assert(reference_count(b) == 1)
	assert(get_reference_count(b) == 1)
	bb3 = new_handle_b_in_domain_b(b)
	assert(reference_count(b) == 2)
	assert(get_reference_count(b) == 2)
	release_handle_domain_b_via_moirai(bb3)
	assert(reference_count(b) == 1)
	assert(get_reference_count(b) == 1)


	B_PTR b2 = clone_handle_b_in_domain_a(b)
	assert(reference_count(b) == 2)
	assert(get_reference_count(b) == 2)
	B_PTR b3 = new_handle_b_in_domain_a(b)
	assert(reference_count(b) == 3)
	assert(get_reference_count(b) == 3)
	release_handle_domain_b_via_moirai(b3)
	assert(reference_count(b) == 2)
	assert(get_reference_count(b) == 2)
	release_handle_domain_b_via_moirai(b2)
	assert(reference_count(b) == 1)
	assert(get_reference_count(b) == 1)
	release_handle_domain_b_via_moirai(b)



#include "moirai/opaque_pointers.hpp"

def test_opaque_pointers_handlers_Cpp_projects_accessing the C API")

	using wrapper = moirai::opaque_pointer_handle
	cat = create_cat()
	wrapper* cat_h = new wrapper(cat)
	assert(get_reference_count(cat) == 1)
	MAMAL_PTR mamal = create_mamal_ptr(cat)
	wrapper* mamal_h = new wrapper(mamal)
	assert(get_reference_count(cat) == 2)
	assert(get_reference_count(mamal) == 2)
	assert(cat_h->get_reference_count() == 2)
	assert(mamal_h->get_reference_count() == 2)
	delete cat_h
	assert(get_reference_count(mamal) == 1)
	assert(mamal_h->get_reference_count() == 1)
	delete mamal_h


