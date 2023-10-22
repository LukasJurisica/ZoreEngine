if (TARGET TracyClient)
	return()
endif()

message(STATUS "Third-party (external): creating target 'TracyClient'")

include(CPM)
CPMAddPackage(
    NAME TracyClient
    GITHUB_REPOSITORY wolfpld/tracy
    GIT_TAG 3601576
)