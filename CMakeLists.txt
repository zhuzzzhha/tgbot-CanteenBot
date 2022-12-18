cmake_minimum_required(VERSION 3.14)
project(course_test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(Boost_USE_MULTITHREADED ON)





find_package(Boost 1.80.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL)
find_package(SQlite3 REQUIRED )




include(FetchContent)
FetchContent_Declare(tgbot-cpp
GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp
GIT_TAG origin/master)
FetchContent_GetProperties(tgbot-cpp)
if(NOT tgbot-cpp_POPULATED)
FetchContent_Populate(tgbot-cpp)
FetchContent_MakeAvailable(tgbot-cpp)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test.db
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(${tgbot-cpp_SOURCE_DIR} ${tgbot-cpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()




include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${SQLite3_INCLUDE_DIRS})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()


add_executable(course_test src/source.cpp)

target_link_libraries(course_test PUBLIC TgBot ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES}  ${SQLite3_LIBRARIES})
