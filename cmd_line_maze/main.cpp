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

#define GRID vector<vector<bool>>

// This is intended to automatically handle out-of-bound conditions.
bool is_in_bounds (const GRID& is_walls, const int x, const int y) {
    return x >= 0 && y >= 0 && x < is_walls.size() && y < is_walls[0].size();
}
bool is_unvisited (const GRID& is_walls, const int x, const int y) {
    if (is_in_bounds(is_walls, x, y))
        return is_walls[x][y];
    return false;
}
bool is_corridor (const GRID& is_walls, const int x, const int y) {
    if (is_in_bounds(is_walls, x, y))
        return !is_walls[x][y];
    return false;
}
bool is_wall (const GRID& is_walls, const int x, const int y) {
    if (is_in_bounds(is_walls, x, y))
        return is_walls[x][y];
    return false;
}

GRID generate (int width, int height) {
    GRID is_walls (width, vector<bool>(height, true));
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
        
        // supposed to be "nchoices = choices.size();" but that erred.
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
// Returns the appropriate tileset for the given style #
array<string, 16> get_tileset(int s) {
    s %= 4;
    if (s==0) return {" ","╷","╵","│","╶","┌","└","├","╴","┐","┘","┤","─","┬","┴","┼"};
    if (s==1) return {" ","╻","╹","┃","╺","┏","┗","┣","╸","┓","┛","┫","━","┳","┻","╋"};
    if (s==2) return {" ","╷","╵","║","╶","╔","╚","╠","╴","╗","╝","╣","═","╦","╩","╬"};
              return {" ","╷","╵","│","╶","╭","╰","├","╴","╮","╯","┤","─","┬","┴","┼"};
}
void display_maze (GRID& is_walls, int style = -1) {
    const u_long
    width = is_walls.size(),
    height = is_walls[0].size();
    
    if (style == -1) {
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                cout << (is_walls[x][y] ? "#" : ".");
            }
            cout << endl;
        }
    } else if (style >= 0 && style <= 7) {
        const array<string, 16> tileset = get_tileset(style);
        int tile;
        for (int y=0; y<height; y+=2) {
            for (int x=0; x<width; x+=2) {
                
                tile = 0;
                if (is_wall(is_walls, x, y+1)) tile += 1;
                if (is_wall(is_walls, x, y-1)) tile += 2;
                if (is_wall(is_walls, x+1, y)) tile += 4;
                if (is_wall(is_walls, x-1, y)) tile += 8;
                cout << tileset[tile];
                
                if (style > 3) {
                    if ((tile > 3 && tile < 8) || (tile > 11 && tile < 16)) {
                        cout << tileset[12];
                    } else {
                        cout << tileset[0];
                    }
                }
            } //END x loop
            cout << endl;
        } //END y loop
    } else {
        throw "style number is invalid";
    }
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        cout << "You messed up the arguments" << endl;
        return -1;
    } else {
        int width = atoi(argv[1]), height = atoi(argv[2]);
        if (width  % 2 == 0) width--;
        if (height % 2 == 0) height--;
        
        bool got_style = false;
        bool got_wide = false;
        //bool got_seed = false;
        int style = -1;
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "l") == 0 ||
                strcmp(argv[i], "line") == 0) {
                if (got_style) {
                    cout << "You messed up the arguments" << endl;
                    return -1;
                }
                got_style = true;
                style = 0;
            } else if (strcmp(argv[i], "b") == 0 ||
                       strcmp(argv[i], "bold") == 0) {
                if (got_style) {
                    cout << "You messed up the arguments" << endl;
                    return -1;
                }
                got_style = true;
                style = 1;
            } else if (strcmp(argv[i], "d") == 0 ||
                       strcmp(argv[i], "double") == 0) {
                if (got_style) {
                    cout << "You messed up the arguments" << endl;
                    return -1;
                }
                got_style = true;
                style = 2;
            } else if (strcmp(argv[i], "c") == 0 ||
                       strcmp(argv[i], "curved") == 0) {
                if (got_style) {
                    cout << "You messed up the arguments" << endl;
                    return -1;
                }
                got_style = true;
                style = 3;
            } else if (strcmp(argv[i], "w") == 0 ||
                       strcmp(argv[i], "wide") == 0) {
                if (got_wide) {
                    cout << "You messed up the arguments" << endl;
                    return -1;
                }
                got_wide = true;
            }
        }
        if (got_wide) {
            if (got_style) {
                style += 4;
            } else {
                cout << "You messed up the arguments" << endl;
                return -1;
            }
        }
        
        GRID is_walls = generate(width, height);
        display_maze(is_walls, style);
        return 0;
    }
}