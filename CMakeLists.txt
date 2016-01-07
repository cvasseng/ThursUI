cmake_minimum_required (VERSION 3.1)
project (tursui)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG=1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

include_directories("src/")
include_directories("include/thurs")

add_library(thursui STATIC 
  src/thurs.color.cpp
  src/thurs.control.cpp
  src/thurs.input.cpp
  src/thurs.os.cpp
  src/thurs.renderer.cpp
  src/thurs.scrollbar.cpp
  src/thurs.skin.cpp
  src/thurs.surface.cpp
  src/thurs.window.cpp
  src/thurs.property.cpp
  src/controls/thurs.control.button.cpp
  src/controls/thurs.control.checkbox.cpp
  src/controls/thurs.control.dropdown.cpp
  src/controls/thurs.control.editbox.cpp
  src/controls/thurs.control.imagegrid.cpp
  src/controls/thurs.control.label.cpp
  src/controls/thurs.control.listbox.cpp
  src/controls/thurs.control.mltext.cpp
  src/controls/thurs.control.progressbar.cpp
  src/controls/thurs.control.rectangle.cpp
  src/controls/thurs.control.skillbar.cpp
  src/controls/thurs.control.slider.cpp
)

target_compile_features(thursui PRIVATE cxx_range_for)
target_compile_features(thursui PRIVATE cxx_decltype_auto)
target_compile_features(thursui PRIVATE cxx_generic_lambdas)

target_link_libraries(thursui jsoncpp)

INSTALL(TARGETS thursui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

INSTALL(DIRECTORY "include/thurs" DESTINATION include)
