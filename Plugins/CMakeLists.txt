add_library(Plugins STATIC src/plugins.cpp src/lua_yaml.cpp)

target_include_directories(Plugins PUBLIC include)

target_link_libraries(Plugins PRIVATE CONAN_PKG::spdlog Server CONAN_PKG::yaml-cpp Emoji CONAN_PKG::sol2)