<div align="center">
<img width="800" alt="pokebowl1" src="https://raw.githubusercontent.com/Forzalab/files/refs/heads/main/ab1e2844-4de8-46c8-bcee-0f4ebbd28a88.webp" />
  <h1 style="font-size: 28px; margin: 10px 0;">PokéBowl©</h1>
  <p><em>A Pokémon-inspired, terminal-native RPG. Powered by Kerney's server, C++, and calories.</em></p>
</div>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-26-blue?logo=cplusplus&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/platform-Linux-lightgrey?logo=linux" alt="Platform">
  <img src="https://img.shields.io/badge/status-WIP-yellow" alt="Status">
</p>

<div align="center">
<h4>Shoutout to all <i>PokéBowl©</i> team members!</h4>
<a href="https://github.com/Clovis-Community-College/rpg-41-pokebowl/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Clovis-Community-College/rpg-41-pokebowl" />
</a>
</p>
</div>

> *Nintendo, please don't sue us. Go sue Kerney for greenlitting the idea of "PokeBowl" instead.*

## What's this game about?
**PokéBowl©** is a Pokemon-inspired, singleplayer, console-based RPG.
- Explore the One Great Lake with arrow keys (⬅️⬆️⬇️➡️), meet your heroes, access your own inventory (`i`, `e`), and complete quests.
- See your Heroes fighting monsters at first contact. Let Dalet heal companions, and protect them from Delta's one-shot punch.

*Defeat Foxtrot the Boss, kill all monsters* to win and conquer the Poké-verse!

### Quick start
Hassle-free experience! Just clone our repo, `make`, run `a.out`, and be in awe. No command argument needed.

### Key commands
| Key | Action |
|-----|--------|
| `← ↑ ↓ →` | Move player |
| `i` | Open inventory |
| `e` | Open equipment table |
| `[SPACE]` | (*Combat mode*) Next combat turn |
| `r` | (*Combat mode*) Abort combat & return to map |
| `b` | Screenshot game state into an URL in BRIDGES |
| `q` | Quit game |

### 🎶 Music
PokéPlayers, enjoy our one and only **custom-made theme song**!

[![🎶 PokéBowl theme music](https://markdown-videos-api.jorgenkh.no/url?url=https%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3DUc0DoiE9-hQ)](https://www.youtube.com/watch?v=Uc0DoiE9-hQ)

## Checklist, 1 2 3
 
### Group

#### Quality
- `✅` **A)** Fun to play — *Everyone*
- `✅` **B)** Decent amount of content / take at least a little while to win — *Everyone*
- `✅` **C)** Has an interesting world map in color — *McKay*
- `✅` **D)** Win and lose — *McKay & Tony*
- `✅` **E)** Combat — *McKay & Tony*
  
#### Documentation
- `✅` **A)** README describes game: directions to win, key commands, CLI params — *Tony*
- `✅` **B)** README uses Markdown: bullet points, colors, embedded logo image — *Tony*
- `✅` **C)** README lists all contributors with who did each bullet point (group + individual); undone items noted — *Tony*

