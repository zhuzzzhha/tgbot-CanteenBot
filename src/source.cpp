
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include<regex>
#include <boost/system/system_error.hpp>
#include "sqlite/sqlite3.h"
#include <tgbot/tgbot.h>
#include <map>
#define SQLITE_UTF16
using namespace std;
using namespace TgBot;
string output = "";
vector<string> adm_keyboard_B;
vector<string> adm_keyboard_G;
int callback(void* NotUsed, int argc, char** argv, char** azColName) {


	for (int i = 0; i < argc; i++) {

		output = output + azColName[i] + ": " + argv[i] + '\n';
		cout << azColName[i] << ": " << argv[i] << '\n';
	}
	output += '\n';
	cout << "Success!" << endl;
	return 0;
}
int Bkeyboard_sql(void* NotUsed, int argc, char** argv, char** azColName) {

	for (int i = 0; i < argc; i++) {
		string s = "b_";
		s += argv[i];

		adm_keyboard_B.push_back(s);
	}

	return 0;
}
int Gkeyboard_sql(void* NotUsed, int argc, char** argv, char** azColName) {

	for (int i = 0; i < argc; i++) {
		string s = "g_";
		s += argv[i];

		adm_keyboard_G.push_back(s);
	}

	return 0;
}
void createOneColumnKeyboard(const std::vector<std::string>& buttonStrings, ReplyKeyboardMarkup::Ptr& kb)
{
	for (size_t i = 0; i < buttonStrings.size(); ++i) {
		vector<::KeyboardButton::Ptr> row;
		KeyboardButton::Ptr button(new KeyboardButton);
		button->text = buttonStrings[i];
		row.push_back(button);
		kb->keyboard.push_back(row);
	}
}
void createKeyboard(const vector<std::vector<string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
	for (size_t i = 0; i < buttonLayout.size(); ++i) {
		vector<KeyboardButton::Ptr> row;
		for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
			KeyboardButton::Ptr button(new KeyboardButton);
			button->text = buttonLayout[i][j];
			row.push_back(button);
		}
		kb->keyboard.push_back(row);
	}
}
sqlite3* db;
char* zErrMsg = 0;
int rc;
string sql;
int main() {

	std::string token = "";
	TgBot::Bot bot(token);

	rc = sqlite3_open("test.db", &db);

	//стартовая клавиатура для выбора роли
	InlineKeyboardMarkup::Ptr startkeyboard(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> startrow0;
	InlineKeyboardButton::Ptr studButton(new InlineKeyboardButton);
	studButton->text = "Студент";
	studButton->callbackData = "Студент";
	startrow0.push_back(studButton);

	InlineKeyboardButton::Ptr adminButton(new InlineKeyboardButton);
	adminButton->text = "Администратор";
	adminButton->callbackData = "Администратор";
	startrow0.push_back(adminButton);
	startkeyboard->inlineKeyboard.push_back(startrow0);

	//клавиатура администратора
	InlineKeyboardMarkup::Ptr adminkeyboardB(new InlineKeyboardMarkup);
	InlineKeyboardMarkup::Ptr adminkeyboardG(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> adrowB0;
	vector<InlineKeyboardButton::Ptr> adrowG0;
	InlineKeyboardButton::Ptr newDishB(new InlineKeyboardButton);
	InlineKeyboardButton::Ptr newDishG(new InlineKeyboardButton);
	newDishB->text = "Добавить новое блюдо";
	newDishB->callbackData = "Добавить новое блюдо_B";
	newDishG->text = "Добавить новое блюдо";
	newDishG->callbackData = "Добавить новое блюдо_G";
	adrowB0.push_back(newDishB);
	adrowG0.push_back(newDishG);

	InlineKeyboardButton::Ptr cleanMenuB(new InlineKeyboardButton);
	InlineKeyboardButton::Ptr cleanMenuG(new InlineKeyboardButton);
	cleanMenuB->text = "Очистить текущее меню";
	cleanMenuB->callbackData = "Очистить текущее меню_B";
	cleanMenuG->text = "Очистить текущее меню";
	cleanMenuG->callbackData = "Очистить текущее меню_G";
	adrowB0.push_back(cleanMenuB);
	adrowG0.push_back(cleanMenuG);

	InlineKeyboardButton::Ptr addDishB(new InlineKeyboardButton);
	InlineKeyboardButton::Ptr addDishG(new InlineKeyboardButton);
	addDishB->text = "Добавить блюдо";
	addDishB->callbackData = "Добавить блюдо_B";
	addDishG->text = "Добавить блюдо";
	addDishG->callbackData = "Добавить блюдо_G";
	adrowB0.push_back(addDishB);
	adrowG0.push_back(addDishG);

	adminkeyboardB->inlineKeyboard.push_back(adrowB0);
	adminkeyboardG->inlineKeyboard.push_back(adrowG0);

	//клавиатура студента
	InlineKeyboardMarkup::Ptr studentkeyboard(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> studrow0;
	InlineKeyboardButton::Ptr CanteenB(new InlineKeyboardButton);
	CanteenB->text = "Столовая Б";
	CanteenB->callbackData = "Canteen B";
	studrow0.push_back(CanteenB);

	InlineKeyboardButton::Ptr CanteenG(new InlineKeyboardButton);
	CanteenG->text = "Столовая Г";
	CanteenG->callbackData = "Canteen G";
	studrow0.push_back(CanteenG);
	studentkeyboard->inlineKeyboard.push_back(studrow0);

	//клавиатура выбора блюда CanteenB, CanteenG
	InlineKeyboardMarkup::Ptr dishkeyboardB(new InlineKeyboardMarkup);
	InlineKeyboardMarkup::Ptr dishkeyboardG(new InlineKeyboardMarkup);
	vector<vector<string>> names{ {"Завтрак","Суп"},
		{"Гарнир","Горячее"},
		{"Салат","Напитки"},
		{"Дессерты","Комплексный обед" } };
	vector<vector<InlineKeyboardButton::Ptr>> dishrowB(4);
	vector<vector<InlineKeyboardButton::Ptr>> dishrowG(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			InlineKeyboardButton::Ptr buttonsB(new InlineKeyboardButton);
			InlineKeyboardButton::Ptr buttonsG(new InlineKeyboardButton);
			buttonsB->text = names[i][j];
			buttonsB->callbackData = names[i][j] + "_B";

			buttonsG->text = names[i][j];
			buttonsG->callbackData = names[i][j] + "_G";

			dishrowB[i].push_back(buttonsB);
			dishrowG[i].push_back(buttonsG);
		}
		dishkeyboardB->inlineKeyboard.push_back(dishrowB[i]);
		dishkeyboardG->inlineKeyboard.push_back(dishrowG[i]);
	}
	//клавиатура администратора для добавления блюда 

	InlineKeyboardMarkup::Ptr admkeyboard_B(new InlineKeyboardMarkup);
	InlineKeyboardMarkup::Ptr admkeyboard_G(new InlineKeyboardMarkup);
	sql = "SELECT Название FROM 'CANTEENB' WHERE Наличие = 0;";
	rc = sqlite3_exec(db, sql.c_str(), Bkeyboard_sql, 0, &zErrMsg);
	sql = "SELECT Название FROM 'CANTEENG' WHERE Наличие = 0;";
	rc = sqlite3_exec(db, sql.c_str(), Gkeyboard_sql, 0, &zErrMsg);
	vector<InlineKeyboardButton::Ptr> row_adm_B(1);
	vector<InlineKeyboardButton::Ptr> row_adm_G(1);
	for (int i = 0; i < adm_keyboard_B.size(); i++)
	{

		InlineKeyboardButton::Ptr button_adm_B (new InlineKeyboardButton);

		string s = adm_keyboard_B[i];
		std::regex regex1("_");
		std::vector<std::string> out(
			std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
			std::sregex_token_iterator()
		);

		button_adm_B->text = out[1];
		button_adm_B->callbackData = adm_keyboard_B[i];
		row_adm_B[0] = button_adm_B;
		admkeyboard_B->inlineKeyboard.push_back(row_adm_B);
	}
	for (int i = 0; i < adm_keyboard_G.size(); i++)
	{

		InlineKeyboardButton::Ptr button_adm_G(new InlineKeyboardButton);

		string s = adm_keyboard_G[i];
		std::regex regex1("_");
		std::vector<std::string> out(
			std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
			std::sregex_token_iterator()
		);

		button_adm_G->text = out[1];
		button_adm_G->callbackData = adm_keyboard_G[i];
		row_adm_G[0] = button_adm_G;
		admkeyboard_G->inlineKeyboard.push_back(row_adm_G);
	}

	// клавиатура выбора типа добавления блюда
	InlineKeyboardMarkup::Ptr typekeyboard(new InlineKeyboardMarkup);
	vector<vector<string>> types{ {"Завтрак_t","Суп_t"},
		{"Гарнир_t","Горячее_t"},
		{"Салат_t","Напитки_t"},
		{"Дессерты_t","Комплексный обед_t" } };
	vector<vector<InlineKeyboardButton::Ptr>> type_row(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			InlineKeyboardButton::Ptr type_button(new InlineKeyboardButton);
			type_button->text = names[i][j];
			type_button->callbackData = types[i][j];
			type_row[i].push_back(type_button);
		}
		typekeyboard->inlineKeyboard.push_back(type_row[i]);
	}

	//отработка команды start
	bot.getEvents().onCommand("start", [&bot, &startkeyboard](Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Привет! Выбери свою роль:", false, 0, startkeyboard);
		cout << "onCommand_start" << endl;
		});
	//обработка команды студент
	bot.getEvents().onCallbackQuery([&bot, &startkeyboard, &studentkeyboard](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Студент")) {
			string response = "Выбери столовую:";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, studentkeyboard, "Markdown");
			cout << "Show menu?";
		}
		});

	//обработка выбора роли
	bot.getEvents().onCallbackQuery([&bot, &startkeyboard, &adminkeyboardB, &adminkeyboardG,&admkeyboard_B, &typekeyboard, &studentkeyboard, &types](CallbackQuery::Ptr query) {
		//отработка команды админ
		cout << "Obrabotka command admin" << endl;
		if (StringTools::startsWith(query->data, "Администратор")) {
			string response = "Введите, пожалуйста, пароль:";
			bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
			bot.getEvents().onCommand("12345678B", [&bot, &admkeyboard_B, &adminkeyboardB, &typekeyboard, &types](Message::Ptr message)
				{
					printf("User wrote %s\n", message->text.c_str());

					string response = "Вход выполнен! Выберите опцию:";
					bot.getApi().sendMessage(message->chat->id, response, false, 0, adminkeyboardB, "Markdown");

				}
			);
			bot.getEvents().onCommand("12345678G", [&bot,&adminkeyboardB, &typekeyboard, &types, &adminkeyboardG](Message::Ptr message)
				{
					printf("User wrote %s\n", message->text.c_str());

					string response = "Вход выполнен! Выберите опцию:";
					bot.getApi().sendMessage(message->chat->id, response, false, 0, adminkeyboardG, "Markdown");

				}
			);
		}
		});

	bot.getEvents().onCallbackQuery([&bot, &adminkeyboardB, &adminkeyboardG,&admkeyboard_B, &admkeyboard_G,&typekeyboard, &types](CallbackQuery::Ptr query)
		{
			//обработка команды очистки текущего меню
			cout << "obrabotka command clean" << endl;
			if (StringTools::startsWith(query->data, "Очистить текущее меню_B")) {
				sql = "UPDATE 'CANTEENB' SET Наличие = '0';";
				rc = sqlite3_open("test.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				string response = "Меню очищено!";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, adminkeyboardB, "Markdown");
			}
			if (StringTools::startsWith(query->data, "Очистить текущее меню_G")) {
				sql = "UPDATE 'CANTEENG' SET Наличие = '0';";
				rc = sqlite3_open("test.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				string response = "Меню очищено!";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, adminkeyboardG, "Markdown");
			}
		});
	bot.getEvents().onCallbackQuery([&bot, &adminkeyboardB,&adminkeyboardG,&admkeyboard_B,&admkeyboard_G, &typekeyboard, &types](CallbackQuery::Ptr query)
		{
			//Обработка команды добавления текущих блюд
			cout << "obrabotka add new dish" << endl;
			if (StringTools::startsWith(query->data, "Добавить блюдо_B")) {
				string response = "Выбери блюдо:";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, admkeyboard_B, "Markdown");
			}
			if (StringTools::startsWith(query->data, "Добавить блюдо_G")) {
				string response = "Выберите блюдо:";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, admkeyboard_G, "Markdown");
			}
		});
	bot.getEvents().onCallbackQuery([&bot, &admkeyboard_B,&row_adm_B](CallbackQuery::Ptr query)
		{
			cout << "obrabotka cur dish" << endl;
			if (query->data.size() > 0 && query->data[0] == 'b')
			{
				std::regex regex1("_");
				string s = query->data;
				std::vector<std::string> out(
					std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
					std::sregex_token_iterator()
				);
				sql = "UPDATE 'CANTEENB' SET Наличие = 1  WHERE Название = '";
				sql = sql + out[1] + "';";
				rc = sqlite3_open("test.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				string response = "Блюдо добавлено!";
				bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
				admkeyboard_B->inlineKeyboard.clear();
				adm_keyboard_B.clear();
				sql = "SELECT Название FROM 'CANTEENB' WHERE Наличие = 0;";
				rc = sqlite3_exec(db, sql.c_str(), Bkeyboard_sql, 0, &zErrMsg);
				row_adm_B.clear();
				row_adm_B.push_back(0);
				for (int i = 0; i < adm_keyboard_B.size(); i++)
				{
					
					InlineKeyboardButton::Ptr button_adm(new InlineKeyboardButton);

					string s = adm_keyboard_B[i];
					std::regex regex1("_");
					std::vector<std::string> out(
						std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
						std::sregex_token_iterator()
					);

					button_adm->text = out[1];
					button_adm->callbackData = adm_keyboard_B[i];
					row_adm_B[0] = button_adm;
					admkeyboard_B->inlineKeyboard.push_back(row_adm_B);
				}
				response = "Выберите блюдо:";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, admkeyboard_B, "Markdown");
			}
		}
	);
	bot.getEvents().onCallbackQuery([&bot, &admkeyboard_G, &row_adm_G](CallbackQuery::Ptr query)
		{
			cout << "obrabotka cur dish" << endl;
			if (query->data.size() > 0 && query->data[0] == 'g')
			{
				std::regex regex1("_");
				string s = query->data;
				std::vector<std::string> out(
					std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
					std::sregex_token_iterator()
				);
				sql = "UPDATE 'CANTEENG' SET Наличие = 1  WHERE Название = '";
				sql = sql + out[1] + "';";
				rc = sqlite3_open("test.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				string response = "Блюдо добавлено!";
				bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
				admkeyboard_G->inlineKeyboard.clear();
				adm_keyboard_G.clear();
				sql = "SELECT Название FROM 'CANTEENG' WHERE Наличие = 0;";
				rc = sqlite3_exec(db, sql.c_str(), Gkeyboard_sql, 0, &zErrMsg);
				row_adm_G.clear();
				row_adm_G.push_back(0);
				for (int i = 0; i < adm_keyboard_G.size(); i++)
				{

					InlineKeyboardButton::Ptr button_adm(new InlineKeyboardButton);

					string s = adm_keyboard_G[i];
					std::regex regex1("_");
					std::vector<std::string> out(
						std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
						std::sregex_token_iterator()
					);

					button_adm->text = out[1];
					button_adm->callbackData = adm_keyboard_G[i];
					row_adm_G[0] = button_adm;
					admkeyboard_G->inlineKeyboard.push_back(row_adm_G);
				}
				response = "Выбери блюдо:";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, admkeyboard_G, "Markdown");
			}
		}
	);

	bot.getEvents().onCallbackQuery([&bot, &adminkeyboardB, &adminkeyboardG,&admkeyboard_B, &typekeyboard, &types](CallbackQuery::Ptr query)
		{
			//обработка команды добавления нового блюда
			if (StringTools::startsWith(query->data, "Добавить новое блюдо_B")) {
				auto cur_query = query->data;
				string response = "Введите тип,название,цену блюда в формате 'тип,название,цена'";
				bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
				bot.getEvents().onNonCommandMessage([&bot, cur_query, &adminkeyboardB](Message::Ptr message) {
					cout << message->text << endl;
					std::string s = message->text;
					std::regex regex1(",");

					std::vector<std::string> out(
						std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
						std::sregex_token_iterator()

					);
					if (out.size() < 3 || out.size() > 3)
					{
						std::string response = "Неверный формат";
						bot.getApi().sendMessage(message->chat->id, response, "Markdown");
					}
					else {
						sql = "INSERT INTO 'CANTEENB' (Наличие, Тип,Название,Цена) VALUES ('1', '" + out[0] + "', '" + out[1] + "', '" + out[2] + "');";
						rc = sqlite3_open("test.db", &db);
						rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
						sql = "";
						out.clear();
						bot.getApi().sendMessage(message->chat->id, "Блюдо создано!", "Markdown");
					}
					});
			}
			if (StringTools::startsWith(query->data, "Добавить новое блюдо_G")) {
				auto cur_query = query->data;
				string response = "Введите тип,название,цену блюда в формате 'тип,название,цена'";
				bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
				bot.getEvents().onNonCommandMessage([&bot, cur_query, &adminkeyboardG](Message::Ptr message) {
					cout << message->text << endl;
					std::string s = message->text;
					std::regex regex1(",");

					std::vector<std::string> out(
						std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
						std::sregex_token_iterator()

					);
					sql = "INSERT INTO 'CANTEENG' (Наличие, Тип,Название,Цена) VALUES ('1', '" + out[0] + "', '" + out[1] + "', '" + out[2] + "');";
					rc = sqlite3_open("test.db", &db);
					rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
					sql = "";
					out.clear();
					bot.getApi().sendMessage(message->chat->id, "Блюдо создано!", "Markdown");
					});
			}
		});
 
	//отработка команды CanteenB
	bot.getEvents().onCallbackQuery([&bot, &studentkeyboard, &dishkeyboardB](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Canteen B")) {
			string response = "Выберите тип:";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, dishkeyboardB, "Markdown");
			output = "";
		}
		});
	//отработка команды CanteenG
	bot.getEvents().onCallbackQuery([&bot, &studentkeyboard, &dishkeyboardG](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Canteen G")) {
			string response = "Выберите тип:";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		});
	//показ меню Canteen B
	bot.getEvents().onCallbackQuery([&bot, &dishkeyboardB, &names](CallbackQuery::Ptr query) {
		auto cur_query = query->data;
		std::regex regex1("_");

		std::vector<std::string> out(
			std::sregex_token_iterator(query->data.begin(), query->data.end(), regex1, -1),
			std::sregex_token_iterator()

		);
		cur_query = out[0];
		for (int i = 0; i < 4; i++)
		{
			if (find(names[i].begin(), names[i].end(), out[0]) != names[i].end() && out[1] == "B")
			{

				sql = "SELECT Название,Цена FROM 'CANTEENB' WHERE Тип='" + out[0] + "' AND Наличие = 1;";
				rc = sqlite3_open("test.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				if (output == "")
					output = "Ничего не найдено :(";
				bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
				output = "";
				break;
			}
		}
		}
	);
	//показ меню Canteen G
	bot.getEvents().onCallbackQuery([&bot, &dishkeyboardG, &names](CallbackQuery::Ptr query) {
		auto cur_query = query->data;
		std::regex regex1("_");

		std::vector<std::string> out(
			std::sregex_token_iterator(query->data.begin(), query->data.end(), regex1, -1),
			std::sregex_token_iterator()

		);
		cur_query = out[0];
		for (int i = 0; i < 4; i++)
		{
			if (find(names[i].begin(), names[i].end(), out[0]) != names[i].end() && out[1] == "G")
			{

				sql = "SELECT Название,Цена FROM 'CANTEENG' WHERE Тип='" + out[0] + "' AND Наличие = 1;";
				cout << sql;
				rc = sqlite3_open("test.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				if (output == "")
					output = "Ничего не найдено :(";
				bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
				output = "";
				break;
			}
		}
		}
	);
	try {
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		bot.getApi().deleteWebhook();

		TgLongPoll longPoll(bot);
		while (true) {
			printf("Long poll started\n");
			longPoll.start();
		}
	}
	catch (exception& e) {
		printf("error: %s\n", e.what());
	}

	return 0;
}
