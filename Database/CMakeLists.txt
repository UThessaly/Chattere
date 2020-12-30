add_library(Database STATIC src/database.cpp)

target_include_directories(Database PUBLIC include)

target_link_libraries(Database PRIVATE CONAN_PKG::spdlog PUBLIC CONAN_PKG::sqlitecpp Protocol Snowflake)