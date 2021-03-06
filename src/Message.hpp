#pragma once

#include "types.hpp"
#include "Singleton.hpp"
#include "PawnDispatcher.hpp"
#include "Callback.hpp"

#include <string>
#include <vector>

#include <json.hpp>

using json = nlohmann::json;


class Message
{
	friend class MessageManager;
private:
	Message() : _valid(false)
	{ }
	Message(MessageId_t pawn_id, json const &data);
public:
	~Message() = default;

private:
	Snowflake_t m_Id;
	MessageId_t m_PawnId = INVALID_MESSAGE_ID;

	ChannelId_t m_Channel = INVALID_CHANNEL_ID;
	UserId_t m_Author = INVALID_USER_ID;

	std::string m_Content;
	bool m_IsTts = false;

	bool m_MentionsEveryone = false;
	std::vector<UserId_t> m_UserMentions;
	std::vector<RoleId_t> m_RoleMentions;

	bool _valid;

	bool m_Persistent;

	enum class ReactionType : int
	{
		REACTION_ADD = 0, // Sent when a user adds a reaction to a message.
		REACTION_REMOVE, // Sent when a user removes a reaction from a message.
		REACTION_REMOVE_ALL, // Sent when a user explicitly removes all reactions from a message.
		REACTION_REMOVE_EMOJI // Sent when a bot removes all instances of a given emoji from the reactions of a message.
	};
public:
	Snowflake_t const &GetId() const
	{
		return m_Id;
	}
	MessageId_t GetPawnId() const
	{
		return m_PawnId;
	}
	ChannelId_t const &GetChannel() const
	{
		return m_Channel;
	}
	UserId_t const &GetAuthor() const
	{
		return m_Author;
	}
	std::string const &GetContent() const
	{
		return m_Content;
	}
	bool IsTts() const
	{
		return m_IsTts;
	}
	bool MentionsEveryone() const
	{
		return m_MentionsEveryone;
	}
	decltype(m_UserMentions) const &GetUserMentions() const
	{
		return m_UserMentions;
	}
	decltype(m_RoleMentions) const &GetRoleMentions() const
	{
		return m_RoleMentions;
	}
	bool Persistent() const
	{
		return m_Persistent;
	}

	bool IsValid() const
	{
		return _valid;
	}
	operator bool() const
	{
		return IsValid();
	}

	void DeleteMessage();
	void AddReaction(Emoji_t const& emoji);
	bool DeleteReaction(EmojiId_t const emojiid);
	bool EditMessage(const std::string& msg, const EmbedId_t embedid = INVALID_EMBED_ID);
	void SetPresistent(bool persistent) { m_Persistent = persistent; };
};


class MessageManager : public Singleton<MessageManager>
{
	friend class Singleton<MessageManager>;
private:
	MessageManager() = default;
	~MessageManager() = default;

private:
	std::map<MessageId_t, Message_t> m_Messages; //PAWN message-id to actual channel map

	MessageId_t m_CreatedMessageId = INVALID_MESSAGE_ID;

public:
	void Initialize();

	MessageId_t GetCreatedMessageId() const
	{
		return m_CreatedMessageId;
	}
	void SetCreatedMessageId(MessageId_t id)
	{
		m_CreatedMessageId = id;
	}

	MessageId_t Create(json const &data);
	bool Delete(MessageId_t id);

	// This is for the cache.
	void CreateFromSnowflake(Snowflake_t channel, Snowflake_t message, pawn_cb::Callback_t&& callback);

	Message_t const &Find(MessageId_t id);
	Message_t const &FindById(Snowflake_t const &sfid);
};
