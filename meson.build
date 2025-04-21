project('castle-game', 'cpp',
  default_options : ['cpp_std=c++17'])

# Source directories
src = meson.source_root() / 'src'
inc = include_directories('include')

# Find dependencies
boost_dep = dependency('boost')
sqlite_dep = dependency('sqlite3')

# Source files
sources = [
  'src/main.cpp',
  'src/server/castle_server.cpp',
  'src/server/game_state.cpp',
  'src/server/player_manager.cpp',
  'src/server/player.cpp',
  'src/server/map.cpp',
  'src/server/resource_manager.cpp',
  'src/server/chat_handler.cpp',
  'src/server/timer.cpp',
  'src/networking/client_connection.cpp',
  'src/networking/message.cpp',
  'src/database/database_manager.cpp',
  'src/factions/faction.cpp',
  'src/factions/specific_factions.cpp',
  'src/shops/shop.cpp',
  'src/shops/specific_shops.cpp',
  'src/units/unit.cpp',
  'src/units/specific_units.cpp',
  'src/upgrades/upgrade.cpp',
  'src/upgrades/specific_upgrades.cpp',
  'src/upgrades/upgrade_manager.cpp',
]

executable('castle-game', 
  sources: sources,
  include_directories: inc,
  dependencies: [
    boost_dep,
    sqlite_dep,
  ])