# ft_irc - Internet Relay Chat Server

*This project has been created as part of the 42 curriculum by scottene and enpassel.*

## Description

ft_irc is a custom implementation of an IRC (Internet Relay Chat) server written in C++98. The project demonstrates network programming concepts, socket management, and the IRC protocol implementation. The server handles multiple simultaneous client connections using non-blocking I/O and the `poll()` system call, supporting essential IRC commands and channel operations.

Key features include:
- Multi-client connection handling with non-blocking sockets
- User authentication and registration (PASS, NICK, USER)
- Channel management (JOIN, PART, KICK, INVITE)
- Private and channel messaging (PRIVMSG, NOTICE)
- Channel modes: invite-only (+i), topic restrictions (+t), password protection (+k), user limits (+l), operator privileges (+o)
- Topic management with operator controls
- Information queries (WHO, WHOIS, LIST, NAMES)
- Graceful server shutdown with signal handling

## Instructions

### Compilation

```bash
make
```

This will compile the project and generate the `ircserv` executable.

Additional Makefile targets:
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Rebuild the project from scratch

### Execution

```bash
./ircserv <port> <password>
```

**Parameters:**
- `port` - Port number for the server to listen on (1-65535, ports below 1024 require root)
- `password` - Server password required for client connections

**Example:**
```bash
./ircserv 6667 mypassword
```

The server will start and display:
```
========================================
ft_irc server started successfully!
Port: 6667
Server name: ft_irc
Press Ctrl+C to stop the server
========================================
```

### Connecting with IRC Clients

#### Using netcat (for testing)
```bash
nc -C 127.0.0.1 6667
PASS mypassword
NICK testuser
USER testuser 0 * :Test User
JOIN #test
PRIVMSG #test :Hello World!
QUIT
```

#### Using other IRC clients
Configure your IRC client with:
- **Server:** 127.0.0.1 (or localhost)
- **Port:** 6667 (or your chosen port)
- **Password:** Your server password

Recommended IRC clients for testing:
- **HexChat** (GUI) - `apt install hexchat`

### Testing

The project includes comprehensive test suites:

1. **Unit Tests:**
```bash
# Test individual components
make -C mains
./mains/test_utils
./mains/test_parser
./mains/test_client
./mains/test_channel
./mains/test_server
```

2. **Integration Tests:**
```bash
# Automated IRC protocol tests
./message.sh

# Python-based functional tests
python3 ircTest.py
```

## Project Structure

```
ft_irc/
├── Makefile
├── README.md
├── includes/              # Header files
│   ├── IRC.hpp           # Main header with includes and constants
│   ├── Server.hpp        # Server class
│   ├── Client.hpp        # Client class
│   ├── Channel.hpp       # Channel class
│   ├── CommandHandler.hpp
│   ├── Utils.hpp
│   └── Parser.hpp
└── src/                  # Source files
    ├── main.cpp
    ├── server/
    │   └── Server.cpp
    ├── client/
    │   └── Client.cpp
    ├── channel/
    │   └── Channel.cpp
    ├── commands/         # IRC command implementations
    │   ├── CommandHandler.cpp
    │   ├── Pass.cpp
    │   ├── Nick.cpp
    │   ├── User.cpp
    │   ├── Join.cpp
    │   ├── Part.cpp
    │   ├── Privmsg.cpp
    │   ├── Notice.cpp
    │   ├── Kick.cpp
    │   ├── Invite.cpp
    │   ├── Topic.cpp
    │   ├── Mode.cpp
    │   ├── Quit.cpp
    │   ├── Ping.cpp
    │   ├── Who.cpp
    │   ├── Names.cpp
    │   └── List.cpp
    ├── utils/
    │   ├── Utils.cpp
    │   └── Parser.cpp
    └── bonus/
        └── Bot.cpp       # Bonus bot feature
```

## Supported IRC Commands

### Authentication
- **PASS** - Authenticate with server password
- **NICK** - Set or change nickname
- **USER** - Set username and realname
- **QUIT** - Disconnect from server

### Channel Operations
- **JOIN** - Join one or more channels
- **PART** - Leave one or more channels
- **KICK** - Remove a user from a channel (operators only)
- **INVITE** - Invite a user to a channel (operators only)
- **TOPIC** - View or set channel topic
- **MODE** - View or change channel modes (operators only)

### Messaging
- **PRIVMSG** - Send private message to user or channel
- **NOTICE** - Send notice to user or channel

### Information Queries
- **WHO** - List users in a channel
- **LIST** - List available channels
- **NAMES** - List users in specific channels

### Server
- **PING** - Keep-alive check

### Bonus
- **BOT** - Simple bot command for entertainment

## Channel Modes

- **+i** (invite-only) - Channel requires invitation to join
- **+t** (topic restricted) - Only operators can change topic
- **+k** (key) - Channel requires password to join
- **+l** (limit) - Limit the number of users in channel
- **+o** (operator) - Give/take channel operator privileges

## Resources

### IRC Protocol Documentation
- [RFC 2812 - Internet Relay Chat: Client Protocol](https://tools.ietf.org/html/rfc2812) - Current IRC protocol specification
- [RFC 1459 - Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459) - Original IRC protocol specification
- [Modern IRC Client Protocol](https://modern.ircdocs.horse/) - Updated IRC documentation with modern conventions

### Network Programming
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) - Comprehensive guide to socket programming in C
- [TCP/IP Sockets in C: Practical Guide for Programmers](https://cs.baylor.edu/~donahoo/practical/CSockets/practical/) - Socket programming reference

### C++ Resources
- [C++ Reference](https://en.cppreference.com/) - C++ standard library documentation
- [C++98 Standard](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/1998/n1905.pdf) - Official C++98 standard document

## Technical Choices

### Non-blocking I/O with poll()
The server uses `poll()` for I/O multiplexing instead of `select()` or `epoll()` as required by the subject. This allows efficient handling of multiple client connections without threading.

### Case-insensitive Comparison
Nicknames and channel names are stored in lowercase internally to ensure case-insensitive comparison, complying with IRC protocol standards.

### Buffer Management
Separate input and output buffers for each client handle partial reads/writes and ensure messages are properly assembled before processing.

### Signal Handling
Proper signal handling (SIGINT, SIGTERM) ensures graceful server shutdown with cleanup of all resources.

### C++98 Compliance
The project strictly adheres to C++98 standard, avoiding C++11 features like smart pointers, lambda functions, or move semantics.

## Author

Created by scottene and enpassel as part of the 42 École curriculum in Lyon, France.