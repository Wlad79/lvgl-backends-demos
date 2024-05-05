cmake_minimum_required(VERSION 3.1)

project(lvgl-demo-sdl2)
#project(lvgl-demo-fb)
#project(lvgl-demo-wl)

include_directories(.)
include_directories(lvgl)
include_directories(lv_drivers)

add_subdirectory(lvgl)
add_subdirectory(lv_drivers)

#for yocto Git-Version:
FIND_PACKAGE(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})

#for sdk version:
#find_package(PkgConfig REQUIRED)
#pkg_search_module(PKG_SDL2 REQUIRED IMPORTED_TARGET sdl2)
#include_directories(${PKG_SDL2_INCLUDE_DIRS})
#set(SDL2_LIBS ${PKG_SDL2_LDFLAGS} )

add_executable(${PROJECT_NAME} main.c mouse_cursor_icon.c)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE lvgl lvgl::examples lvgl::demos lvgl::drivers ${SDL2_LIBRARIES} xkbcommon)

#for sdk version:
#target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lvgl::examples lvgl::demos
#	lvgl::drivers wayland-client wayland-cursor xkbcommon ${SDL2_LIBS})
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
