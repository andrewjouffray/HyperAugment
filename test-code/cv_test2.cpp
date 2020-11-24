#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <random>


using namespace std;
using namespace cv;


template<typename T>
T random(T from, T to) {
        random_device                    rand_dev;
        mt19937                          generator(rand_dev());
        uniform_real_distribution<T>    distr(from, to);
	return distr(generator);
}


int main() {


	Mat src = imread("/home/andrew/Pictures/profile.png");

	// rows = height and cols = width


	float value4 = random<float>(0.50f, 0.55f);
	float value9 = random<float>(0.30f, 0.60f);
	float value8 = random<float>(0.30f, 0.80f);
	cout << "value4: " + to_string(value4) << endl;	
	cout << "value9: " + to_string(value9) << endl;
	cout << "value8: " + to_string(value8) << endl;
	// points2f means a 2d set of floating point sigle precision vatiables
	// source
    	Point2f srcTri[3];
    	srcTri[0] = Point2f( 0.f, 0.f );
    	srcTri[1] = Point2f( src.cols - 1.f, 0.f );
   	srcTri[2] = Point2f( 0.f, src.rows - 1.f );

	//destination
    	Point2f dstTri[3];
    	dstTri[0] = Point2f( 0.f, src.rows*value4 );
    	dstTri[1] = Point2f( src.cols*value9, src.rows*0.55f );
    	dstTri[2] = Point2f( src.cols*0.35f, src.rows*value8 );

	// create a wrap matrix
    	Mat warp_mat = getAffineTransform( srcTri, dstTri );
    	Mat warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    	warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

    	Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
    	double angle = -50.0;
    	double scale = 0.6;

    	Mat rot_mat = getRotationMatrix2D( center, angle, scale );

    	Mat warp_rotate_dst;
    	warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );
    	imshow( "Source image", src );
    	imshow( "Warp", warp_dst );
    	imshow( "Warp + Rotate", warp_rotate_dst );
    	waitKey();
    	return 0;


}
