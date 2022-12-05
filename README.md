# course_work
## Сборка проекта
#### Установка tgbot-cpp и зависимостей на Windows:
1) Проверьте версию Cmake для сборки\
```cmake --version```
версия должна быть не ниже 3.14
2) Включите FetchContent в ваш CMake build script:

```
include(FetchContent)
```
3) Объявите источник:
```
FetchContent_Declare(tgbot-cpp
GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp```
GIT_TAG origin/master)
```
4) CMake скачает репозиторий и включит в субдиректорию
```
FetchContent_GetProperties(tgbot-cpp)
if(NOT tgbot-cpp_POPULATED)
FetchContent_Populate(tgbotcpp)
add_subdirectory(${tgbot-cpp_SOURCE_DIR} ${tgbot-cpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
```
5) Укажите target
```
target_link_libraries(my_target PUBLIC tgbot-cpp)
```
### Установка boost
1) Скачиваем [MinGW installer mingw-w64-install.exe from Sourceforge](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/)
2) Скачиваем [boost_1_80_0.zip source from Sourceforge](https://sourceforge.net/projects/boost/files/boost/1.80.0/)
3) Устанавливем MinGW
4) Добавляем в системные пути переменных сред
```
set PATH=%PATH%;C:\MinGW\bin
```
```
set PATH=%PATH%;C:\MinGW\bin
```
5) Распакуйте boost_1_80_0 в install/boost_1_80_0
6) Создайте директории для сборки и установки
```
mkdir C:\boost-build
mkdir C:\install\boost_1_68_0\boost-build
mkdir C:\boost
```
Подробнее об установке boost: [здесь](https://gist.github.com/zrsmithson/0b72e0cb58d0cb946fc48b5c88511da8)
### Установка Sqlite
1) Выбираем [нужну версию](https://sqlite.org/download.html), скачиваем и устанавливаем
