#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

class bot {
    Rect front_left_rect;
    Rect front_right_rect;
    Rect back_rect;
    char front_left_color;
    char front_right_color;
    char back_color;
};

Rect rect;
Mat imagen,screenBuffer;

int x, y, last_x, last_y, drawing;

VideoCapture cap;

inline void draw( int x, int y ){
    imagen.copyTo( screenBuffer );	
    rectangle( screenBuffer, cvPoint(last_x,last_y), cvPoint( x, y ),
            CV_RGB( 255, 0, 0 ), 1 );
}

void on_mouse( int event, int x, int y, int flags, void* param )
{
    //Select mouse Event
    if( event == CV_EVENT_LBUTTONDOWN ){
        last_x = x;
        last_y = y;
        rect.width = x-last_x;
        rect.height = y-last_y;
        rect.x = last_x;
        rect.y = last_y;
        drawing = 1;
    }
    else if( event == CV_EVENT_LBUTTONUP ){
        if( drawing ){
            drawing = 0;
            rect.width = x - last_x;
            rect.height = y - last_y;
        }
    }
    else if( event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON ){
        if( drawing )
            draw( x, y );
    }
}

Rect select_rect( Mat image )
{	
    char c;
    image.copyTo(imagen);
    namedWindow("image",1);
    do{
        if(cap.isOpened())
            cap >> imagen;
        imagen.copyTo( screenBuffer );
        while( 1 ){
            setMouseCallback( "image", on_mouse, NULL);
            imshow( "image", screenBuffer );
            c = waitKey( 10 );
            if( c == 27 ) {
                return Rect();
                break;
            }
            else if( c =='r') {
                break;
            }
            else if( c=='s' ){
                destroyAllWindows();
                return rect;
            }
        }
    }while( c == 'r' );
}

int main(int argc, char **argv) {
    cap.open(argv[1]);

    Mat image, imageHSV;
    Mat yellowLUT;

    yellowLUT = Mat::zeros(5,256, CV_8UC3);

    Mat point = Mat::zeros(1,1,CV_8UC3);

    char c = 0;
    while(waitKey(25) != 's') {
        cap >> image;
        imshow("img",image);
    }

    while( c != 27 ) { 
        Rect rect = select_rect(image);

        GaussianBlur( image, image, Size( 3, 3 ), 0, 0, BORDER_DEFAULT );
        cvtColor(image, image, CV_BGR2HSV);

        Vec3b Pixel;
        for(int i = rect.x; i<rect.x + rect.width; i++) {
            for(int j=rect.y; j<rect.y + rect.height; j++) {
                Pixel = image.at<Vec3b>(j, i);
                yellowLUT.at<Vec3b>(0,Pixel.val[0])[0] = 255;
                yellowLUT.at<Vec3b>(0,Pixel.val[1])[1] = 255;
                yellowLUT.at<Vec3b>(0,Pixel.val[2])[2] = 255;
                yellowLUT.at<Vec3b>(1,Pixel.val[0])[0] = 255;
                yellowLUT.at<Vec3b>(1,Pixel.val[1])[1] = 255;
                yellowLUT.at<Vec3b>(1,Pixel.val[2])[2] = 255;
                yellowLUT.at<Vec3b>(2,Pixel.val[0])[0] = 255;
                yellowLUT.at<Vec3b>(2,Pixel.val[1])[1] = 255;
                yellowLUT.at<Vec3b>(2,Pixel.val[2])[2] = 255;
                yellowLUT.at<Vec3b>(3,Pixel.val[0])[0] = 255;
                yellowLUT.at<Vec3b>(3,Pixel.val[1])[1] = 255;
                yellowLUT.at<Vec3b>(3,Pixel.val[2])[2] = 255;
                yellowLUT.at<Vec3b>(4,Pixel.val[0])[0] = 255;
                yellowLUT.at<Vec3b>(4,Pixel.val[1])[1] = 255;
                yellowLUT.at<Vec3b>(4,Pixel.val[2])[2] = 255;
            }
        }

    imshow("img", image);
    c = waitKey(0);
    }
    GaussianBlur(yellowLUT, yellowLUT, Size(29,29), 0,0,BORDER_DEFAULT );
    GaussianBlur(yellowLUT, yellowLUT, Size(29,29), 0,0,BORDER_DEFAULT );
    GaussianBlur(yellowLUT, yellowLUT, Size(29,29), 0,0,BORDER_DEFAULT );
    GaussianBlur(yellowLUT, yellowLUT, Size(29,29), 0,0,BORDER_DEFAULT );

    Rect src_rect = Rect(0,3,256,1);
    yellowLUT = yellowLUT(src_rect);
    imwrite("yellowLUT.png", yellowLUT);

    return 0;
}
