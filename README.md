# Knight-s-Tour-Solver

*Overview*

This project implements an algorithm to find and validate knight's move paths on an N x M chessboard (default: 4x4). It allows:

@ Setting a starting position for the knight.

@ Generating a tree of all possible knight moves.

@ Finding a path that covers the entire board.

@ Saving the path to a binary file and validating it.


*Features*

Recursive backtracking to find a full board coverage path.

Tree-based search for all possible knight paths.

Dynamic memory management using linked lists and trees.

Binary file handling for saving and loading knight paths.
                                     
*Files*

main.c – Handles user interaction and menu logic.

func7.c – Implements knight's pathfinding algorithms.

head.h – Contains type definitions and function prototypes.


*Important Functions*


findAllPossiblePaths() – Generates a tree of all possible knight moves.

findKnightPathCoveringAllBoard() – Searches for a knight path that covers the entire board.

saveListToBinFile() – Saves a knight path to a binary file.

checkAndDisplayPathFromFile() – Validates and loads a path from a binary file.





