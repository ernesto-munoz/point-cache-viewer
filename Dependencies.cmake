# Dependecies of the project (raylib, box2d, ...)

include(FetchContent)

# find_package(raylib CONFIG REQUIRED)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)
FetchContent_MakeAvailable(raylib)

# needed by the alembic repository
set(ALEMBIC_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    Imath
    GIT_REPOSITORY https://github.com/AcademySoftwareFoundation/Imath.git
    GIT_TAG v3.1.9
)
FetchContent_MakeAvailable(Imath)

target_include_directories(Imath INTERFACE $<BUILD_INTERFACE:${imath_SOURCE_DIR}/src>)

FetchContent_Declare(
    alembic
    GIT_REPOSITORY https://github.com/alembic/alembic.git
    GIT_TAG 1.8.12
)

macro(install)
endmacro()
macro(export)
endmacro()
FetchContent_MakeAvailable(alembic)
unset(install)

# the imgui is needed for the raylibimggui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.9
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(rlimgui)

add_library(rlimgui STATIC
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(rlimgui PUBLIC imgui raylib)

# file dialog simple api
FetchContent_Declare(
    tinyfiledialogs
    GIT_REPOSITORY https://github.com/native-toolkit/tinyfiledialogs.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(tinyfiledialogs)

add_library(tinyfiledialogs STATIC ${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c)
target_compile_options(tinyfiledialogs PRIVATE /FIwindows.h /FIcommdlg.h)
target_include_directories(tinyfiledialogs PUBLIC ${tinyfiledialogs_SOURCE_DIR})

# universal scene description api USD
# set(CMAKE_PREFIX_PATH "C:/Users/aokuma/code/thirdparty/OpenUSD/install" ${CMAKE_PREFIX_PATH})
# find_package(pxr REQUIRED)