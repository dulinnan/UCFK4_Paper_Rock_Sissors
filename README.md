# ![](/logo.png) ENCE260 Assignment

A fun game programmed running on AVR ATmega32 micro-controller.

**Developers**: Luke McIntyre (lmc211) and Linnan Du (ldu32)

**Publish Date**: October 2016


## Setting up

1. Get ready with the AVR dependencies:
```
 sudo apt-get install git gcc-avr binutils-avr avr-libc dfu-programmer
```

2. Clone the UCFK4 git repository: This contains all the necessary drivers and utilities to run the battleships application
```
 git clone https://eng-git.canterbury.ac.nz/steve.weddell/ence260-ucfk4.git
```

3. Clone the our little game: It needs to be cloned into the assignment directory in the ucfk4 folder
```
 cd ence260-ucfk4/assignment
 git clone https://github.com/dulinnan/UCFK4_Paper_Rock_Sissors.git
 cd UCFK4_Paper_Rock_Sissors
```


## File Structure
  - `game.c`: Contains the main game task scheduling, logic and game phase tracking
  - `button.c`, `button.h`: Contain all routines related to board manipulation, game opinion selection
  - `display_symbol.c`, `display_symbol.h`: Contains display routines
  - `Makefile`: Contains the basic Makefile for game.c


## Usage

From within this directory, the following commands can be run

  - `make`: Compiles source code and builds object files
  - `make program`: Loads program into UCFK4 flash memory
  - `make clean`: Removes old object files from directory

Run `make` and then `make program` to start playing!


## Game Play
**Selecting the object**: There will be a question mark on the loading screen, which indicates the players to choose thier object. Use the navswitch to select the object:
- Nav left: Rock;
- Nav right: Paper;
- Nav down: Sissor;

**Starting**: Once both players have chosen the item, make sure the boards are within IR range, and players should push the nav button to compare the chosen items.

**Result**: After both players have confirmed their choice, the screen will firstly show the winner who will get a smiley face emoji, and the loser will get a unhappy face emoji. And then the object the local player chose will be shown on the screen, followed by the other player's object.

**End Of Game**: Press the R13 button to start the game again!
