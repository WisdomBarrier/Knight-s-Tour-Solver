#ifndef HEAD_H
#define HEAD_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>

#define N 4 
#define M 4

typedef char boardPos[2];

typedef struct _move {
    char rows, cols;
} Move;

typedef struct _movesArray {
    unsigned int size;
    Move* moves;
} movesArray;

typedef struct _boardPosArray {
    unsigned int size;
    boardPos* positions;
} boardPosArray;

typedef struct _moveCell {
    Move move;
    struct _moveCell* next, * prev;
} moveCell;

typedef struct _movesList {
    moveCell* head;
    moveCell* tail;
} movesList;

typedef struct _treeNodeListCell {
    struct _treeNode* node;
    struct _treeNodeListCell* next;
} treeNodeListCell;

typedef struct _treeNode {
    boardPos position;
    treeNodeListCell* next_possible_positions;
} treeNode;

typedef struct _pathTree {
    treeNode* head;
} pathTree;

typedef struct _boardPosCell {
    boardPos position;
    struct _boardPosCell* next;
} boardPosCell;

typedef boardPosCell boardPosList;

boardPosArray** validMoves(movesArray** moves, char** board);
int display(movesList* moves_list, boardPos start, char** board);
bool isPositionVisited(boardPos position, char** visited);
void markPositionVisited(boardPos position, char** visited);
void unmarkPositionVisited(boardPos position, char** visited);
treeNode* buildTree(boardPos position, char** board, char** visited, boardPosArray** validMovesArray);
void freeValidMovesArray(boardPosArray** validMovesArray);
pathTree findAllPossiblePaths(boardPos* startingPosition, char** board);
boardPosCell* createNewCell(boardPos position, boardPosCell* next);
bool findPathRec(boardPos position, char** board, char** visited, boardPosCell** path, int cellsCovered);
boardPosList* findKnightPathCoveringAllBoard(boardPos* startingPosition, char** board);
void freeMovesList(movesList* list);
void freeBoardPosList(boardPosList* list);
void freeBoard(char** board);
void saveListToBinFile(char* file_name, boardPosList* pos_list);
int checkAndDisplayPathFromFile(char* file_name);
#pragma once
#endif #pragma once
