#include "IRC.hpp"

void CommandHandler::handleInvite(Client* client, const ParsedCommand& cmd)
{
    if (cmd.params.empty())
    {
        sendError(client, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
        return;
    }

    if (cmd.params.size() < 2)
    {
        sendError(client, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
        return;
    }

    std::string targetNick = cmd.params[0];
    std::string channelName = cmd.params[1];
    Client* targetClient = _server.getClientByNickname(targetNick);
    if (!targetClient)
    {
        sendError(client, ERR_NOSUCHNICK, targetNick, "No such nick/channel");
        return;
    }
    Channel* channel = _server.getChannel(channelName);
    if (!channel)
    {
        sendError(client, ERR_NOSUCHCHANNEL, channelName, "No such channel");
        return;
    }
    if (!channel->isOperator(client))
    {
        sendError(client, ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
        return;
    }
    if (channel->hasMember(targetClient->getNickname()))
    {
        sendError(client, ERR_USERONCHANNEL, targetNick + " " + channelName, "is already on channel");
        return;
    }
    channel->addInvite(targetClient->getNickname());
    std::string inviteMsg = ":" + client->getPrefix() + " INVITE " + targetNick + " :" + channelName;
    _server.sendToClient(targetClient->getFd(), inviteMsg);
}