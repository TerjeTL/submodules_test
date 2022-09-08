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

add_subdirectory("external/glfw")
add_subdirectory("external/freetype")

target_include_directories(${PROJECT_NAME}
	PUBLIC "external/glfw/include"
	PUBLIC "external/freetype/include"
)

target_link_directories(${PROJECT_NAME}
	PRIVATE "external/glfw/src"
	PRIVATE "external/freetype/src"
)
