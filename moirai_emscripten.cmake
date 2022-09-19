# moirai_emscripten.cmake

# https://github.com/emscripten-core/emscripten/issues/15276
# https://github.com/pyodide/pyodide/blob/main/packages/geos/meta.yaml

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-s EXPORT_ALL=1 -s SIDE_MODULE=1")
set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "-s EXPORT_ALL=1 -s SIDE_MODULE=1")
set(CMAKE_STRIP FALSE)  # used by default in pybind11 on .so modules