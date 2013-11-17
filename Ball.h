#pragma once
#include <cv.h>
#include <highgui.h>

#include "global_var.h"
#include "colors.h"
#include "main.h"
#include "contours.h"
#include "geometry.h"

using namespace  cv;
using namespace std;

class Ball {
    public:
        Mat *mask;
        Rect location;
        Vector3D pos;
        Point velocity;
        Point prev_velocity;
        Point center;
        Point prev_center;
        char color;
        void update( Mat &image );
        void FindCenter();
        Point predict_location(Point&, int, float );
        void calculate_velocity();
        Ball( Size image_size );
        Ball();
};
