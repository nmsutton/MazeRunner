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
	/*
	int res_x = 80;
	int res_y = 80;
	*/

	int res_x = 1080;
	int res_y = 1900;

    Mat image;
    Mat image_section;
    Mat image_result;
    Mat filtered_image;//(res_x, res_y, COLOR_BGR2GRAY);
    image = imread(imageName, IMREAD_COLOR );   // Read the file
    int visual_rows = 13;//4;//12;
    int visual_cols = 25;//4;//24;
    int sample_x = (int) res_x/12;//visual_rows;
    int sample_y = (int) res_y/24;//visual_cols;
    Size image_result_size;
    // convert input into predefined size for processing
    resize(image, image, Size(res_y,res_x));
    resize(image, image_result, Size(sample_x,sample_y));
    resize(image, filtered_image, Size(res_y,res_x));
    //filtered_image.size() = Size(res_x, res_y);
    //resize()
    // only using grey scale
    cvtColor(image, image, COLOR_BGR2GRAY);
    cvtColor(image_result, image_result, COLOR_BGR2GRAY);
    cvtColor(filtered_image, filtered_image, COLOR_BGR2GRAY);

    for (int vr = 0; vr < (visual_rows - 1); vr++) {
    	for (int vc = 0; vc < (visual_cols - 1); vc++) {
    		image_section = image(Range(sample_x*vr, sample_x*(vr+1)),Range(sample_y*vc, sample_y*(vc+1)));

    		image_result = vision::compare_gabor_filter(image_section, image_result, 0, 0, sample_x, sample_y);
    		image_result_size = image_result.size();//Size(sample_x, sample_y);//image_result.size();

        	image_result.copyTo(filtered_image(Rect(image_result_size.width*vc, image_result_size.height*vr, image_result_size.width, image_result_size.height)));
    		//image_result.copyTo(filtered_image(Rect(sample_x*vc, sample_y*vr, sample_x, sample_y)));
    	}

		cout<<"row "<<vr<<" processed\n";
    }

    //cout<<image;

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    //imshow( "Display window", image );                   // Show our image inside it.
    imshow( "Display window", filtered_image );                   // Show our image inside it.

    waitKey(0);


    cout<<"finished processing";
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
	double greatest_similarity = 0.0;
	//int highest_similarity = 0;
	//Point2f src_center;
	Mat rot_mat;
	Mat rotated_result;
	Mat gabor_match;
	Mat kernel1;

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
				similarity_score = similarity_score + abs(pixel_brightness - gabor);//max_brightness - abs(pixel_brightness - gabor);
			}
		}
		//cout<<similarity_score<<"\t"<<rot<<"\n";

		// find closest match
		//similarity_matches[rotation] = similarity_score;
		if (similarity_score > greatest_similarity) {
			//highest_similarity = rotation;

			//rotated_result.copyTo(gabor_match);
			//filter2D(image, gabor_match, CV_32F, rotated_result);
			int THETA_ROTATIONS = 4;
			double theta[THETA_ROTATIONS] ;
			theta[0] = 0;
			theta[1] = 45;
			theta[2] = 90;
			theta[3] = 135;
			/*theta[4] = 180;
			theta[5] = 225;
			theta[6] = 270;
			theta[7] = 315;*/
			double sig = 5;
			double lm = 8;//14;//8;
			double gm = .5;//(1.0/1.2);
			double ps = 0;
			for (int j = 0; j<THETA_ROTATIONS; j++)
			{
				kernel1 = cv::getGaborKernel(cv::Size(90,80), sig, theta[j], lm, gm, ps, CV_32F);
				filter2D(image, gabor_match, CV_64F, kernel1);
				greatest_similarity = similarity_score;
			}
			//cout<<"ss: "<<similarity_score<<" gs: "<<greatest_similarity<<" rot: "<<rot<<"\n";
		}
		//prior_similarity = similarity_score;
	}

	return gabor_match;
}

std::vector<double> vision::moves_detected()
{
	/*
	 * Generating a highly simplified velocity modulated current (I vel ) for testing
	 */
	double move_forward_linear = .1;
	int total_moves = 15;
	std::vector<double> detected_moves;
	for (int i = 0; i < total_moves; i++) {
		detected_moves.push_back(move_forward_linear);
	}
	return detected_moves;
}


