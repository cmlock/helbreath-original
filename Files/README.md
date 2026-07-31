## Server Runtime Files

Deployable server files: executables, configs, and per-world data used to run a live Helbreath server.

### Structure

- `GateServer.exe`, `MainLServer.exe`, `WorldLServer.exe` — gate, login, and world server binaries.
- `*.cfg` — server configuration: gate/login/world servers, items, magic, NPCs, quests, skills.
- `GameServers/` — per-zone game server instances, see [its README](GameServers/README.md).
- `GameData/` — shared faction/occupy-flag data (Aresden and Elvine).
- `Account/`, `Character/` — account and character save data, bucketed into `AscIIxxx` folders by name hash.
