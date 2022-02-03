
ADD_EXECUTABLE(test_ThickBoolean test_ThickBoolean.cpp)
ADD_TEST(test_ThickBoolean test_ThickBoolean)

add_executable(test_ThickInterval test_ThickInterval.cpp)
TARGET_LINK_LIBRARIES(test_ThickInterval ${LIBS} )
ADD_TEST(test_ThickInterval test_ThickInterval)

add_executable(test_ThickPaving test_ThickPaving.cpp)
TARGET_LINK_LIBRARIES(test_ThickPaving ${LIBS} ibex-robotics-paving2)
ADD_TEST(test_ThickPaving test_ThickPaving)
