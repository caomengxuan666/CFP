add_library(CaponCrashDebug CrashHandler.cpp CrashHandlerImpl.cpp)

target_link_libraries(CaponCrashDebug PUBLIC CaponLogging ws2_32 dbghelp)

target_include_directories(CaponCrashDebug PUBLIC ${CMAKE_BINARY_DIR}/generated)