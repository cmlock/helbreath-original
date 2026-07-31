# Helbreath Server 3.82

Legacy C++ MMO client/server stack for Helbreath. Built with Visual Studio (2026 recommended); no CMake/Makefile/CI — each component is a standalone `.sln`/`.vcxproj`.

- Client 3.82, default 800x600 resolution, Windows 10/11 compatible.
- HGServer 2.24b, modified to work with the client resolution.
- HB Server Files 3.51 (2.25a), default configs and maps.
- Helbreath USA 3.72 installer.

## Repository Structure

### `Client/` — Win32 game client
DirectDraw/DirectInput/DirectSound game client.
- `*.cpp` / `*.h` — core client source: game loop, rendering, input, networking, UI.
- `Client.sln` / `Client.vcxproj` — Visual Studio solution and project files.
- `DDRAW.*`, `DINPUT.*`, `DSOUND.*`, `DXGUID.LIB` — DirectX 7-era SDK headers and import libs.
- `resource.rc`, `*.ico` — Win32 resources and icons.
- `LOGIN.CFG` — default client login configuration.
- `CxImage/` — vendored third-party image library (BMP/JPEG/PNG/GIF/TIFF decoding). `xima*.cpp/h` per-format codecs; own `CxImage.vcxproj`/`cximage.sln`, built as a Client dependency.
- `jpeg/` — vendored third-party IJG libjpeg, used by CxImage for JPEG support. `j*.c/h` sources; own `Jpeg.vcxproj`/`Jpeg.sln`, built as a CxImage dependency.

### `HGServer/` — game server
The code that started the Helbreath private server revolution.
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

### `Files/` — deployable server runtime
Executables, configs, and per-world data used to run a live server.
- `GateServer.exe`, `MainLServer.exe`, `WorldLServer.exe` — gate, login, and world server binaries.
- `*.cfg` — server configuration: gate/login/world servers, items, magic, NPCs, quests, skills.
- `GameData/` — shared faction/occupy-flag data (Aresden and Elvine).
- `Account/`, `Character/` — account and character save data, bucketed into `AscIIxxx` folders by name hash.
- `GameServers/` — per-zone HGServer instances, one folder each: `Aresden/`, `Elvine/`, `Middleland/`, `Dungeons/`, `Heldenian/`, `Apocalypse/`, `Neutrals/`, `Huntzones/`, `ToH/`. Each contains:
  - `GServer.cfg` — that zone's server configuration.
  - `MAPDATA/` — `.amd` map files for the zone, with `.txt` companions.
  - `GameData/` — zone-specific runtime data (e.g. GUID files).
  - `GameConfigs/` (sibling folder, shared across zones) — admin list, ban list, Crusade config, schedule, global settings.

### `Helbreath/` — client game assets
Content consumed by the Client at runtime.
- `SPRITES/` — `.pak` sprite archives for characters, monsters, items, tiles, and UI.
- `MAPDATA/` — `.amd` map files for towns, dungeons, and fields.
- `SOUNDS/` — `.wav` sound effects.
- `MUSIC/` — `.wav` background music tracks.
- `FONTS/` — `.fnt` bitmap fonts (English and Korean/Hangul).
- `CONTENTS/` — text-based game data: item/skill/magic configs, NPC message lists, banned word list.

### `Tools/` — third-party utility archives
Zipped/rar'd tools for Helbreath server operators: AnMap, EditPro2, HB Server Manager, HB Pak, HB Master, Magic CFG, Pak Builder, Contents Maker, Dupe Checker, HB Char Edit, HBx Tools (Special Item Number Generator), HB Calculator, Skill Calculator.

### `CONTENTS/`, `SPRITES/` (repo root)
Legacy loose asset files (`LOGIN.CFG`, `GameDialog2.pak`) left at the repo root, duplicating content under `Helbreath/`.

## Notes
- No build automation exists — build each `.sln` in Visual Studio individually (Client depends on CxImage depends on jpeg; HGServer is independent).
- Per-folder `README.md` files mirror the sections above for GitHub browsing; update both if structure changes.
