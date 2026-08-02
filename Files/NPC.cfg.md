# `NPC.cfg` Quick Reference

Defines every NPC/monster template (up to `DEF_MAXNPCTYPES` = 200) loaded by HGServer. Parsed field-by-field
in `HGServer/Game.cpp` → `CGame::_bDecodeNpcConfigFileContents`, applied to spawned NPCs in `CGame::_bInitNpcAttr`.
Struct definitions live in `HGServer/Npc.h`.

## How parsing actually works

The file is **not** a real INI — it's tokenized on `= \t\n` only. `[CONFIG]`, `[ITEMS]`, and `//` comments are
never specially recognized; every token is simply checked for `memcmp(token, "Npc", 3) == 0` and skipped if it
doesn't match. That also means:

- Section headers and comments are decorative only — safe to rearrange or remove.
- A stray word starting with `Npc` inside a comment would be misread as the start of a new record.
- `[LevelCap]` / `world-server-max-level` at the bottom of the file is **dead config** — the only code that
  would read it (`Game.cpp` ~line 16603) is commented out. The real player level cap comes from
  `max-player-level` in `GServer.cfg`, not here.

Each `Npc = ...` line has **28 whitespace/`=`-separated fields**: a name, 26 numeric fields consumed by the
parser, and one trailing numeric field the parser reads but never stores (see below).

## Field order (as actually read by the parser — not the order in the file's header comment)

The in-file header comment (`;-----Name...GoldMin--GoldMax-ExpMin--ExpMax--...`) has **Gold and Exp swapped**
from what the code actually reads. Trust this table, not that comment.

| # | Field | Struct member | Notes |
|---|-------|---------------|-------|
| 1 | Name | `m_cNpcName` | Max 20 chars; matched by exact name when spawning |
| 2 | Type | `m_sType` | Unique numeric template ID. Hardcoded elsewhere in `Game.cpp` (e.g. `21`=Guard, `34`=Dummy, `64`=Crops) for special-case behavior |
| 3 | HitDice | `m_iHitDice` | Base for HP roll. ≤5: `iDice(HitDice,4)+HitDice`; >5: `HitDice*4 + HitDice + iDice(1,HitDice)` |
| 4 | DefenseRatio | `m_iDefenseRatio` | |
| 5 | HitRatio | `m_iHitRatio` | |
| 6 | MinBravery | `m_iMinBravery` | Minimum "bravery" before the NPC flees/engages. Also doubles as remaining hits-to-complete for structures (`m_iBuildCount` starts at this value) |
| 7 | ExpDiceMin | `m_iExpDiceMin` | **Exp**, not Gold — see swap note above |
| 8 | ExpDiceMax | `m_iExpDiceMax` | Exp awarded on kill = `iDice(1, ExpDiceMax-ExpDiceMin) + ExpDiceMin`, rolled once at spawn |
| 9 | GoldDiceMin | `m_iGoldDiceMin` | **Gold**, not Exp — see swap note above |
| 10 | GoldDiceMax | `m_iGoldDiceMax` | Gold drop amount = `iDice(1, GoldDiceMax-GoldDiceMin) + GoldDiceMin`, only rolled if the gold-drop branch hits. Types `21` (Guard), `34` (Dummy), `64` (Crops) never drop gold regardless of these values |
| 11 | AttackDiceThrow (ADT) | `m_cAttackDiceThrow` | Melee damage = `iDice(ADT, ADR)` |
| 12 | AttackDiceRange (ADR) | `m_cAttackDiceRange` | |
| 13 | Size | `m_cSize` | `0` = Small-Medium, `1` = Large |
| 14 | Side | `m_cSide` | Faction: `0` = neutral, `1` = Aresden, `2` = Elvine |
| 15 | ActionLimit | `m_cActionLimit` | Behavior class: `0` normal, `1` move-only + can't die, `2` stationary + can't die, `3` Dummy (training target), `4`/`5` structure/crop (tracks `m_iBuildCount`, hit by specific weapon types) |
| 16 | ActionTime | `m_dwActionTime` | Milliseconds between AI think/move ticks (jittered ±0-300ms at runtime) |
| 17 | ResistMagic | `m_cResistMagic` | Can be negative (e.g. Guards use `-10`, i.e. extra vulnerable to magic) |
| 18 | MagicLevel | `m_cMagicLevel` | `0` = non-caster |
| 19 | DayOfWeekLimit | `m_cDayOfWeekLimit` | `0`-`6` restricts spawn to that `SYSTEMTIME::wDayOfWeek`; `10` = no restriction (any value ≥10 is treated as unrestricted) |
| 20 | ChatMsgPresence | `m_cChatMsgPresence` | `1` = NPC has associated chat text (the actual-text-loading branch for this is unimplemented/empty in the current code) |
| 21 | TargetSearchRange | `m_cTargetSearchRange` | Tiles the NPC scans for a target |
| 22 | RegenTime | `m_dwRegenTime` | Milliseconds after death before respawn |
| 23 | Attribute | `m_cAttribute` | Magic element: `0` none, `1` Earth, `2` Sky, `3` Fire, `4` Water |
| 24 | AbsDamage | `m_iAbsDamage` | Positive = % magic damage reduction; negative = % physical damage reduction |
| 25 | MaxMana | `m_iMaxMana` | |
| 26 | MagicHitRatio | `m_iMagicHitRatio` | |
| 27 | AttackRange | `m_iAttackRange` | `1`+ means ranged attack is possible |
| 28 | *(unused)* | — | Present in every row (labeled `AreaSize` in the header comment) but the parser stops at field 27 and never reads it — purely vestigial |

## Sections

- `[CONFIG]` / `[ITEMS]` — decorative headers only, not parsed (see above).
- `[LevelCap]` / `world-server-max-level` — dead config, not parsed by any active code path (see above).

## Gotchas worth remembering

- Editing gold/exp values: remember field 7-8 is Exp, field 9-10 is Gold, opposite of the file's own header comment.
- Adding a new NPC: give it a `Type` not already used elsewhere in `Game.cpp`'s many `switch (m_sType)` blocks,
  or you'll silently inherit special-case behavior from an unrelated monster.
- `ActionLimit` values `3`/`4`/`5` are load-bearing for Dummies/structures/crops — don't reuse them casually for
  new regular monsters.
