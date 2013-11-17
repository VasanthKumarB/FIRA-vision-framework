#include "Robot.h"

bool Robot::currentBall_with_bot_func(Point currentBall_pos)
{
    double dist = distanc(currentBall_pos,back_center);
    if(dist<23)
        return true;
    else
        return false;
}

Robot::Robot(){
    pos.x = 0, pos.y = 0, rotation = 0;
    mask = new Mat[3];
}

Robot::Robot(Mat &image){
    pos.x = 0, pos.y = 0, rotation = 0;
    mask = new Mat[3];
    mask[0] = Mat( Size(image.cols,image.rows), CV_8UC1 );
    mask[1] = Mat( Size(image.cols,image.rows), CV_8UC1 );
    mask[2] = Mat( Size(image.cols,image.rows), CV_8UC1 );
    cout << "image details our bot " << image.rows << image.cols << endl;
}

Robot::~Robot(){
    mask[0].release();
    mask[1].release();
    mask[2].release();
    delete mask;
}

double Robot::position(){
    Point center;
    center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );
}

double Robot::orientation(){
    rotation = angl( Point( 1280,back_center.y ), back_center, front_center );
    distance = sqrt(pow(bot_center.x-dst_point.x,2)+pow(bot_center.y-dst_point.y,2));
}

void Robot::FindCenter(){

    vector <int> Area_base;
    int Area_frontl=0;
    int Area_frontr=0;	
    vector <RotatedRect> BaseCenter = all_contours( mask[0], Area_base,0 );

    Point frontl_center = Point( 0, 0 );
    Point frontr_center = Point( 0, 0 );
    RotatedRect front_left;

    RotatedRect front_right;
    back_center = Point( 0, 0 );

    for( int i = 0; i < BaseCenter.size(); i++ ){

        front_left = closest_contour( mask[1], BaseCenter[i].center, Area_frontl );
        front_right = closest_contour( mask[2], BaseCenter[i].center, Area_frontr );

        cout << "front_left center = " << front_left.center << endl;
        if( front_left.center.x != 0 && front_right.center.x !=0 ){
            float angle_check =angl(front_right.center,BaseCenter[i].center,front_left.center); 
            if( //angle_check > 40 && angle_check < 80  &&
                    distanc(front_left.center,BaseCenter[i].center) < BOT_LENGTH && 
                    distanc(front_right.center,BaseCenter[i].center) < BOT_LENGTH )
            {
                back_center = BaseCenter[i].center;
                frontl_center = front_left.center;
                frontr_center = front_right.center;

                break;
            }	

        }
    }
    front_center = Point( ( frontl_center.x + frontr_center.x ) / 2, ( frontl_center.y + frontr_center.y ) / 2 );
}

void Robot::update(Mat &image){

    cout << "location = " << location << endl;
    Mat roi_image;
    roi_image = image(location);

    cvtColor(roi_image, roi_image, CV_BGR2HSV);

    mask[0] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);
    mask[1] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);
    mask[2] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);

    medianBlur(mask[0], mask[0], 3);
    medianBlur(mask[1], mask[1], 3);
    medianBlur(mask[2], mask[2], 3);
  
    pick_color( roi_image,mask[0], basecolor );
    pick_color( roi_image,mask[1], lcolor );
    pick_color( roi_image,mask[2], rcolor );

    imshow("mask0", mask[0]);

    cvtColor(roi_image, roi_image, CV_HSV2BGR);

    double ball_detection_time = (double)cvGetTickCount();
    FindCenter();

    cout << "find center = " << ((double)cvGetTickCount() -
            ball_detection_time)/(1000.0*(double)cvGetTickFrequency()) << '\t';
    //   cvtColor(roi_image, roi_image, CV_HSV2BGR);

    if( front_center.x != 0 && back_center.x != 0 ){
        //Taking care for the relative position changes due to ROI.
        prev_center = bot_center;
        front_center = Point( front_center.x + location.x, front_center.y + location.y );
        back_center = Point( back_center.x + location.x, back_center.y + location.y );
        bot_center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );
        update_location( location, bot_center );
        velocity = bot_center - prev_center;
    }
    else{
        expand_location( location );
    }
    Rect image_rect = Rect(0,0,image.cols, image.rows);
    limit_location_within_arena( location, image_rect );

    position();
    orientation();
}


