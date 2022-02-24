set( SRCS
      pyibex_images.cpp
   )

set( HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_GeoImage.h
  )

include_directories(.)

PYBIND11_ADD_MODULE(image SHARED ${SRCS})
target_link_libraries(image PRIVATE Ibex::ibex)

add_custom_command(TARGET image POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:image>" "${PYTHON_PACKAGE_DIR}/pyibex/image"
)
