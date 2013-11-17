#pragma once
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "global_var.h"

using namespace cv;
using namespace std;

void draw_box( Mat img, Rect rect );
void draw_circle( Mat img, Point2f center, int radius );
static void onMouseRect( int event, int x, int y, int, void* );
Rect selectRect( Mat argm );
static void onMouseSelect( int event, int x, int y, int, void* );
Point2f* selectPoint(Mat src);
Mat getTransformMat( Point2f* srcq, Mat image );
Mat perspectiveArena(Mat warp_Mat,Mat src);
Mat radial_error_correction(Mat src);
void find_perspective_transform(Point2f srcQuad[4]);
Point find_transformed_point(Point src);
Point find_inv_transformed_point(Point src);

