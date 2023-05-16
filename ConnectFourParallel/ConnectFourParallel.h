#include "ConfigParallel.h"
void printBoard(vector<vector<int> >&);
int userMove();
void makeMove(vector<vector<int> >&, int, unsigned int);
void errorMessage(int);
int aiMove();
vector<vector<int> > copyBoard(vector<vector<int> >);
bool winningMove(vector<vector<int> >&, unsigned int);
int scoreSet(vector<unsigned int>, unsigned int);
int tabScore(vector<vector<int> >, unsigned int);
array<int, 2> miniMax(vector<vector<int> >&, unsigned int, unsigned int);
array<int, 2> miniMaxParallel(vector<vector<int> >& b, unsigned int d, unsigned int p);
int heurFunction(unsigned int, unsigned int, unsigned int);
