# Starship

## Features
- Juicy Camera Shaking and Camera Following
- Elastic collision physics

## Gallery
> Demo   
> ![](Docs/Starship_1.gif)

## How to control
```
Player Ship Controls
- **Movement:**
  - **Left JoyStick**: control direction and thrust	
  - **S**: Turn Left
  - **F**: Turn Right
  - **E**: Thrust Forward
- **Shooting:**
  - **Space [XBOX A Button]**: Fire Bullet
- **Respawning:**
  - **N**: Respawn Player Ship

Debugging
- **F1**: Toggle Debug Drawing for all entities

Asteroid Management
- **I**: Spawn a Random Asteroid

Game State Management
- **F8**: Hard Reset the Game
- **P**: Toggle Pause/Unpause the Game
- **T**: Toggle time scale between 0.1 and 1.0
- **O**: Unpause the Game for one frame of Update, then Pause Again
- **N/Space [Xbox Start]** enter the game
- **ESC [Xbox Back]** leave game, and again to quit
```

## How to run
Go to `PROJECT_NAME/Run/` and Run `PROJECT_NAME_Release_x64.exe`

## How to build
1. Clone Project
```bash
git clone --recurse-submodules https://github.com/cloud-sail/Starship.git
```
2. Open Solution `PROJECT_NAME.sln` file
- In Project Property Pages
  - Debugging->Command: `$(TargetFileName)`
  - Debugging->Working Directory: `$(SolutionDir)Run/`

