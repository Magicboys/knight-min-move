//Authored by: Tyler Watson
//Date: 5/9/2023
//Algorithms 2 Chess Homework

#include <iostream>
#include <queue>
#include <list>

//A board tile is true if the knight is there and false if the tile is free
bool board[8][8];

//Endgoal location
int goalPositionX;
int goalPositionY;

//Knight starting position
int knightStartingPositionX;
int knightStartingPositionY;

//Method Headers
void InitializeBoard();
bool isLegal(int origX, int origY, int newX, int newY);
void PrintBoard();
void PrintPaths(std::list<std::list<std::pair<int, int>>> paths);

int main()
{
    InitializeBoard();

    std::list<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;

    std::queue<std::pair<int, int>> positionsToExpand;
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            if (board[i][k] && (knightStartingPositionX != i && knightStartingPositionY != k)) {
                positionsToExpand.push({ i, k });
                moves.push_back({ {knightStartingPositionY, knightStartingPositionX}, {i, k} });
            }
        }
    }
    
    PrintBoard();

    while (!positionsToExpand.empty()) {
        std::pair<int, int> tempLocation = positionsToExpand.front();
        positionsToExpand.pop();
        //Find moves within current position
        for (int i = tempLocation.first - 2; i <= tempLocation.first + 2; i++) {
            for (int k = tempLocation.second - 2; k <= tempLocation.second + 2; k++) {
                if (i >= 0 && i < 8 && k >= 0 && k < 8) {
                    if (isLegal(tempLocation.second, tempLocation.first, k, i) && !board[i][k] && (positionsToExpand.back().first != i && positionsToExpand.back().second != k)) {
                        board[i][k] = true;
                        
                        //each element in moves stores the original position -> new position
                        moves.push_back({ {tempLocation.first, tempLocation.second}, {i, k} });
                        positionsToExpand.push({ i, k });
                    }
                }
            }
        }
    }

    //Now start at finish and build paths to the original starting position. The first time a path touches the starting position that will be the best min path. Else we continue until we visit all possible.
    std::list<std::list<std::pair<int, int>>> paths;

    auto j = moves.begin();
    while (j != moves.end()) {
        std::pair<std::pair<int, int>, std::pair<int, int>> move = *j;
        if (move.first.second == knightStartingPositionX && move.first.first == knightStartingPositionY) {
            std::list<std::pair<int, int>> localPath;
            localPath.push_back(move.first);
            localPath.push_back(move.second);
            paths.push_back(localPath);
            j = moves.erase(j);
        }
        else {
            j++;
        }
    }
    
    std::list<std::pair<int, int>> bestPath;
    while (!moves.empty()) {
        std::list<std::list<std::pair<int, int>>> newPaths;
        for (auto localPath = paths.begin(); localPath != paths.end(); localPath++) {
            auto move = moves.begin();
            while (move != moves.end()) {
                std::list<std::pair<int, int>> newLocal = *localPath;
                std::pair<std::pair<int, int>, std::pair<int, int>>newMove = *move;
                
                if (newLocal.back().second == newMove.first.second && newLocal.back().first == newMove.first.first) {
                    if (newMove.second.first == goalPositionY && newMove.second.second == goalPositionX) {
                        newLocal.push_back(newMove.second);
                        newPaths.push_back(newLocal);
                        move = moves.erase(move);
                        bestPath = newLocal;
                    }
                    else {
                        newLocal.push_back(newMove.second);
                        newPaths.push_back(newLocal);
                        move = moves.erase(move);
                    }
                    
                }
                else {
                    move++;
                }
            }
        }
        paths = newPaths;
        PrintPaths(paths);
        if (!bestPath.empty()) break;
    }
    if (!bestPath.empty()) {
        std::cout << "-----------------------" << std::endl;
        std::cout << "SHORTEST PATH FOUND!" << std::endl;
        std::cout << "STARTING POSITION: (" << knightStartingPositionX << "," << knightStartingPositionY << ")" << std::endl;
        std::cout << "ENDING POSITION: (" << goalPositionX<< "," << goalPositionY << ")" << std::endl;
        std::cout << "PATH: [";
        for (auto localMove = bestPath.begin(); localMove != bestPath.end(); localMove++) {
            std::pair<int, int> move = *localMove;
            if (move.first != goalPositionY && move.second != goalPositionX) {
                std::cout << "(" << move.second << "," << move.first << ") -> ";
            }
            else {
                std::cout << "(" << move.second << "," << move.first << ")";
            }
        }
        std::cout << "]" << std::endl;
        std::cout << "MIN PATH SIZE: " << bestPath.size() << std::endl;
    }
    else {
        std::cout << "-----------------------" << std::endl;
        std::cout << "No path found!" << std::endl;
    }
    
}

void PrintPaths(std::list<std::list<std::pair<int, int>>> paths) {
    std::cout << "---- PRINTING ALL PATHS ----" << std::endl;
    for (auto localPath = paths.begin(); localPath != paths.end(); localPath++) {
        std::list<std::pair<int, int>> path = *localPath;
        std::cout << "PATH: [";
        for (auto localMove = path.begin(); localMove != path.end(); localMove++) {
            std::pair<int, int> move = *localMove;
            std::cout << "(" << move.second << "," << move.first << ") ";
        }
        std::cout << "]" << std::endl;
    }
}


//Place the knight at a location on the board and initialize all other tiles as true
void InitializeBoard() {
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            board[i][k] = false;
        }
    }

    //Initialize starting positions
    goalPositionX = 1;
    goalPositionY = 3;

    knightStartingPositionX = 5;
    knightStartingPositionY = 5;
    board[knightStartingPositionX][knightStartingPositionY] = true;

    //Find moves within starting range
    for (int i = knightStartingPositionY-2; i <= knightStartingPositionY+2; i++) {
        for (int k = knightStartingPositionX-2; k <= knightStartingPositionX+2; k++) {
            if (i >= 0 && i < 8 && k >= 0 && k < 8) {
                if (isLegal(knightStartingPositionX, knightStartingPositionY, k, i)) {
                    board[i][k] = true;
                }
            }
        }
    }
}

//Assumes that the passed in values are within the board and returns true if it is a valid move for a knight
bool isLegal(int origX, int origY, int newX, int newY) {
    if (newX == origX - 2) {
        if (newY == origY + 1) {
            return true;
        }
        if (newY == origY - 1) {
            return true;
        }
    }
    if (newX == origX + 2) {
        if (newY == origY + 1) {
            return true;
        }
        if (newY == origY - 1) {
            return true;
        }
    }
    if (newY == origY+2) {
        if (newX == origX + 1) {
            return true;
        }
        if (newX == origX - 1) {
            return true;
        }
    }
    if (newY == origY-2) {
        if (newX == origX + 1) {
            return true;
        }
        if (newX == origX - 1) {
            return true;
        }
    }

    //If no conditional was triggered, then the move must be invalid
    return false;
}

// Q indicates the goal position, K indicates the knight position, 0 is a tile either in a path or still undetermined, and X is a board tile that is not part of a path.
void PrintBoard() {
    std::cout << "---------------------------------" << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << "| ";
        for (int k = 0; k < 8; k++) {
            if (k == goalPositionX && i == goalPositionY) {
                std::cout << "Q | ";
            }
            else {
                if (k == knightStartingPositionX && i == knightStartingPositionY) {
                    std::cout << "K | ";
                } else if (!board[i][k]) {
                    std::cout << "X | ";
                }
                else {
                    std::cout << "0 | ";
                }
            }
            
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}
