
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include<stdio.h>
#include<regex>
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
	cout << "Success!" << endl;
	return 0;
}
int keyboard_sql(void* NotUsed, int argc, char** argv, char** azColName) {

	for (int i = 0; i < argc; i++) {
		string s = "_";
		s += argv[i];

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

string str_slice(string s)
{
	string temp = "'";
	int i = 0;
	while (s[i] != ',')
	{
		temp += s[i];
		i++;
	}
	i += 2;
	temp += "', '";
	while (i < s.size())
	{
		temp += s[i];
		i++;
	}
	temp += "'";
	return temp;
}
sqlite3* db;
char* zErrMsg = 0;
int rc;
string sql;
int main() {


	TgBot::Bot bot("5499577758:AAEomE4kaYJlRSnu53F6c3kGCks2A5C8hYQ");

	rc = sqlite3_open("data.db", &db);
	sql = "UPDATE 'CANTEENB 'SET AVAILABILITY = '1';";
	rc = sqlite3_exec(db, sql.c_str(), keyboard_sql, 0, &zErrMsg);



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

	InlineKeyboardButton::Ptr addDish(new InlineKeyboardButton);
	addDish->text = "Add dish";
	addDish->callbackData = "Add dish";
	adrow0.push_back(addDish);

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

	/*InlineKeyboardButton::Ptr CanteenK(new InlineKeyboardButton);
	CanteenK->text = "Canteen K";
	CanteenK->callbackData = "Canteen K";
	studrow0.push_back(CanteenK);

	InlineKeyboardButton::Ptr CanteenL(new InlineKeyboardButton);
	CanteenL->text = "Canteen L";
	CanteenL->callbackData = "Canteen L";
	studrow0.push_back(CanteenL);
	*/

	studentkeyboard->inlineKeyboard.push_back(studrow0);

	//клавиатура выбора блюда CanteenB, CanteenG
	InlineKeyboardMarkup::Ptr dishkeyboardB(new InlineKeyboardMarkup);
	InlineKeyboardMarkup::Ptr dishkeyboardG(new InlineKeyboardMarkup);
	vector<vector<string>> names{ {"Breakfast","Soup"},
		{"Garnish","Hot Dishes"},
		{"Salad","Drinks"},
		{"Desserts","Baked goods" } };
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

	InlineKeyboardMarkup::Ptr admkeyboard(new InlineKeyboardMarkup);
	sql = "SELECT NAME FROM 'CANTEENB' WHERE AVAILABILITY = 0;";
	rc = sqlite3_exec(db, sql.c_str(), keyboard_sql, 0, &zErrMsg);
	vector<InlineKeyboardButton::Ptr> row_adm(1);
	for (int i = 0; i < adm_keyboard.size(); i++)
	{

		InlineKeyboardButton::Ptr button_adm(new InlineKeyboardButton);

		string s = adm_keyboard[i];
		std::regex regex1("_");
		std::vector<std::string> out(
			std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
			std::sregex_token_iterator()
		);

		button_adm->text = out[1];
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
		bot.getApi().sendMessage(message->chat->id, "Hi! Please choose role:", false, 0, startkeyboard);
		cout << "onCommand_start"<<endl;
		});
	//обработка команды студент
	bot.getEvents().onCallbackQuery([&bot, &startkeyboard, &studentkeyboard](CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "Student")) {
			string response = "Show menu?";
			bot.getApi().sendMessage(query->message->chat->id, response, false, 0, studentkeyboard, "Markdown");
			cout << "Show menu?";
		}
		});

	//обработка выбора роли
	bot.getEvents().onCallbackQuery([&bot, &startkeyboard, &adminkeyboard, &admkeyboard, &typekeyboard, &studentkeyboard, &types](CallbackQuery::Ptr query) {
		//отработка команды админ
		cout << "Obrabotka command admin"<<endl;
		if (StringTools::startsWith(query->data, "Admin")) {
			string response = "Please enter the password";
			cout << response<<endl;
			bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
			bot.getEvents().onCommand("12345678B",[&bot, &admkeyboard, &adminkeyboard, &typekeyboard, &types](Message::Ptr message)
				{
					printf("User wrote %s\n", message->text.c_str());
					
						string response = "Right! Choose option: ";
						bot.getApi().sendMessage(message->chat->id, response, false, 0, adminkeyboard, "Markdown");
					
				}
			);
		}
		});

	bot.getEvents().onCallbackQuery([&bot, &adminkeyboard, &admkeyboard, &typekeyboard, &types](CallbackQuery::Ptr query)
		{
			//обработка команды очистки текущего меню
			cout << "obrabotka command clean"<<endl;
			if (StringTools::startsWith(query->data, "Clean current menu")) {
				sql = "UPDATE 'CANTEENB' SET AVAILABILITY = '0';";
				rc = sqlite3_open("data.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				string response = "Menu was cleaned!";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, adminkeyboard, "Markdown");
			}
		});
	bot.getEvents().onCallbackQuery([&bot, &adminkeyboard, &admkeyboard, &typekeyboard, &types](CallbackQuery::Ptr query)
		{
			//Обработка команды добавления текущих блюд
			cout << "obrabotka add new dish"<<endl;
			if (StringTools::startsWith(query->data, "Add dish")) {
				string response = "Choose dish:";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, admkeyboard, "Markdown");
			}
		});
	bot.getEvents().onCallbackQuery([&bot, &admkeyboard,&row_adm](CallbackQuery::Ptr query)
		{
			cout << "obrabotka cur dish" << endl;
			if (query->data.size() > 0 && query->data[0] == '_')
			{
				std::regex regex1("_");
				string s = query->data;
				std::vector<std::string> out(
					std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
					std::sregex_token_iterator()
				);
				sql = "UPDATE 'CANTEENB' SET AVAILABILITY = 1  WHERE NAME = '";
				sql = sql + out[1] + "';";
				rc = sqlite3_open("data.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				cout << sql;
				string response = "Dish was added!";
				bot.getApi().sendMessage(query->message->chat->id, response, "Markdown");
				admkeyboard->inlineKeyboard.clear();
				adm_keyboard.clear();
				sql = "SELECT NAME FROM 'CANTEENB' WHERE AVAILABILITY = 0;";
				rc = sqlite3_exec(db, sql.c_str(), keyboard_sql, 0, &zErrMsg);
				row_adm.clear();
				row_adm.push_back(0);
				for (int i = 0; i < adm_keyboard.size(); i++)
				{
					cout << adm_keyboard[i] << endl;

					InlineKeyboardButton::Ptr button_adm(new InlineKeyboardButton);

					string s = adm_keyboard[i];
					std::regex regex1("_");
					std::vector<std::string> out(
						std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
						std::sregex_token_iterator()
					);

					button_adm->text = out[1];
					button_adm->callbackData = adm_keyboard[i];
					row_adm[0] = button_adm;
					admkeyboard->inlineKeyboard.push_back(row_adm);
				}
				response = "Choose dish:";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, admkeyboard, "Markdown");
			}
		}
	);
	

	bot.getEvents().onCallbackQuery([&bot, &adminkeyboard, &admkeyboard, &typekeyboard, &types](CallbackQuery::Ptr query)
		{
			//обработка команды добавления нового блюда
			if (StringTools::startsWith(query->data, "Add new dish")) {
				string response = "Please, choose type!";
				bot.getApi().sendMessage(query->message->chat->id, response, false, 0, typekeyboard, "Markdown");
			}
		});
	bot.getEvents().onCallbackQuery([&bot, &typekeyboard, &adminkeyboard, &types](CallbackQuery::Ptr query)
		{
			cout << query->data << endl;
			auto cur_query = query->data;
			for (int i = 0; i < 4; i++)
			{
				if (find(types[i].begin(), types[i].end(), cur_query) != types[i].end())

				{
					string response = "Please, write down the name and price in format 'name,price'";
					bot.getApi().sendMessage(query->message->chat->id, response);
					bot.getEvents().onNonCommandMessage([&bot, cur_query, &adminkeyboard](Message::Ptr message) {
						cout << message->text << endl;
						std::string s = message->text;
						std::regex regex1(",");

						std::vector<std::string> out(
							std::sregex_token_iterator(s.begin(), s.end(), regex1, -1),
							std::sregex_token_iterator()

						);
						std::regex regex2("_");

						std::vector<std::string> out_2(
							std::sregex_token_iterator(cur_query.begin(), cur_query.end(), regex2, -1),
							std::sregex_token_iterator()

						);
						sql = "INSERT INTO 'CANTEENB' (AVAILABILITY, TYPE,NAME,PRICE) VALUES ('1', '" + out_2[0] + "', '" + out[0] + "', '" + out[1] + "');";
						rc = sqlite3_open("data.db", &db);
						rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
						sql = "";

						bot.getApi().sendMessage(message->chat->id, "Done!","Markdown");
						});
					cur_query = "";

				}
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
	/*отработка команды CanteenK
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
		});*/
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

				sql = "SELECT NAME,PRICE FROM 'CANTEENB' WHERE TYPE='" + out[0] + "' AND AVAILABILITY = 1;";
				cout << sql;
				rc = sqlite3_open("data.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				std::cout << output << std::endl;
				if (output == "")
					output = "Not found :(";
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

				sql = "SELECT NAME,PRICE FROM 'CANTEENG' WHERE TYPE='" + out[0] + "' AND AVAILABILITY = 1;";
				cout << sql;
				rc = sqlite3_open("data.db", &db);
				rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
				std::cout << output << std::endl;
				if (output == "")
					output = "Not found :(";
				bot.getApi().sendMessage(query->message->chat->id, output, false, 0, dishkeyboardG, "Markdown");
				output = "";

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
