#ifndef VISION_H
#define VISION_H

//#include "vision.h"
#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class vision
{
public:
    vision();
    ~vision();
    void stream_input();
    int gabor_filter(Mat image, int region_x, int region_y, int length_x, int length_y);
    Mat test_gabor_filter(Mat image, Mat image_result, int region_x, int region_y, int length_x, int length_y);
};

#endif // VISION_H
