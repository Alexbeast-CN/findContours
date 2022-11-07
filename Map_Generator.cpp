//
// Created by Daoming Chen 2022/10/24
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define MAP_WIDTH       500
#define MAP_HEIGHT      300
#define RESOLUTION      0.05
#define OUTER_POLYGON   2
#define OBSTABLE_NUM    3


// store point from mouse click
struct MouseParams{
    MouseParams(const cv::Mat& img_){
        img = img_.clone();
        points = {};
    }
    std::vector<cv::Point> points;
    cv::Mat img;
};

// mouse callback function
void onMouseHandle(int event, int x, int y, int flags, void* params)
{
    MouseParams* mp = (MouseParams*)params;
    switch (event)
    {
    case cv::EVENT_LBUTTONDOWN:
    {
        cv::circle(mp->img, cv::Point(x,y), 3, cv::Scalar(0, 64, 255), -1);
        cv::imshow("Map display", mp->img);
        std::cout<<"<point> x: "<<x<<", y: "<<y<<std::endl;
        mp->points.push_back(cv::Point(x, y));
        break;
    }
    default:
    {
        break;
    }
    }
}

int main(int argc, char** argv) {
    cv::Mat img( MAP_HEIGHT , MAP_WIDTH, CV_8UC3,
                  cv::Scalar(255, 255, 255));

    // Select outside polygon points by mouse click
    cv::namedWindow("Map display");
    cv::imshow("Map display", img);
    std::cout << "You can create " << OUTER_POLYGON << " polygons" << std::endl;
    std::cout << "Please select outside polygon points by mouse click, \nconfirm selection by any keyboard input" << std::endl;

    std::vector<std::vector<cv::Point>> outer_polygons;

    for (int i = 0; i < OUTER_POLYGON; i++){
        MouseParams outside_pram(img);
        std::cout << std::endl << "Now please create polygon # " << i+1 << std::endl;
        cv::setMouseCallback("Map display", onMouseHandle, (void*)&(outside_pram));
        cv::waitKey();
        std::cout << "Press a key to continue..." << std::endl;

        auto out_pts = outside_pram.points;
        //    draw polygon from points
        cv::fillConvexPoly(img, out_pts.data(), out_pts.size(), cv::Scalar(1, 1, 1));
        cv::polylines(img, out_pts, true, cv::Scalar(254,254,254),4);
        cv::imshow("Map display", img);
        outer_polygons.push_back(out_pts);
    }
    

    // Select inside polygon points by mouse click
    cv::Mat img2 = img.clone();
    std::cout << "You can create " << OBSTABLE_NUM << " obstacles" << std::endl;
    std::cout << std::endl << "Please select polygon points by mouse click, \nconfirm selection by any keyboard input" << std::endl;

    for (int i = 0; i < OBSTABLE_NUM; i++){
        MouseParams inside_pram(img2);
        std::cout << std::endl << "Now please create obstacle # " << i+1 << std::endl;
        cv::setMouseCallback("Map display", onMouseHandle, (void*)&(inside_pram));
        cv::waitKey();

        auto in_pts = inside_pram.points;
        //    draw polygon from points
        cv::fillConvexPoly(img2, in_pts.data(), in_pts.size(), cv::Scalar(254,254,254));
        // cv::polylines(img2, in_pts, true, cv::Scalar(254,254,254),4);
//        img2 = inside_pram.img;
        cv::imshow("Map display", img2);

    }


    // Select origin point by mouse click
    std::cout << std::endl << "Please select an origin point, \nconfirm selection by any keyboard input" << std::endl;
    MouseParams origin_pram(img2);
    auto img3 = origin_pram.img.clone();
    cv::setMouseCallback("Map display", onMouseHandle, (void*)&(origin_pram));
    cv::waitKey();

    auto origin_pt = origin_pram.points[0];

    // save the image
    std::string filename = "poly";
    // convert the image to grey scale
    cv::Mat grey_img;
    cv::cvtColor(img2, grey_img, cv::COLOR_BGR2GRAY);
    cv::imwrite("../data/"+filename+"grey.png", grey_img);

    // save the grey image in txt format
    std::ofstream outfile;
    outfile.open("../data/"+filename+".txt");

    outfile << "width: " <<  MAP_WIDTH << std::endl;
    outfile << "height: " << MAP_HEIGHT << std::endl;
    outfile << "resolution: 0.05" << std::endl;
    outfile << "origin: " << origin_pt.x << " " << origin_pt.y << std::endl;
    outfile << "state_grid: " << MAP_WIDTH*MAP_HEIGHT << std::endl;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            outfile << (int)grey_img.at<uchar>(i, j) << " ";
        }
        outfile << std::endl;
    }

    // save the outside polygon points as region identifier
    std::ofstream outfile2;
    int n = outer_polygons.size();
    outfile2.open(filename+"_regions.txt");
    outfile2 << "Regions: " << n << std::endl;
    for (auto &out_pts : outer_polygons){
        outfile2<< "Region1: " << out_pts.size()*2;
        for (auto pt : out_pts)
            outfile2 << " " << (pt.x - origin_pt.x)*RESOLUTION << " " << (origin_pt.y - pt.y)*RESOLUTION;
    }
        
    return 0;
}
