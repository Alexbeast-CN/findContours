#include <vector>
#include <string>
#include <iostream>

typedef std::vector<int>                Listi;
typedef std::vector<Listi>              Mat2i;
typedef std::pair<int, int>             p;
typedef std::vector<p>                  pList;
typedef std::vector<pList>              pMat;
typedef unsigned char                   uint8;

class FindContours
{
private:
    Mat2i grid{{1,1,1,1,1,1,1,0,0},
               {1,0,0,1,0,0,1,0,1},
               {1,0,0,1,0,0,1,0,0},
               {1,1,1,1,1,1,1,0,0}};
    
    int pad_size = 1;
    int rows = grid.size();
    int cols = grid[0].size();
    int LNBD = 1;
    int NBD = 1;
    int MAX_BODER_NUMBER;

    void reset(){
        pad(pad_size);
        LNBD = 1;
        NBD = 1;
        MAX_BODER_NUMBER = rows*cols;
    }
    

public:
    FindContours(/* args */){
        // grid;
    };

    void pad(int pad_size);

    void rm_pad(int pad_size);

    inline void mapLoader(const Mat2i &map){
        grid = map;
        rows = grid.size();
        cols = grid[0].size();
        reset();
    }

    void Display();
    
    p findNeighbor(const p &center, const p &start, bool clock_wise);

    void board_follow(const p &center, const p &start);

    void raster_scan();

    Mat2i getGrid(){
        Mat2i grid_copy = grid;
        return grid_copy;
    };

};



