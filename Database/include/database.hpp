#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "packet.hpp"

namespace chattere::database
{
    class Database
    {
    public:
        Database(std::string path);
        std::vector<std::shared_ptr<protocol::User>> GetUsers() const;
        std::shared_ptr<protocol::User> GetUserById(const std::int64_t id) const;
        std::shared_ptr<protocol::User> GetUserByUsername(const std::string &username) const;

        std::shared_ptr<protocol::User> CreateUser(const std::string &username, const std::string &hashedPassword) const;

        std::vector<std::shared_ptr<protocol::BasicChannel>> GetChannels() const;
        std::vector<std::shared_ptr<protocol::BasicChannel>> GetChannelsByType(const protocol::BasicChannel::ChannelCase type) const;
        std::vector<std::shared_ptr<protocol::BasicChannel>> GetChannelsByName(const std::string &name) const;
        std::shared_ptr<protocol::BasicChannel> GetChannelById(std::int64_t id) const;

        // std::shared_ptr<protocol::User> CreateTextChannel(const std::string &name, const std::int64_t parent_id);
        std::shared_ptr<protocol::BasicChannel> CreateChannel(const std::shared_ptr<protocol::BasicChannel> channel);

        std::vector<std::shared_ptr<protocol::BasicChannel>> GetMessages(const std::int64_t channel_id) const;
        std::shared_ptr<protocol::BasicChannel> GetMessage(const std::int64_t id) const;

    private:
        std::shared_ptr<SQLite::Database> m_database;
        std::string m_path;
        //     leveldb::DB *m_database;
    };
} // namespace chattere::database