#include "IRC.hpp"

void CommandHandler::handleQuit(Client* client, const ParsedCommand& cmd)
{
	std::string reason = "Quit";
	if (!cmd.params.empty())
	    reason = cmd.params[0];

	std::string quitMsg = ":" + client->getPrefix() + " QUIT :" + reason;

	const std::set<std::string>& channels = client->getChannels();
	for (std::set<std::string>::const_iterator it = channels.begin();
	     it != channels.end(); ++it)
	{
	    _server.broadcastToChannel(*it, quitMsg, client->getFd());
	}

	std::string errorMsg = "ERROR :Closing Link: " + client->getHostname() +
	                       " (Quit: " + reason + ")";
	_server.sendToClient(client->getFd(), errorMsg);

	client->markForDisconnection();
}