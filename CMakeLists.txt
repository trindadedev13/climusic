cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME climusic)
project(${PROJECT_NAME}
    LANGUAGES
    C)

option(ASAN "Enables Address Sanitizer" OFF)
if (ASAN)
  message(STATUS "AddressSanitizer enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

set(SOURCES
    src/main.c
    src/cm_colors.c
    src/cm_file.c
    src/cm_gui.c
    src/cm_online.c
    src/cm_screen.c
    src/cm_string.c
    src/cm_sound.c)

set(HEADERS
    include/cm_bool.h
    include/cm_buffer.h
    include/cm_colors.h
    include/cm_file.h
    include/cm_gui.h
    include/cm_inline.h
    include/cm_online.h
    include/cm_screen.h
    include/cm_string.h
    include/cm_sound.h
    include/cm_vec.h)

find_package(SDL2)
find_package(SDL2_mixer)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL2
    SDL2_mixer
    ncurses
    curl)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)