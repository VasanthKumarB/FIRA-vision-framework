#include "colors.h"

Scalar green_l(55,100,100),green_u(70,255,255);
Scalar blue_l(95,180,180),blue_u(110,255,255);
Scalar violet_l(13,50,120),violet_u(23,220,255);
Scalar yellow_l(25,120,100),yellow_u(35,255,255);
Scalar orange_l(3,100,100),orange_u(13,255,255);

void pick_color( Mat &image, Mat &mask, char color ){
    int y,x;

    switch( color ){

        case 'g' :
            mask = Scalar(0);
			inRange(image,green_l,green_u,mask);
            break;

        case 'v' :
            mask = Scalar(0);
			inRange(image,violet_l,violet_u,mask);
            break;

        case 'o' :
            mask = Scalar(0);
            inRange(image,orange_l,orange_u,mask);
            break;

        case 'b' :
            mask = Scalar(0);
            cout << "blue l " << blue_l << blue_u << endl;
            inRange(image,blue_l,blue_u,mask);
            break;

        case 'y' :
            mask = Scalar(0);
            double ball_detection_time = (double)cvGetTickCount();
            inRange(image, yellow_l, yellow_u,mask);

//            cout << "time inrange = " << ((double)cvGetTickCount() -
//                    ball_detection_time)/(1000.0*(double)cvGetTickFrequency()) << '\n';
//
//            ball_detection_time = (double)cvGetTickCount();
//            
//            Vec3b Pixel;
//            for( y = 0; y < image.rows; y++ ){
//                for( x = 0; x < image.cols; x++ ){
//                    Pixel = image.at<Vec3b>(y,x);
//                    mask.at<uchar>(y,x) = 255 * 
//                        ( yellowLUT_bool[0][Pixel.val[0]] &&
//                          yellowLUT_bool[1][Pixel.val[1]] && yellowLUT_bool[2][Pixel.val[2]] );
//                }
//            }
//

            break;
    }

}
