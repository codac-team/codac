configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pyIbex_Setting.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pyIbex_Setting.h
)

# Installation process handled by setup.py
set(PYTHON_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python_package")
file(MAKE_DIRECTORY ${PYTHON_PACKAGE_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/pyibex/" "${PYTHON_PACKAGE_DIR}/pyibex")
FILE(GENERATE OUTPUT "${PYTHON_PACKAGE_DIR}/pyibex/version.py" CONTENT "__version__=\"${PROJECT_VERSION}\"")


if(NOT WIN32)
  set(FORCE_PYTHON_EXT "ext_modules=[Extension(\"pyibex.empty\", sources=[\"empty.c\"])],")
  # trick to generate a correct wheel.
  file(WRITE "${PYTHON_PACKAGE_DIR}/empty.c" "// empty source file")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
  ${PYTHON_PACKAGE_DIR}/setup.py
)

set( pip_dir "${CMAKE_CURRENT_BINARY_DIR}/pip_package")
include_directories(.)
# include_directories(pyibexlib)
add_subdirectory(binding)
add_subdirectory(pyibex_lib)
# add_subdirectory(geometry)
add_subdirectory(pyibex_image)
add_subdirectory(pyibex_thickset)


# install( DIRECTORY "${PYTHON_PACKAGE_DIR}/pyibex" DESTINATION "${PYTHON_INSTDIR}")


add_custom_target(pip_package)
add_custom_command(TARGET pip_package PRE_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ARGS setup.py bdist_wheel -d ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
)

add_custom_target(install_python)
add_custom_command(TARGET install_python PRE_BUILD
    COMMAND pip install --upgrade .
    WORKING_DIRECTORY ${PYTHON_PACKAGE_DIR}
)

# add_dependencies(install_python pyibex image)
