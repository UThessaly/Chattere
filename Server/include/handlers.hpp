#include "server.hpp"
#include "packet.hpp"
#include "emoji.hpp"
#include "snowflake.hpp"

namespace chattere::handlers::serverbound
{
    static auto OnAuthRequest = [](std::tuple<Server *, std::shared_ptr<net::ClientSocket>, protocol::Packet *> data) {
        auto &[server, client, packet] = data;

        auto auth_request = packet->auth_request();

        auto database = server->GetDatabase();

        auto user = database.GetUserByUsername(auth_request.username());

        if (!user)
        {
            server->GetConsoleLogger()->error(R"(    {}  User {} not found)", chattere::EMOJIS["sad"], auth_request.username());

            protocol::Packet packet;

            auto errorPacket = new protocol::Error();
            errorPacket->set_error(10001);
            errorPacket->set_error_str("User Doesn't Exist");

            packet.set_allocated_error(errorPacket);

            client->GetSocket()->Send(protocol::packet_to_buffer(&packet));
            return;
        }

        server->GetConsoleLogger()->info(R"(    {}  User {} has logged in with user id {})", chattere::EMOJIS["smile"], auth_request.username(), user->id());
        server->AssingSocketToUser(client->GetId(), user->id());

        // protocol::Packet on_ready_packet;

        // auto on_ready = new protocol::OnReadyPacket();

        // auto user_data = *user;
        // on_ready->set_allocated_user(&user_data);

        // on_ready_packet.set_allocated_on_ready(on_ready);
        // client->GetSocket()->Send(protocol::packet_to_buffer(&on_ready_packet));
    };

    static auto OnSignupRequest = [](std::tuple<Server *, std::shared_ptr<net::ClientSocket>, protocol::Packet *> data) {
        auto &[server, client, packet] = data;
        auto &database = server->GetDatabase();

        auto signup_request = packet->signup_request();

        auto dbUser = database.GetUserByUsername(signup_request.username());

        if (static_cast<bool>(dbUser))
        {
            server->GetConsoleLogger()->error(R"(    {}  User {} already exists)", chattere::EMOJIS["sad"], signup_request.username());

            protocol::Packet packet;

            auto errorPacket = new protocol::Error();
            errorPacket->set_error(10002);
            errorPacket->set_error_str("User Already Exists Exist");

            packet.set_allocated_error(errorPacket);

            client->GetSocket()->Send(protocol::packet_to_buffer(&packet));
            return;
        }

        auto user = server->GetDatabase().CreateUser(signup_request.username(), std::string("totally_hashed_passwords") + signup_request.password());
        server->GetConsoleLogger()->info(R"(    {}  New User with id {} and username {})", chattere::EMOJIS["smile"], user->id(), user->username());
    };

    static auto OnChat = [](std::tuple<Server *, std::shared_ptr<net::ClientSocket>, protocol::Packet *> data) {
        auto &[server, client, packet] = data;
        auto &database = server->GetDatabase();

        auto user_id = server->UserFromClient(client->GetId());
        auto user = server->GetDatabase().GetUserById(user_id);

        if (!user)
        {
            return;
        }

        auto chat_message = packet->chat();
        server->GetConsoleLogger()->info(R"(    {}  {}: {})", chattere::EMOJIS["speech_balloon"], user->username(), chat_message.content());
    };
} // namespace chattere::handlers::serverbound
