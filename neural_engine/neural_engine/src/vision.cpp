#include "vision.h"
#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

vision::vision()
{
}

vision::~vision()
{
}

void vision::stream_input() {

	String imageName("../sample_views/Screenshot from 2017-07-07 21-25-07.png"); // by default

    Mat image;
    image = imread(imageName, IMREAD_COLOR );   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);

}

