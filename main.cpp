#include <random>
#include <chrono>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

#include "cardClass.h"
#include "discordlogic.cpp"
#include "sqllogic.cpp"
#include "randomlogic.cpp"
#include "envlogic.cpp"

#define test

constexpr int REACTIONTIME{5};
const char* API_KEY = "API_KEY";
const char* DB_USER = "DB_USER";
const char* DB_PASSWORD = "DB_PASSWORD";
const char* DB_CONNECTION = "DB_CONNECTION";
const char* DB_NAME = "DB_NAME";




class react_collector : dpp::reaction_collector
{
    public:
    react_collector(dpp::cluster* cl, dpp::snowflake id) : dpp::reaction_collector(cl, REACTIONTIME, id) { }
 
    /* Override the "completed" event and then output the number of collected reactions as a message. */
    virtual void completed(const std::vector<dpp::collected_reaction>& list) override {
        if (list.size()) {
            owner->message_create(dpp::message(list[0].react_channel.id, "You claimed!!!"));
        }
    }
};

int main() {
    //loading in .env file (irrelevant to program logic)
    load_env_file(".env");

    // set up sql database connection
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect(std::getenv(DB_CONNECTION), std::getenv(DB_USER), std::getenv(DB_PASSWORD));;
    con->setSchema(std::getenv(DB_NAME));

    //set seedl
    std::srand(std::time(nullptr));

    /* Setup the bot */
    dpp::cluster bot(std::getenv(API_KEY));
    bot.on_log(dpp::utility::cout_logger());
    bot.log(dpp::ll_info,"Entereted main");

    react_collector* r = nullptr;
    // Refactor if poossible and figure out how lambdas work
    /* The event is fired when someone issues your commands */
    bot.on_slashcommand([&bot,&r,&con](const dpp::slashcommand_t& event) {
        bot.log(dpp::ll_info,"Entered slash command");
        if (checkForCommand(event,"roll")) 
        {
            bot.log(dpp::ll_info,"Entered roll");
            // queries a random object from the database 
            sql::ResultSet *res = createRandomSQLQueryGems(con);
            Mudae::Card tempCard = gemSQLResultToCard(res);
            dpp::embed embed = makeCard(tempCard);
            dpp::message msg(event.command.channel_id, embed);
            if(r == nullptr)
            {
                r = new react_collector(&bot, msg.id);
            }
            event.reply(msg);
            r = nullptr;
            free(res);
        }
        if (checkForCommand(event,"checkimg"))
        {
            bot.log(dpp::ll_info,"Checked for card");
            std::string id = std::get<std::string>(event.get_parameter("id"));

            //checks if paramater actually contains number
            if((stoi(id) > getMaxId(con)))
            {
                event.reply(std::string("Card out of range: ") + std::to_string(getMaxId(con)) + std::string(" is the largest number"));
            }
            else if(containsNumber(id))
            {
                bot.log(dpp::ll_info,"Valid card");
                sql::ResultSet *res = makeSQLRequestForSpecificId(con,id);
                Mudae::Card tempCard = gemSQLResultToCard(res);
                dpp::embed embed = makeCard(tempCard);
                dpp::message msg(event.command.channel_id, embed);
                event.reply(msg);
                free(res);
            }
            else 
            {
                event.reply("Please input a valid ID number");
            }
        }

    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) 
        {
            // checkImg command
            dpp::slashcommand checkImg("checkimg", "Jaime pull that up", bot.me.id);
            checkImg.add_option(dpp::command_option(dpp::co_string, "id", "id of image you're pulling up", true));
            bot.global_command_create(checkImg);

            // roll command 
            bot.global_command_create(dpp::slashcommand("roll", "test your luck", bot.me.id));
        }
    });


    //starts bot
    bot.start(dpp::st_wait);
    free(r);
    free(driver);
    free(con);
    return 0;
}