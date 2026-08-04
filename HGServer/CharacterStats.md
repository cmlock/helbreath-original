# Character Stat Mechanics Reference

What each of the seven character stats (`STR`, `DEX`, `VIT`, `INT`, `MAG`, `Charisma`, `Luck`) actually does in `HGServer/Game.cpp`. Citations are `Game.cpp:<line>` unless noted. Member variables: `m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma` (Client.h:114), `m_iLuck` (Client.h:116).

Several formulas use an "Angelic" bonus (`m_iAngelicStr/Dex/Int/Mag`, Client.h:40) added to the base stat before the calculation. These come only from equipped Angelic Pendant items — `CalcTotalItemEffect` zeroes them each recompute (32216-32219) and sets them from the pendant's upgrade-level nibble: `((m_dwAttribute & 0xF0000000) >> 28) + 1` (32580-32603). **There is no Angelic variant for VIT, Charisma, or Luck.**

## Central derived-stat functions

- **`CalcTotalItemEffect` (32194)** — the real recompute, run on login, equip/unequip, and stat change. Sets `m_iHitRatio = 0` (32230) and `m_iDefenseRatio = m_iDex * 2` (32231), later adds `m_iAngelicDex * 2` (32757). Clamps current HP/MP/SP to the max-pool getters below (32758-32760).
- **`___RestorePlayerCharacteristics` (31832) is dead code.** Line 31839 is an unconditional `return;` before any of its stat-redistribution logic runs. Ignore it — it is not live.
- **Max-pool getters:**
  - `iGetMaxHP` (39488): `Vit*3 + Level*2 + (Str+AngelicStr)/2`, then reduced by the `m_iSideEffect_MaxHPdown` item side-effect if set (39496-39497).
  - `iGetMaxMP` (39502): `2*(Mag+AngelicMag) + 2*Level + (Int+AngelicInt)/2`.
  - `iGetMaxSP` (39513): `2*(Str+AngelicStr) + 2*Level`.

## STR

| Effect | Formula | Location |
|---|---|---|
| Max SP | `2*(Str+AngelicStr) + 2*Level` | 39519 (dup. 29460, 29495) |
| Max HP (partial) | `+(Str+AngelicStr)/2` | 39494 (dup. 29411, 19824) |
| Bare-hand damage | `iDice(1, (Str+AngelicStr)/12)`, min 1 | 52817-52819 |
| Melee weapon damage | `dmg += dmg * ((Str+AngelicStr)/5) / 100` — ≈+0.2%/point | 52833-52845 |
| Missile (bow) damage | `+= iDice(1, (Str+AngelicStr)/20)` | 52857-52858 |
| Carry capacity | `(Str+AngelicStr)*500 + Level*500` | 25304 |
| Equip weight gate | item equippable only if `weight <= (Str+AngelicStr)*100` | 12560 |
| Armor STR requirement | req-code 10: needs `(Str+AngelicStr) >= value5` or auto-unequip | 12567-12573 |
| Attack/move speed | equipped-item speed penalty reduced by `(Str+AngelicStr)/13` (floored 0) | 12804, 12825 |
| Skill caps | Mining, Hand-Attack, Manufacturing capped at `(Str+AngelicStr)*2` | 25452-25457, 55761-55763, 55801-55803, 55821-55823 |

## DEX

| Effect | Formula | Location |
|---|---|---|
| Defense Ratio (dodge) | base `Dex*2` (+ `AngelicDex*2`) — sole stat source of DR | 32231, 32757 |
| Hit Ratio bonus | `+((Dex+AngelicDex) - 50)` if over 50 | 53231-53233 |
| Armor DEX requirement | req-code 11: needs `(Dex+AngelicDex) >= value5` | 12574-12580 |
| Skill caps | Fishing, Archery, Short-Sword, Long-Sword, Fencing, Axe, Shield, Hammer capped at `(Dex+AngelicDex)*2` | 25484-25489, 55765-55791, 55817-55818 |

DR is the sole denominator of the physical to-hit formula (see Cross-cutting, below). There is no separate parry/dodge roll — shield block comes from the shield's flat `m_iDamageAbsorption_Shield` item value (32729), not from DEX.

## VIT (no Angelic variant)

| Effect | Formula | Location |
|---|---|---|
| Max HP (dominant term) | `Vit*3` | 39494 |
| HP regen | `iDice(1, Vit)`, floored at `Vit/2` | 52320-52322 |
| SP regen (sole driver) | `iDice(1, Vit/3)` | 16889 |
| Physical damage mitigation | incoming AP reduced by `iDice(1, Vit/10) - 1` | 53336-53337 |
| Magic damage mitigation | incoming magic dmg reduced by `iDice(1, Vit/10) - 1` | 28184, 28670, 29118 |
| Armor VIT requirement | req-code 12: needs `Vit >= value5` (base VIT only) | 12582 |
| Skill cap | Poison-Resistance capped at `Vit*2` | 25503, 55805-55806 |

