#include "IRC.hpp"

void CommandHandler::handlePing(Client* client, const ParsedCommand& cmd)
{
	std::string token = "";
	if (!cmd.params.empty())
	    token = cmd.params[0];

	std::string pongMsg = ":" + _server.getServerName() + " PONG " +
	                      _server.getServerName();
	if (!token.empty())
	    pongMsg += " :" + token;

	_server.sendToClient(client->getFd(), pongMsg);
}