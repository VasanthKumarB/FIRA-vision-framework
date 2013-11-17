#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

Scalar blue_l(80,120,100),blue_u(90,255,255);


int main(int argc, char **argv) {

    Mat yellowLUT = Mat::zeros(1,256, CV_8UC3);

    Mat point = Mat::zeros(1,1,CV_8UC3);
   
    Vec3b Pixel;

    for(int i = blue_l.val[0]; i <= blue_u.val[0]; i++)
        for (int j = blue_l.val[1]; j <= blue_u.val[1]; j++) {
            for (int k = blue_l.val[2]; k <= blue_u.val[2]; k++) {
                point.at<Vec3b>(0,0)[0] = i; 
                point.at<Vec3b>(0,0)[1] = j;
                point.at<Vec3b>(0,0)[2] = k;
                
                Pixel = point.at<Vec3b>(0, 0);
                
                yellowLUT.at<Vec3b>(0,Pixel.val[0])[0] = 255;
                yellowLUT.at<Vec3b>(0,Pixel.val[1])[1] = 255;
                yellowLUT.at<Vec3b>(0,Pixel.val[2])[2] = 255;
            }
        }

    imwrite("blueLUT.jpg", yellowLUT);

    return 0;
}
