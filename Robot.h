#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>

#include "colors.h"
#include "contours.h"
#include "main.h"
#include "geometry.h"
#include "global_var.h"

using namespace cv;
using namespace std;

class Robot{

    public:
        char basecolor;
		char lcolor;
		char rcolor;
        Mat* mask;
        Vector3D pos;
        Rect location;
        Point front_center, back_center, bot_center;
        Point prev_center;
		double distance;
		Point dst_point;
        Point velocity;
        double rotation;
        Robot( Mat &image);
        Robot();
        void FindCenter();
        double position();
        double orientation();
        void update(Mat &image);
        bool currentBall_with_bot_func(Point currentBall_pos);
        ~Robot();
};

extern Robot bot[NUM_OF_OUR_BOTS];
