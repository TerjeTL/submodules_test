# CMakeList.txt : CMake project for SubmodulesTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "SubmodulesTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SubmodulesTest PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME}
	"glfw"
	"freetype"
	"HDF5"
)