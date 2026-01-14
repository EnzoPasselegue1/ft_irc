#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>
# include <set>

class Client
{
private:
	/* ================================================================== */
	/*                    ATTRIBUTS PRIVÉS                                */
	/* ================================================================== */
	
	// Socket file descriptor
	int         _fd;
	
	// Identification information
	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _hostname;
	
	// Status
	bool        _passwordProvided;
	bool        _registered;
	bool        _shouldDisconnect;
    bool        _markedForDisconnection;
	
	// Channels
	std::set<std::string> _channels;
	
	std::string _inputBuffer;
	std::string _outputBuffer;

	/* ================================================================== */
	/*                CONSTRUCTORS FORBIDDEN                           */
	/* ================================================================== */
	Client();
	Client(const Client& other);
	Client& operator=(const Client& other);

public:
	/* ================================================================== */
	/*                    CONSTRUCTOR / DESTRUCTOR                      */
	/* ================================================================== */
	
    Client(int fd, const std::string& hostname);
    ~Client();

    /* ================================================================== */
    /*                        IDENTIFICATION                              */
    /* ================================================================== */
    void                            setNickname(const std::string& nickname);
    const std::string&              getNickname() const;
    void                            setUsername(const std::string& username);
    const std::string&              getUsername() const;
    void                            setRealname(const std::string& realname);
    const std::string&              getRealname() const;
    const std::string&              getHostname() const;
    std::string                     getPrefix() const;

    /* ========================================================================== */
    /*                   AUTHENTICATION STATUS                                  */
    /* ========================================================================== */
    void                            setPasswordProvided(bool provided);
    bool                            hasPasswordProvided() const;
    void                            setRegistered(bool registered);
    bool                            isRegistered() const;

    /* ========================================================================== */
    /*                    CHANNEL MANAGEMENT                                    */
    /* ========================================================================== */
    void                            joinChannel(const std::string& channelName);
    void                            leaveChannel(const std::string& channelName);
    bool                            isInChannel(const std::string& channelName) const;
    const std::set<std::string>&    getChannels() const;

    /* ========================================================================== */
    /*                    BUFFER MANAGEMENT                                     */
    /* ========================================================================== */
    void                            appendToInputBuffer(const std::string& data);
    std::string&                    getInputBuffer();
    void                            appendToOutputBuffer(const std::string& data);
    std::string&                    getOutputBuffer();
    void                            clearInputBuffer();
    void                            trimOutputBuffer(size_t bytes);
    bool                            hasDataToSend() const;

    /* ========================================================================== */
    /*                         GETTERS                                         */
    /* ========================================================================== */
    int                             getFd() const;
    void                            markForDisconnection();
    bool                            isMarkedForDisconnection() const { return _markedForDisconnection; }
    bool                            shouldDisconnect() const;
};

#endif