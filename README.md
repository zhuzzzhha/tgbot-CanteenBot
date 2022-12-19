# CanteenBot

## Telegram-бот для просмотра актуального меню столовых НИТУ МИСИС
Данный бот позволяет студентам просматривать меню столовых, выбирая категорию (завтрак, горячее, гарнир и т.д.)
Бот имеет отдельный интерфейс для студента и администратора. Для запуска используйте команду /start.
С помощью интерактивной клавиатуры выберите роль: <br />
![image](https://user-images.githubusercontent.com/90785698/208379496-3c75ec0c-7012-44d7-b88a-5651dc1bf8c3.png) <br />
Бот выводит информацию о блюдах и ценах: <br />
![image](https://user-images.githubusercontent.com/90785698/208379723-6e5e7e40-5da9-4cde-92b5-1ff9ce8106a1.png) <br />
Вход в роли администратора осуществляется по паролю. Функционал администратора включает в себя добавление нового блюда, очистку меню и добавление блюда в текущее меню:<br />
![image](https://user-images.githubusercontent.com/90785698/208380017-519ca063-04f1-4d05-97a1-6243a02035f1.png) <br />

# Установка проекта
### Установим необходимые зависимости
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

### Сборка проекта
Для сборки используйте CMake версии не ниже 3.14 и [CMakeLists](https://github.com/zhuzzzhha/course_work/blob/main/CMakeLists.txt)
В целях безопасности token бота зашифрован. Вставьте значения токена из инструкции.
