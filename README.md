# Castle Game Server

![App logo](Sample.png?raw=true)

## About

Experimental game server for any generic fantasy real-time strategy games

## Prerequisites

- GCC C++ Compiler 14.2.1 or above
- C++17 compliant compiler
- Meson build system (0.56.0 or newer)
- Ninja build system (1.12.1 or newer)

## Dependencies

- Boost 1.87.0 or newer
- SQLite 3.49.1 or newer

## Installation

On Debian/Ubuntu:

```sh
apt install meson ninja-build libboost-all-dev libsqlite3-dev
```

On Arch Linux:

```sh
pacman -S meson ninja boost sqlite
```

## Build & run

The build system used in this project is meson. First, setup a build directory:

```sh
mkdir build && cd build
meson setup ..
```

After generating the build files, build with ninja:

```sh
ninja
```

Finally run the executable:

```sh
./castle-game
```

## Implementation requirements

You need to implement your own:

- Items and inventory system
- Faction definitions and mechanics
- Unit types and behaviors
- Map layouts and generation
- Game mechanics and rules

## Project structure

```
castle-game/
├── src/                          # Source code directory
│   ├── main.cpp                  # Entry point of the server application
│   ├── server/                   # Core Server Logic
│   │   ├── castle_server.cpp     # CastleServer class implementation
│   │   ├── castle_server.h
│   │   ├── game_state.cpp        # GameState class implementation
│   │   ├── game_state.h
│   │   ├── player_manager.cpp    # PlayerManager class implementation
│   │   ├── player_manager.h
│   │   ├── map.cpp               # Map class implementation
│   │   ├── map.h
│   │   ├── resource_manager.cpp  # ResourceManager class implementation
│   │   ├── resource_manager.h
│   │   ├── chat_handler.cpp      # ChatHandler class implementation
│   │   ├── chat_handler.h
│   │   ├── timer.cpp             # Timer class implementation
│   │   ├── timer.h
│   │   └── client_connection.cpp # Boost.Asio individual client connections
│   │   └── client_connection.h
│   ├── units/                    # Unit-related classes
│   │   ├── unit.cpp              # Base Unit class implementation
│   │   ├── unit.h
│   │   ├── hero.cpp              # Hero class implementation
│   │   ├── hero.h
│   │   ├── building.cpp          # Building class implementation
│   │   ├── building.h
│   │   ├── unit_type.cpp         # UnitType class implementation
│   │   ├── unit_type.h
│   │   └── abilities/            # Ability definitions
│   │       ├── ability.cpp       # Base Ability class
│   │       └── ability.h
│   ├── factions/                 # Faction Definitions
│   │   ├── faction.cpp           # Faction class implementation
│   │   ├── faction.h
│   ├── items/                    # Item definitions
│   │   ├── item.cpp              # Base Item Class
│   │   └── item.h
│   ├── upgrades/                 # Upgrade Definitions
│   │   ├── upgrade.cpp           # Base Upgrade class
│   │   ├── upgrade.h
│   │   └── upgrade_manager.cpp   # UpgradeManager implementation
│   │   └── upgrade_manager.h
│   ├── shops/                    # Shop definitions
│   │   ├── shop.cpp              # Shop Class
│   │   ├── shop.h
│   ├── database/                 # Database interaction code (SQLite)
│   │   ├── database_manager.cpp  # Handles SQLite connections and queries
│   │   ├── database_manager.h
│   ├── networking/               # Boost.Asio related code
│   │   └── ... (Custom network protocol implementations, if any)
│   └── utils/                    # Utility functions and helper classes
│       ├── logging.cpp           # Simple logging implementation
│       ├── logging.h
│       └── config_parser.cpp     # Parses configuration and server settings
│       └── config_parser.h
├── include/                      # Header files
│   ├── server/
│   ├── units/
│   ├── factions/
│   ├── items/
│   ├── upgrades/
│   ├── shops/
│   ├── database/
│   ├── networking/
│   └── utils/
├── data/                         # Game Data (maps, unit definitions, etc.)
│   ├── maps/                     # Map files (e.g., custom map formats)
│   │   └── example_map.txt
│   ├── units/                    # Unit definition files
│   │   └── unit_definitions.proto
│   ├── factions/                 # Faction data files
│   │   └── faction_data.proto
├── build/                        # Meson Build System output directory (generated)
├── meson.build                   # Meson project file
├── README.md                     # Project documentation
└── .gitignore                    # Git ignore file

```

