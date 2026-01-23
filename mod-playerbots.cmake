# mod-playerbots cmake configuration
# Adds additional library dependencies for the playerbots module

# Boost thread is needed for PlayerbotCommandServer and RandomPlayerbotMgr
find_package(Boost REQUIRED COMPONENTS thread)

target_link_libraries(modules
  PUBLIC
    Boost::thread)
