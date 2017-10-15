#ifndef VISION_H
#define VISION_H

//#include "vision.h"
#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

class vision
{
public:
    vision();
    ~vision();
    void stream_input();
    Mat compare_gabor_filter(Mat image, Mat image_result, int region_x, int region_y, int length_x, int length_y);
    std::vector<double> moves_detected();
    //std::vector<double> detected_moves;
};

#endif // VISION_H
