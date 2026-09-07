# CMakeList.txt : CMake project for duplication, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("point-cache-viewer")

include(Dependencies.cmake)  # dependencies of the project

# Add source to this project's executable.
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS "core/*.cpp")
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS "core/*.h")
add_library(core ${CORE_SOURCES} ${CORE_HEADERS})
target_compile_features(core PUBLIC cxx_std_20)
target_link_libraries(core PUBLIC raylib Alembic Imath)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core/src)


file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "app/*.cpp")
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS "app/*.h")
add_executable (point-cache-viewer ${APP_SOURCES})
target_compile_features(point-cache-viewer PUBLIC cxx_std_20)
target_link_libraries(point-cache-viewer PUBLIC core rlimgui imgui tinyfiledialogs)

# TODO: Add tests and install targets if needed.

add_custom_command(TARGET point-cache-viewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/app/data $<TARGET_FILE_DIR:point-cache-viewer>/data
)