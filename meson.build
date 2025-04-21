project('castle-game', 'cpp',
  default_options : ['cpp_std=c++17'])

src = meson.source_root() / 'src'

# Find Boost
boost_dep = dependency('boost')

# Find SQLite3
sqlite_dep = dependency('sqlite3')

executable('castle-game', 
  sources: [src / 'main.cpp'],
  dependencies: [
    boost_dep,
    sqlite_dep,
  ])