##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

def get_name_py(x):
    """
    get_name_py
    
    get_name_py Wrapper function for get_name
    
    :param x Python type equivalent for C++ type ANIMAL_PTR
    :export
    """
    x_xptr = cinterop.getExternalXptr(x)
    result = nativelib.get_name(x_xptr)
    return cinterop.mkExternalObjRef(result, 'char*')


def create_cat_py():
    """
    create_cat_py
    
    create_cat_py Wrapper function for create_cat
    
    :export
    """
    result = nativelib.create_cat()
    return cinterop.mkExternalObjRef(result, 'CAT_PTR')


def create_dog_py():
    """
    create_dog_py
    
    create_dog_py Wrapper function for create_dog
    
    :export
    """
    result = nativelib.create_dog()
    return cinterop.mkExternalObjRef(result, 'DOG_PTR')


def put_in_dog_kenel_py(d):
    """
    put_in_dog_kenel_py
    
    put_in_dog_kenel_py Wrapper function for put_in_dog_kenel
    
    :param d Python type equivalent for C++ type DOG_PTR
    :export
    """
    d_xptr = cinterop.getExternalXptr(d)
    result = nativelib.put_in_dog_kenel(d_xptr)
    return cinterop.mkExternalObjRef(result, 'int')


def create_homo_erectus_py():
    """
    create_homo_erectus_py
    
    create_homo_erectus_py Wrapper function for create_homo_erectus
    
    :export
    """
    result = nativelib.create_homo_erectus()
    return cinterop.mkExternalObjRef(result, 'HOMO_ERECTUS_PTR')


def free_string_py(x):
    """
    free_string_py
    
    free_string_py Wrapper function for free_string
    
    :param x Python type equivalent for C++ type char*
    :export
    """
    nativelib.free_string(x)


def release_handle_py(ptr):
    """
    release_handle_py
    
    release_handle_py Wrapper function for release_handle
    
    :param ptr Python type equivalent for C++ type VOID_PTR_PROVIDER_PTR
    :export
    """
    ptr_xptr = cinterop.getExternalXptr(ptr)
    nativelib.release_handle(ptr_xptr)


def release_handle_species_via_moirai_py(ptr):
    """
    release_handle_species_via_moirai_py
    
    release_handle_species_via_moirai_py Wrapper function for release_handle_species_via_moirai
    
    :param ptr Python type equivalent for C++ type VOID_PTR_PROVIDER_PTR
    :export
    """
    ptr_xptr = cinterop.getExternalXptr(ptr)
    nativelib.release_handle_species_via_moirai(ptr_xptr)


def reference_count_py(ptr):
    """
    reference_count_py
    
    reference_count_py Wrapper function for reference_count
    
    :param ptr Python type equivalent for C++ type VOID_PTR_PROVIDER_PTR
    :export
    """
    ptr_xptr = cinterop.getExternalXptr(ptr)
    result = nativelib.reference_count(ptr_xptr)
    return cinterop.mkExternalObjRef(result, 'int')


def create_mamal_ptr_py(cat):
    """
    create_mamal_ptr_py
    
    create_mamal_ptr_py Wrapper function for create_mamal_ptr
    
    :param cat Python type equivalent for C++ type CAT_PTR
    :export
    """
    cat_xptr = cinterop.getExternalXptr(cat)
    result = nativelib.create_mamal_ptr(cat_xptr)
    return cinterop.mkExternalObjRef(result, 'MAMAL_PTR')


