#include "vision.h"
#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <math.h>

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

	int res_x = 80;
	int res_y = 80;

	/*
	int res_x = 1080;
	int res_y = 1900;
	*/

    Mat image;
    Mat image_result;
    image = imread(imageName, IMREAD_COLOR );   // Read the file
    // convert input into predefined size for processing
    resize(image, image, Size(res_y,res_x));
    resize(image, image_result, Size(res_y,res_x));
    // only using grey scale
    cvtColor(image, image, COLOR_BGR2GRAY);
    cvtColor(image_result, image_result, COLOR_BGR2GRAY);

    image_result = vision::test_gabor_filter(image, image_result, 0, 0, res_y, res_x);

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    //imshow( "Display window", image );                   // Show our image inside it.
    imshow( "Display window", image_result );                   // Show our image inside it.

    waitKey(0);

}

/*void resize_image(Mat image, int x_size, int y_size) {
	int interpolation = INTER_AREA;
	double factor = 0.0;

	Size dsize = Size(x_size, y_size);
	resize(image, image, dsize, factor, factor, interpolation);
	//resize(image, image, dsize);
}*/

int vision::gabor_filter(Mat image, int region_x, int region_y, int length_x, int length_y) {
	/*
	 * gabor rotation that most closely matches image sample region is assigned the
	 * gabor match result
	 *
	 * reference: https://stackoverflow.com/questions/7899108/opencv-get-pixel-channel-value-from-mat-image
	 */

	// gabor variables
	double sigmaX = 1.0;
	double sigmaY= 2.0;
	double k = (1.0/0.5); // preferred spatial frequency
	double phi = 0;//1-pi;//0; // preferred spatial phase
	//double a = 1.0/15.0;
	double pi = M_PI;
	double gabor = 0.0;
	double x, y;
	double total_rotations = 8.0;
	// opencv variables
	double pixel_brightness;
	double max_brightness;
	double similarity_score;
	double similarity_matches[(int) total_rotations];// = new double[8];
	double prior_similarity = 0.0;
	int highest_similarity = 0;

	for (int rotation = 0; rotation < total_rotations; rotation++) {
		similarity_score = 0;
		for (int i = region_x; i < length_x; i++) {
			for (int j = region_y; j < length_y; j++) {
				pixel_brightness = (double) image.at<uchar>(j,i);
				// compute gabor filter
				gabor = (1/(2*pi*sigmaX-sigmaY))*exp(((-(pow(x,2)))/(2*sigmaX,2))-(pow(y,2)/(2*sigmaY,2)))*cos((k*x)-phi);
				similarity_score = max_brightness = abs(pixel_brightness - gabor);
			}
		}
		// find closest match
		similarity_matches[rotation] = similarity_score;
		if (similarity_score > prior_similarity) {
			highest_similarity = rotation;
		}
		prior_similarity = similarity_score;
	}

	return highest_similarity;
}

Mat vision::test_gabor_filter(Mat image, Mat image_result, int region_x, int region_y, int length_x, int length_y) {
	/*
	 * gabor rotation that most closely matches image sample region is assigned the
	 * gabor match result
	 *
	 * reference: https://stackoverflow.com/questions/7899108/opencv-get-pixel-channel-value-from-mat-image
	 */

	// gabor variables
	double sigmaX = 5.0;
	double sigmaY= 10.0;
	double k = (1.0/1.5); // preferred spatial frequency
	double phi = 0;//1-pi;//0; // preferred spatial phase
	//double a = 1.0/15.0;
	double pi = M_PI;
	double gabor = 0.0;

	double x, y, x_p, y_p;
	double total_rotations = 8.0;
	double s_a = 1.0; // spatial aspect ratio
	double s_d = 1.0; // sigma/standard deviation of the Gaussian envelope
	double w_s = 1.0; // wavelength of the sinusoidal factor
	double p_o = 0.0; // phase offset
	double rot = 0.0; // rotation, orientation of the normal to the parallel stripes of a Gabor function
	// opencv variables
	double pixel_brightness;
	double max_brightness;
	double similarity_score;
	double similarity_matches[(int) total_rotations];// = new double[total_rotations];
	double prior_similarity = 0.0;
	int highest_similarity = 0;

	for (int rotation = 0; rotation < total_rotations; rotation++) {
		similarity_score = 0;
		for (int i = region_x; i < length_x; i++) {
			for (int j = region_y; j < length_y; j++) {
				pixel_brightness = (double) image.at<uchar>(j,i);
				x = i-40;
				y = j-40;
				// compute gabor filter
				gabor = (1/(2*pi*sigmaX*sigmaY))*exp(-(pow(x,2)/(2*pow(sigmaX,2)))-(pow(y,2)/(2*pow(sigmaY,2))))*cos((k*x)-phi);
				x_p = x*cos(rot)+y*sin(rot);
				y_p = -x*sin(rot)+y*cos(rot);
				//gabor = exp(-(pow(x_p,2)+pow(s_a,2)*pow(y_p,2))/(2*pow(s_d,2)))*cos((2*pi*(x_p/w_s))+p_o);
				similarity_score = max_brightness = abs(pixel_brightness - gabor);
				gabor = (gabor * 10000) + 130;//20;

				image_result.at<uchar>(j,i) = (int) gabor;//125;//(uchar) gabor;
				if ((int) gabor > 15) {
				//cout<<(int) gabor<<"\n";
				}
			}
		}
		// find closest match
		similarity_matches[rotation] = similarity_score;
		if (similarity_score > prior_similarity) {
			highest_similarity = rotation;
		}
		prior_similarity = similarity_score;
	}

	return image_result;
}


