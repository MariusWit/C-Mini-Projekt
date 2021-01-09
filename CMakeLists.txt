# CMakeList.txt: CMake-Projekt für "C-Mini-Projekt-[Schiffe versenken]". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

set(PROJEKTNAME C-Mini-Projekt-Schiffe_versenken)

project (${PROJEKTNAME})

# -Wall -Wextra -Wshadow -Werror -g -D_POSIX_C_SOURCE=200809L
set(SHARED_FLAGS " -lncurses")
set(CMAKE_C_FLAGS "-std=c11 ${SHARED_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++11 ${SHARED_FLAGS}")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(schiffe_versenken "src/schiffe_versenken.c" "include/schiffe_versenken.h" "src/oberflaeche.c" "include/oberflaeche.h" "src/felder_2d.c" "include/felder_2d.h" "src/input_int.c" "include/input_int.h" "src/main.c"  "include/spieler.h")

target_link_libraries(schiffe_versenken ${CURSES_LIBRARIES})

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
