set( SRCS
			sweepTest.cpp
			# sweepTest_v2.cpp
		)
set( HEADERS
			# sweepTest.h
			# sweepTest_v2.h
    #   exploredAreaTools.h

		)

FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

add_library(pyibex_exploredArea ${SRCS} ${HEADERS})
if(NOT MSVC)
	# Make sure C++11/14 are enabled
	target_compile_options(pyibex_exploredArea PUBLIC ${CPP_STANDARD})
endif()
target_link_libraries(pyibex_exploredArea ${LIBS} )

INSTALL( TARGETS pyibex_exploredArea DESTINATION lib)

install(FILES ${HEADERS} DESTINATION "include/pyibex/thickset")


# include_directories(../Paving2)
