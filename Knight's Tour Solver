#include "head.h"

// Check if the position is within the board limits, not an obstacle and L shape
boardPosArray** validMoves(movesArray** moves, char** board) {
    // Allocate validMovesArray
    boardPosArray** validMovesArray = (boardPosArray**)malloc(N * sizeof(boardPosArray*));
    for (int i = 0; i < N; i++) {
        validMovesArray[i] = (boardPosArray*)malloc(M * sizeof(boardPosArray));
        for (int j = 0; j < M; j++) {
            validMovesArray[i][j].size = 0;
            validMovesArray[i][j].positions = NULL;

            // Skip obstacles and clear moves
            if (board[i][j] == '*') {
                free(moves[i][j].moves);
                moves[i][j].size = 0;
                moves[i][j].moves = NULL;
                continue;
            }

            // Temporary storage for valid moves
            Move* tempMoves = (Move*)malloc(moves[i][j].size * sizeof(Move));
            int validCount = 0;

            // Process input moves
            for (int k = 0; k < moves[i][j].size; k++) {
                Move move = moves[i][j].moves[k];
                int newRow = i + move.rows;
                int newCol = j + move.cols;

                // Validate move
                if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < M && board[newRow][newCol] != '*') {
                    // Keep the move
                    tempMoves[validCount] = move;
                    validCount++;

                    // Add to validMovesArray
                    validMovesArray[i][j].size++;
                    validMovesArray[i][j].positions = (boardPos*)realloc(
                        validMovesArray[i][j].positions,
                        validMovesArray[i][j].size * sizeof(boardPos)
                    );
                    validMovesArray[i][j].positions[validMovesArray[i][j].size - 1][0] = 'A' + newRow;
                    validMovesArray[i][j].positions[validMovesArray[i][j].size - 1][1] = '1' + newCol;
                }
            }

            // Update moves array
            free(moves[i][j].moves);
            moves[i][j].size = validCount;
            moves[i][j].moves = (Move*)malloc(validCount * sizeof(Move));
            memcpy(moves[i][j].moves, tempMoves, validCount * sizeof(Move));
            free(tempMoves);
        }
    }

    return validMovesArray;
}

// Display moves from a given starting position, removes cells with invalid moves (visited or has obsticles) from the moves list and returns number or moves removed
int display(movesList* moves_list, boardPos start, char** board) {
    // Initialize variables
    int removedMoves = 0;
    bool visited[N][M] = { false };  // Track visited cells
    int row = start[0] - 'A';      // Convert board position to indices
    int col = start[1] - '1';

    // Mark the start position as visited
    visited[row][col] = true;

    // Create a copy of the board to modify for display
    char displayBoard[N][M];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (board[i][j] == '*') {
                displayBoard[i][j] = '*';  // Preserve obstacles
            }
            else {
                displayBoard[i][j] = ' ';  // Initialize unvisited cells as empty
            }
        }
    }

    // Place the starting marker
    displayBoard[row][col] = '#';

    moveCell* current = moves_list->head;
    int moveNumber = 1;

    // Possible knight moves (L-shape)
    int knightMoves[8][2] = {
        { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 },
        { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 }
    };

    while (current) {
        // Calculate the new position based on the knight move
        int newRow = row + current->move.rows;
        int newCol = col + current->move.cols;
        // Check if the move is valid: within bounds, not visited, and not a '*'
        bool validMove = false;
        for (int i = 0; i < 8; i++) {
            if (current->move.rows == knightMoves[i][0] && current->move.cols == knightMoves[i][1]) {
                validMove = true;
                break;
            }
        }
        if (newRow < 0 || newRow >= N || newCol < 0 || newCol >= M ||
            board[newRow][newCol] == '*' || visited[newRow][newCol] || !validMove) {
            // Invalid move: remove it from the list
            moveCell* toDelete = current;
            current = current->next;

            // Remove from the list
            if (toDelete->prev) toDelete->prev->next = toDelete->next;
            if (toDelete->next) toDelete->next->prev = toDelete->prev;
            if (toDelete == moves_list->head) moves_list->head = toDelete->next;
            if (toDelete == moves_list->tail) moves_list->tail = toDelete->prev;

            free(toDelete);
            removedMoves++;
        }
        else {
            // Valid move: update position and mark as visited
            row = newRow;
            col = newCol;
            visited[row][col] = true;

            // Update the display board
            displayBoard[row][col] = '0' + moveNumber++;
            current = current->next;
        }
    }

    // Print the game board
    printf("Game Board:\n");

    // Print column headers (with consistent width)
    printf("     ");  // Extra space before column numbers for alignment
    for (int j = 0; j < M; j++) {
        printf("%-5d", j + 1);  
    }
    printf("\n");

    // Print grid lines and row labels
    for (int i = 0; i < N; i++) {
       
        printf("%-3c", 'A' + i);

        // Print each cell in the row with consistent spacing
        for (int j = 0; j < M; j++) {
            if (displayBoard[i][j] == '*') {
                printf("[ * ]");
            }
            else {
                printf("[ %c ]", displayBoard[i][j]);
            }
        }
        printf("\n");
    }

    // Return the number of removed moves
    return removedMoves;
}

