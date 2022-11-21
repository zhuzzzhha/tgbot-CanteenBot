
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include<stdio.h>
#include <boost/system/system_error.hpp>
#include "sqlite/sqlite3.h"
#include <tgbot/tgbot.h>
#include <map>
using namespace std;
using namespace TgBot;
string output = "";
vector<string> adm_keyboard;
int callback(void* NotUsed, int argc, char** argv, char** azColName) {


	for (int i = 0; i < argc; i++) {

		output = output + azColName[i] + ": " + argv[i] + '\n';
		cout << azColName[i] << ": " << argv[i] << '\n';
	}
	output += '\n';

	return 0;
}
int keyboard_sql(void* NotUsed, int argc, char** argv, char** azColName) {


	for (int i = 0; i < argc; i++) {
		string s = argv[i];

		adm_keyboard.push_back(s);
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


	TgBot::Bot bot("5499577758:AAEomE4kaYJlRSnu53F6c3kGCks2A5C8hYQ");

	rc = sqlite3_open("test.db", &db);

	//стартовая клавиатура для выбора роли
	InlineKeyboardMarkup::Ptr startkeyboard(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> startrow0;
	InlineKeyboardButton::Ptr studButton(new InlineKeyboardButton);
	studButton->text = "Student";
	studButton->callbackData = "Student";
	startrow0.push_back(studButton);

	InlineKeyboardButton::Ptr adminButton(new InlineKeyboardButton);
	adminButton->text = "Admin";
	adminButton->callbackData = "Admin";
	startrow0.push_back(adminButton);
	startkeyboard->inlineKeyboard.push_back(startrow0);

	//клавиатура администратора
	InlineKeyboardMarkup::Ptr adminkeyboard(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> adrow0;
	InlineKeyboardButton::Ptr newDish(new InlineKeyboardButton);
	newDish->text = "Add new dish";
	newDish->callbackData = "Add new dish";
	adrow0.push_back(newDish);

	InlineKeyboardButton::Ptr cleanMenu(new InlineKeyboardButton);
	cleanMenu->text = "Clean current menu";
	cleanMenu->callbackData = "Clean current menu";
	adrow0.push_back(cleanMenu);

	adminkeyboard->inlineKeyboard.push_back(adrow0);

	//клавиатура студента
	InlineKeyboardMarkup::Ptr studentkeyboard(new InlineKeyboardMarkup);
	vector<InlineKeyboardButton::Ptr> studrow0;
	InlineKeyboardButton::Ptr CanteenB(new InlineKeyboardButton);
	CanteenB->text = "Canteen B";
	CanteenB->callbackData = "Canteen B";
	studrow0.push_back(CanteenB);

	InlineKeyboardButton::Ptr CanteenG(new InlineKeyboardButton);
	CanteenG->text = "Canteen G";
	CanteenG->callbackData = "Canteen G";
	studrow0.push_back(CanteenG);

	InlineKeyboardButton::Ptr CanteenK(new InlineKeyboardButton);
	CanteenK->text = "Canteen K";
	CanteenK->callbackData = "Canteen K";
	studrow0.push_back(CanteenK);

	InlineKeyboardButton::Ptr CanteenL(new InlineKeyboardButton);
	CanteenL->text = "Canteen L";
	CanteenL->callbackData = "Canteen L";
	studrow0.push_back(CanteenL);


	studentkeyboard->inlineKeyboard.push_back(studrow0);


	//клавиатура выбора блюда CanteenB
	InlineKeyboardMarkup::Ptr dishkeyboardB(new InlineKeyboardMarkup);
	vector<vector<string>> names{ {"Breakfast","Soup"},
		{"Garnish","Hot Dishes"},
		{"Salad","Drinks"},
		{"Desserts","Complex lunch" } };
	vector<vector<string>> buttons_b{ {"Breakfast_B","Soup_B"},
		{"Garnish_B","Hot Dishes_B"},
		{"Salad_B","Drinks_B"},
		{"Desserts_B","Complex lunch_B" } };
	vector<vector<InlineKeyboardButton::Ptr>> dishrow(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			InlineKeyboardButton::Ptr buttonsB(new InlineKeyboardButton);
			buttonsB->text = names[i][j];
			buttonsB->callbackData = buttons_b[i][j];
			dishrow[i].push_back(buttonsB);
		}
		dishkeyboardB->inlineKeyboard.push_back(dishrow[i]);
	}
	//функция отрисовки клавиатуры для столовых
	/*vector<InlineKeyboardMarkup::Ptr> temp(4);
	map<string,InlineKeyboardMarkup::Ptr> canteen_keyboard =
	{ {"B",temp[0]},
		{"G",temp[1]},
		{"K",temp[2]},
		{"L",temp[3]} };
	vector<string> liters {"B","G","K","L"};
	for (int k = 0; k < 4; k++)
	{
		InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);

		for (int i = 0; i < 4; i++)
		{
			vector<InlineKeyboardButton::Ptr>dishrow;
			for (int j = 0; j < 2; j++)
			{
				InlineKeyboardButton::Ptr buttons(new InlineKeyboardButton);
				buttons->text = names[i][j];
				buttons->callbackData = names[i][j] + "_" + liters[k];
				dishrow.push_back(buttons);
			}
			canteen_keyboard[liters[k]]->inlineKeyboard.push_back(dishrow);
		}

	}*/
	//клавиатура выбора блюда CanteenG
	InlineKeyboardMarkup::Ptr dishkeyboardG(new InlineKeyboardMarkup);
	vector<vector<string>> namesG{ {"Breakfast_G","Soup_G"},
		{"Garnish_G","Hot Dishes_G"},
		{"Salad_G","Drinks_G"},
		{"Desserts_G","Complex lunch_G" } };

	vector<vector<InlineKeyboardButton::Ptr>> dishrowG(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			InlineKeyboardButton::Ptr button_g(new InlineKeyboardButton);
			button_g->text = names[i][j];
			button_g->callbackData = namesG[i][j];
			dishrowG[i].push_back(button_g);
		}
		dishkeyboardG->inlineKeyboard.push_back(dishrowG[i]);
	}
	//клавиатура администратора для добавления блюда 
	InlineKeyboardMarkup::Ptr admkeyboard(new InlineKeyboardMarkup);
	sql = "SELECT NAME FROM 'CANTEENB' WHERE AVAILABILITY = 1 AND ID>1;";
	rc = sqlite3_exec(db, sql.c_str(), keyboard_sql, 0, &zErrMsg);
	vector<InlineKeyboardButton::Ptr> row_adm(1);
	for (int i = 0; i < adm_keyboard.size(); i++)
	{

		InlineKeyboardButton::Ptr button_adm(new InlineKeyboardButton);
		button_adm->text = adm_keyboard[i];
		button_adm->callbackData = adm_keyboard[i];
		row_adm[0] = button_adm;
		admkeyboard->inlineKeyboard.push_back(row_adm);
	}
	// клавиатура выбора типа добавления блюда
	InlineKeyboardMarkup::Ptr typekeyboard(new InlineKeyboardMarkup);
	vector<vector<string>> types{ {"Breakfast_t","Soup_t"},
		{"Garnish_t","Hot Dishes_t"},
		{"Salad_t","Drinks_t"},
		{"Desserts_t","Complex lunch_t" } };
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
		bot.getApi().sendMessage(message->chat->id, "Hi! Please choose your role", false, 0, startkeyboard);
		});

	//отработка команды студент
	bot.getEvents().onCallbackQuery([&bot, &startkeyboard, &studentkeyboard](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Student")) {
			string response = "Show menu?";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, studentkeyboard, "Markdown");
		}
		});

	//отработка команды админ
	bot.getEvents().onCallbackQuery([&bot, &startkeyboard, &adminkeyboard](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Admin")) {
			string response = "Please enter the password";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, adminkeyboard, "Markdown");
		}
		});
	bot.getEvents().onCallbackQuery([&bot, &adminkeyboard](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Clean current menu")) {
			sql = "UPDATE 'CANTEENB' SET AVAILABILITY = 0;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			string response = "Done!";
			bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
		}
		});
	bot.getEvents().onCallbackQuery([&bot, &adminkeyboard, &typekeyboard](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Add new dish")) {
			string response = "Please, choose type!";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, typekeyboard, "Markdown");
			bot.getEvents().onCallbackQuery([&bot, &typekeyboard](CallbackQuery::Ptr query)
				{
					if (StringTools::startsWith(query->data, "Breakfast_t"))
					{
						string response = "Please, write down the name";
						bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
					}
					bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
						printf("User wrote %s\n", message->text.c_str());
						sql = "INSERT INTO 'CANTEENB' (ID,TYPE,NAME,PRICE) VALUES (1, Breakfast) "
						bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
						});

				});
		}
		});
	//после ввода пароля
	// 
	// 
	//отработка команды CanteenB
	bot.getEvents().onCallbackQuery([&bot, &studentkeyboard, &dishkeyboardB](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Canteen B")) {
			string response = "Choose type";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, dishkeyboardB, "Markdown");
			output = "";
		}
		});
	//отработка команды CanteenG
	bot.getEvents().onCallbackQuery([&bot, &studentkeyboard, &dishkeyboardG](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Canteen G")) {
			string response = "Choose type";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		});
	//отработка команды CanteenK
	bot.getEvents().onCallbackQuery([&bot, &studentkeyboard, &dishkeyboardB](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Canteen K")) {
			string response = "Choose type";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, dishkeyboardB, "Markdown");
			output = "";
		}
		});
	//отработка команды CanteenL
	bot.getEvents().onCallbackQuery([&bot, &studentkeyboard, &dishkeyboardB](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Canteen L")) {
			string response = "Choose type";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, dishkeyboardB, "Markdown");
			output = "";
		}
		});
	//показ меню Canteen B
	bot.getEvents().onCallbackQuery([&bot, &dishkeyboardB](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Soup_B"))
		{
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Soup' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";
		}

		if (StringTools::startsWith(query->data, "Breakfast_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Breakfast' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Hot Dishes_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Hot Dishes' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Garnish_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Garnish' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Salad_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Salad' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Desserts_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Desserts';";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Complex lunch_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Complex lunch' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Drinks_B")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='Drinks' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			std::cout << output << std::endl;
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardB, "Markdown");
			output = "";

		}
		});
	//показ меню Canteen G
	bot.getEvents().onCallbackQuery([&bot, &dishkeyboardG](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Soup_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Soup' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Breakfast_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Breakfast' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";

		}
		if (StringTools::startsWith(query->data, "Hot Dishes_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Hot Dishes' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		if (StringTools::startsWith(query->data, "Garnish_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Garnish' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		if (StringTools::startsWith(query->data, "Salad_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Salad' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		if (StringTools::startsWith(query->data, "Desserts_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Desserts' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		if (StringTools::startsWith(query->data, "Complex lunch_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Complex lunch' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		if (StringTools::startsWith(query->data, "Drinks_G")) {
			sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='Drinks' AND AVAILABILITY = 1;";
			rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
			bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
			output = "";
		}
		});

	signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });
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
