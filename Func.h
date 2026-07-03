#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
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

#endif
