if (TARGET glm)
	return()
endif()

message(STATUS "Third-party (external): creating target 'glm'")

include(CPM)
CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 5c46b9c
	DOWNLOAD_ONLY ON
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "${CMAKE_BINARY_DIR}/_deps/glm-src")