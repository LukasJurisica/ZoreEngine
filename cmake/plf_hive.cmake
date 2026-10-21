if (TARGET plf_hive)
	return()
endif()

message(STATUS "Third-party (external): creating target 'plf_hive'")

include(CPM)
CPMAddPackage(
    NAME plf_hive
    GITHUB_REPOSITORY mattreecebentley/plf_hive
    GIT_TAG f2bf743
	DOWNLOAD_ONLY ON
)

add_library(plf_hive INTERFACE)
target_include_directories(plf_hive INTERFACE "${CMAKE_BINARY_DIR}/_deps/plf_hive-src")