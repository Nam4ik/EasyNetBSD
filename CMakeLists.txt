
cmake_minimum_required(VERSION 3.10)


project(netbsd-installer)


set(CMAKE_C_STANDARD 99)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)


include_directories(${GTK3_INCLUDE_DIRS})


set(SOURCES
    AutoNBSD/MainGUI.c
    AutoNBSD/DiskTools.c
)


add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})

# target_compile_options(${PROJECT_NAME} PRIVATE -I/usr/include)


# target_compile_definitions(${PROJECT_NAME} PRIVATE _NETBSD_SOURCE)

# include(CheckIncludeFile)
# check_include_file(sys/types.h HAVE_SYS_TYPES_H)
# if(NOT HAVE_SYS_TYPES_H)
#     message(FATAL_ERROR "Required header sys/types.h not found")
# endif()