Note: VIT gates how high the Poison-Resistance *skill* can train — VIT itself never enters a poison-damage roll directly.

## INT

| Effect | Formula | Location |
|---|---|---|
| Max MP (partial) | `+(Int+AngelicInt)/2` | 39508 |
| Spell success chance | roll gains `((Int+AngelicInt) - 50)/2` if over 50 | 17446-17447 |
| Spell learning gate | can only learn if `reqInt <= (Int+AngelicInt)` | 21485 |
| Spell retention gate | known spell revoked (mastery reset to 0) if its `m_sIntLimit > (Int+AngelicInt)` | 24019-24021 |
| Armor INT requirement | req-code 13: needs `(Int+AngelicInt) >= value5` | 12589 |
| Special items | Stormbringer needs `Int+AngelicInt >= 65` (12615); Resurrection wands auto-grant magic[94] if `Int+AngelicInt > 99 && Mag+AngelicMag > 99` (12626, 3704) | |
| Skill caps | Farming, Alchemy, First-Aid, Pretend-Corpse capped at `(Int+AngelicInt)*2` | 25495, 55797-55798, 55809-55810, 55813-55814 |

## MAG

| Effect | Formula | Location |
|---|---|---|
| Max MP (dominant term) | `2*(Mag+AngelicMag)` | 39508 |
| MP regen (sole driver) | `iDice(1, Mag+AngelicMag)` | 16857 |
| Spell damage | `dmg += dmg * ((Mag+AngelicMag)/3.3) / 100` — ≈+0.30%/point | 28022-28026, 28512, 28926 |
| Magic effect/power roll | `iResult = SkillMastery[magic] + ((Mag+AngelicMag) - 50)` if over 50 | 17520-17521 |
| Beneficial-spell duration | `×(1.0 + (Mag+AngelicMag)/200)` — uncapped, +(Mag/2)% duration | 17338-17350 |
| Magic resistance | `resist = SkillMastery[resist] + AddMR + ((Mag+AngelicMag)-50 if >50) + AddResistMagic` | 29534-29537 |
| Armor MAG requirement | req-code 14: needs `(Mag+AngelicMag) >= value5` | 12596 |
| Skill caps | Magic, Staff-Attack capped at `(Mag+AngelicMag)*2` | 25469, 55757-55758, 55793-55794 |

## Charisma (no Angelic variant)

| Effect | Formula | Location |
|---|---|---|
| Shop discount | `discountRatio% = (Charisma-10)/4`, capped so discount never exceeds `(cost/2)-1` | 13351, 13358-13366 |
| Summon/construction points | `Charisma*300` (capped at `DEF_MAXSUMMONPOINTS`) | 2095-2099, 20886-20890, 55250 |
| Guild creation / command gate | requires `Level >= 20 && Charisma >= 20` | 13137-13139, 13058 |
| Armor Charisma requirement | req-code 15: needs `Charisma >= value5` | 12603 |

Charisma has **no** combat, HP/MP/SP, regen, damage, or resistance effect, and is in no skill cap.

## Luck — vestigial, no mechanical effect

Every reference to `m_iLuck` in the entire `HGServer/` tree:
- Declaration — Client.h:116.
- Load from save file — `m_iLuck = atoi(token);` (6016).
- Debug/save print — `"character-LUCK = %d"` (7497).

Nothing else reads it — no combat, drop-rate, crafting, fishing, mining, or economy formula ever consumes `m_iLuck`. It is stored and displayed only.

(There's an unrelated boolean `m_bIsLuckyEffect` — a temporary ~1-in-10 death-save on a lethal magic hit, set by an *equipped item's* effect value at 32515-32517 and checked at 28188-28189. It is not driven by the `m_iLuck` stat and shouldn't be confused with it.)

## Cross-cutting combat formulas

- **Physical to-hit** (`bCheckAttackSuccess`, ~53312): `hit% = clamp((attackerHitRatio / targetDefenseRatio) * 50, MIN, MAX)`. Attacker side = weapon-skill mastery + flat `+50` (52861) + DEX-over-50 bonus (53232). Defender side = `Dex*2 + AngelicDex*2` plus item DR add-ons.
- **Magic to-hit** (`bCheckResistingMagicSuccess`, ~29558): `hit% = clamp((castHitRatio / targetMagicResist) * 50, MIN, MAX)`, where resist includes the MAG-over-50 bonus.
- **Super-attack/critical charges are not stat-based** — max charges = `Level/10` (3899, 28222, 28710, 29165, 53660, 53725); charge accrues from a periodic counter or an item's `m_iAddChargeCritical`, never from STR/DEX/Luck.
- `DEF_CHARPOINTLIMIT` is the per-stat hard cap enforced on stat-up (23778-23783, 24128-24143).

## Summary

STR, DEX, VIT, INT, and MAG each drive multiple real combat/survival systems. Charisma has three narrow, non-combat uses (shop discount, summon points, guild-creation gate). Luck does nothing at all beyond being stored.
