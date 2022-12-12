cmake_minimum_required(VERSION 3.11)
project(course_test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(Boost_USE_MULTITHREADED ON)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/boost_1_80_0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/local/boost_1_80_0/libs")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/tgbot-cpp/include")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/sqlite")



find_package(Boost 1.80.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL)

include_directories(/usr/local/include  ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${SQLITE3_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

find_package (SQLite3)



set(Libs
    /usr/local/lib/libTgBot.a
    ${CMAKE_THREAD_LIBS_INIT} 
    ${Boost_LIBRARIES} 
    ${SQLITE3_LIBRARIES} 
    ${OPENSSL_LIBRARIES}
    )

add_executable(course_test src/source.cpp)

target_link_libraries(course_test /usr/local/lib/libTgBot.a TgBot ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES}  ${SQLITE3_LIBRARIES})
