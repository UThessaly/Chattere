#include "database.hpp"
#include <spdlog/spdlog.h>
#include "packet.hpp"
#include "snowflake.hpp"

namespace chattere::database
{

    // std::shared_ptr<protocol::BasicChannel> createCategoryChannel(const std::shared_ptr<SQLite::Database> db, const std::shared_ptr<protocol::BasicChannel> channel)
    // {
    //     auto channel_id = CreateSnowflake();
    //     auto category_channel = channel->category_channel();

    //     auto name = category_channel.name();
    //     auto parent_id = category_channel.parent_id();
    //     auto position = category_channel.position();

    //     SQLite::Statement query(*db, R"(INSERT INTO channels (Id, Name, ParentId, Position, Type)
    //                                             VALUES (:id, :name, :parent_id, :position, :type);
    //                             )");

    //     query.bind(":id", channel_id);
    //     query.bind(":name", name);
    //     query.bind(":parent_id", parent_id);
    //     query.bind(":position", position);
    //     query.bind(":type", channel->channel_case());

    //     auto result = query.exec();

    //     channel->set_id(channel_id);
    //     return channel;
    // }

    std::shared_ptr<protocol::BasicChannel> createTextChannel(const std::shared_ptr<SQLite::Database> db, const std::shared_ptr<protocol::BasicChannel> channel)
    {
        auto channel_id = CreateSnowflake();
        auto category_channel = channel->text_channel();

        auto name = category_channel.name();
        auto parent_id = category_channel.parent_id();
        auto position = category_channel.position();

        SQLite::Statement query(*db, R"(INSERT INTO channels (Id, Name, ParentId, Position, Type)
                                                VALUES (:id, :name, :parent_id, :position, :type);
                                )");

        query.bind(":id", channel_id);
        query.bind(":name", name);
        query.bind(":parent_id", parent_id);
        query.bind(":position", position);
        query.bind(":type", channel->channel_case());

        auto result = query.exec();

        channel->set_id(channel_id);
        return channel;
    }

    Database::Database(std::string path) : m_path(path)
    {
        m_database = std::make_shared<SQLite::Database>(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Statement make_table_users(*m_database, R"(CREATE TABLE IF NOT EXISTS users (Id int64 NOT NULL,
                                                                                        Username varchar(32) NOT NULL, 
                                                                                        Password varchar(512),
                                                                                        PRIMARY KEY (Id)
                                                                                       );
                        )");

        SQLite::Statement make_table_channels(*m_database, R"(CREATE TABLE IF NOT EXISTS channels (Id int64 NOT NULL,
                                                                                                   Name varchar(64) NOT NULL,
                                                                                                   ParentId int64,
                                                                                                   Position int,
                                                                                                   Type int,
                                                                                                   PRIMARY KEY (Id)
                                                             );
                                            )");

