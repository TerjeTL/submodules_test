# CMakeList.txt : CMake project for SubmodulesTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "SubmodulesTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SubmodulesTest PROPERTY CXX_STANDARD 20)
endif()

# Morphologica code requires a number of libraries, collected into 'CORE' and 'GL'.
set(MORPH_LIBS_CORE ${ARMADILLO_LIBRARY} ${ARMADILLO_LIBRARIES} ${HDF5_C_LIBRARIES} ${LAPACK_LIBRARIES})
set(MORPH_LIBS_GL ${OpenCV_LIBS} OpenGL Freetype glfw)

target_link_libraries(${PROJECT_NAME}
	"glfw"
	"freetype"
	${JSONCPP_LIBRARIES}
	libglew_static
	${MORPH_LIBS_CORE}
	${MORPH_LIBS_GL}
	#"hdf5"
)