// Check if a position is already visited
bool isPositionVisited(boardPos position, char** visited) {
    return visited[position[0] - 'A'][position[1] - '1'] == 1;
}

// Mark a position as visited
void markPositionVisited(boardPos position, char** visited) {
    visited[position[0] - 'A'][position[1] - '1'] = 1;
}

// Unmark a position as visited
void unmarkPositionVisited(boardPos position, char** visited) {
    visited[position[0] - 'A'][position[1] - '1'] = 0;
}

// Build a tree of possible knight moves
treeNode* buildTree(boardPos position, char** board, char** visited, boardPosArray** validMovesArray) {
    // Track row/col conversion
    int row = position[0] - 'A';
    int col = position[1] - '1';

    // Check if position is valid
    if (row < 0 || row >= N || col < 0 || col >= M) {
        return NULL;
    }

    // Create the current tree node
    treeNode* currentNode = (treeNode*)malloc(sizeof(treeNode));
    currentNode->position[0] = position[0];
    currentNode->position[1] = position[1];
    currentNode->next_possible_positions = NULL;

    // Mark as visited
    markPositionVisited(position, visited);

    // Get valid moves for the current position
    boardPosArray* movesArray = &validMovesArray[row][col];

    treeNodeListCell* lastCell = NULL;
    for (int i = 0; i < movesArray->size; i++) {
        boardPos nextPos = { movesArray->positions[i][0], movesArray->positions[i][1] };
        // Check if the move is blocked or already visited
        int nextRow = nextPos[0] - 'A';
        int nextCol = nextPos[1] - '1';
        if (nextRow < 0 || nextRow >= N || nextCol < 0 || nextCol >= M) {
            continue;
        }
        if (isPositionVisited(nextPos, visited)) {
            continue;
        }
        if (board[nextRow][nextCol] == '*') {
            continue;
        }

        // Recursively build subtree
        treeNode* childNode = buildTree(nextPos, board, visited, validMovesArray);

        if (childNode == NULL) {
            continue;
        }

        // Link child to current node
        treeNodeListCell* newCell = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
        newCell->node = childNode;
        newCell->next = NULL;

        if (currentNode->next_possible_positions == NULL) {
            currentNode->next_possible_positions = newCell;
        }
        else {
            lastCell->next = newCell;
        }
        lastCell = newCell;
    }

    // Unmark before backtracking
    unmarkPositionVisited(position, visited);
    return currentNode;
}

void freeValidMovesArray(boardPosArray** validMovesArray) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(validMovesArray[i][j].positions);
        }
        free(validMovesArray[i]);
    }
    free(validMovesArray);
}

// Builds a tree for all the possible knight paths from a given starting position
pathTree findAllPossiblePaths(boardPos* startingPosition, char** board) {
    // Initialize the tree structure
    pathTree result;

    // Create the visited matrix (same size as board)
    char** visited = (char**)malloc(N * sizeof(char*));
    for (int i = 0; i < N; i++) {
        visited[i] = (char*)calloc(M, sizeof(char));
    }

    movesArray** moves = (movesArray**)malloc(N * sizeof(movesArray*));
    for (int i = 0; i < N; i++) {
        moves[i] = (movesArray*)malloc(M * sizeof(movesArray));
    }

    // Initialize the moves array for every position on the board
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            moves[i][j].size = 8;
            moves[i][j].moves = (Move*)malloc(8 * sizeof(Move));
            moves[i][j].moves[0] = (Move){ 2, 1 };
            moves[i][j].moves[1] = (Move){ 2, -1 };
            moves[i][j].moves[2] = (Move){ -2, 1 };
            moves[i][j].moves[3] = (Move){ -2, -1 };
            moves[i][j].moves[4] = (Move){ 1, 2 };
            moves[i][j].moves[5] = (Move){ 1, -2 };
            moves[i][j].moves[6] = (Move){ -1, 2 };
            moves[i][j].moves[7] = (Move){ -1, -2 };
        }
    }

    // Load valid moves
    boardPosArray** validMovesArray = validMoves(moves, board);
    // Build the tree starting from the given position
    result.head = buildTree(*startingPosition, board, visited, validMovesArray);
    // Free the visited matrix
    for (int i = 0; i < N; i++) {
        free(visited[i]);
    }
    free(visited);

    // Free the moves array
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(moves[i][j].moves);
        }
        free(moves[i]);
    }
    free(moves);
    freeValidMovesArray(validMovesArray);

    return result;
}