        SQLite::Statement make_table_messages(*m_database, R"(CREATE TABLE IF NOT EXISTS messages (Id int64 NOT NULL,
                                                                                                   Type int NOT NULL DEFAULT 11,
                                                                                                   Content varchar(64) NOT NULL,
                                                                                                   ChannelId int64 NOT NULL,
                                                                                                   AuthorId int64 NOT NULL,
                                                                                                   Recipient_1 int64,
                                                                                                   Recipient_2 int64,
                                                                                                   PRIMARY KEY(Id)
                                                             );
                                              )");

        make_table_channels.exec();
        make_table_messages.exec();
        make_table_users.exec();
    }

    std::vector<std::shared_ptr<protocol::User>> Database::GetUsers() const
    {
        std::vector<std::shared_ptr<protocol::User>> users;

        SQLite::Statement query(*m_database, R"(SELECT Id, Username FROM users)");

        while (query.executeStep())
        {
            auto id = static_cast<std::int64_t>(query.getColumn("Id").getInt64());
            auto username = query.getColumn("Username").getString();

            auto user = std::make_shared<protocol::User>();
            user->set_id(id);
            user->set_username(username);

            users.push_back(user);
        }

        return users;
    }

    std::shared_ptr<protocol::User> Database::GetUserById(const std::int64_t id) const
    {
        SQLite::Statement query(*m_database, R"(SELECT Id, Username FROM users WHERE Id == :id)");
        query.bind(":id", id);

        auto t = query.executeStep();

        if (!t)
        {
            return nullptr;
        }

        auto user = std::make_shared<protocol::User>();

        user->set_id(query.getColumn("Id").getInt64());
        user->set_username(query.getColumn("Username").getString());

        return user;
    };

    std::shared_ptr<protocol::User> Database::GetUserByUsername(const std::string &username) const
    {
        SQLite::Statement query(*m_database, R"(SELECT Id, Username FROM users WHERE Username == :username)");
        query.bind(":username", username);

        auto t = query.executeStep();

        if (!t)
        {
            return nullptr;
        }

        auto user = std::make_shared<protocol::User>();

        user->set_id(query.getColumn("Id").getInt64());
        user->set_username(query.getColumn("Username").getString());

        return user;
    };

    std::shared_ptr<protocol::User> Database::CreateUser(const std::string &username, const std::string &hashedPassword) const
    {
        auto user_id = CreateSnowflake();

        SQLite::Statement query(*m_database, R"(INSERT INTO users (Id, Username, Password)
                                                VALUES (:id, :username, :password);
                                )");

        query.bind(":id", user_id);
        query.bind(":username", username);
        query.bind(":password", hashedPassword);

        auto result = query.exec();

        std::shared_ptr user = std::make_shared<protocol::User>();
        user->set_id(user_id);

        {
            auto _ = username;
            user->set_username(_);
        }

        return user;
    }

    std::vector<std::shared_ptr<protocol::BasicChannel>> Database::GetChannels() const
    {
        std::vector<std::shared_ptr<protocol::BasicChannel>> channels;

        SQLite::Statement query(*m_database, R"(SELECT * FROM channels)");

        while (query.executeStep())
        {
            auto id = static_cast<std::int64_t>(query.getColumn("Id").getInt64());
            auto type = query.getColumn("Type").getInt();

            auto channel = std::make_shared<protocol::BasicChannel>();

            channel->set_id(id);
            switch (type)
            {
            // case protocol::BasicChannel::ChannelCase::kCategoryChannel:
            // {
            //     auto name = query.getColumn("Name").getString();
            //     auto parent_id = static_cast<std::int64_t>(query.getColumn("ParentId").getInt64());
            //     auto position = query.getColumn("Position").getInt();

            //     auto category_channel = new protocol::CategoryChannel();
            //     category_channel->set_name(name);
            //     category_channel->set_parent_id(parent_id);
            //     category_channel->set_position(position);

            //     channel->set_allocated_category_channel(category_channel);
            //     break;
            // }
            case protocol::BasicChannel::ChannelCase::kDmChannel:
            {
                auto receipient_1 = static_cast<std::int64_t>(query.getColumn("Recipient_1").getInt64());
                auto receipient_2 = static_cast<std::int64_t>(query.getColumn("Recipient_1").getInt64());

                auto dm_channel = new protocol::DMChannel();
                dm_channel->set_user_ids(0, receipient_1);
                dm_channel->set_user_ids(1, receipient_2);

                channel->set_allocated_dm_channel(dm_channel);
                break;
            }
            case protocol::BasicChannel::ChannelCase::kTextChannel:
            {
                auto name = query.getColumn("Name").getString();
                auto parent_id = static_cast<std::int64_t>(query.getColumn("ParentId").getInt64());
                auto position = query.getColumn("Position").getInt();

                auto text_channel = new protocol::TextChannel();
                text_channel->set_name(name);
                text_channel->set_parent_id(parent_id);
                text_channel->set_position(position);

                channel->set_allocated_text_channel(text_channel);
                break;
            }
            default:
                continue;
            }

            channels.push_back(channel);
        }

        return channels;
    }

    std::vector<std::shared_ptr<protocol::BasicChannel>> Database::GetChannelsByType(const protocol::BasicChannel::ChannelCase type) const
    {
    }

    std::shared_ptr<protocol::BasicChannel> Database::GetChannelById(std::int64_t id) const
    {
    }

    std::vector<std::shared_ptr<protocol::BasicChannel>> Database::GetChannelsByName(const std::string &name) const
    {
    }

    std::shared_ptr<protocol::BasicChannel> Database::CreateChannel(const std::shared_ptr<protocol::BasicChannel> channel)
    {
        switch (channel->channel_case())
        {
        case protocol::BasicChannel::ChannelCase::CHANNEL_NOT_SET:
        {
            spdlog::critical("Channel Type {} is not a valid type", channel->channel_case());
            return nullptr;
        }
        // case protocol::BasicChannel::ChannelCase::kCategoryChannel:
        // {
        //     return createCategoryChannel(m_database, channel);
        // }
        case protocol::BasicChannel::ChannelCase::kTextChannel:
        {
            return createTextChannel(m_database, channel);
        }
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<protocol::BasicChannel>> Database::GetMessages(const std::int64_t channel_id) const {}

    std::shared_ptr<protocol::BasicChannel> Database::GetMessage(const std::int64_t id) const {}

    // std::shared_ptr<protocol::User> Database::GetUserByUsername(std::int64_t id) const {

    // };

} // namespace chattere::database