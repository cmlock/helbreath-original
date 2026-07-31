## Helbreath Server Sources

The code that started the Helbreath private server revolution.

### Structure

- `Wmain.cpp`, `Game.cpp/h` — server entry point and main game loop.
- `Client.cpp/h`, `XSocket.cpp/h`, `NetMessages.h` — player connections and networking.
- `Npc.cpp/h`, `Item.cpp/h`, `Magic.cpp/h`, `Skill.cpp/h` — core gameplay systems: NPCs, items, magic, skills.
- `Map.cpp/h`, `Tile.cpp/h`, `Teleport*.cpp/h` — world/map data and movement.
- `GuildsMan.cpp/h`, `CrusadeCore.cpp/h`, `StrategicPoint.cpp/h`, `OccupyFlag.cpp/h` — guild and Crusade (territory war) systems.
- `Quest.cpp/h`, `BuildItem.cpp/h`, `Mineral.cpp/h`, `Fish.cpp/h`, `Portion.cpp/h` — quests, crafting, mining, fishing, potions.
- `DynamicObject.cpp/h`, `Structure.cpp/h`, `TempNpcItem.cpp/h` — world objects and player-built structures.
- `IrcBot.cpp/h` — IRC bot integration.
- `DebugDialog.cpp/h` — Win32 debug/admin UI.
- `HGserver.sln` / `HGserver.vcxproj` — Visual Studio solution and project files.
- `Changes.txt` — version history and changelog.