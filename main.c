#include "head.h"

void freeTree(treeNode* node) {
    if (!node) return;
    treeNodeListCell* curr = node->next_possible_positions;
    while (curr) {
        freeTree(curr->node);
        treeNodeListCell* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(node);
}

void printMenu() {
    printf("\nMain Menu:\n");
    printf("1. Enter a knight's starting position\n");
    printf("2. Create all possible knight paths\n");
    printf("3. Find a knight path covering all board\n");
    printf("4. Save knight path covering all board to file and check\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void getStartingPosition(boardPos* start) {
    char input[10];
    while (1) {
        printf("Enter starting position (e.g., A1): ");
        if (fgets(input, sizeof(input), stdin)) {
            // Remove newline and validate length
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }
            if (strlen(input) < 2) {
                printf("Invalid input! ");
                continue;
            }

            // Manual ASCII checks
            char first = input[0];
            char second = input[1];

            // Convert to uppercase
            if (first >= 'a' && first <= 'z') {
                first -= 32; // Convert lowercase to uppercase
            }

            // Validate characters
            if (!((first >= 'A' && first <= 'Z') &&
                (second >= '0' && second <= '9'))) {
                printf("Invalid format! ");
                continue;
            }

            // Validate position
            int row = first - 'A';
            int col = second - '0' - 1; // Convert '1' to 0-based index

            if (row >= 0 && row < N && col >= 0 && col < M) {
                (*start)[0] = first;
                (*start)[1] = second;
                return;
            }
        }
        printf("Invalid position! ");
    }
}

void initializeBoard(char*** board) {
    *board = (char**)malloc(N * sizeof(char*));
    for (int i = 0; i < N; i++) {
        (*board)[i] = (char*)malloc(M * sizeof(char));
        for (int j = 0; j < M; j++) {
            (*board)[i][j] = ' ';
        }
    }
}

int main() {
    int choice;
    char filename[100];
    boardPos start = { 0 };
    pathTree tree = { 0 };
    boardPosList* fullPath = NULL;
    int initialized = 0; // 0 - nothing initialized, 1 - position set, 2 - tree built, 3 - path found

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Invalid input!\n");
            continue;
        }
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
        case 1: {
            getStartingPosition(&start);
            printf("Starting position set to: %c%c\n", start[0], start[1]);
            initialized = 1;
            break;
        }

        case 2: {
            if (initialized < 1) {
                printf("Error: Please set starting position first!\n");
                break;
            }

            // Initialize empty board
            char** board;
            initializeBoard(&board);

            // Build the tree
            tree = findAllPossiblePaths(&start, board);
            initialized = 2;
            printf("All possible paths tree built!\n");

            // Cleanup
            freeBoard(board);
            break;
        }

        case 3: {
            if (initialized < 1) {
                printf("Error: Please set starting position first!\n");
                break;
            }

            // Build tree if not already built
            if (initialized < 2) {
                printf("Building paths tree first...\n");
                char** board;
                initializeBoard(&board);
                tree = findAllPossiblePaths(&start, board);
                freeBoard(board);
                initialized = 2;
            }

            // Find full path
            char** board;
            initializeBoard(&board);

            fullPath = findKnightPathCoveringAllBoard(&start, board);
            if (fullPath) {
                printf("Full coverage path found!\n");
                initialized = 3;
            }
            else {
                printf("No path covering all board found!\n");
            }
            freeBoard(board);
            break;
        }

        case 4: {
            if (!fullPath) {
                printf("No full coverage path available. Try finding one first!\n");
                break;
            }

            printf("Enter filename to save: ");
            if (fgets(filename, sizeof(filename), stdin)) {
                size_t len = strlen(filename);
                if (len > 0 && filename[len - 1] == '\n') {
                    filename[len - 1] = '\0'; // Remove newline
                }
            }

            saveListToBinFile(filename, fullPath);
            printf("Path saved to %s\n", filename);

            int result = checkAndDisplayPathFromFile(filename);
            switch (result) {
            case -1: printf("Error: File not found!\n"); break;
            case 1: printf("Invalid knight path!\n"); break;
            case 2: printf("Valid path covering entire board!\n"); break;
            case 3: printf("Valid path but doesn't cover entire board!\n"); break;
            default: printf("Unknown result!\n");
            }
            break;
        }

        case 5: {
            // Cleanup resources
            if (initialized >= 2) freeTree(tree.head);
            if (initialized >= 3) freeBoardPosList(fullPath);
            printf("Exiting...\n");
            return 0;
        }

        default: {
            printf("Invalid choice! Please enter 1-5.\n");
        }
        }
    }
    return 0;
}

