#include <stdio.h>
#include <iostream>
#include <vector>
#include <limits.h>
#include <array>
#include <sstream>
#include <tbb/tbb.h>
#include "tbb/task_group.h"
#include <tbb/tick_count.h>
#include <chrono>

using namespace std;
using namespace tbb;

unsigned int NUM_COL = 7; // how wide is the board
unsigned int NUM_ROW = 6; // how tall
unsigned int PLAYER = 1; // player number
unsigned int COMPUTER = 2; // AI number
unsigned int MAX_DEPTH = 7; // the default "difficulty" of the computer controlled AI

bool gameOver = false; // flag for if game is over
unsigned int turns = 0; // count for # turns
unsigned int currentPlayer = PLAYER; // current player
mutex mtx;
