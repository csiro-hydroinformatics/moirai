
// This code gen code is a lot of, well, code. 
// And overkill for this small test if it were started from scratch. 
// It is used on a substantial private codebase though, 
// so we just reuse almost as is.

open System
open System.IO
open System.Collections.Generic

#r "../../../../c-api-wrapper-generation/engine/ApiWrapperGenerator/bin/Debug/net6.0/ApiWrapperGenerator.dll"

let (+/) path1 path2 = Path.Combine(path1, path2)
open ApiWrapperGenerator

let prependHeaderTemplate = "##################
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
from {0}.wrap.ffi_interop import marshal, {1}
# phase out importing from {0}.classes, to prevent cyclic imports
# from {0}.classes import wrap_cffi_native_handle
# Additional specific imports for this package
{3}

if TYPE_CHECKING:
{2}
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
        release_native = {4}
    return __wrap_cffi_native_handle(obj, type_id, release_native)

def char_array_to_py(values:CffiData, dispose:bool=True) -> str:
    pystring = marshal.c_string_as_py_string(values)
    if dispose:
        {1}.free_string(values)
    return pystring

def dispose_shared_pointer_py(ptr:Any) -> None:
    # Upon a process terminating, somehow wrap_as_pointer_handle can end up being None,
    # leading to a TypeError: 'NoneType' object is not callable.
    # This is a nuisance, and hard to fully diagnose.
    # So, we will use the following workaround to guard against it. See WIRADA-659.
    if wrap_as_pointer_handle is None:
        return
    ptr_xptr = wrap_as_pointer_handle(ptr)
    # Upon a process terminating, somehow '{1}' can end up being None,
    # leading to a TypeError: 'NoneType' object is not callable.
    # This is a nuisance, and hard to fully diagnose.
    # So, we will use the following workaround to guard against it. See WIRADA-659.
    if {1} is not None: #  and {1}.DisposeSharedPointer is not None:
        {1}.DisposeSharedPointer(ptr_xptr.ptr)

"

let makePrependCodegen pkgName libName wrapperClassNames otherImports nativeDisposeFunction = String.Format(prependHeaderTemplate, pkgName, libName, wrapperClassNames, otherImports, nativeDisposeFunction)


let createPyWrapGen (pkgName:string, cffiObjName:string, wrapperClassNames:string, otherImports:string, nativeDisposeFunction:string, typeMap:Dictionary<string, string>) : PythonCffiWrapperGenerator = 
    let pyCffiGen = PythonCffiWrapperGenerator()

    pyCffiGen.FunctionNamePostfix <- "_py"
    pyCffiGen.ApiCallPostfix <- ""
    pyCffiGen.GeneratePyDocstringDoc <- true 
    let prependHeader = makePrependCodegen pkgName cffiObjName wrapperClassNames otherImports nativeDisposeFunction

    pyCffiGen.ApiCallPrefix <- cffiObjName + "." 
    pyCffiGen.PrependOutputFile <- prependHeader

    // pyCffiGen.RoxygenDocPostamble <- "#' @export") 

    pyCffiGen.CreateXptrObjRefFunction <- "custom_wrap_cffi_native_handle"
    pyCffiGen.GetXptrFromObjRefFunction <- "unwrap_cffi_native_handle"

    pyCffiGen.SetTransientArgConversion( "char**"  , "_charpp", "C_ARGNAME = wrap_as_pointer_handle(marshal.as_arrayof_bytes(RCPP_ARGNAME))", "# clean C_ARGNAME ?")
    pyCffiGen.SetTypeMap("char*", "str")
    pyCffiGen.SetTransientArgConversion( "char*"  , "_charp", "C_ARGNAME = wrap_as_pointer_handle(as_bytes(RCPP_ARGNAME))", "# no cleanup for char*?")
    pyCffiGen.SetTypeMap("const char*", "str")
    pyCffiGen.SetTransientArgConversion( "const char*" , "_c_charp", "C_ARGNAME = wrap_as_pointer_handle(as_bytes(RCPP_ARGNAME))", "# no cleanup for const char*")

    // TODO?
    pyCffiGen.SetTransientArgConversion( "double**", "_doublepp", "C_ARGNAME = wrap_as_pointer_handle(as_double_ptr_array(RCPP_ARGNAME))", "# delete[] C_ARGNAME")

    pyCffiGen.SetTransientArgConversion( ".*_PTR"  , "_xptr", "C_ARGNAME = wrap_as_pointer_handle(RCPP_ARGNAME)", "")  
    pyCffiGen.ClearCustomWrappers()

    pyCffiGen.FunctionWrappers <- "@check_exceptions"

    let returnscharptrptr = pyCffiGen.ReturnsCharPtrPtrWrapper()
    pyCffiGen.AddCustomWrapper(returnscharptrptr)
    let returnsdoubleptr = pyCffiGen.ReturnsDoublePtrWrapper()
    pyCffiGen.AddCustomWrapper(returnsdoubleptr)

    pyCffiGen.SetTypeMap("character_vector*", "List")
    pyCffiGen.SetReturnedValueConversion("character_vector*", "character_vector_to_list(C_ARGNAME, dispose=True)")

    pyCffiGen.SetTypeMap("char*", "str")
    pyCffiGen.SetReturnedValueConversion("char*", "char_array_to_py(C_ARGNAME, dispose=True)")

    pyCffiGen.SetTypeMap("date_time_to_second", "datetime")

    pyCffiGen.SetTransientArgConversion("date_time_to_second", "_datetime", 
        "C_ARGNAME = marshal.datetime_to_dtts(RCPP_ARGNAME)", 
        "# C_ARGNAME - no cleanup needed?")

    pyCffiGen.SetTransientArgConversion("double*", "_numarray", 
        // Note that we can use shallow=True only if we know for sure the API will copy the data for internal purposes e.g. Input Player. Otherwise, dangerous
        "C_ARGNAME = marshal.as_c_double_array(RCPP_ARGNAME, shallow=True)", 
        "# C_ARGNAME - no cleanup needed?")

    pyCffiGen.SetReturnedValueConversion("date_time_to_second", "marshal.as_datetime(C_ARGNAME)")

    pyCffiGen.SetTypeMap("named_values_vector*", "Dict[str,float]")
    pyCffiGen.SetTransientArgConversion("named_values_vector*", "_nvv", 
        "C_ARGNAME = marshal.dict_to_named_values(RCPP_ARGNAME)",
        "# C_ARGNAME - no cleanup needed?")

    pyCffiGen.SetTypeMap("values_vector*", "List[float]")
    pyCffiGen.SetTransientArgConversion("values_vector*", "_vv", 
        "C_ARGNAME = marshal.create_values_struct(RCPP_ARGNAME)",
        "# C_ARGNAME - no cleanup needed?")

    pyCffiGen.SetTypeMap("string_string_map*", "Dict[str,str]")
    pyCffiGen.SetTransientArgConversion("string_string_map*", "_dict", 
        "C_ARGNAME = marshal.dict_to_string_map(RCPP_ARGNAME)",
        "# C_ARGNAME - no cleanup needed?")

    pyCffiGen.SetTypeMap("named_values_vector*", "Dict[str,float]")
    pyCffiGen.SetReturnedValueConversion("named_values_vector*", 
        "named_values_to_py(C_ARGNAME, dispose=True)")

    let addTypeMapping (key:string, value:string) : unit = 
        pyCffiGen.SetTypeMap(key, value)
        let xx = String.Format("custom_wrap_cffi_native_handle(C_ARGNAME, '{0}')", key)
        pyCffiGen.SetReturnedValueConversion(key, xx)

    for kvp in typeMap do
        addTypeMapping (kvp.Key, kvp.Value)

    pyCffiGen

