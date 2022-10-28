#include "./include/FindContours.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <fstream>
#include <sstream>
#include <cassert>

Mat2i dataReader(std::string &filename){
    std::ifstream map_f;
    map_f.open(filename);
    if (!map_f) {
        std::cout << "fail opening map file!" << std::endl;
        return Mat2i();
    }

    std::string str;
    if (!getline(map_f, str)) {
        std::cout << "empty file!" << std::endl;
        return Mat2i();
    }
    std::cout << "Successfully read map file" << std::endl;

    int    width, height;

    std::stringstream ss(str);
    std::string       name;
    ss >> name;
    assert(name == "width:");
    ss >> width;

    getline(map_f, str);
    ss = std::stringstream(str);
    ss >> name;
    assert(name == "height:");
    ss >> height;

    Mat2i grid(height, Listi(width, 0));
    int state = 0;
    int i = 0;

    while (getline(map_f, str)) {
        ss = std::stringstream(str);
        ss >> name;
        if (name == "state_grid:"){
            while(getline(map_f, str)){
                std::cout << str.size() << std::endl;
                ss = std::stringstream(str);
                while (ss >> state){
                    if (state > 250)
                        grid[i/width][i%width] = 0;
                    else
                        grid[i/width][i%width] = 1;
                    i++;
                }
            }
        }
    }

    return grid;

}

void MatDisplay(const Mat2i &mat, const std::string &name){
    int rows = mat.size();
    int cols = mat[0].size();
    cv::Mat mat_cv(rows, cols, CV_8UC1);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mat_cv.at<uchar>(i, j) = abs(mat[i][j])*60;

    cv::namedWindow(name, cv::WINDOW_KEEPRATIO);
    cv::imshow(name, mat_cv);
    cv::waitKey(0);
}

int main(int, char**) {
    // Mat2i mat{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //           {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    //           {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    //           {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
    //           {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
    //           {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    //           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
  
    std::string filename = "../data/CPP_origin_map.txt";
    Mat2i mat = dataReader(filename);
    FindContours fc;
    fc.mapLoader(mat);
    MatDisplay(mat, "origin");
    // fc.Display();
    fc.raster_scan();
    Mat2i mat2 = fc.getGrid();
    MatDisplay(mat2, "contour");
    // fc.Display();
    return 0;
}
