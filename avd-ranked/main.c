#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

// Define the struct
struct AVDGame {
    char current_player[6];    // "Alpha" or "Digits"
    char alpha_pieces[10];     // Available pieces for Alpha (A,B,C)
    char digits_pieces[10];    // Available pieces for Digits (1,2,3)
    int alpha_count;          // Number of Alpha pieces
    int digits_count;         // Number of Digits pieces
    char board[9];            // 3x3 game board
};

// Function prototypes
int is_full(struct AVDGame* game);
void display_board(struct AVDGame* game);
int make_move(struct AVDGame* game, int position, char piece);
int* check_win(struct AVDGame* game);
void strip_newline(char* str);
void show_instructions(void);

// The play function
void play(struct AVDGame* game) {
    char input[10];
    
    // Ask if user wants instructions
    printf("Welcome to Alpha vs Digits!\n");
    printf("Would you like to see the instructions? (y/n): ");
    fgets(input, 10, stdin);
    strip_newline(input);
    if (toupper(input[0]) == 'Y') {
        show_instructions();
    }
    
    while (!is_full(game)) {
        display_board(game);
        printf("%s's turn. Pick a piece: ", game->current_player);
        
        char* pieces = (strcmp(game->current_player, "Alpha") == 0) ? 
                       game->alpha_pieces : game->digits_pieces;
        int count = (strcmp(game->current_player, "Alpha") == 0) ? 
                    game->alpha_count : game->digits_count;
        for (int i = 0; i < count; i++) {
            printf("%c ", pieces[i]);
        }
        printf("\n");

        printf("Enter position (1-9) or 0 to quit: ");
        fgets(input, 10, stdin);
        strip_newline(input);
        int position = atoi(input);
        
        if (position == 0) {
            printf("Alpha vs Digits Quit. Thanks for playing!\n");
            printf("Special thanks to DJ, Leo, and Grok for making this game possible!\n");
            z80_delay_ms(1500);
            return;
        }

        printf("Enter piece: ");
        fgets(input, 10, stdin);
        strip_newline(input);
        char piece = toupper(input[0]);
        
        if (piece == '0') {
            printf("Alpha vs Digits Quit. Thanks for playing!\n");
            printf("Special thanks to DJ, Leo, and Grok for making this game possible!\n");
            z80_delay_ms(1500);
            return;
        }

        if (!make_move(game, position, piece)) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        int* win = check_win(game);
        if (win[0]) {
            display_board(game);
            printf("Alpha vs Digits Over: %s wins with [%d, %d, %d]!\n",
                   win[1] ? "Digits" : "Alpha",
                   win[2] + 1, win[3] + 1, win[4] + 1);
            printf("Special thanks to DJ, Leo, and Grok for making this game possible!\n");
            return;
        }

        strcpy(game->current_player, 
               strcmp(game->current_player, "Alpha") == 0 ? "Digits" : "Alpha");
    }
    
    display_board(game);
    printf("Alpha vs Digits over! Draw.\n");
    printf("Special thanks to DJ, Leo, and Grok for making this game possible!\n");
    z80_delay_ms(1500);
}

// Supporting functions
int is_full(struct AVDGame* game) {
    for (int i = 0; i < 9; i++) {
        if (game->board[i] == ' ') return 0;  // Not full if any space is empty
    }
    return 1;  // Full if no spaces
}

void display_board(struct AVDGame* game) {
    printf("\n");
    for (int i = 0; i < 9; i += 3) {
        printf(" %c | %c | %c \n", 
               game->board[i], game->board[i+1], game->board[i+2]);
        if (i < 6) printf("---+---+---\n");
    }
    printf("\n");
}

int make_move(struct AVDGame* game, int position, char piece) {
    if (position < 1 || position > 9) {
        return 0;  // Invalid position
    }
    
    int pos = position - 1;  // Convert to 0-based index
    char current = game->board[pos];
    
    // Check if piece is valid for current player
    char* pieces = (strcmp(game->current_player, "Alpha") == 0) ? 
                   game->alpha_pieces : game->digits_pieces;  // FIXED HERE
    int count = (strcmp(game->current_player, "Alpha") == 0) ? 
                game->alpha_count : game->digits_count;
    int valid_piece = 0;
    for (int i = 0; i < count; i++) {
        if (pieces[i] == piece) {
            valid_piece = 1;
            break;
        }
    }
    if (!valid_piece) {
        return 0;  // Invalid piece
    }

    // Check ranking rules
    if (current == ' ') {  // Empty space, any piece can go
        game->board[pos] = piece;
        return 1;
    }
    // Overwrite rules
    if (piece == '1' && (current == 'B' || current == 'C')) {
        game->board[pos] = piece;
        return 1;
    }
    if (piece == 'A' && (current == '2' || current == '3')) {
        game->board[pos] = piece;
        return 1;
    }
    if (piece == '2' && current == 'C') {
        game->board[pos] = piece;
        return 1;
    }
    if (piece == 'B' && current == '3') {
        game->board[pos] = piece;
        return 1;
    }
    
    return 0;  // Can't overwrite
}

