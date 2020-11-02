# add_executable(weatherParserUT weatherParserUT.cpp)
# target_link_libraries(weatherParserUT gtest_main weatherParserObj)

add_executable(xmasLightUT xmas_light_unittest.cpp)
target_include_directories(xmasLightUT PRIVATE "${PROJECT_SRC}")
target_link_libraries(xmasLightUT gtest_main )

add_executable(xmasLightNewUT xmas_light_new_unittest.cpp)
target_include_directories(xmasLightNewUT PRIVATE "${PROJECT_SRC}")
target_link_libraries(xmasLightNewUT gtest_main )