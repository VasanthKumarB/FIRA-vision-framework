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

class Environment;

class OpponentRobot {
    public:
        static int total_no_of_objects;
        Mat *mask;
        Rect location;
        Point center;
        Vector3D pos;
        char color;
        void update(Mat &image, Environment *);
        void FindCenter(OpponentRobot *);
        OpponentRobot(Mat &image);
        OpponentRobot();
        int OBJECT_NUM;
};
