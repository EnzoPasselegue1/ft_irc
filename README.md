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
- **irssi** (Terminal) - `apt install irssi`
- **WeeChat** (Terminal) - `apt install weechat`

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

## IRC Protocol Implementation

### Connection Flow
```
Client                              Server
  |                                   |
  |--- PASS password ---------------->|
  |                                   | (authenticate)
  |--- NICK nickname ---------------->|
  |                                   | (check uniqueness)
  |--- USER username 0 * :realname -->|
  |                                   | (registration complete)
  |<-- :server 001 nickname :Welcome -|
  |                                   |
  |--- JOIN #channel ---------------->|
  |<-- :nick JOIN #channel -----------|
  |                                   |
  |--- PRIVMSG #channel :Hello! ----->|
  |<-- :nick PRIVMSG #channel :Hi! ---|
  |                                   |
  |--- QUIT :Goodbye ---------------->|
  |<-- ERROR :Closing connection -----|
```

### Message Format

IRC messages follow this structure:
```
[:prefix] COMMAND [parameters] [:trailing]

Examples:
NICK alice
PRIVMSG #test :Hello everyone!
:alice!~alice@host JOIN #test
:server 001 alice :Welcome to the IRC Network
```

### How It Works

1. **Server Initialization**
   - Create listening socket
   - Bind to specified port
   - Set non-blocking mode
   - Start accepting connections

2. **Client Connection**
   - Accept new client socket
   - Add to poll() file descriptor set
   - Wait for authentication (PASS, NICK, USER)

3. **Command Processing**
   - Read data into client buffer
   - Parse complete IRC messages (ending with \r\n)
   - Route to appropriate command handler
   - Execute command with validation

4. **Channel Management**
   - Track channel membership
   - Enforce channel modes and permissions
   - Broadcast messages to channel members
   - Handle operator privileges