#### Consistency of work
- `✅` **A)** [Screenshot of game state, 1 week](https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/main/README.md#game-state-at-week-1) — *Tony*
- `✅` **B)** [Screenshot of GitHub commit log showing consistent commits over time](https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/main/README.md#commit-log) — *Tony*
 
### Individual
 
#### A — "Inheritor of Suffering" (Tony)
- `✅` Pure virtual / abstract `Actor` class
- `✅` Stationary objects, Heroes, Monsters inheriting from `Actor`
- `✅` `Hero` class with 4+ subclasses.
- `✅` `Monster` class with 5+ subclasses  including a **Boss**
- `✅` Player controls a party of 4–6 heroes, walks around, kills monsters, does quests
- `✅` README: ![description of class hierarchy](https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/main/README.md#actor-class) + ![diagram showing class relationships](https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/main/README.md#class-diagram) + ![code snapshots](https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/main/README.md#code-snapshots)
 
#### B — "BRIDGEngineer" (George)
- `✅` `HasInitiative` class with `speed` member (1–40); Hero and Monster inherit from it
- `✅` On combat start: roll d20 + speed, sort fastest-first into circular linked list
- `✅` Circular linked list for turn order: advance on turn end, wrap at tail, remove on death
- `✅` Snapshot command sends current turn order visualization to BRIDGES at any point in combat
- `✅` Works for any combat
- `✅` Each BRIDGES node labeled with actor name + initiative value
- `✅` [Screenshot of BRIDGES combat visualization embedded in README](https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/main/README.md#bridges-combat)
 
#### C — "JJK Curse Lord" (McKay)
- `✅` ncurses full-screen UI in raw mode, no Cin/Cout line scrolling
- `✅` Arrow key movement
- `✅` Color
- `✅` Scrollable world map with towns, lakes, and other features
- `✅` Combat displayed on screen
- `✅` Inventory displayed on screen
- `✅` Party moves on map when arrow keys pressed
- `✅` Demonstrates reasonable ncurses proficiency
 
#### D — "Sephiroth, Master of the Tree" (Jovanni)
- `✅` BST inventory system using **`custom BST`**
- `✅` Print all items in inventory in alphabetical order —
- `✅` Support multiple copies of same item
- `✅` Pick up and drop items
- `✅` Buy items with gold from merchant; sell non-Key Items to merchant
- `✅` Key Items: cannot be dropped; removed only by completing their quest
- `✅` Monsters have randomly generated inventories that drop on defeat
- `✅` Items have varied stats (damage, speed modifier, healing amount, merchant cost, etc.)
- `✅` Coordinated with McKay (C) for inventory display, Tony (A) for hero/monster inventory ownership
 
#### E — "Scared Balloon" (Dillion)
- `✅` Dynamic weather system
- `🚧` Quest system (quest 3 incomplete)
- `✅` Hash table implemented and used as part of the system

## Screenshots/snapshots

### Commit log
> [!NOTE]
> These are all **permalinks**.

> [!NOTE]
> - `Forzalab` == `ho_1041841` == Tony
> - `ScytheSlayin` == `??` == Dillion
> - `mediumsomething` == `laura_1002948` == George
> - `robles_0965625` == Jovanni
> - `MckaySea` == `McKay Seamons` == McKay

##### Week 1
https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/6262139c2c7df1b9115a83995b6a95eeb6f09a31/log/week1.txt#L1-L93
##### Week 2
https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/6262139c2c7df1b9115a83995b6a95eeb6f09a31/log/week2.txt#L1-L128
##### Week 3
https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/6262139c2c7df1b9115a83995b6a95eeb6f09a31/log/week3.txt#L1-L144

### Game state at week 1

![Game state](https://raw.githubusercontent.com/Forzalab/files/refs/heads/main/Screenshot_20260412_204949_Termux.png)

### BRIDGES combat

<img width="683" height="582" alt="BRIDGEScombat" src="https://github.com/user-attachments/assets/8cd035cf-ad82-464e-a401-2c740f02f63c" />

### Class hierachy of `actor.h`
- `Actor` is an abstract base class for object with a position, and `Wall` + `NonWall` inherits from it.
    - **Stationary objects** (`Drop` and `Merchant`) inherit from `Wall`. 
    - **Moving objects** (`Hero` and 
`Monsters`) inherit from `NonWall`.
- `Hero` and `Monster` also inherit from `HasInitiative`. This gives them a speed value (1 through 40) for combat turn order.    
- `Hero` subclasses include Aleph, Bet, Gimel, Dalet, He, Vav, Zayin, and Chet.
- `Monster` subclasses include Alpha, Bravo, Charlie, Delta, Echo, Foxtrot, Golf, and Hotel. `Foxtrot` is the game's **Boss**.

#### Class diagram
```mermaid
---
  config:
    class:
      hideEmptyMembersBox: true
---
classDiagram
    direction TB

    Actor <|-- Wall
    Actor <|-- NonWall
    Wall <|-- Merchant
    Wall <|-- Drop
    NonWall <|-- Hero
    NonWall <|-- Monster
    HasInitiative <|-- Hero
    HasInitiative <|-- Monster

    class Hero {
        <<has-subclasses>>
        Aleph
        Bet
        Gimel
        Dalet
        He
        Vav
        Zayin
        Chet
    }

    class Monster {
        <<has-subclasses>>
        Alpha
        Bravo
        Charlie
        Delta
        Echo
        Golf
        Hotel
        Foxtrot - BOSS
    }

```

#### Code snippet snapshots (scrollable)

https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/fd7d663ba1c1438278978bee77c4ccb368df05b5/actor.h#L1-L333

https://github.com/Clovis-Community-College/rpg-41-pokebowl/blob/75d291e5ec1c21728fb204fb634450ebc78f4722/actor.cc#L1-L350