// Create a new cell in the linked list
boardPosCell* createNewCell(boardPos position, boardPosCell* next) {
    boardPosCell* newCell = (boardPosCell*)malloc(sizeof(boardPosCell));
    newCell->position[0] = position[0];
    newCell->position[1] = position[1];
    newCell->next = next;
    return newCell;
}

// Recursive backtracking function
bool findPathRec(boardPos position, char** board, char** visited, boardPosCell** path, int cellsCovered) {
    int row = position[0] - 'A';
    int col = position[1] - '1';

    // Mark the current position as visited
    markPositionVisited(position, visited);

    // Add the position to the path
    *path = createNewCell(position, *path);

    // If we have covered all cells, return true
    if (cellsCovered == N * M) {
        return true;
    }

    // Get valid moves from the current position
    Move knightMoves[8] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (int i = 0; i < 8; i++) {
        int newRow = row + knightMoves[i].rows;
        int newCol = col + knightMoves[i].cols;

        // Check if the new position is within the board and not visited
        if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < M &&
            board[newRow][newCol] != '*' && !visited[newRow][newCol]) {

            boardPos nextPos = { 'A' + newRow, '1' + newCol };

            // Recursively try to find a path from the new position
            if (findPathRec(nextPos, board, visited, path, cellsCovered + 1)) {
                return true;
            }
        }
    }

    // Backtrack: remove the position from the path and unmark it
    boardPosCell* temp = *path;
    *path = (*path)->next;
    free(temp);
    unmarkPositionVisited(position, visited);

    return false;
}

// function to find the knight's path covering all board
boardPosList* findKnightPathCoveringAllBoard(boardPos* startingPosition, char** board) {
    // Create the visited matrix (same size as board)
    char** visited = (char**)malloc(N * sizeof(char*));
    for (int i = 0; i < N; i++) {
        visited[i] = (char*)calloc(M, sizeof(char));
    }

    boardPosList* path = NULL;

    // Start the recursive backtracking
    if (!findPathRec(*startingPosition, board, visited, &path, 1)) {
        // No path found, free memory and return NULL
        for (int i = 0; i < N; i++) {
            free(visited[i]);
        }
        free(visited);
        return NULL;
    }

    // Free the visited matrix
    for (int i = 0; i < N; i++) {
        free(visited[i]);
    }
    free(visited);

    return path;
}

