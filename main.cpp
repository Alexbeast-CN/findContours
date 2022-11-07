//
// Created by Daoming Chen 2022/10/24
// 

#include "./include/FindContours.h"
#include <fstream>
#include <sstream>
#include <cassert>


// remember to comment find opencv package in the CMakeLists.txt if turn OPENCVDISPLAY off.
// #define OPENCVDISPLAY 

#ifdef OPENCVDISPLAY
    #include "opencv2/opencv.hpp"
    #include "opencv2/highgui.hpp"
    #include "opencv2/imgproc.hpp"

    void MatDisplay(const Mat2i &mat, const std::string &name){
        int rows = mat.size();
        int cols = mat[0].size();
        cv::Mat mat_cv(rows, cols, CV_8UC1);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                mat_cv.at<uchar>(i, j) = abs(mat[i][j])*30;
                // if (mat[i][j] == 1)
                //     mat_cv.at<uchar>(i, j) = 30;
                // else if (mat[i][j] > 1)
                //     mat_cv.at<uchar>(i, j) = 255;

        cv::namedWindow(name, cv::WINDOW_KEEPRATIO);
        cv::imshow(name, mat_cv);
        std::cout << "Press any key to continue..." << std::endl;
        cv::waitKey();
        // save the image into outputs
        cv::imwrite("../outputs/" + name + ".jpg", mat_cv);
    }
#endif

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

int main(int, char**) {
    FindContours fc;

    

    #ifdef OPENCVDISPLAY
        std::string name        = "poly";
        std::string filename    = "../data/" + name + ".txt";

        Mat2i mat = dataReader(filename);
        fc.mapLoader(mat);
        MatDisplay(mat, name + "_origin");

        fc.raster_scan();
        Mat2i mat2 = fc.getGrid();
        MatDisplay(mat2, name + "_contour");
    #else
        //// A small mat for test.
        Mat2i mat1{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                   {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                   {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
                   {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
                   {0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0},
                   {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
                   {0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0},
                   {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                   {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                   {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

        Mat2i mat2{{1,1,1,1,1,1,1},
                   {1,1,1,1,1,1,1},
                   {1,1,0,0,0,1,1},
                   {0,1,1,0,0,1,1},
                   {0,0,1,1,0,1,1},
                   {0,0,0,1,1,1,1},
                   {0,0,0,0,1,1,1}};

        Mat2i mat3{{1,1,1,1,1,1,1},
                   {0,0,0,0,0,0,0}};

        fc.mapLoader(mat1);
        fc.Display();

        fc.raster_scan();
        fc.Display();
    #endif

    return 0;
}
