#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Running till player dead
#include <stdbool.h>
bool operating = true;
bool gameWon = false;
bool Exit = false;
bool hasKey = false;
bool level_complete = false;

// ========================
// Variables
int currentLevel = 1;

// ========================
// ASCII CODE COLOR
#define RESET       "\033[0m"
#define BLUE        "\033[34m"
#define BROWN       "\033[38;5;94m"         // '.' moving blocks
#define RED         "\033[31m"              // # 
#define GREEN       "\033[32m"              // Win Stuff?

// ========================
// ASCII CODE COLOR CHARACTERS
#define CYAN        "\033[1;96m"            // P
#define YELLOW      "\033[1;93m"            // $
#define MAGENTA     "\033[1;95m"            // ^
#define ORANGE      "\033[1;38;5;214m"      // X
#define RED_BOLD    "\033[1;91m"            // E
#define GREEN_BOLD  "\033[1;92m"            // K

// ========================
// Black BACKGROUND
#define BLACK_BG "\u001b[40m"

typedef struct {
    int Player_pos[2];  
    int Enemy_pos[2];
    int Traps_[2];
    int Treasure_[2];
    int Exit[2];
    int Key_pos[2];
} Location;

typedef struct {
    int health;
    int score;
} Player;

void delay(int seconds) {
    Sleep(seconds * 1000);
}

void randomSpawning(Location *position, int r, int c) {
    // --- Enemy ---
    position->Enemy_pos[0] = rand() % (r - 2) + 1;
    position->Enemy_pos[1] = rand() % (c - 2) + 1;

    // --- Trap ---
    position->Traps_[0] = rand() % (r - 2) + 1;
    position->Traps_[1] = rand() % (c - 2) + 1;

    // --- Tresure ---
    position->Treasure_[0] = rand() % (r - 2) + 1;
    position->Treasure_[1] = rand() % (c - 2) + 1;

    // --- Exit --- // at the edge opisite to player but too lazy to change sorryy
    position->Exit[0] = r - 2;
    position->Exit[1] = c - 2;

    // --- Key ---
    position->Key_pos[0] = rand() % (r - 2) + 1;
    position->Key_pos[1] = rand() % (c - 2) + 1;
}

void drawLocation (Location *player, int r, int c) {
    char edge = '#';
    const char *move = ". ";
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            // POSITON ----------------------------------------------------
            if (i == player->Player_pos[0] && j == player->Player_pos[1]) {
                printf(BLACK_BG);
                printf(CYAN"\033[38;5;172m%c "RESET, 'P');
                printf(RESET);
            }
            else if (i == player->Enemy_pos[0] && j == player->Enemy_pos[1]) {
                printf(BLACK_BG);
                printf(RED_BOLD"%c "RESET, 'E');
                printf(RESET);
            }
            else if (i == player->Traps_[0] && j == player->Traps_[1]) {
                printf(BLACK_BG);
                printf(MAGENTA"%c "RESET, '^');
                printf(RESET);
            }
            else if (i == player->Treasure_[0] && j == player->Treasure_[1]) {
                printf(BLACK_BG);
                printf(YELLOW"%c "RESET, '$');
                printf(RESET);
            }
            else if (i == player->Key_pos[0] && j == player->Key_pos[1]) {
                printf(BLACK_BG);
                printf(GREEN_BOLD"%c "RESET, 'K');
                printf(RESET);
            }
            else if (i == player->Exit[0] && j == player->Exit[1]) {
                printf(BLACK_BG);
                printf(ORANGE"%c "RESET, 'X');
                printf(RESET);
            }

            // EDGE / MOVE ---------------------------------------------------------
            else if (i == 0 || i == r - 1 || j == 0 || j == c - 1) {
                printf(BLACK_BG);
                printf(RED"%c "RESET, edge);
                printf(RESET);
            } 
            else {
                printf(BLACK_BG);
                printf(BROWN"%s"RESET, move);
                printf(RESET);
            }
            
        }
        printf("\n");
    }
}

