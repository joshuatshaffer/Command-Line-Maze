//
//  main.cpp
//  cmd_line_maze
//
//  Created by Joshua on 9/1/15.
//  Copyright (c) 2015 Joshua T Shaffer. All rights reserved.
//

#include <iostream>
#include <array>
#include <vector>
using namespace std;

#define WIDTH 31
#define HEIGHT 21
#define GRID array<array<bool, HEIGHT>, WIDTH>

// This is intended to automaticaly handel out of bounds conditions.
bool is_unvisited (const GRID& is_walls, const int x, const int y) {
    if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT) {
        return is_walls[x][y];
    }
    return false;
}

GRID generate () {
    GRID is_walls;
    for (int y=0; y<HEIGHT; y++) {
        for (int x=0; x<WIDTH; x++) {
            is_walls[x][y] = true;
        }
    }
    
    int fx = 1, fy = 1;
    vector<pair<int, int>> could_turn;
    
    while (true) {
        
        vector<char> choices;
        if (is_unvisited(is_walls, fx, fy + 2))
            choices.push_back('N');
        if (is_unvisited(is_walls, fx, fy - 2))
            choices.push_back('S');
        if (is_unvisited(is_walls, fx + 2, fy))
            choices.push_back('E');
        if (is_unvisited(is_walls, fx - 2, fy))
            choices.push_back('W');
        
        // suppost to be "nchoices = choices.size();" but that erred.
        const long& nchoices = distance(choices.begin(), choices.end());
        
        if (nchoices < 1) {
            if (could_turn.empty()) {
                break;
            } else {
                pair<int, int>& place = could_turn.back();
                fx = place.first;
                fy = place.second;
                could_turn.pop_back();
                continue;
            }
        } else if (nchoices > 1) {
            could_turn.push_back(make_pair(fx, fy));
        }

        char& chosen = choices[rand() % nchoices];
        
        if (chosen == 'N') {
            is_walls[fx][++fy] = false;
            is_walls[fx][++fy] = false;
        } else if (chosen == 'S') {
            is_walls[fx][--fy] = false;
            is_walls[fx][--fy] = false;
        } else if (chosen == 'E') {
            is_walls[++fx][fy] = false;
            is_walls[++fx][fy] = false;
        } else if (chosen == 'W') {
            is_walls[--fx][fy] = false;
            is_walls[--fx][fy] = false;
        }
    }
    return is_walls;
}

void display_maze (GRID& is_walls) {
    for (int y=0; y<HEIGHT; y++) {
        for (int x=0; x<WIDTH; x++) {
            cout << (is_walls[x][y] ? "#" : ".");
        }
        cout << endl;
    }
}

int main(int argc, const char * argv[]) {
    cout << "Started" << endl;
    
    GRID is_walls = generate();
    
    display_maze(is_walls);
    
    cout << "Ended" << endl;
    return 0;
}