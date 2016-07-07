# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


set(SRC
  main.cpp
)

set(HEADERS

)

add_executable(ex_integ-msinx ${SRC})

target_link_libraries(ex_integ-msinx
  ${LIBS}
  tube-core
  tube-graphics)