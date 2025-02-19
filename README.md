#Spaghetti Western README
Spaghetti Western is a two-player cowboy duel game inspired by the arcade game Gun Fight from 1975. Each player controls one gunman and aims to kill the other player while collecting powerup items, dodging and blocking their opponent’s bullets, and navigating the environment.
The game is written in C++, using the standard library for basic data structures and memory management and used the external library raylib for other game-based logic and functionality. 

##Rules and Conditions  
Players start each round with one health. This can be increased to a maximum of two health and one armour by collecting and using items that spawn throughout the round. Players start with the default weapon being the revolver. It is a six-bullet, one-damage pistol, that can penetrate tumbleweed obstacles but not others. There are other weapons available.  
If players run out of ammo, they can replenish their weapon by collecting an ammo pickup or a rife pickup to get a new, different weapon. A player cannot move and shoot simultaneously, they must be stationary. However, a player can reload while moving. 
Each round the environment is random, with a different amount of distinct and total obstacles that are spawned in random locations. Obstacles differ in their health, dimensions, movement patterns and penetrability.   
Items spawn on a delay. A random item is spawned on either side of the level.  
The first player to four points wins, a point is scored when a player kills the other. After a kill, the round, weapons, items, and player attributes resets to default.  

##Controls
Controls are fixed
Player 1 Controls
- Move Up – W | 
- Move Left – A | 
- Move Right – D |
- Move Down  - S |
- Use Item – E |
- Fire Weapon – F | 
- Reload Weapon – R

Player 2 
- Move Up – Up Arrow |
- Move Left – Left Arrow | 
- Move Right – Right Arrow | 
- Move Down  - Down Arrow | 
- Use Item –  Backslash (/) | 
- Fire Weapon – Comma (,) | 
- Reload Weapon – Full Stop (.) | 
 
##Items and Weapons

###Weapon Descriptions
Revolver - The default weapon, 6 base ammo, 1 base damage, can penetrate tumbleweed obstacles
Rifle	- A weapon that can be picked up, 4 base ammo, 2 base damage, can penetrate and destroy all obstacles, slower fire rate, faster bullet speed than revolver

###Item	Descriptions
Health Item	- Increase health to a maximum of 2
Armour Item	- Increase armour to a maximum of 1, an instance of armour absorbs a single instance of damage, regardless of how much damage is actually done
Ammo Item	 - Refill the current weapon to maximum ammo
Strawman Item	- Create a 1 health straw gunman obstacle in front of the player with the same height and width of the player
Rifle Pickup - Change the current weapon to a rifle

##Setup
1. Download a .zip file of the repository 
2. Extract all files 
3. Open the spaghetti-western folder. This should only be folder necessary to play the game
4. Run the file gun_fight.exe
5. Play :D

##Credits and Other Notes
Project Manager: Raffaele Hughes  
Lead Developer: Raffaele Hughes  
Level Designer: Raffaele Hughes  
UI and HUD Designer: Raffaele Hughes  
System Architect and Designer: Raffaele Hughes  
Lead Artist : Raffaele Hughes  
Voice Acting: Raffaele Hughes  

Other sound effects were obtained from freesound.org. All art and animations are original, created using the software Aseprite
If you notice any bugs or wish there to be any features not included, please contact me :)   

A shortlist for potential features includes:
•	Sprint meter
•	New weapon: dynamite
•	Menu sounds
•	New level type: train level 
•	New firing style for revolver: fan the hammer 
•	Customisable controls
