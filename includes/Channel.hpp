#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Client;
class Channel
{
private:
    /* ================================================================== */
	/*                    ATTRIBUTS PRIVÉS                                */
	/* ================================================================== */
	// Identification
	std::string         _name;
	
	// Topic
	std::string         _topic;
	std::string         _topicSetter;
	time_t              _topicTime;
	
	// Modes
	bool                _inviteOnly;
	bool                _topicRestricted;
	std::string         _key;
	size_t              _userLimit;
	
	// Member and operator lists
	std::set<Client*>   _members;
	std::set<Client*>   _operators;
	
	// Invitations
	std::set<std::string> _invitedUsers;

	/* ================================================================== */
	/*                CONSTRUCTORS FORBIDDEN                             */
	/* ================================================================== */
	Channel();
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
    
public:
    Channel(const std::string& name);
    ~Channel();

    /* ========================================================================== */
    /*                    MEMBER MANAGEMENT                                     */
    /* ========================================================================== */
    bool                        addMember(Client* client);
    void                        removeMember(Client* client);
    bool                        isMember(Client* client) const;
    bool                        hasMember(const std::string& nickname) const;
    Client*                     getMemberByNickname(const std::string& nickname) const;
    const std::set<Client*>&    getMembers() const;
    size_t                      getMemberCount() const;
    bool                        isEmpty() const;

    /* ========================================================================== */
    /*                    OPERATOR MANAGEMENT                                  */
    /* ========================================================================== */
    bool                        addOperator(Client* client);
    void                        removeOperator(Client* client);
    bool                        isOperator(Client* client) const;
    bool                        isOperator(const std::string& nickname) const;

    /* ========================================================================== */
    /*                    MODE MANAGEMENT                                       */
    /* ========================================================================== */
    void                        setInviteOnly(bool enabled);
    bool                        isInviteOnly() const;

    void                        setTopicRestricted(bool enabled);
    bool                        isTopicRestricted() const;
    void                        setKey(const std::string& key);
    const std::string&          getKey() const;
    bool                        hasKey() const;
    bool                        checkKey(const std::string& key) const;
    void                        setUserLimit(size_t limit);
    size_t                      getUserLimit() const;
    bool                        hasUserLimit() const;
    bool                        isFull() const;
    std::string                 getModeString() const;
    std::string                 getModeStringWithParams() const;

    /* ========================================================================== */
    /*                    TOPIC MANAGEMENT                                        */
    /* ========================================================================== */
    void                        setTopic(const std::string& topic, const std::string& setterNick);
    const std::string&          getTopic() const;
    const std::string&          getTopicSetter() const;
    time_t                      getTopicTime() const;
    bool                        hasTopic() const;
    /* ========================================================================== */
    /*                    INVITATION MANAGEMENT                                 */
    /* ========================================================================== */
    void                        addInvite(const std::string& nickname);
    void                        removeInvite(const std::string& nickname);
    bool                        isInvited(const std::string& nickname) const;

    /* ========================================================================== */
    /*                         GETTERS                                         */
    /* ========================================================================== */
    const std::string&          getName() const;
    std::string                 getNamesList() const;
};

#endif