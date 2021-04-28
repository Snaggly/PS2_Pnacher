project(PS2-Pnacher CXX)

cmake_minimum_required(VERSION 3.2.0)

set(PROJECT_TARGET ps2pnacher)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(CDIO REQUIRED libcdio++)

set(SRC_DIR src)
set(HEAD_DIR include)
set(HEADERS
  ${HEAD_DIR}/mainapp.h
  ${HEAD_DIR}/ElfTools.h
  ${HEAD_DIR}/IsoTools.h
  ${HEAD_DIR}/Patcher.h
  ${HEAD_DIR}/PnachTools.h
)
set(SOURCES 
  ${SRC_DIR}/main.cpp 
  ${SRC_DIR}/mainapp.cpp
  ${SRC_DIR}/ElfTools.cpp
  ${SRC_DIR}/IsoTools.cpp
  ${SRC_DIR}/Patcher.cpp
  ${SRC_DIR}/PnachTools.cpp
  ${HEADERS}
)

add_executable(${PROJECT_TARGET} ${SOURCES})

target_include_directories(${PROJECT_TARGET} PRIVATE ${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_TARGET} PRIVATE ${GTKMM_LIBRARIES} iso9660++ iso9660 cdio++ cdio)

install(TARGETS ${PROJECT_TARGET} RUNTIME DESTINATION bin)

add_subdirectory(res)
