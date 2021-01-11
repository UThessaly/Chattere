local json = require('json')

local plugin = Plugin.new("ExamplePlugin")

plugin:on("message", function (event)
    print("Lua Event Message from " .. event:get_user():get_name())
end)

plugin:on("command", function (event)
    if(event:get_command() == "/broadcast") then
        print("BROADCAST!")
    end
end)

local ping = function (sender, command, args)
    print("pong")
end

command("/ping", ping)

plugin:register()
