include(FetchContent)
FetchContent_Declare(tgbot-cpp
GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp
GIT_TAG origin/master)
FetchContent_GetProperties(tgbot-cpp)
if(NOT tgbot-cpp_POPULATED)
FetchContent_Populate(tgbotcpp)
add_subdirectory(${tgbot-cpp_SOURCE_DIR} ${tgbot-cpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(bot PUBLIC tgbot)
