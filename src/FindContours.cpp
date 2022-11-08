#include "FindContours.h"

void FindContours::pad(int pad_size){
    Mat2i grid_pad(rows+2*pad_size, Listi(cols+2*pad_size, 0));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++)
            grid_pad[i+pad_size][j+pad_size] = grid[i][j];
    }
    grid = grid_pad;
    rows = grid.size();
    cols = grid[0].size();
}

void FindContours::rm_pad(int pad_size){
    Mat2i grid_rm(rows-2*pad_size, Listi(cols-2*pad_size, 0));
    for (int i = 0; i < rows-2*pad_size; i++){
        for (int j = 0; j < cols-2*pad_size; j++)
            grid_rm[i][j] = grid[i+pad_size][j+pad_size];
    }
    grid = grid_rm;
    rows = grid.size();
    cols = grid[0].size();
}

void FindContours::Display(){
    for (int i = 0; i < rows; i++){
        std::cout << "\033[37m[ "; 
        for (int j = 0; j < cols; j++){
            if (grid[i][j] == 0)
                std::cout << " " << "\033[37m" << grid[i][j] << " ";
            else if (grid[i][j] > 0)
                std::cout << " " << "\033[31m" << grid[i][j] << " ";
            else
                std::cout << "\033[31m" << grid[i][j] << " ";
        }
        std::cout << "\033[37m" << "]" << std::endl;
    }
    std::cout << std::endl;
}

// Look for the first non-zero pixel around the current pixel.
// If there is no non-zero pixel, return (-1, -1).
p FindContours::findNeighbor(const p &center, const p &start, bool ClockWise=true){
    int weight = 1;
    if (!ClockWise) weight = -1;

    pList neighbors{p(0, 0), p(0, 1), p(0,2),
                    p(1,2), p(2,2), p(2,1),
                    p(2,0), p(1,0)};
    Mat2i index{{0, 1, 2},
                {7, 0, 3},
                {6, 5, 4}};
    int start_ind = index[start.first - center.first + 1]
            [start.second - center.second + 1];

    for (int i = 1; i < neighbors.size()+1; i++){
        int cur_ind = (start_ind + i*weight + 8) % 8;
        int x = center.first + neighbors[cur_ind].first - 1;
        int y = center.second + neighbors[cur_ind].second - 1;
        if(grid[x][y] != 0){
            // std::cout << grid[x][y] << std::endl;
            return p(x, y);  
        } 
    }
    return p(-1, -1);
}


// It works but not logically clean.
/*
void FindContours::board_follow(const p &center, const p &start, bool ClockWise=true){
    int weight = 1;
    if (!ClockWise) weight = -1;

    p end_loop = findNeighbor(center, start, ClockWise);

    if (end_loop == p(-1, -1)){
        grid[center.first][center.second] = -NBD;
        return;
    }

    p new_center    = center;
    p neighbor      = end_loop;
    p new_neighbor  = findNeighbor(new_center, neighbor, !ClockWise);

    while (new_neighbor != p(-1, -1)){
        int x = new_center.first;
        int y = new_center.second;
        if (new_neighbor.first - neighbor.first <= 0)
            weight = -1;
        
        if (grid[x][y] >= 0){
            if (grid[x][y-1] == 0 && grid[x][y+1] == 0)
                grid[x][y] = -NBD*weight;
            else if (grid[x][y+1] == 0)
                grid[x][y] = -NBD;
            else if (grid[x][y] == 1 && grid[x][y+1] != 0)
                grid[x][y] = NBD;
        } 

        
       if (new_neighbor == center && new_center == end_loop)
            return; 

        neighbor        = new_center;
        new_center      = new_neighbor;
        new_neighbor    = findNeighbor(new_center, neighbor, !ClockWise);
    }

}
*/

void FindContours::board_follow(const p &center, const p &start, bool ClockWise=true){
    bool open_polygon = true;
    std::vector<p> board;

    grid[center.first][center.second] = NBD;

    p new_center    = center;
    p neighbor      = start;
    p new_neighbor  = findNeighbor(new_center, neighbor, ClockWise);

    while (new_neighbor != p(-1, -1)){
        int x = new_center.first;
        int y = new_center.second;
        board.push_back(new_center);

        if (new_neighbor == center){
            for (auto &p : board)
                grid[p.first][p.second] = NBD;
            return; 
        }

        neighbor        = new_center;
        new_center      = new_neighbor;
        new_neighbor    = findNeighbor(new_center, neighbor, ClockWise);
    }
    
    if (open_polygon){
        for (auto &p : board)
                grid[p.first][p.second] = NBD;
            return; 
    }
}

void FindContours::raster_scan(){
    for (int i = 0; i < rows; i++){
        LNBD = 1;
        for (int j = 0; j < cols; j++){
            // find the starting point of the boarder.
            if (grid[i][j] == 1 && grid[i][j-1] == 0){
                LNBD = NBD;
                NBD += 1;
                std :: cout << "out " << NBD << std::endl;
                board_follow(p(i, j), p(i, j-1), false);
                board_type.push_back("out");
                // Display();
            }
            // find the starting point of the hole.
            else if (grid[i][j] == 1 && grid [i][j+1] == 0){
                LNBD = NBD;
                NBD += 1;
                std :: cout << "in " << NBD << std::endl;
                board_follow(p(i, j), p(i, j+1), true);
                board_type.push_back("in");
                // Display();
            }
        }
    }
    rm_pad(pad_size);    
}