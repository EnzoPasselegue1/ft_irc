#include "IRC.hpp"

void CommandHandler::handleNotice(Client* client, const ParsedCommand& cmd)
{

	if (cmd.params.empty())
	    return;

	if (cmd.params.size() < 2 || cmd.params[1].empty())
	    return;

	std::string target = cmd.params[0];
	std::string message = cmd.params[1];

	std::string fullMsg = ":" + client->getPrefix() + " NOTICE " + target + " :" + message;

	if (target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!')
	{
	    Channel* channel = _server.getChannel(target);
	    if (!channel)
	        return;

	    if (!channel->isMember(client))
	        return;

	    // Diffuser à tous les membres sauf l'expéditeur
	    _server.broadcastToChannel(target, fullMsg, client->getFd());
	}
	else
	{
	    Client* recipient = _server.getClientByNickname(target);
	    if (!recipient)
	        return;

	    _server.sendToClient(recipient->getFd(), fullMsg);
	}
}