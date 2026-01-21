#include "IRC.hpp"

void CommandHandler::handlePrivmsg(Client* client, const ParsedCommand& cmd)
{

	if (cmd.params.empty())
	{
	    sendError(client, ERR_NORECIPIENT, "", "No recipient given (PRIVMSG)");
	    return;
	}

	if (cmd.params.size() < 2 || cmd.params[1].empty())
	{
	    sendError(client, ERR_NOTEXTTOSEND, "", "No text to send");
	    return;
	}

	std::string target = cmd.params[0];
	std::string message = cmd.params[1];

	std::string fullMsg = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message;

	if (target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!')
	{
	    Channel* channel = _server.getChannel(target);
	    if (!channel)
	    {
	        sendError(client, ERR_NOSUCHCHANNEL, target, "No such channel");
	        return;
	    }

	    if (!channel->isMember(client))
	    {
	        sendError(client, ERR_CANNOTSENDTOCHAN, target, "Cannot send to channel");
	        return;
	    }

	    // Diffuser à tous les membres sauf l'expéditeur
	    _server.broadcastToChannel(target, fullMsg, client->getFd());
	}
	else
	{
	    Client* recipient = _server.getClientByNickname(target);
	    if (!recipient)
	    {
	        sendError(client, ERR_NOSUCHNICK, target, "No such nick/channel");
	        return;
	    }

	    _server.sendToClient(recipient->getFd(), fullMsg);
	}
}