## Class diagram keys

```
+:      Public
-:      Private
~:      Protected
<>:     Association/Dependency
1..\*:  One or more
1:      Exactly one
0..1:   Zero or one (optional)
```

## Class diagram

```
+-----------------------+
|     CastleServer      |
+-----------------------+
- server_socket: tcp::socket
- game_state: GameState
- player_manager: PlayerManager
- map: Map
- resource_manager: ResourceManager
- chat_handler: ChatHandler
- timer: Timer
- cheat_enabled: bool (default: false)
- game_speed: float (default: 1.0)

+ start() : void
+ stop() : void
+ handle_client(tcp::socket): ClientConnection
+ get_game_state() : GameState*
+ set_cheat_enabled(bool) : void
+ set_game_speed(float) : void
<> 1..* PlayerManager
<> 1 GameState
<> 1 Map
<> 1 ResourceManager
<> 1 ChatHandler
<> 1 Timer

+-----------------------+
|     GameState         |
+-----------------------+
- victory_state: VictoryState (enum: None, Player1Win, Player2Win, Draw)
- player_scores: std::map<PlayerID, int>
- elapsed_time: long long
- is_game_over: bool (default: false)

+ get_victory_state() : VictoryState
+ set_victory_state(VictoryState) : void
+ update_player_score(PlayerID, int) : void
+ get_player_score(PlayerID) : int
+ is_game_over() : bool
+ set_game_over(bool) : void

+-----------------------+
|    PlayerManager      |
+-----------------------+
- players: std::map<PlayerID, Player*>
- next_player_id: PlayerID (auto increment)
- team_assignments: std::map<TeamNumber, std::vector<PlayerID>>

+ create_player(Faction, Color, ResourceHandicapPercentage) : Player*
+ remove_player(PlayerID) : void
+ get_player(PlayerID) : Player*
+ assign_to_team(PlayerID, TeamNumber) : void
+ get_players_in_team(TeamNumber) : std::vector<PlayerID>

+-----------------------+
|        Player         |
+-----------------------+
- player_id: PlayerID
- faction: Faction (enum)
- color: Color (struct/enum)
- resource_handicap_percentage: float
- gold: int
- lumber: int
- team_number: TeamNumber (optional, default: None)
- units: std::vector<Unit*>
- buildings: std::vector<Building*>
- hero: Hero*
- upgrades: UpgradeManager

+ get_player_id() : PlayerID
+ get_faction() : Faction
+ get_color() : Color
+ get_resource_handicap_percentage() : float
+ add_gold(int) : void
+ subtract_gold(int) : void
+ add_lumber(int) : void
+ subtract_lumber(int) : void
+ assign_to_team(TeamNumber) : void
+ get_team_number() : TeamNumber
+ get_units() : std::vector<Unit*>
+ get_buildings() : std::vector<Building*>
+ set_hero(Hero*) : void
+ get_hero() : Hero*

+-----------------------+
|        Map            |
+-----------------------+
- map_data: std::vector<std::vector<Tile>>  // 2D grid of tiles
- fog_of_war: FogOfWar (enum) // Unexplored, Explored, AlwaysVisible
- tile_size: int

+ get_tile(int x, int y) : Tile*
+ set_fog_of_war(FogOfWar) : void
+ get_fog_of_war() : FogOfWar
+ load_map(const std::string& filename) : void // Loads from file (e.g., a custom format)

+-----------------------+
|   ResourceManager    |
+-----------------------+
- gold_generation_rate: float
- lumber_generation_rate: float
- upkeep_cost: int

+ calculate_resource_income(Player*) : ResourceDelta // Struct with gold and lumber changes
+ apply_upkeep(Player*) : void

+-----------------------+
|     ChatHandler      |
+-----------------------+
- chat_log: std::vector<ChatMessage>  // Stores messages (timestamp, player ID, message)

+ send_message(PlayerID, const std::string& message) : void
+ receive_message(tcp::socket, const std::string& message) : void
+ get_chat_log() : std::vector<ChatMessage>

+-----------------------+
|        Timer         |
+-----------------------+
- elapsed_time: long long
- interval: long long (milliseconds)

+ start() : void
+ stop() : void
+ tick() : bool // Returns true when the interval has passed.

+-----------------------+
|      Faction         |
+-----------------------+
- name: std::string
- color_preference: Color  // Default color for this faction
- starting_units: std::vector<UnitType> // List of units available at start

+-----------------------+
|       Color          |
+-----------------------+
- red: int (0-255)
- green: int (0-255)
- blue: int (0-255)

+-----------------------+
|      UnitType        |
+-----------------------+
- name: std::string
- cost_gold: int
- cost_lumber: int
- health: int
- attack: int
- armor: int
- speed: int
- abilities: std::vector<Ability>

+-----------------------+
|        Unit          |
+-----------------------+
- unit_type: UnitType
- owner: Player*
- x: int
- y: int
- health: int
- max_health: int
- attack: int
- armor: int
- speed: int

+ move(int dx, int dy) : void
+ attack(Unit*) : void
+ take_damage(int damage) : void
+ get_owner() : Player*

+-----------------------+
|      Building        |
+-----------------------+
- building_type: BuildingType
- owner: Player*
- x: int
- y: int
- health: int
- max_health: int

+ repair(int amount) : void
+ take_damage(int damage) : void

+-----------------------+
|    BuildingType      |
+-----------------------+
- name: std::string
- cost_gold: int
- cost_lumber: int
- health: int
- production_rate: float // Units per second it can produce.

+-----------------------+
|       Hero           |
+-----------------------+
- unit: Unit  // Inherits from Unit
- xp: int
- level: int
- intelligence: int
- agility: int
- strength: int
- inventory: std::vector<Item*>
- hp: int
- mana: int

+ gain_xp(int amount) : void
+ level_up() : void
+ use_ability(Ability, Unit*) : void // Or target location.

+-----------------------+
|       Ability        |
+-----------------------+
- name: std::string
- cooldown: float
- mana_cost: int
- effect: AbilityEffect (enum)  // Damage, Heal, Buff, Debuff, etc.

+-----------------------+
|      Item            |
+-----------------------+
- name: std::string
- type: ItemType (enum) // Weapon, Armor, Potion, Scroll...
- stats_bonus: StatBonus (struct with bonuses to strength, agility, intelligence...)

+-----------------------+
|    UpgradeManager    |
+-----------------------+
- upgrades: std::map<UpgradeID, Upgrade*>

+-----------------------+
|       Upgrade        |
+-----------------------+
- upgrade_id: UpgradeID
- name: std::string
- cost_gold: int
- cost_lumber: int
- effect: UpgradeEffect (struct with stat changes, resource generation boosts...)

+-----------------------+
|    Shop              |
+-----------------------+
- location_x: int
- location_y: int
- items: std::map<Item*, int> // Item and quantity available.
- cooldowns: std::map<Item*, long long> // Time until item is restocked

+ buy_item(Player* player, Item* item) : bool  // Returns true if successful.
+ restock() : void
```

## Known problems

No error handling and logging are in place.

## Contributing

Changes are welcome. Breaking this project is highly encouraged!

## License

This is free and unencumbered software released into the public domain under The Unlicense.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

See [UNLICENSE](LICENSE) for full details.
