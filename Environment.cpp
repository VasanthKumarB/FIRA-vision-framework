#include "Environment.h"

Point2f* pts;

void Environment::init_image( bool load_pts_file) {

    image = Mat::zeros(cap.get(4),cap.get(3),CV_8UC3);
    arena_center = Point(image.cols/2,image.rows/2);

    for(int i=0; i < NUM_OF_OUR_BOTS; i++) {
        home[i].location = Rect(0, 0, image.cols, image.rows);
    }

    for(int i=0; i < NUM_OF_OPP_BOTS; i++) {
        opponent[i].location = Rect(0, 0, image.cols, image.rows);
    }
    currentBall.location = Rect(0, 0, image.cols, image.rows);

    pts = new Point2f [4];

    FILE *pFile;
    if(load_pts_file) {
        pFile = fopen("arena_pts.txt", "r");
        for( int m = 0; m < 4; m++ ) {
            fscanf(pFile, "%f %f\n", &pts[m].x, &pts[m].y);
        }
        cap >> image;
        Mat rview = radial_error_correction(image);

    }
    else
    {
        cap >> image;
        pts = selectPoint(image);
        radial_error_correction(image);
        pFile = fopen("arena_pts.txt", "w+");
        for( int m = 0; m < 4 ; m++) {
            fprintf(pFile, "%d %d\n", int(pts[m].x), int(pts[m].y));
        } 
        cout << "file writing complete" << endl;
    }

    fclose(pFile);
    find_perspective_transform(pts);
    destroyAllWindows(); 

    namedWindow("image", CV_WINDOW_NORMAL);
}

void Environment::update_Robot()
{
    for( int i=0;i < NUM_OF_OUR_BOTS;i++)
        home[i].update(image);
}

void Environment::update_OpponentRobot()
{
    for( int i=0;i < NUM_OF_OPP_BOTS;i++)
        opponent[i].update(image, this);
}

void Environment::updateframe(){
    double time_for_loop = (double)cvGetTickCount();

    // getting image from camera
    cap >> image;

    double time_bot_update = (double)cvGetTickCount();

    update_Robot();
    update_OpponentRobot();

    time_bot_update = ((double)cvGetTickCount() - time_bot_update)/(1000.0*(double)cvGetTickFrequency());

    double currentBall_detection_time = (double)cvGetTickCount();
}

void Environment::Text(string text) {

    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontScale = 2;
    int thickness = 3;

    int baseline=0;
    Size textSize = getTextSize(text, fontFace,
            fontScale, thickness, &baseline);
    baseline += thickness;

    // center the text
    Point textOrg(image.cols-textSize.width,textSize.height);

    // draw the box
    rectangle(image, textOrg + Point(0, baseline),
            textOrg + Point(textSize.width, -textSize.height),
            Scalar(0,0,255));
    // ... and the baseline first
    line(image, textOrg + Point(0, thickness),
            textOrg + Point(textSize.width, thickness),
            Scalar(0, 0, 255));

    // then put the text itself
    putText(image, text, textOrg, fontFace, fontScale,
            Scalar(0,0,255), thickness, 8);   
}

void Environment::display() {

    circle( image, arena_center, 10, CV_RGB( 180, 180, 255 ), -1, 8, 0 );

    for( int i = 0; i < NUM_OF_OUR_BOTS; i++ ){
        circle( image, home[i].front_center, 5, CV_RGB( 255, 0, 0 ), -1, 8, 0 );
        circle( image, home[i].back_center, 2, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
        line( image, home[i].front_center, home[i].back_center,
                CV_RGB( 255, 255, 255 ), 2, 8, 0);
        rectangle( image, Point( home[i].location.x, home[i].location.y ),
                Point( home[i].location.x + home[i].location.width, home[i].location.y + home[i].location.height ),
                Scalar( 255, 0, 0, 0 ), 1, 4, 0 );
    }

    for( int i = 0; i < NUM_OF_OPP_BOTS; i++ ){
        circle( image, opponent[i].center, 5, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
        rectangle( image, Point( opponent[i].location.x, opponent[i].location.y ),
                Point( opponent[i].location.x + opponent[i].location.width, opponent[i].location.y + opponent[i].location.height ),
                Scalar( 0, 0, 255, 0 ), 1, 4, 0 );
    }

    circle( image, currentBall.center, 3, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
    circle( image, currentBall.center, 40, CV_RGB( 255, 255, 255 ), 2, 8, 0 );

    circle( image, find_inv_transformed_point(find_transformed_point(pts[0])), 20, CV_RGB( 255, 255, 255 ), 2, 8, 0 );
    circle( image, find_inv_transformed_point(find_transformed_point(pts[1])), 20, CV_RGB( 255, 255, 255 ), 2, 8, 0 );
    circle( image, find_inv_transformed_point(find_transformed_point(pts[2])), 20, CV_RGB( 255, 255, 255 ), 2, 8, 0 );
    circle( image, find_inv_transformed_point(find_transformed_point(pts[3])), 20, CV_RGB( 255, 255, 255 ), 2, 8, 0 );

    imshow("image",image);

    c = waitKey(1);
    if(c == ' ')
        c=waitKey(0);
}
