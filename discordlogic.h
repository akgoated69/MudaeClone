#ifndef DISCORDLOGIC
#define DISCORDLOGIC

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <dpp/dpp.h>
#include "cardClass.h"
#include "sqllogic.h"

dpp::embed makeCard(Mudae::Card c, sql::Connection* con);

bool checkForCommand(const dpp::slashcommand_t& event, const std::string& command);

void sendMsgWEmbed(const dpp::slashcommand_t& event,dpp::embed& embed);

void createBotCommand(std::string command, std::string description, dpp::cluster& bot);

std::string vectorStringToMessage(std::vector<std::string> values);


#endif