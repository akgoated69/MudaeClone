#include <random>
#include <chrono>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <ctime>

#include <dpp/dpp.h>
#include <dpp/unicode_emoji.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

#include "cardClass.h"
#include "discordlogic.h"
#include "sqllogic.h"
#include "randomlogic.h"
#include "envlogic.h"

#define test

constexpr int REACTIONTIME{5};
const char* API_KEY = "API_KEY";
const char* DB_USER = "DB_USER";
const char* DB_PASSWORD = "DB_PASSWORD";
const char* DB_CONNECTION = "DB_CONNECTION";
const char* DB_NAME = "DB_NAME";

std::regex const image_regex(R"(https?:\/\/\S+\.(?:png|jpe?g|gif|bmp|webp|svg)(?:\?\S*)?$)", std::regex_constants::icase);

class rolls_react : public dpp::reaction_collector {
public:
    rolls_react(dpp::cluster* cl, dpp::snowflake id, sql::Connection* connection, Mudae::Card rolledCard)
        : dpp::reaction_collector(cl, REACTIONTIME, id), con(connection), rolledCard(rolledCard) {}

    virtual void completed(const std::vector<dpp::collected_reaction>& list) override {
        if (!list.empty()) 
        {
            std::string tempUser = list.front().react_user.format_username();
            if (!checkIfUserInDB(con, tempUser)) 
            {
                uploadUserID(con, tempUser);
            }
            if (checkIfCardClaimed(con,std::to_string(rolledCard.id)))
            {
                assignCardOwner(con,std::to_string(rolledCard.id),tempUser);
                owner->message_create(dpp::message(list[0].react_channel.id, tempUser + std::string(" claimed card id: ") + std::to_string(rolledCard.id)));
            }
            else
            {
                owner->message_create(dpp::message(list[0].react_channel.id, "This card is owned"));

            }
        }
    }

    private:
    sql::Connection* con; // store pointer to your connection
    Mudae::Card rolledCard;
};

int main() {
    load_env_file(".env");

    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect(std::getenv(DB_CONNECTION), std::getenv(DB_USER), std::getenv(DB_PASSWORD));
    delete driver;
    
    // set up sql database connection
    con->setSchema(std::getenv(DB_NAME));

    //set seedl
    std::srand(std::time(nullptr));

    /* Setup the bot */
    dpp::cluster bot(std::getenv(API_KEY));

    //creates logging
    bot.on_log(dpp::utility::cout_logger());
    bot.log(dpp::ll_info,"Entereted main");

    // handles discord reaction detection, reused for all messages
    rolls_react* r = nullptr;


    // Refactor if poossible and figure out how lambdas work
    /* The event is fired when someone issues your commands */
    bot.on_slashcommand([&bot,&r,&con](const dpp::slashcommand_t& event) {
        bot.log(dpp::ll_info,"Entered slash command");

        const std::string sender = event.command.get_issuing_user().format_username();

        if (checkForCommand(event,"roll")) 
        {
            bot.log(dpp::ll_info,"Entered roll");

            Mudae::Card tempCard= gemSQLResultToCard(createRandomSQLQueryGems(con));
            // queries a random card turns it into card then embed, then message object
            dpp::message msg(event.command.channel_id, makeCard(tempCard,con));

            // creates a new reaction collector for the message just created
            if(r == nullptr)
            {
                bot.log(dpp::ll_info,"User reacted to roll");
                r = new rolls_react(&bot, msg.id,con,tempCard);
            }

            //sends message 
            setClaimTimeForUser(con,sender);
            event.reply(msg);

            // memory management
            r = nullptr;
        }

        if (checkForCommand(event,"checkimg"))
        {
            bot.log(dpp::ll_info,"Checked for card");

            // gets specific ID number of card called 
            std::string id = std::get<std::string>(event.get_parameter("id"));

            /* checks if number is in the range of the database*/
            if((stoi(id) > getMaxId(con) && stoi(id) > 0))
            {
                //error message 
                event.reply(std::string("Card out of range: ") + std::to_string(getMaxId(con)) + std::string(" is the largest number"));
            }
            // valid case 
            else if(containsNumber(id))
            {
                bot.log(dpp::ll_info,"Valid card");

                Mudae::Card tempCard(gemSQLResultToCard(makeSQLRequestForSpecificId(con,id)));
                dpp::embed tempEmbed(makeCard(tempCard,con));

                dpp::message msg = dpp::message (event.command.channel_id, tempEmbed);


                // this creates an sql read request for the image at specific id, turns it into a card, then embed, then message, and sends it
                event.reply(msg);
            }
            else // this case is intended to handle letters
            {
                event.reply("Please input a valid ID number");
            }
        }

        if(checkForCommand(event,"addimg"))
        {

            bot.log(dpp::ll_info,"Card is being added to gems.");
            std::string link = std::get<std::string>(event.get_parameter("imagelink"));
            if(std::regex_match(link,image_regex)){
                uploadCardToDb(con,link);
                bot.log(dpp::ll_info,"Made it past database upload.");
                event.reply(std::string("Your card has been succesfully uploaded and has an ID number: ") + std::to_string((getMaxId(con))));
            } 
            else 
            {
                event.reply("Your link is not an image. Please upload a valid link. An invalid link could pollute the database.");
            }
        }

        if(checkForCommand(event,"checkinventory"))
        {
            bot.log(dpp::ll_info,sender +" checked inventory");
            if(!checkIfUserInDB(con,sender))
            {
                bot.log(dpp::ll_info,"User not in database");
                event.reply("User is not in database");
            }
            else if(checkIfUserHasCards(con,sender))
            {
                bot.log(dpp::ll_info,"User has no cards");
                event.reply("User has no cards");
            }
            else 
            {
                bot.log(dpp::ll_info,"User exists and has cards");
                event.reply(userString(sender) + vectorStringToMessage(intVectorToString(getAllCardBelongingToUser(con,sender))));
            }

        }

        if(checkForCommand(event,"exit"))
        {
            event.reply("Shutting down");
            bot.log(dpp::ll_info,"Shutting down");
            delete r;
            delete con;
            exit(0);
        }

        if(checkForCommand(event,"getdatetime"))
        {
            bot.log(dpp::ll_info,"User requested date time");
            event.reply(getCurrentDateTime(con));
        }

    });

    // this whole section is what is responsible for the autofill of / commands
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) 
        {
            // command to add image to database with option to upload a link
            dpp::slashcommand addImg("addimg","WARNING: DO NOT ADD SOME BULLSHIT",bot.me.id);
            addImg.add_option(dpp::command_option(dpp::co_string,"imagelink","has to be imgur or discord",true));
            bot.global_command_create(addImg);

            // checkImg command with id selection option
            dpp::slashcommand checkImg("checkimg", "Jaime pull that up", bot.me.id); 
            checkImg.add_option(dpp::command_option(dpp::co_string, "id", "id of image you're pulling up", true));    
            bot.global_command_create(checkImg);


            // roll command 
            bot.global_command_create(dpp::slashcommand("roll", "test your luck", bot.me.id));

            bot.global_command_create(dpp::slashcommand("exit","turns off bot",bot.me.id));

            bot.global_command_create(dpp::slashcommand("checkinventory","Check cards that belong to you",bot.me.id));

            bot.global_command_create(dpp::slashcommand("getdatetime","returns date and time",bot.me.id));
        }
    });


    //starts bot
    bot.start(dpp::st_wait);

    delete r;
    delete con;
    delete driver;

}