5. **Message Broadcasting**
   - Send to individual clients (PRIVMSG user)
   - Send to all channel members (PRIVMSG #channel)
   - Handle NOTICE (no automatic replies)

6. **Clean Shutdown**
   - Signal handler catches SIGINT/SIGTERM
   - Send QUIT to all connected clients
   - Close all sockets
   - Free all resources

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
- **WHOIS** - Get information about a user
- **LIST** - List available channels
- **NAMES** - List users in specific channels

### Server
- **PING** - Keep-alive check (server responds with PONG)

### Bonus
- **BOT** - Simple bot command for entertainment

## Channel Modes

The server supports the following channel modes:

| Mode | Parameter | Description |
|------|-----------|-------------|
| **+i** | - | Invite-only: Users must be invited to join |
| **+t** | - | Topic restriction: Only operators can change topic |
| **+k** | `<key>` | Password protection: Requires password to join |
| **+l** | `<limit>` | User limit: Maximum number of users allowed |
| **+o** | `<nickname>` | Operator privilege: Grant/revoke operator status |

**Examples:**
```
MODE #channel +i              # Set invite-only
MODE #channel +k secret123    # Set password
MODE #channel +l 50           # Set user limit to 50
MODE #channel +o alice        # Give operator to alice
MODE #channel -i              # Remove invite-only
MODE #channel +ik secret123   # Combine modes
```

## Technical Details

### Non-blocking I/O with poll()

The server uses `poll()` for efficient I/O multiplexing:
```cpp
struct pollfd fds[MAX_CLIENTS];

// Setup
fds[0].fd = server_socket;
fds[0].events = POLLIN;  // Monitor for incoming connections

// Poll loop
while (running) {
    int ret = poll(fds, nfds, -1);
    
    if (fds[0].revents & POLLIN) {
        // Accept new connection
    }
    
    for (int i = 1; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
            // Read from client
        }
        if (fds[i].revents & POLLOUT) {
            // Write to client
        }
    }
}
```

**Advantages:**
- No busy-waiting
- Scales better than select()
- Simple to use and maintain

### Buffer Management

Each client has separate input/output buffers:
```
Input Buffer:
[PASS sec] [ret\r\n] [NICK alice\r\n] [USER ...]
    ^                      ^
    incomplete            complete message
    
Output Buffer:
[:server 001 alice :Welcome\r\n] [waiting to send]
```

Messages are only processed when complete (ending with \r\n).

### Case-insensitive Comparison

IRC protocol specifies case-insensitive nicknames and channels:
- `Alice`, `alice`, `ALICE` are the same user
- `#Test`, `#test`, `#TEST` are the same channel

Implementation: Store in lowercase internally, display with original case.

### Signal Handling
```cpp
volatile sig_atomic_t g_running = 1;

void signal_handler(int signum) {
    g_running = 0;  // Request clean shutdown
}

// In main loop
while (g_running) {
    poll(...);
    // Handle connections
}

// Cleanup
close_all_sockets();
```

## Requirements

- C++98 compliant compiler (g++, clang++)
- Linux/Unix operating system
- Standard C++ library
- POSIX socket API

## Known Limitations

- IPv4 only (no IPv6 support)
- No SSL/TLS encryption
- Basic command set (no OPER, KILL, etc.)
- No server-to-server linking
- No persistent channel/user database
- C++98 only (no modern C++ features)

## Resources

### IRC Protocol Documentation
- [RFC 2812 - Internet Relay Chat: Client Protocol](https://tools.ietf.org/html/rfc2812) - Current IRC protocol specification
- [RFC 1459 - Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459) - Original IRC protocol specification
- [Modern IRC Client Protocol](https://modern.ircdocs.horse/) - Updated IRC documentation with modern conventions
- [IRC Numeric Replies](https://www.alien.net.au/irc/irc2numerics.html) - Complete list of IRC response codes

### Network Programming
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) - Comprehensive guide to socket programming
- [TCP/IP Sockets in C: Practical Guide for Programmers](https://cs.baylor.edu/~donahoo/practical/CSockets/practical/) - Socket programming reference
- [The Linux Programming Interface](http://man7.org/tlpi/) - System programming and networking

### C++ Resources
- [C++ Reference](https://en.cppreference.com/) - C++ standard library documentation
- [C++98 Standard](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/1998/n1905.pdf) - Official C++98 standard document
- [Effective C++](https://www.aristeia.com/books.html) by Scott Meyers - Best practices

## Technical Choices

### Non-blocking I/O with poll()
The server uses `poll()` for I/O multiplexing instead of `select()` or `epoll()` as required by the subject. This allows efficient handling of multiple client connections without threading, avoiding the complexity of race conditions and synchronization.

### Case-insensitive Comparison
Nicknames and channel names are stored in lowercase internally to ensure case-insensitive comparison, complying with IRC protocol standards (RFC 2812 section 2.2).

### Buffer Management
Separate input and output buffers for each client handle partial reads/writes and ensure messages are properly assembled before processing. This prevents issues with TCP packet fragmentation.

### Message Parsing
Messages are parsed only when complete (\r\n delimiter found). Incomplete messages remain in the buffer until more data arrives, ensuring proper command processing.

### Error Handling
Comprehensive error handling with IRC numeric replies:
- 400-series: Client errors (ERR_NOSUCHNICK, ERR_NOSUCHCHANNEL)
- 500-series: Server errors (ERR_UNKNOWNCOMMAND)

### Signal Handling
Proper signal handling (SIGINT, SIGTERM) ensures graceful server shutdown with cleanup of all resources. Uses `volatile sig_atomic_t` for thread-safe shutdown flag.

### C++98 Compliance
The project strictly adheres to C++98 standard, avoiding C++11 features like smart pointers, lambda functions, or move semantics. This ensures compatibility and demonstrates understanding of resource management.

## Troubleshooting

### "Address already in use"
**Problem:** Port already bound by another process  
**Solution:** Wait 60 seconds or use different port  
**Alternative:** `sudo lsof -i :6667` to find and kill process

### "Connection refused"
**Problem:** Server not running or firewall blocking  
**Solution:** Check server is running, verify firewall rules

### Client disconnects immediately
**Problem:** Wrong password or authentication failure  
**Solution:** Verify PASS command sent before NICK/USER

### Channel modes not working
**Problem:** Non-operator trying to set modes  
**Solution:** Only channel operators can set modes

## Authors

Created by **scottene** and **enpassel** as part of the 42 École curriculum in Lyon, France.

## License

This project is part of the 42 curriculum and follows school guidelines.
