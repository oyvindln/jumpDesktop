## jumpDesktop

jumpDesktop is a simple multiplayer platform game where the objective is to score points by killing the other players. Each player has a yinyang meter, when two players intersect, the player with the most centered yinyang-meter will kill the other player. The value in the meter is increased when standing still on a platform, and decreases when the player is in the air.

The game was originally made for a school project a number of years back, but has been ported to Qt5.

## Controls
The player is controlled by clicking on in the game window, which makes the player jump in the direction of the click.

## Hosting and joining a game
A game is hosted by using the host option in the main menu. Select a map (not selecting a map will give a blank map.), player name and port, and click host.

Players can join the game by using the join game option in the main menu, choosing a name and entering the IP of the host.

Once players have joined, the host can start the game by pressing ```Start game```
## How to build

### Dependencies
The game is written in C++, and depends on Qt (5.xx). A recent compiler is also needed as some C++11 functionality is used.

### Building
Building should be as simple as doing:
```
qmake && make
```
in the main directory.

The ```images``` and ```Levels``` folders must be in the same directory as the binary for the game to work properly.

## Other notes
- The game is multiplayer only, there is no AI, the single player menu option only opens a 1-player test area.

- The game probably doesn't reset properly after finishing a match, so it should be restarded for further matches.

- The game was formerly named NinjaJump, but a different name is used here as there is already a different game by that name.
