
set(BINARY course_test)


add_executable(${BINARY} source.cpp)
target_include_directories(${BINARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${BINARY} ${all_libs})
