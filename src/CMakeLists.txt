add_library(weatherParserObj weatherParser.cpp)
target_include_directories(weatherParserObj PUBLIC ${PROJECT_SRC})

add_executable(xmas_light_main xmas_light_main.cpp)