let excludedApiFuncs = [|"free_string"; "DeleteDoubleArray"|]

let apiFilter = HeaderFilter()
let nsw = [|"#"; "//"; "*"|]
apiFilter.NotStartsWith <- nsw    // '*' is to exclude from multi-line comment blocks including doxygen doc.

apiFilter.ContainsNone <- excludedApiFuncs 


// ######################################################

let pkgName = "testmoirai"
let cffiObjName = "testmoirai_so"

let wrapperClassNames="    from testmoirai.classes import (
        Animal,
        Mamal,
        Dog,
        Cat,
        HomoErectus,
    )
"

let typeMap = Dictionary<string, string>()
typeMap.Add("ANIMAL_PTR", "'Animal'")
typeMap.Add("MAMAL_PTR", "'Mamal'")
typeMap.Add("DOG_PTR", "'Dog'")
typeMap.Add("CAT_PTR", "'Cat'")
typeMap.Add("HOMO_ERECTUS_PTR", "'HomoErectus'")

let otherImports = "
import testmoirai.wrap.ffi_interop as _m_wrap
"

// using dispose_shared_pointer_py to work around WIRADA-659
let nativeDisposeFunction = "dispose_shared_pointer_py"

let pyCffiGen = createPyWrapGen (pkgName, cffiObjName, wrapperClassNames, otherImports, nativeDisposeFunction, typeMap)
pyCffiGen.FunctionWrappers <- "@_m_wrap.check_exceptions"


let exportModifierPattern = [|"SPECIES_API"|] 
apiFilter.ContainsAny <- exportModifierPattern
apiFilter.ToRemove <- exportModifierPattern

//   gen <- create_wrapper_generator(pyCffiGen, apiFilter)
let gen =  WrapperGenerator(pyCffiGen, apiFilter)

//   swiftr_include_dir <- file.path( pkg_path_swiftr(swiftSrcPath), 'inst/include')
let srcDir = if (Environment.OSVersion.Platform = PlatformID.Win32NT) then @"C:\src"  else "/home/per202/src"

let root = srcDir +/ "moirai"
let apiHeaderFile = root +/ "tests" +/ "moirai_test_lib" +/ "c_interop_api.h"

let outfile = root +/ "tests" +/ "python_test_bindings" +/ "testmoirai" +/ "wrap" +/ "testmoirai_wrap_generated.py"

gen.CreateWrapperHeader(apiHeaderFile, outfile)
