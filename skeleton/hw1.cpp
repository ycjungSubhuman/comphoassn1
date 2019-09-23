#include <iostream>
#include <algorithm>
#include <limits>
#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Vec2i align(
		const Mat1f& I1,
		const Mat1f& I2,
		int search_window_radius)
{
	Vec2i best_translation(0,0);
	// TODO
	//  Implement single-scale alignment search
	return best_translation;
}

Vec2i align_multiscale(
		const Mat1f& I1,
		const Mat1f& I2,
		int search_window_radius)
{
	Vec2i best_translation(0,0);
	// TODO
	//  Implement multi-scale alignment search
	return best_translation;
}

int main()
{
	const char* imname = "00892u.tif";

	Mat fullim0 = imread(imname, IMREAD_UNCHANGED);
	int im_type = fullim0.type();

	Mat1f fullim;

	if(im_type == CV_8UC1) {
		fullim0.convertTo(fullim, CV_32F, 1./255.);
	}
	else {
		fullim0.convertTo(fullim, CV_32F, 1./65535.);
	}
	// compute the height of each part (just 1/3 of total)
	int height = fullim.rows / 3;

	// separate color channels
	Mat1f B = fullim(Rect(0,0,fullim.cols,height));
	Mat1f G = fullim(Rect(0,height,fullim.cols,height));
	Mat1f R = fullim(Rect(0,height*2,fullim.cols,height));

	// crop image boundaries
	int c = 300;
	Mat1f B_sub = B(Rect(c,c,B.cols-2*c,B.rows-2*c));
	Mat1f G_sub = G(Rect(c,c,B.cols-2*c,B.rows-2*c));
	Mat1f R_sub = R(Rect(c,c,B.cols-2*c,B.rows-2*c));

	// align images
	Vec2i aG = align(G_sub, B_sub, 128);
	Vec2i aR = align(R_sub, B_sub, 128);
	cout << aG << endl;
	cout << aR << endl;

	// create a color image (3D array)
	int crop_x = max(abs(aG[1]), abs(aR[1]));
	int crop_y = max(abs(aG[0]), abs(aR[0]));
	B = B(Rect(crop_x, crop_y, B.cols-2*crop_x, B.rows-2*crop_y));
	G = G(Rect(crop_x - aG[1], crop_y - aG[0], G.cols-2*crop_x, G.rows-2*crop_y));
	R = R(Rect(crop_x - aR[1], crop_y - aR[0], R.cols-2*crop_x, R.rows-2*crop_y));
	Mat1f BGR[] = {B,G,R};
	Mat3f colorim;
	merge(BGR, 3, colorim);

	Mat tmp;
	if(im_type == CV_8UC1) {
		colorim.convertTo(tmp,CV_8U,255.);
	}
	else {
		colorim.convertTo(tmp,CV_16U,65535.);
	}
	imwrite(format("result_%s", imname), tmp);

	return 0;
}

