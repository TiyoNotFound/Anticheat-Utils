
<h1 align="center">Anti-Cheat Include for SA-MP</h1>

<p align="center">
  A basic framework for cheat detection in SA-MP (San Andreas Multiplayer) servers.
</p>

---

## Features

- **Aimbot Detection:** Monitors aiming behavior and flags players with suspicious aim positions.

- **Speedhack Detection:** Tracks player movement speed and identifies potential speed hackers.

- **Rapid Fire Detection (Placeholder):** Detects abnormal shooting patterns indicating rapid fire cheats.

- **Health/Armor Hack Detection (Placeholder):** Checks for abnormal health and armor values, exposing cheat tools.

- **Wallhack Detection (Placeholder):** Identifies cheats that allow players to see through walls.

- **Callback System:** Provides the `OnPlayerCheatDetected` callback when a cheater is detected.

- **Initialization and Player Update Hook:** Initializes data structures and checks for cheats during player updates.

- **Customizable Constants:** Easily configurable thresholds for different cheat detection mechanisms.

- **Distance Calculation Utility:** Calculates the distance between two points in the game world.

## Advantages

- **Maintain a Fair Gaming Environment:** Prevents cheating to create a fair and enjoyable gaming experience for all players.

- **Basic Cheat Detection:** Provides essential features to catch common cheating behaviors in SA-MP.

- **Customizable and Expandable:** Allows customization and expansion for more sophisticated cheat detection methods.

- **Early Detection of Cheaters:** Identifies potential cheaters and triggers appropriate actions to maintain server integrity.

- **Easy Integration:** Ready-to-use include with clear comments and constants for easy configuration.

---

## Installation

1. Copy the "anticheat.pwn" file to your SA-MP server's "pawno/include" directory.

2. Include "anticheat" in your main script: `#include <anticheat>`

3. Implement the functions in your game mode script, making sure to customize the placeholder functions for specific cheat detection logic.

---

## Usage

- Use the provided functions such as `CheckAimbot`, `CheckSpeedhack`, etc., to detect potential cheaters.

- Customize the constants (`MAX_AIM_DISTANCE`, `MAX_PLAYER_SPEED`, etc.) to set appropriate thresholds for cheat detection.

- Implement the `OnPlayerCheatDetected` callback to take actions against suspected cheaters, such as kicking or banning them from the server.

---

## Disclaimer

This anti-cheat include is a starting point for cheat detection in SA-MP servers. It may not cover all possible cheat detection scenarios and may require further refinement based on your specific server's needs. Additionally, for a more robust and comprehensive anti-cheat solution, consider utilizing third-party libraries and resources developed specifically for SA-MP server security.

---
