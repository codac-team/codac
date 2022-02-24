FUNCTION(version_from_git)
find_package(Git)
# Generate a git-describe version string from Git repository tags
if(GIT_EXECUTABLE AND NOT DEFINED FOOBAR_VERSION)
  execute_process(
	COMMAND 
	${GIT_EXECUTABLE} describe --tags --abbrev=0 --match "v*"
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
    RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
	)
  if(NOT GIT_DESCRIBE_ERROR_CODE)
    set(PROJECT_VERSION	 ${GIT_DESCRIBE_VERSION})
  endif()
endif()
if(NOT DEFINED PROJECT_VERSION)
  set(PROJECT_VERSION v0.0.0-unknown)
  message(WARNING "Failed to determine PROJECT_VERSION from repository tags. Using default version \"${PROJECT_VERSION}\".")
endif()
set(PROJECT_VERSION ${PROJECT_VERSION} PARENT_SCOPE)
message(STATUS "PROJECT_VERSION ${PROJECT_VERSION}")
ENDFUNCTION(version_from_git)
