#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>

#include "arena.h"
#include "global_var.h"
#include "Environment.h"

using namespace cv;
using namespace std;

bool currentBall_with_bot;
int frame = 0;

Environment env; 

char c;

Mat yellowLUT;
bool yellowLUT_bool[3][256];

int main(int argc,char** argv)
{
    Point destinations[10];
    int no_of_dest = 2;
    int count_of_dest = 0;
    float tol=30;

    bool load_pts_file = true;
    if(argc >= 2 && strcmp(argv[1],"f") == 0)
        load_pts_file = false;

#ifdef Camera
    env.init(0, load_pts_file);
#else
    env.init(argv[2], load_pts_file);
#endif

    yellowLUT = imread( "../calibrator/yellowLUT.png" , 1);

    for( int y = 0; y < yellowLUT.rows; y++ ){
        for( int x = 0; x < yellowLUT.cols; x++ ){
            yellowLUT_bool[0][x] = (bool)yellowLUT.at<Vec3b>(0,x)[0];
            cout << yellowLUT_bool[0][x] << ' ';
        }
    }
    cout << endl;
    for( int y = 0; y < yellowLUT.rows; y++ ){
        for( int x = 0; x < yellowLUT.cols; x++ ){
            yellowLUT_bool[1][x] = (bool)yellowLUT.at<Vec3b>(0,x)[1];
            cout << yellowLUT_bool[1][x] << ' ';
        }
    }
    cout << endl;
    for( int y = 0; y < yellowLUT.rows; y++ ){
        for( int x = 0; x < yellowLUT.cols; x++ ){
            yellowLUT_bool[2][x] = (bool)yellowLUT.at<Vec3b>(0,x)[2];
            cout << yellowLUT_bool[2][x] << ' ';
        }
    }
    cout << endl;

    int f=0;

    env.home[0].basecolor = 'b';
    env.home[0].lcolor = 'g';
    env.home[0].rcolor = 'g';
    env.currentBall.color = 'o';

    double time_for_loop = 0;
    int angle_buffer=0;
    int min_velocity = 50;
    int velocity = 60;
    int max_velocity = 110;
    int vel_count = 0;

//    VideoWriter out("go_to_a_point.avi",CV_FOURCC('M','J','P','G'),40, Size(1280,1024));

    int prev_bot_angle = 0;
    queue<Point> prediction;
    queue<Point> vel;
    queue<Point> cen;
    int n = 1;
    Point velocity_exp;
    Point velocity_lin;
    bool reached = false;

    while( c != 27 )
    {
        frame++;

        destinations[0] = env.currentBall.center; //for env.currentBall

        if(count_of_dest == no_of_dest || currentBall_with_bot == 0)
            count_of_dest = 0;

        if(env.currentBall.velocity.x != 1000 && env.currentBall.prev_velocity.x != 1000) {
            prediction.push(env.currentBall.predict_location(velocity_exp,n, 0.51));
            vel.push(env.currentBall.predict_location(velocity_lin,n, 1.0));
            cen.push(env.currentBall.center);
        }
        env.updateframe();

        if(prediction.size() > n) {
            circle(env.image,prediction.front(),5,Scalar(0,0,255),-1);
            circle(env.image,vel.front(),5,Scalar(255,0,0),-1);
            circle(env.image,Point(cen.front()),15,Scalar(255,0,0),1);
            cen.pop();
            vel.pop();
            prediction.pop();
        } 

        env.display();

        c = waitKey(1);
    }

    destroyAllWindows();
}
