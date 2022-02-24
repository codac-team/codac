include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/exploredArea)
include_directories(.)
PYBIND11_ADD_MODULE(thickset SHARED
    pyibex_thickset.cpp
    pyibex_ThickInterval.cpp
)
target_link_libraries(thickset PRIVATE pyibex_thickset )
add_custom_command(TARGET thickset POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:thickset>" "${PYTHON_PACKAGE_DIR}/pyibex/thickset"
)


PYBIND11_ADD_MODULE(thickimage SHARED
    pyibex_GeoImages.cpp
)

target_link_libraries(thickimage PRIVATE Ibex::ibex pyibex_thickset)

add_custom_command(TARGET thickimage POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:thickimage>" "${PYTHON_PACKAGE_DIR}/pyibex/thickset"
)


# explored Area
# PYBIND11_ADD_MODULE(sweeptest SHARED pyibex_TestExploredArea.cpp )
# target_link_libraries(sweeptest PRIVATE Ibex::ibex pyibex_thickset)

# add_custom_command(TARGET sweeptest POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:sweeptest>" "${PYTHON_PACKAGE_DIR}/pyibex/thickset"
# )
