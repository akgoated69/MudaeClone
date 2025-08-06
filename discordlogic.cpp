#include <dpp/dpp.h>
#include "cardClass.h"

dpp::embed makeCard(Mudae::Card c);
bool checkForCommand(const dpp::slashcommand&, const std::string& = "roll");

//creates embed for a card based on card details and returns it
dpp::embed makeCard(Mudae::Card c)
{
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::aloe_vera_green)
        .set_title(std::to_string(c.id))
        .set_image(c.imgurLink);
    return embed;
}

// checks for specified command in chat, default command argument "roll"
bool checkForCommand(const dpp::slashcommand_t& event, const std::string& command)
{
    return event.command.get_command_name() == command;
}

void sendMsgWEmbed(const dpp::slashcommand_t& event,dpp::embed& embed)
{
    dpp::message msg(event.command.channel_id, embed);
    event.reply(msg);
}

void createBotCommand(std::string command, std::string description, dpp::cluster& bot)
{
    if (dpp::run_once<struct register_bot_commands>()) 
    {
        bot.global_command_create(dpp::slashcommand(command, description, bot.me.id));
    }
}





