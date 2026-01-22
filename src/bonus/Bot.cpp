#include "IRC.hpp"

void CommandHandler::handleBot(Client* client, const ParsedCommand& cmd)
{
    if (cmd.params.empty())
    {
        sendError(client, ERR_NEEDMOREPARAMS, "BOT", "Need more parameters");
        return;
    }

    std::string subject;
    for (size_t i = 0; i < cmd.params.size(); ++i)
    {
        if (i)
            subject += " ";
        subject += cmd.params[i];
    }

    long score = 0;
    for (size_t i = 0; i < subject.size(); ++i)
        score += static_cast<unsigned char>(subject[i]);

    std::string verdict = (score % 2) ? "c'est de droite." : "c'est de gauche.";
    std::string reply = ":" + _server.getServerName() + " NOTICE "
                      + client->getNickname() + " :" + verdict;

    _server.sendToClient(client->getFd(), reply);
}