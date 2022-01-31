set( SRCS
      SepPaving.cpp
      PSetNode_impl.cpp
      pyibex_paving.cpp
		)
set( HEADERS
      PSetNode.h
      SepPaving.h
      PavingVisitor.h
      # vibes.h
		)

include_directories(.)
# FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

# message(STATUS ${SRCS} ${HEADERS})
# add_library(pyibex-image ${SRCS} ${HEADERS})
# if(NOT MSVC)
#   # Make sure C++11/14 are enabled
#   target_compile_options(pyibex-image  PUBLIC ${PYBIND11_CPP_STANDARD})
# endif()
# target_link_libraries(pyibex-image ${LIBS})

# INSTALL( TARGETS pyibex-image DESTINATION lib)
# install(FILES ${HEADERS} DESTINATION "include/pyibex-image")


PYBIND11_ADD_MODULE(paving SHARED  ${SRCS} )
target_link_libraries(paving PRIVATE Ibex::ibex)

add_custom_command(TARGET paving POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:paving>" "${PYTHON_PACKAGE_DIR}/pyibex"
)
