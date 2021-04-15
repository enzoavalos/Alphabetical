# Aplhabetical
### Quiz game where players must answer a set of questions based on the letters of the alphabet!

## Topics involved:
- Binary search trees
- Circular linked lists
- Pointers
- File handling
- Struct implementation

## Steps to get started:
**Prerequisites:** It will be needed for you to download the files present in the repo and save them all in the same directory.

**Windows:** An .exe file is included in the repo so you can run the program directly, you are welcome :)  
**Linux & Mac:** If you are using Linux or Mac, the .exe file won´t work for you, and a few extra steps will be needed:
- To compile the program you must download either an IDE or a (surprise) compiler:
  - I recommend [code::blocks](https://www.codeblocks.org/) as it is a complete and lightweigh IDE which will serve you just fine, but feel free to use any other ([VS Code](https://code.visualstudio.com/) or [Xcode](https://developer.apple.com/xcode/) for Mac are also good ideas).


## Actions in the app:
### The application allow users to do the following set of actions:
- Add players, as long as they aren´t already registered
- See all players registered
- See the 10 players with the highest scores
- Play a match between 2 players
- Delete players

## Game rules:
- Each player has a set of 26 questions, one for each letter in the alphabet, related with definitions of words found in the dictionary.
- The players take turns to answer
- If the current player answers the question correctly, he continues playing. Contrary case, if the player answers incorrectly or skips the question, the turn is then passed onto the other player
- While on the match, enter **"skp"** to skip any question you don´t know, which will remain pending and will be asked again when all other letters are finished.
- The game ends when one of the players has answwered all of his questions, without taking into account the state of the other player (if he still has pending questions).

## Extra Notes:
- In the repo is included a file named **stored_questions** which contains all the questions you will need to play the game. Furthermore, a pdf file is included with all the questions and it´s answers, so that you can keep track of them.
