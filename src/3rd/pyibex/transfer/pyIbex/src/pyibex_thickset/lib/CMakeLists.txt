set( SRCS
			ThickPaving.cpp
			ThickTest.cpp
      # SepPaving.cpp
			# ThickInterval.cpp
			# pyIbex_Paving2.cpp
      # PSetNode_impl.cpp
	  vibes.cpp
	#   sweepTest.cpp

		)
set( HEADERS
			ThickPaving.h
			ThickTest.h
			ThickTest.h
			ThickBoolean.h
			PavingVisitor.h
			ThickInterval.h
			PNode.h
			PNode_impl.hpp
      # PSetNode.h

      # SepPaving.h
		ThickBox.h
		ThickBox_impl.hpp
		ThickFunction.h
		GeoMapper.h
		vibes.h
		# sweepTest.h
				# sweepTest_v2.h
		# exploredAreaTools.h

		)
include_directories(.)
# FILE(COPY ${HEA/DERS} DESTINATION ${HEADERS_DIR})

add_library(pyibex_thickset ${SRCS} ${HEADERS})
if(NOT MSVC)
	# Make sure C++11/14 are enabled
	target_compile_options(pyibex_thickset PUBLIC ${CPP_STANDARD})
endif()
target_link_libraries(pyibex_thickset Ibex::ibex)
target_include_directories(pyibex_thickset PUBLIC ${CMAKE_SOURCE_BINARY_DIR})

# INSTALL( TARGETS pyibex_thickset DESTINATION lib)

# install(FILES ${HEADERS} DESTINATION "include/pyibex/thickset")

# add_subdirectory(test)
