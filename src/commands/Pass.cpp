#include "IRC.hpp"

void CommandHandler::handlePass(Client* client, const ParsedCommand& cmd)
{
	if (client->isRegistered())
	{
	    sendError(client, ERR_ALREADYREGISTERED, "", "You may not reregister");
	    return;
	}

	if (cmd.params.empty())
	{
	    sendError(client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
	    return;
	}

	if (cmd.params[0] == _server.getPassword())
	{
	    client->setPasswordProvided(true);
	}
	else
	{
	    sendError(client, ERR_PASSWDMISMATCH, "", "Password incorrect");
	    client->markForDisconnection();
	}
}