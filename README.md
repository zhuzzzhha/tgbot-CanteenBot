# course_work
## Сборка проекта
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

### Скачивание и установка tgbot-cpp
Скачать на Windows можно с помощью vcpkg
```
1) > git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
```
Для Windows x64:
```
> .\vcpkg\vcpkg install tgbot-cpp:x64-windows
```
Для Windows x86
```
> .\vcpkg\vcpkg install tgbot-cpp
```
Для установки используте [CMakeLists] (https://github.com/zhuzzzhha/course_work/blob/main/CMakeLists.txt)
