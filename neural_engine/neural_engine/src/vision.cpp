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

    image_result = vision::compare_gabor_filter(image, image_result, 0, 0, res_y, res_x);

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    //imshow( "Display window", image );                   // Show our image inside it.
    imshow( "Display window", image_result );                   // Show our image inside it.

    waitKey(0);

}

Mat vision::compare_gabor_filter(Mat image, Mat image_result, int region_x, int region_y, int length_x, int length_y) {
	/*
	 * gabor rotation that most closely matches image sample region is assigned the
	 * gabor match result
	 *
	 * reference: https://stackoverflow.com/questions/7899108/opencv-get-pixel-channel-value-from-mat-image
	 */

	// gabor variables
	double sigmaX = 7.0;
	double sigmaY= 14.0;
	double k = (1.0/1.2); // preferred spatial frequency
	double phi = 0;//1-pi;//0; // preferred spatial phase
	double pi = M_PI;
	double gabor = 0.0;
	double x, y;
	double total_rotations = 8.0;
	double rot = 0.0; // rotation, orientation of the normal to the parallel stripes of a Gabor function
	double offset = 40; // offset to shift gabor from center at 0 to center + and offset on x and y
	// opencv variables
	double pixel_brightness;
	double max_brightness = 255;
	double similarity_score;
	double similarity_matches[(int) total_rotations];// = new double[total_rotations];
	double prior_similarity = 0.0;
	int highest_similarity = 0;
	//Point2f src_center;
	Mat rot_mat;
	Mat rotated_result;
	Mat gabor_match;

	for (int rotation = 0; rotation < total_rotations; rotation++) {
		similarity_score = 0;

		for (int i = region_x; i < length_x; i++) {
			for (int j = region_y; j < length_y; j++) {
				pixel_brightness = (double) image.at<uchar>(j,i);
				x = i-offset;
				y = j-offset;

				// compute gabor filter
				gabor = (1/(2*pi*sigmaX*sigmaY))*exp(-(pow(x,2)/(2*pow(sigmaX,2)))-(pow(y,2)/(2*pow(sigmaY,2))))*cos((k*x)-phi);

				// Exponent used below to enhance contrast
				gabor = pow((gabor * 10000),1.99) + 0;//20;

				image_result.at<uchar>(j,i) = (int) gabor;//125;//(uchar) gabor;

				/*if ((int) gabor > 15) {
					cout<<(int) gabor<<"\n";
				}*/
			}
		}
		// rotate
		rot = rotation * (360/total_rotations);
		Point2f src_center(image_result.cols/2.0F, image_result.rows/2.0F);
		rot_mat = getRotationMatrix2D(src_center, rot, 1.0);
		warpAffine(image_result, rotated_result, rot_mat, image_result.size());

		// score
		for (int i = region_x; i < length_x; i++) {
			for (int j = region_y; j < length_y; j++) {
				gabor = (double) rotated_result.at<uchar>(j, i);
				similarity_score = similarity_score + max_brightness - abs(pixel_brightness - gabor);
			}
		}
		cout<<similarity_score<<"\t"<<rot<<"\n";

		// find closest match
		similarity_matches[rotation] = similarity_score;
		if (similarity_score > prior_similarity) {
			highest_similarity = rotation;
			gabor_match = rotated_result;
		}
		prior_similarity = similarity_score;
	}

	return gabor_match;
}


