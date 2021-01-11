add_library(PluginManager STATIC src/plugin-manager.cpp)

target_include_directories(PluginManager PUBLIC include)

target_link_libraries(PluginManager PRIVATE CONAN_PKG::spdlog CONAN_PKG::yaml-cpp Emoji CONAN_PKG::sol2)