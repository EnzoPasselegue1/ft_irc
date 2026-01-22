#include "IRC.hpp"

void CommandHandler::handleBot(Client* client, const ParsedCommand& cmd)
{
    std::string botMessage = "Hello " + client->getNickname() + "! I am your friendly IRC bot.";
    std::string fullMsg = ":" + _server.getServerName() + " NOTICE " + client->getNickname() + " :" + botMessage;

    _server.sendToClient(client->getFd(), fullMsg);

    (void)cmd; // To avoid unused parameter warning
}