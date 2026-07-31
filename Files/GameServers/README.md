## Game Servers

Each subfolder is a standalone HGServer zone instance with its own config and map data.

### Structure

- `Aresden/`, `Elvine/`, `Middleland/`, `Dungeons/`, `Heldenian/`, `Apocalypse/`, `Neutrals/`, `Huntzones/`, `ToH/` — one folder per world/zone server.
- Each zone folder contains:
  - `GServer.cfg` — that zone's server configuration.
  - `MAPDATA/` — `.amd` map files for the zone, with `.txt` companions.
  - `GameData/` — zone-specific runtime data (e.g. GUID files).
- `GameConfigs/` — settings shared across all zones: admin list, ban list, Crusade config, schedule, global settings.
