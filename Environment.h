#pragma once
#include <iostream>
#include <boost/thread/thread.hpp>
#include <highgui.h>
#include <cv.h>
#include <math.h>
#include "global_var.h"
#include "geometry.h"
#include "Robot.h"
#include "arena.h"
#include "main.h"
#include "OpponentRobot.h"
#include "Ball.h"

using namespace cv;
using namespace std;

class Environment {

#ifdef Camera
    Ximea cap;
#else
    VideoCapture cap;
#endif

    public:
        Mat image;
        Point arena_center;
        Mat image_mat;
        Size image_size;
    
        Robot home[NUM_OF_OUR_BOTS];
        OpponentRobot opponent[NUM_OF_OPP_BOTS];
        Ball currentBall;
        Vector3D predictedBall;
        Bounds fieldBounds, goalBounds;
        long gameState;
        long whosBall;
    
        void init_image(bool load_pts_file);
    
        template <class Device>
            void init(Device arg, bool load_pts_file) {
                cap.open(arg);
                init_image(load_pts_file);
            }
    
        void updateframe();
        void Text(string text);
        void update_Robot();
        void update_OpponentRobot();
        void display();
};



