#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "packet.hpp"

namespace chattere::database
{
    /**
     * @brief An Interface for the Database
     */
    class Database
    {
    public:
        Database(std::string path);

        /**
         * @brief Get all the users in the Database
         * 
         * @return std::vector<std::shared_ptr<protocol::User>> 
         */
        std::vector<std::shared_ptr<protocol::User>> GetUsers() const;

        /**
         * @brief Get a User By Id
         * 
         * @param id 
         * @return std::shared_ptr<protocol::User> 
         */
        std::shared_ptr<protocol::User> GetUserById(const std::int64_t id) const;

        /**
         * @brief Get a User By Username
         * 
         * @param username 
         * @return std::shared_ptr<protocol::User> 
         */
        std::shared_ptr<protocol::User> GetUserByUsername(const std::string &username) const;

        /**
         * @brief Store a user in the Database
         * 
         * @param username 
         * @param hashedPassword 
         * @return std::shared_ptr<protocol::User> 
         */
        std::shared_ptr<protocol::User> CreateUser(const std::string &username, const std::string &hashedPassword) const;

        /**
         * @brief Get the Channels object
         * 
         * @deprecated
         * 
         * @return std::vector<std::shared_ptr<protocol::BasicChannel>> 
         */
        std::vector<std::shared_ptr<protocol::BasicChannel>> GetChannels() const;

        /**
         * @brief Get the Channels By Type object
         * @deprecated
         * 
         * @param type 
         * @return std::vector<std::shared_ptr<protocol::BasicChannel>> 
         */
        std::vector<std::shared_ptr<protocol::BasicChannel>> GetChannelsByType(const protocol::BasicChannel::ChannelCase type) const;

        /**
         * @brief Get the Channels By Name object
         * @deprecated
         * 
         * @param name 
         * @return std::vector<std::shared_ptr<protocol::BasicChannel>> 
         */
        std::vector<std::shared_ptr<protocol::BasicChannel>> GetChannelsByName(const std::string &name) const;

        /**
         * @brief Get the Channel By Id object
         * @deprecated
         * 
         * @param id 
         * @return std::shared_ptr<protocol::BasicChannel> 
         */
        std::shared_ptr<protocol::BasicChannel> GetChannelById(std::int64_t id) const;

        // std::shared_ptr<protocol::User> CreateTextChannel(const std::string &name, const std::int64_t parent_id);
        /**
         * @brief Create a Channel object
         * @deprecated
         * 
         * @param channel 
         * @return std::shared_ptr<protocol::BasicChannel> 
         */
        std::shared_ptr<protocol::BasicChannel> CreateChannel(const std::shared_ptr<protocol::BasicChannel> channel);

        /**
         * @brief Get the Messages object
         * @deprecated
         * 
         * @param channel_id 
         * @return std::vector<std::shared_ptr<protocol::BasicChannel>> 
         */
        std::vector<std::shared_ptr<protocol::BasicChannel>> GetMessages(const std::int64_t channel_id) const;

        /**
         * @brief Get the Message object
         * @deprecated
         * 
         * @param id 
         * @return std::shared_ptr<protocol::BasicChannel> 
         */
        std::shared_ptr<protocol::BasicChannel> GetMessage(const std::int64_t id) const;

    private:
        std::shared_ptr<SQLite::Database> m_database;
        std::string m_path;
        //     leveldb::DB *m_database;
    };
} // namespace chattere::database