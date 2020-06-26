# maze
A maze game contains special tiles, walls and portals.

1. Compile the game by running make in the terminal. If it is not installed, install it using sudo apt
install make.

2. Run the game in the terminal (./maze <filename>). 

* @ - player
* $ - target/s
* The maze can contain more than one target tile, and to win the player needs to visit all of them (order is not important). 
* tile types: 
1. Vertical (|) and horizontal (-) walls. A vertical wall tile cannot be entered from left/right, only from top/bottom. The other way around for horizontal walls.
2. A trap tile looks like a floor tile, but when the player enters it, the game ends. 
3. A secret wall tile looks like a regular wall tile, but the player can enter it. 
4. Keys (&) and doors (+). When the player enters a key tile, she picks it up. Player without keys cannot enter a door tile.
