#include "IRC.hpp"

void CommandHandler::handleKick(Client* client, const ParsedCommand& cmd)
{
	if (cmd.params.size() < 2)
	{
	    sendError(client, ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters");
	    return;
	}

	std::string channelName = cmd.params[0];
	std::string targetNick = cmd.params[1];
	std::string reason = (cmd.params.size() > 2) ? cmd.params[2] : client->getNickname();

	Channel* channel = _server.getChannel(channelName);
	if (!channel)
	{
	    sendError(client, ERR_NOSUCHCHANNEL, channelName, "No such channel");
	    return;
	}

	if (!channel->isMember(client))
	{
	    sendError(client, ERR_NOTONCHANNEL, channelName, "You're not on that channel");
	    return;
	}

	if (!channel->isOperator(client))
	{
	    sendError(client, ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
	    return;
	}

	Client* target = channel->getMemberByNickname(targetNick);
	if (!target)
	{
	    sendError(client, ERR_USERNOTINCHANNEL, targetNick + " " + channelName,
	              "They aren't on that channel");
	    return;
	}

	std::string kickMsg = ":" + client->getPrefix() + " KICK " + channelName +
	                      " " + targetNick + " :" + reason;
	_server.broadcastToChannel(channelName, kickMsg, -1);

	channel->removeMember(target);
	target->leaveChannel(channelName);

	if (channel->isEmpty())
	    _server.removeChannel(channelName);
}