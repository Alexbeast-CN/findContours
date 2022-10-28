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

p FindContours::findNeighbor(const p &center, const p &start, bool clock_wise=true){
    int weight = -1;
    if (clock_wise) weight = 1;

    pList neighbors{p(0, 0), p(0, 1), p(0,2),
                    p(1,2), p(2,2), p(2,1),
                    p(2,0), p(1,0)};
    Mat2i index{{0, 1, 2},
                {7, 9, 3},
                {6, 5, 4}};
    int start_ind = index[start.first - center.first + 1]
            [start.second - center.second + 1];

    for (int i = 1; i < neighbors.size()+1; i++){
        int cur_ind = (start_ind + i*weight + 8) % 8;
        int x = center.first + neighbors[cur_ind].first - 1;
        int y = center.second + neighbors[cur_ind].second - 1;
        if(grid[x][y] != 0) 
            return p(x, y);
    }
    return p(-1, -1);
}

void FindContours::board_follow(const p &center, const p &start){
    p ij  = center;
    p ij2 = start;
    p ij1 = findNeighbor(ij, ij2, true);

    int weight = 1;
    int x = ij1.first;
    int y = ij1.second;

    if (ij1 == p(-1, -1)){
        grid[ij.first][ij.second] = -NBD;
        return;
    }

    ij2 = ij1;
    p ij3 = ij;

    for (int k = 0; k < MAX_BODER_NUMBER; k++){
        // step 3.3
        p ij4 = findNeighbor(ij3, ij2, false);
        x = ij3.first;
        y = ij3.second;
        if (ij4.first - ij2.first <= 0)
            weight = -1;
        
        if (grid[x][y] >= 0){
            if (grid[x][y-1] == 0 && grid[x][y+1] == 0)
                grid[x][y] = -NBD*weight;
            else if (grid[x][y+1] == 0)
                grid[x][y] = -NBD;
            else if (grid[x][y] == 1 && grid[x][y+1] != 0)
                grid[x][y] = NBD;
        }
        

       if (ij4 == ij && ij3 == ij1)
            return; 

        ij2 = ij3;
        ij3 = ij4;
    }   

}

void FindContours::raster_scan(){
    for (int i = 0; i < rows; i++){
        LNBD = 1;
        for (int j = 0; j < cols; j++){
            if (abs(grid[i][j]) > 1)
                LNBD = abs(grid[i][j]);

            if (grid[i][j] >= 1){
                if (grid[i][j] == 1 && grid[i][j-1] == 0){
                    NBD += 1;
                    board_follow(p(i, j), p(i, j-1));
                }
                else if (grid[i][j] > 1 && grid [i][j+1] == 0){
                    NBD += 1;
                    board_follow(p(i, j), p(i, j+1));
                }
            }
        }
    }
    rm_pad(pad_size);    
}