// Save a list of positions to a binary file
void saveListToBinFile(char* file_name, boardPosList* pos_list) {
    FILE* file = fopen(file_name, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    int count = 0;
    boardPosCell* curr = pos_list;
    while (curr) { // Count elements
        count++;
        curr = curr->next;
    }
    fwrite(&count, sizeof(int), 1, file); // Write count to file

    unsigned char buffer = 0;
    int bit_count = 0;
    curr = pos_list;

    while (curr) {
        unsigned char row = curr->position[0] - 'A'; // Convert row
        unsigned char col = curr->position[1] - '1'; // Convert column
        unsigned char packed = ((row & 0x07) << 3) | (col & 0x07); // Pack into 6 bits

        for (int i = 5; i >= 0; i--) { // Write 6 bits
            buffer = (buffer << 1) | ((packed >> i) & 1);
            bit_count++;
            if (bit_count == 8) { // Write full byte
                fwrite(&buffer, sizeof(unsigned char), 1, file);
                buffer = 0;
                bit_count = 0;
            }
        }
        curr = curr->next;
    }

    if (bit_count > 0) { // Write remaining bits (padded with zeros)
        buffer <<= (8 - bit_count);
        fwrite(&buffer, sizeof(unsigned char), 1, file);
    }

    fclose(file);
}

void freeMovesList(movesList* list) {
    if (list == NULL) return;

    moveCell* current = list->head;
    while (current != NULL) {
        moveCell* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void freeBoardPosList(boardPosList* list) {
    while (list != NULL) {
        boardPosList* temp = list;
        list = list->next;
        free(temp);
    }
}

void freeBoard(char** board) {

    if (board == NULL) return;

    for (int i = 0; i < N; i++) {
        free(board[i]);
    }

    free(board);
}

int checkAndDisplayPathFromFile(char* file_name) {
    FILE* file = fopen(file_name, "rb");
    if (!file) return -1;

    // Read the number of positions
    int count;
    fread(&count, sizeof(int), 1, file);

    // Read positions from the file into a linked list 
    boardPosList* pos_list = NULL;
    boardPosList* tail = NULL;
    unsigned char buffer = 0;
    int bit_count = 0;

    for (int i = 0; i < count; i++) {
        unsigned char packed = 0;
        for (int bit = 0; bit < 6; bit++) {
            if (bit_count == 0) {
                fread(&buffer, sizeof(unsigned char), 1, file);
                bit_count = 8;
            }
            packed = (packed << 1) | ((buffer >> (bit_count - 1)) & 1);
            bit_count--;
        }

        unsigned char row = (packed >> 3) & 0x07;
        unsigned char col = packed & 0x07;
        boardPos pos = { 'A' + row, '1' + col };

        boardPosList* new_node = (boardPosList*)malloc(sizeof(boardPosList));
        new_node->position[0] = pos[0];
        new_node->position[1] = pos[1];
        new_node->next = NULL;

        if (!pos_list) pos_list = tail = new_node;
        else tail->next = new_node, tail = new_node;
    }
    fclose(file);

    // Validate the path and remove revisits
    bool visited[N][M] = { false };
    boardPosList* cleaned_list = NULL;
    boardPosList* cleaned_tail = NULL;
    boardPosList* curr = pos_list;
    int cells_covered = 0;
    int invalid_moves = 0;

    // Check if the path is a valid knight's path
    boardPos prev_pos;
    if (curr) {
        prev_pos[0] = curr->position[0];
        prev_pos[1] = curr->position[1];
        int row = prev_pos[0] - 'A';
        int col = prev_pos[1] - '1';
        if (row < 0 || row >= N || col < 0 || col >= M) invalid_moves++;
        else {
            visited[row][col] = true;
            cells_covered++;
            // Add to cleaned list
            boardPosList* node = (boardPosList*)malloc(sizeof(boardPosList));
            node->position[0] = prev_pos[0];
            node->position[1] = prev_pos[1];
            node->next = NULL;
            cleaned_list = cleaned_tail = node;
        }
        curr = curr->next;
    }

    while (curr) {
        int prev_row = prev_pos[0] - 'A';
        int prev_col = prev_pos[1] - '1';
        int curr_row = curr->position[0] - 'A';
        int curr_col = curr->position[1] - '1';

        // Check knight move validity (L-shape)
        int dr = abs(curr_row - prev_row);
        int dc = abs(curr_col - prev_col);
        if (!((dr == 1 && dc == 2) || (dr == 2 && dc == 1))) invalid_moves++;

        // Check bounds and obstacles
        if (curr_row < 0 || curr_row >= N || curr_col < 0 || curr_col >= M) invalid_moves++;
        else if (visited[curr_row][curr_col]) {
            // Skip revisit; do not add to cleaned list
            curr = curr->next;
            continue;
        }
        else {
            visited[curr_row][curr_col] = true;
            cells_covered++;
        }

        // Add to cleaned list
        boardPosList* node = (boardPosList*)malloc(sizeof(boardPosList));
        node->position[0] = curr->position[0];
        node->position[1] = curr->position[1];
        node->next = NULL;
        if (!cleaned_list) cleaned_list = cleaned_tail = node;
        else cleaned_tail->next = node, cleaned_tail = node;

        prev_pos[0] = curr->position[0];
        prev_pos[1] = curr->position[1];
        curr = curr->next;
    }

    // Return 1 if invalid moves or original path is invalid
    if (invalid_moves > 0) {
        freeBoardPosList(pos_list);
        freeBoardPosList(cleaned_list);
        return 1;
    }

    // Convert cleaned path to movesList for display
    movesList* moves = NULL;
    if (cleaned_list) {
        moves = (movesList*)malloc(sizeof(movesList));
        moves->head = moves->tail = NULL;
        boardPosList* curr = cleaned_list->next;
        boardPos prev = { cleaned_list->position[0], cleaned_list->position[1] };
        while (curr) {
            int dr = curr->position[0] - prev[0];
            int dc = curr->position[1] - prev[1];
            moveCell* mc = (moveCell*)malloc(sizeof(moveCell));
            mc->move.rows = dr;
            mc->move.cols = dc;
            mc->next = NULL;

            if (!moves->head) moves->head = moves->tail = mc;
            else moves->tail->next = mc, moves->tail = mc;

            prev[0] = curr->position[0];
            prev[1] = curr->position[1];
            curr = curr->next;
        }
    }

    // Display the cleaned path
    char** board = (char**)malloc(N * sizeof(char*));
    for (int i = 0; i < N; i++) {
        board[i] = (char*)malloc(M * sizeof(char));
        for (int j = 0; j < M; j++) {
            board[i][j] = ' ';
        }
    }
    int removed = display(moves, cleaned_list->position, board);
    freeMovesList(moves);
    freeBoard(board);

    // Check coverage
    bool full_coverage = (cells_covered == N * M);
    freeBoardPosList(pos_list);
    freeBoardPosList(cleaned_list);

    return full_coverage ? 2 : 3;
}

