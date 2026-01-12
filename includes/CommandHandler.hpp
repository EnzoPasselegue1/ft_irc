#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <string>
#include <vector>
#include <map>

class Client;
class Server;
class Channel;

struct ParsedCommand
{
    std::string prefix;
    std::string command;
    std::vector<std::string> params;
};

class CommandHandler
{
    private:
        Server& _server;
        void handlePass(Client* client, const ParsedCommand& cmd);
    

    public:
        CommandHandler(Server& server);
        ~CommandHandler();

        void handleCommand(Client* client, const std::string& rawCommand);
        ParsedCommand parseCommand(const std::string& rawCommand);
        void sendError(Client* client, const std::string& errorCode, const std::string& target, const std::string& message);
};

#endif