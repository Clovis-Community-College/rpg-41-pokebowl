# <img src="https://i.redd.it/is0wgptfp0x01.jpg"/> Pokébowl RPG

> *slogan or short mythical text here. never gonna give u up, never gonna let u down, never gonna run around and desert u*

## What is it?
blablabla. balbalbalbalajsjdhrgeuehehegeheb.

---

## Key commands

| Key | Action |
|-----|--------|
| Arrow keys | Move actors |
| `i` | Open inventory |
| `c` | View combat order |
| `q` | Quit game |
| **[any key]** | **[unimplemented]** |

---

## Checklist

---
 
### Group

#### Quality
- **A)** Is it a fun game? — `[unimplemented]` — *Everyone*
- **B)** Does it have a decent amount of content / take at least a little while to win? — `[unimplemented]` — *Everyone*
- **C)** Does it have an interesting world map in color? — `[unimplemented]` — *name*
- **D)** Can you win and lose? — `[unimplemented]` — *name*
- **E)** Is there combat? — `[unimplemented]` — *name*
  
#### Documentation
- **A)** README describes game: directions to win, key commands, CLI params — `[IN PROGRESS]` — *Tony*
- **B)** README uses Markdown: bullet points, colors, embedded logo image — `[IN PROGRESS]` — *Tony*
- **C)** README lists all contributors with who did each bullet point (group + individual); undone items noted — `[IN PROGRESS]` — *Tony*

#### Consistency of work
- **A)** Screenshot of game state as of one week in (embedded below) — `[unimplemented]` — *name*
- **B)** Screenshot of GitHub commit log showing consistent commits over time (embedded below) — `[unimplemented]` — *name*
 
---
 
### Individual
 
#### A — "Inheritor of Suffering" (Tony)
- Pure virtual / abstract `Actor` class — `[unimplemented]`
- Stationary objects, Heroes, Monsters inheriting from `Actor` — `[unimplemented]`
- `Hero` class with 4+ subclasses (e.g. Wizard, Fighter, Thief, Cleric, unimplemented, unimplemented) — `[unimplemented]`
- `Monster` class with 5+ subclasses including a **Boss** (killing Boss = win condition) — `[unimplemented]`
- Player controls a party of 4–6 heroes, walks around, kills monsters, does quests — `[unimplemented]`
- README: description of class hierarchy + diagram showing class relationships — `[unimplemented]`
 
#### B — "BRIDGEngineer" (George)
- `HasInitiative` class with `speed` member (1–40); Hero and Monster inherit from it — `[unimplemented]`
- On combat start: roll d20 + speed, sort fastest-first into circular linked list — `[unimplemented]`
- Circular linked list for turn order: advance on turn end, wrap at tail, remove on death — `[unimplemented]`
- Snapshot command sends current turn order visualization to BRIDGES at any point in combat — `[unimplemented]`
- Works for any combat (not hardcoded to one fight) — `[unimplemented]`
- Each BRIDGES node labeled with actor name + initiative value — `[unimplemented]`
- Screenshot of BRIDGES combat visualization embedded in README — `[unimplemented]`
 
#### C — "JJK Curse Lord" (McKay)
- ncurses full-screen UI in raw mode, no Cin/Cout line scrolling — `[unimplemented]`
- Arrow key movement — `[unimplemented]`
- Color — `[unimplemented]`
- Scrollable world map with towns, lakes, and other features — `[unimplemented]`
- Combat displayed on screen — `[unimplemented]`
- Inventory displayed on screen — `[unimplemented]`
- Party moves on map when arrow keys pressed — `[unimplemented]`
- Demonstrates reasonable ncurses proficiency — `[unimplemented]`
 
#### D — "Sephiroth, Master of the Tree" (Jovanni)
- BST inventory system using `[custom BST/std::multimap]` — `[unimplemented]`
- Print all items in inventory in alphabetical order — `[unimplemented]`
- Support multiple copies of same item (e.g. 100 arrows, 2 potions) — `[unimplemented]`
- Pick up and drop items — `[unimplemented]`
- Buy items with gold from merchant; sell non-Key Items to merchant — `[unimplemented]`
- Key Items: cannot be dropped; removed only by completing their quest — `[unimplemented]`
- Monsters have randomly generated inventories that drop on defeat — `[unimplemented]`
- Items have varied stats (damage, speed modifier, healing amount, merchant cost, etc.) — `[unimplemented]`
- Coordinated with McKay (C) for inventory display, Tony (A) for hero/monster inventory ownership — `[unimplemented]`
 
#### E — "Scared Balloon" (Dillion)
- Dynamic weather system — `[unimplemented]`
- Quest system — `[unimplemented]`
- Hash table implemented and used as part of the system — `[unimplemented]`
 
---

## Screenshots

### Game state at week 1

```
[ unimplemented.png ]
```

### Commit log

```
[ unimplemented.png ]
```

### BRIDGES combat

```
[ unimplemented.png ]
```
