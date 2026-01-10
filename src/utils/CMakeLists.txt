file(GLOB_RECURSE SOURCES *.cpp *.h)

add_library(CFPUtils STATIC ${SOURCES})

target_compile_options(CFPUtils PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
target_compile_options(CFPUtils PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
