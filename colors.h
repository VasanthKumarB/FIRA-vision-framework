#pragma once
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <vector>
#include "common.h"
#include "global_var.h"

using namespace cv;
using namespace std;

extern Mat yellowLUT;
extern bool yellowLUT_bool[3][256];

void pick_color( Mat &image, Mat &mask, char color );
void pick_basecolor( Mat mask, CvRect location, char color );
void pick_frontcolor( Mat mask, CvRect location, char color );