bool input_correction(char W_A_S_D) {
    if (W_A_S_D == 'w' || W_A_S_D == 'a' || W_A_S_D == 's' || W_A_S_D == 'd' || W_A_S_D == 'q') {
        return true;
    } 
    return false;
}

char UI() {
    char userinput;
    if (scanf(" %c", &userinput) == 1) {
        if (userinput >= 'A' && userinput <= 'Z') {
                userinput += 32; 
            }

        if (userinput >= 'a' && userinput <= 'z') {
            if (input_correction(userinput) != 0) {
                return userinput;
            } 
        }
    }
    printf("Wrong INPUT Try Again\n");
    return '\0';
}

void updateCharacters(Player *Neak_lg, Location *player) {
    if (player->Player_pos[0] == player->Enemy_pos[0] && player->Player_pos[1] == player->Enemy_pos[1] &&
        player->Enemy_pos[0] >= 0 && player->Enemy_pos[1] >= 0) {
        printf(RED"*AHHHHH (You got stab and lost 50 points of health)\n"RESET);
        delay(3);
        Neak_lg->health -= 50;
        player->Enemy_pos[0] = -1;
        player->Enemy_pos[1] = -1;
        return;
    }

    if (player->Player_pos[0] == player->Traps_[0] && player->Player_pos[1] == player->Traps_[1] &&
        player->Traps_[0] >= 0 && player->Traps_[1] >= 0) {
        printf(MAGENTA"*OWWWW (Bruh how did you not see that there is a trap T-T lost 20 health)\n"RESET);
        delay(3);
        Neak_lg->health -= 20;
        player->Traps_[0] = -1;
        player->Traps_[1] = -1;
        return;
    }

    if (player->Player_pos[0] == player->Treasure_[0] && player->Player_pos[1] == player->Treasure_[1] &&
        player->Treasure_[0] >= 0 && player->Treasure_[1] >= 0) {
        printf(YELLOW"*TREASUREEE (Money Money Money hehe)\n"RESET);
        delay(3);
        Neak_lg->score += 100;
        player->Treasure_[0] = -1;
        player->Treasure_[1] = -1;
        return;
    }

    if (player->Player_pos[0] == player->Key_pos[0] && player->Player_pos[1] == player->Key_pos[1] &&
        player->Key_pos[0] >= 0 && player->Key_pos[1] >= 0) {
        printf(GREEN_BOLD"\n*AYEEEE (The KEY to get out of here)\n"RESET);
        delay(3);
        hasKey = true;
        player->Key_pos[0] = -1;
        player->Key_pos[1] = -1;
        return;
    }

    if (player->Player_pos[0] == player->Exit[0] && player->Player_pos[1] == player->Exit[1] &&
        player->Exit[0] >= 0 && player->Exit[1] >= 0) {
        if (hasKey) {
            switch (currentLevel) {
                case 1: 
                    printf(GREEN "\nEntering Floor 2" RESET); 
                    fflush(stdout);
                    for (int i=0; i<3; i++) {
                        delay(1);
                        printf(GREEN"."RESET);
                        fflush(stdout);
                    }
                    printf("\n");
                    level_complete = true; break;
                case 2: 
                    printf(GREEN "\nEntering Floor 3"RESET);
                    for (int i=0; i<3; i++) {
                        delay(1);
                        printf(GREEN"."RESET);
                        fflush(stdout);
                    }
                    printf("\n");

                    level_complete = true; break;
                case 3:
                    if (Neak_lg->health == 100 && Neak_lg->score == 300) {
                    system("cls");
                    printf(GREEN "YOU ESCAPEEEE THE DUNGEON WOWWWW\n" RESET); level_complete = true; break;
                    delay(3);
                    printf("WAITTT");
                    for (int i=0; i<6; i++) {
                        delay(1);
                        printf(GREEN"."RESET);
                        fflush(stdout);
                    }
                    printf("\n");
                    system("cls");
                    printf("           CONGRADULATION YOU GOT A FREE CANDY FOR HAVING A PERFECT SCORE");
                }
                    system("cls");
                    printf(GREEN "YOU ESCAPEEEE THE DUNGEON WOWWWW\n" RESET); level_complete = true; break;
            }
            delay(3);
        } else {
            printf(RED "JOBBB IT'S LOCKED (go find the key bro)\n" RESET);
            delay(3);
        }
    }
}

