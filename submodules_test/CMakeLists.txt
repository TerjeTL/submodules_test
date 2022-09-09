# CMakeList.txt : CMake project for SubmodulesTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# From CMAKE_SYSTEM work out which of __OSX__, __GLN__, __NIX__ or
# __WIN__ are required
message(STATUS "Operating system: " ${CMAKE_SYSTEM})
if(CMAKE_SYSTEM MATCHES Linux.*)
  set(MORPH_HOST_DEFINITION "-D__GLN__")
elseif(CMAKE_SYSTEM MATCHES BSD.*)
  set(MORPH_HOST_DEFINITION "-D__NIX__")
elseif(APPLE)
  set(MORPH_HOST_DEFINITION "-D__OSX__")
elseif(CMAKE_SYSTEM MATCHES Win.*)
  set(MORPH_HOST_DEFINITION "-D__WIN__")
else()
  message(ERROR " Operating system not supported: " ${CMAKE_SYSTEM})
endif()

# Using c++-14 (calls to std::decay_t in number_type.h). c++-17
# required to allow brace initializers for the morph::Vector class
set (CMAKE_CXX_STANDARD 17)

# Add the host definition to CXXFLAGS along with other switches
if (APPLE)
    set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -O3")
else()
  if(NOT CMAKE_SYSTEM MATCHES Win.*)
    # To use Intel compiler, you can call cmake as: `cmake -DCMAKE_CXX_COMPILER=icpc ..` or `CXX=icpc cmake ..`
    if (CMAKE_CXX_COMPILER_ID MATCHES Intel)
      set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -std=c++17 -xHOST -O3 -D__ICC__")
    else () # GCC or Clang
      if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
        # Add compiler version check, to ensure gcc is version 7 or later.
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.1)
          message(FATAL_ERROR "GCC version must be at least 7.1! Use CXX=g++-7 cmake (options) and don't forget to clean out your build directory first!")
        else()
          message(STATUS "GCC version ${CMAKE_CXX_COMPILER_VERSION} OK!")
        endif()
      endif()
      set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -Wall -g -Wfatal-errors -Wno-unused-result -Wno-unknown-pragmas -march=native -O3")
    endif()
  else() # Windows
    # Set flags for Windows. /EHsc required for use of exceptions
    set(CMAKE_CXX_FLAGS "${MORPH_HOST_DEFINITION} -DNOMINMAX /EHsc")
	message(STATUS "${MORPH_HOST_DEFINITION} -DNOMINMAX /EHsc")
  endif() # CMAKE_SYSTEM MATCHES Win.*
endif()

# Following `cmake --help-policy CMP0072`
set(OpenGL_GL_PREFERENCE "GLVND")

# Additional GL compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL3_PROTOTYPES -DGL_GLEXT_PROTOTYPES")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_GLEW")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "SubmodulesTest.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SubmodulesTest PROPERTY CXX_STANDARD 20)
endif()

# Assuming that you installed morphologica in-tree (i.e. 'next to' XXX.cpp).
set(MORPH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external/morphologica")
include_directories(BEFORE "${MORPH_INCLUDE_PATH}")
include_directories(BEFORE "${MORPH_INCLUDE_PATH}/include")

# Morphologica code requires a number of libraries, collected into 'CORE' and 'GL'.
set(MORPH_LIBS_CORE ${ARMADILLO_LIBRARY} ${ARMADILLO_LIBRARIES} ${HDF5_C_LIBRARIES} ${LAPACK_LIBRARIES})
set(MORPH_LIBS_GL ${OpenCV_LIBS} ${OPENGL_LIBRARIES} "freetype" "glfw")

target_link_libraries(${PROJECT_NAME}
	${JSONCPP_LIBRARIES}
	libglew_shared
	${MORPH_LIBS_CORE}
	${MORPH_LIBS_GL}
	#"hdf5"
)