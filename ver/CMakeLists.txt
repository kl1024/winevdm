file(GLOB SOURCE *.c *.cpp *.rc)
add_library(ver SHARED ${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/ver.def ver.dll16.obj)
include_directories(../wine)
add_definitions(-D_X86_ -D__WINESRC__ -D__i386__ -DHAVE_STRNCASECMP -DHAVE__STRNICMP -D_WINTERNL_ -DNtCurrentTeb=NtCurrentTeb__ -DDECLSPEC_HIDDEN= -Dstrncasecmp=_strnicmp)
spec_build(ver.dll16 ver)
target_link_libraries(ver libwine winecrt0 krnl386 version.lib)
set_target_properties(ver PROPERTIES SUFFIX ".dll16")
