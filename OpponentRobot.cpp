#include "OpponentRobot.h"
#include "Environment.h"

int OpponentRobot::total_no_of_objects;

OpponentRobot::OpponentRobot(){
    OBJECT_NUM = total_no_of_objects++;
    color = OPP_JERSEY_COLOR;
    center = Point( 0, 0 );
    mask = new Mat[1];
}

OpponentRobot::OpponentRobot( Mat &image ){
    OBJECT_NUM = total_no_of_objects++;
    color = OPP_JERSEY_COLOR;
    center = Point( 0, 0 );
    location = Rect(0,0,image.cols, image.rows);
    mask = new Mat[1];
    mask[0] = Mat( Size(image.cols, image.rows), CV_8UC1 );
}

void OpponentRobot::FindCenter(OpponentRobot *opponent){	
    vector <int> area;
    vector <RotatedRect> opp_jersey_rrect = all_contours( mask[0], area, 0 );
    center = Point ( 0, 0 );
    int count = 0;
    for( int i = 0 ; i < opp_jersey_rrect.size() ; i++ ){
        count = 0;
        for( int j = 0 ; j < OBJECT_NUM ; j++ ){
            if( (abs(int(opp_jersey_rrect[i].center.x + 0.5) +
                int(this->location.x + 0.5) - opponent[j].center.x) > 2 ) or
                (abs(int(opp_jersey_rrect[i].center.y + 0.5) +
                int(this->location.y + 0.5) - opponent[j].center.y) > 2) ){
                count++;
            }
        }
        if( count == OBJECT_NUM ){
            center = opp_jersey_rrect[i].center;
            break;
        }
    }
}

void OpponentRobot::update(Mat &image, Environment *env){

    Mat roi_image;
    image(location).copyTo(roi_image);
    cvtColor(roi_image, roi_image, CV_BGR2HSV);

    mask = new Mat[1];
    mask[0] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);

    pick_color( roi_image, mask[0], color );
    medianBlur(mask[0], mask[0], 3);
    imshow("opp bot mask", mask[0]);
    FindCenter(env->opponent);

    cvtColor(roi_image, roi_image, CV_HSV2BGR);

    if( center.x != 0 ){
        center = Point( center.x + location.x, center.y + location.y );
        update_location( location, center );
    }
    else{
        expand_location( location );
    }

    Rect image_rect = Rect(0,0,image.cols, image.rows);
    limit_location_within_arena( location, image_rect );

}