int* check_win(struct AVDGame* game) {
    static int result[5] = {0, 0, 0, 0, 0};  // [win?, digits?, pos1, pos2, pos3]
    
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},  // Rows
        {0,3,6}, {1,4,7}, {2,5,8},  // Columns
        {0,4,8}, {2,4,6}            // Diagonals
    };
    
    for (int i = 0; i < 8; i++) {
        char p1 = game->board[wins[i][0]];
        char p2 = game->board[wins[i][1]];
        char p3 = game->board[wins[i][2]];
        
        // Check if all three are Alpha (A, B, C)
        if ((p1 == 'A' || p1 == 'B' || p1 == 'C') &&
            (p2 == 'A' || p2 == 'B' || p2 == 'C') &&
            (p3 == 'A' || p3 == 'B' || p3 == 'C') &&
            p1 != ' ' && p2 != ' ' && p3 != ' ') {
            result[0] = 1;  // Win found
            result[1] = 0;  // Alpha win
            result[2] = wins[i][0];
            result[3] = wins[i][1];
            result[4] = wins[i][2];
            return result;
        }
        // Check if all three are Digits (1, 2, 3)
        if ((p1 == '1' || p1 == '2' || p1 == '3') &&
            (p2 == '1' || p2 == '2' || p2 == '3') &&
            (p3 == '1' || p3 == '2' || p3 == '3') &&
            p1 != ' ' && p2 != ' ' && p3 != ' ') {
            result[0] = 1;  // Win found
            result[1] = 1;  // Digits win
            result[2] = wins[i][0];
            result[3] = wins[i][1];
            result[4] = wins[i][2];
            return result;
        }
    }
    return result;
}

void strip_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void show_instructions(void) {
    printf("\n=== Alpha vs Digits Instructions ===\n");
    printf("Alpha vs Digits is a strategic tic-tac-toe variant played on a 3x3 board.\n");
    printf("Two players, Alpha and Digits, take turns placing pieces.\n\n");
    printf("Pieces and Ranks:\n");
    printf("- Alpha pieces: A (highest), B (middle), C (lowest)\n");
    printf("- Digits pieces: 1 (highest), 2 (middle), 3 (lowest)\n");
    cprintf("Press enter to continue.\r\n");
    cgetc();  // Wait for any key press before exiting
    printf("Overwrite Rules:\n");
    printf("- 1 can overwrite B or C\n");
    printf("- A can overwrite 2 or 3\n");
    printf("- 2 can overwrite C\n");
    printf("- B can overwrite 3\n");
    printf("- Empty spaces ( ) can take any piece\n\n");
    cprintf("Press enter to continue.\r\n");
    cgetc();  // Wait for any key press before exiting
    printf("How to Play:\n");
    printf("- Enter a position (1-9) to place a piece, or 0 to quit\n");
    printf("- Positions are numbered like this:\n");
    printf(" 1 | 2 | 3 \n");
    printf("---+---+---\n");
    printf(" 4 | 5 | 6 \n");
    printf("---+---+---\n");
    printf(" 7 | 8 | 9 \n");
    printf("- Then enter a piece from your available set\n\n");
    cprintf("Press enter to continue.\r\n");
    cgetc();  // Wait for any key press before exiting 
    printf("Winning:\n");
    printf("- Win by getting three Alpha pieces (A, B, or C) or three Digits pieces\n");
    printf("  (1, 2, or 3) in a row, column, or diagonal\n");
    printf("- The game ends in a draw if the board fills without a win\n");
    printf("====================================\n\n");
}

// Main function to start the game
int main() {
    struct AVDGame game;
    strcpy(game.current_player, "Alpha");
    strcpy(game.alpha_pieces, "ABBCCCC");  // A,B,B,C,C,C,C
    strcpy(game.digits_pieces, "1223333"); // 1,2,2,3,3,3,3
    game.alpha_count = 7;
    game.digits_count = 7;
    for (int i = 0; i < 9; i++) {
        game.board[i] = ' ';
    }
    
    play(&game);
    return 0;
}