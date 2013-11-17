#include "Ball.h"

Ball::Ball() {
    color = 'o';
    center = Point( 0, 0 );
    mask = new Mat[1];
}

Ball::Ball( Size image_size ){
    color = 'o';
    center = Point( 0, 0 );
    location = Rect(0,0,image_size.width, image_size.height);
    mask = new Mat[1];
    mask[0] = Mat( image_size, CV_8UC1 );
}

void Ball::FindCenter(){	
    int counter = 0;
    RotatedRect Ball_rrect = largest_contour( mask[0], counter );
    if( Ball_rrect.center.x != 0 )
        center = Ball_rrect.center;
    else
        center = Point(0,0);

}

void Ball::calculate_velocity()
{
    prev_velocity = velocity;
    if(center.x != 0 && prev_center.x != 0) {
        velocity.x = center.x - prev_center.x;
        velocity.y  = center.y - prev_center.y;
    }
    else {
        velocity = Point(1000, 1000);
    }
}

Point Ball::predict_location(Point &velocity_temp, int t, float k) {
    velocity_temp = k*(velocity) + float(1.0-k)*velocity_temp;
    Point ret = Point(center.x, center.y);
    for(; t>0; t--) {
        ret += Point(velocity_temp.x , velocity_temp.y);
    }
    return ret;
}

void Ball::update( Mat &image ){

    Mat roi_image = image(location);

    GaussianBlur( roi_image, roi_image, Size( 3, 3 ), 0, 0 );

    cvtColor(roi_image, roi_image, CV_BGR2HSV);

    mask[0] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);

    pick_color( roi_image, mask[0], color );
    imshow("Ball mask", mask[0]);

    prev_center = center;

    FindCenter();

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

    calculate_velocity();

}

