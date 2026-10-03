if (TARGET tlse)
	return()
endif()

message(STATUS "Third-party (external): creating target 'tlse'")

set(BUILD_EXAMPLES OFF)
set(TLSE_COMPILE_DEFINITIONS
	TLS_AMALGAMATION
	CACHE STRING "TLSe compile definitions"
	FORCE
)

include(CPM)
CPMAddPackage(
    NAME tlse
    GITHUB_REPOSITORY eduardsui/tlse
    GIT_TAG 4ddb3ac
)