void movement (Player *Neak_lg, Location *player, char UI, int row, int column) {
    if (UI == 'q') {
        operating = false;
        return;
    }

    int Movement_Row = player->Player_pos[0];
    int Movement_Column = player->Player_pos[1];

    switch (UI) {
        case 'w': Movement_Row--; break;
        case 'a': Movement_Column--; break;
        case 's': Movement_Row++; break;
        case 'd': Movement_Column++; break;
    }

    if (Movement_Row <= 0 || Movement_Row >= row - 1 ||
        Movement_Column <= 0 || Movement_Column >= column - 1) {
        printf("Bro there's a wall can't you see...\n");
        delay(3);
        return;
    }

    player->Player_pos[0] = Movement_Row;
    player->Player_pos[1] = Movement_Column;
    
    updateCharacters(Neak_lg, player);
}

void Level1 (Location *player) {
    int row=5; int column=15; 
    drawLocation(player, row, column);
}

void Level2 (Location *player) {
    int row=10; int column=25; 
    drawLocation(player, row, column);
}

void Level3 (Location *player) {
    int row=15; int column=30; 
    drawLocation(player, row, column);
}


void nextLevel(Location *player, int row, int column){
    currentLevel++;
    level_complete = false;
    hasKey = false;
    gameWon = false;

    player->Player_pos[0] = 2;
    player->Player_pos[1] = 2;

    player->Enemy_pos[0] = 0;
    player->Enemy_pos[1] = 0;

    if (currentLevel == 2) {
        row = 10;
        column = 25;
    } else if (currentLevel == 3) {
        row = 15;
        column = 30;
    }
}


int main () {
    srand(time(NULL));
    printf(BLUE "            ========================================\n"
                "                    WELCOME TO DUNGEON ESCAPE       \n"
                "            ========================================\n" RESET);

    char name[100];
    printf("Gimme your name: --- ");
    scanf("%s", name);

    printf("Loading");
    fflush(stdout);
    for (int i=0; i<3; i++) {
        delay(1);
        printf(".");
        fflush(stdout);
    }
    printf("\n");

    clock_t start = clock();
    while ((clock() - start) < 3 * CLOCKS_PER_SEC);

    Location player = {{2, 2}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    Player Neak_lg = {100, 0};

    int row = 5;
    int column = 15;

    while (operating) {
        if (currentLevel == 4) {
            return 0;
        }

        system("cls");
        printf(BLUE"--- Level %d ---\n"RESET, currentLevel);
        printf("Player: "GREEN"%s "RESET"     Health:"GREEN"%d"RESET"    Score:"GREEN"%d"RESET"\n", name, Neak_lg.health, Neak_lg.score);

        switch (currentLevel) {
            case 1:
                if (player.Enemy_pos[0] == 0 && player.Enemy_pos[1] == 0) {
                    randomSpawning(&player, 5, 15);
                }
                row = 5; column = 15; Level1(&player); break;
            case 2:
                if (player.Enemy_pos[0] == 0 && player.Enemy_pos[1] == 0) {
                    randomSpawning(&player, 10, 25);
                }
                row = 10; column = 25; Level2(&player); break;
            case 3:
                if (player.Enemy_pos[0] == 0 && player.Enemy_pos[1] == 0) {
                    randomSpawning(&player, 15, 30);
                }
                row = 15; column = 30; Level3(&player); break;
            default: operating = false; break;
        }

        if (!operating) {
            break;
        }

        printf("Controls: W - Move UP\n"
               "          A - Move LEFT\n"
               "          S - Move DOWN\n"
               "          D - Move RIGHT\n"
               "          Q - Exit Game\n");
        printf("Enter action: ");

        char Input = UI();
        movement(&Neak_lg, &player, Input, row, column);

        if (level_complete) {
            nextLevel(&player, row, column);
            if (!operating) {
                break;
            }
            continue;
        }

        if (Neak_lg.health <= 0) {
            operating = false;
            break;
        }
    }
    return 0;
}