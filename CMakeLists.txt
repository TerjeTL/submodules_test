# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.15)

project(SubmodulesTest)

# Include sub-projects.
add_subdirectory("submodules_test")


#		DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT},
					please checkout submodules")
		endif()
	endif()
endif()

#		CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
	message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/freetype/CMakeLists.txt")
	message(FATAL_ERROR "The freetype submodules was not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/jsoncpp/CMakeLists.txt")
	message(FATAL_ERROR "The jsoncpp submodules was not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/opencv/CMakeLists.txt")
	message(FATAL_ERROR "The opencv submodules was not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()


if(WIN32)
add_subdirectory(external/morphologica/buildtools)
endif()


#find_package(HDF5)
#find_package(openCV REQUIRED PATHS "${PROJECT_SOURCE_DIR}/external/opencv/cmake/")
#find_package(OpenCV)

# Tell the program where the morph fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMORPH_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/external/morphologica/fonts\\\"\"")

add_subdirectory("external/glfw")
add_subdirectory("external/freetype")
add_subdirectory("external/jsoncpp")
add_subdirectory("external/glew")

#find_package(Armadillo REQUIRED)
find_package(HDF5 REQUIRED)
find_package(OpenGL REQUIRED)

set(ENV{PKG_CONFIG_PATH} "${CMAKE_SOURCE_DIR}/out/build/x64-debug/external/jsoncpp/pkg-config/")
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

#get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
#include_directories(${JSON_INC_PATH})

# Define collections of includes that have to be made for morphologica
set(MORPH_INC_CORE ${ARMADILLO_INCLUDE_DIR} ${ARMADILLO_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})
set(MORPH_INC_GL ${OpenCV_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
include_directories(${MORPH_INC_CORE} ${MORPH_INC_GL})

set(HDF5DIR "C:/Program Files/HDF_Group/HDF5/1.12.2")

target_include_directories(${PROJECT_NAME}
	PUBLIC "external/glfw/include"
	PUBLIC "external/freetype/include"
	PUBLIC "${HDF5DIR}/include"
	PUBLIC "D:/Development/C++/libraries/armadillo-11.2.4/include"
	#PUBLIC "external/jsoncpp/include"
)

target_link_directories(${PROJECT_NAME}
	PUBLIC "external/glfw/src"
	PUBLIC "external/freetype/src"
	PUBLIC "${HDF5DIR}/lib"
	PUBLIC "D:/Development/C++/libraries/armadillo-11.2.4/src"
	#PRIVATE "external/jsoncpp/src"
)
