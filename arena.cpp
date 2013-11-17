#include "arena.h"

Mat imageR;
Mat imageC;
int flag = 0,flagc=0;
int cnt = 0;
bool drawing_box = false;
bool sel = false;
Rect box;
Point2f point;

void draw_box( Mat img, Rect rect ){

	rectangle( img,rect,cvScalar(0xff,0x00,0x00) );
}

void draw_circle( Mat img, Point2f center, int radius ){

	circle( img,center,radius,cvScalar(0x00,0xff,0x00),CV_FILLED );
}

static void onMouseRect( int event, int x, int y, int, void* )
{
    switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			if( drawing_box ){
				box.width = x-box.x;
				box.height = y-box.y;
			}
			break;

		case CV_EVENT_LBUTTONDOWN:
			drawing_box = true;
			box = cvRect( x, y, 0, 0 );
			break;

		case CV_EVENT_LBUTTONUP:
			drawing_box = false;
			
			if( box.width < 0 ){
				box.x += box.width;
				box.width *= -1;
			}
			if( box.height < 0 ){
				box.y += box.height;
				box.height *= -1;
			}
			box.x+=1;
			box.y+=1;
			box.width-=1;
			box.height-=1;
			draw_box( imageR, box );
			
 			flag=1;
 			
			break;
		}
}

Rect selectRect( Mat argm )
{
    Mat image0 , ROI;
    Rect b;
    char c;
    
    argm.copyTo(image0);
    
    if( image0.empty() )
    {
       printf("Image empty\n");
        return Rect();
    }
 
    image0.copyTo(imageR);

    namedWindow( "image", 1 );
    setMouseCallback( "image", onMouseRect, 0 );
    
    while(1)
    {	
  
    	if(drawing_box)
    	{
    		draw_box(imageR,box);
    	}
    	
    	if(flag==1)
    	{
    		
    		ROI = imageR(box);
    		imshow("ROI",ROI);
    		c = waitKey(0);
    		if(c=='c')
    		{
    			break;
    		}
    		else
    		{
    			ROI.release();
    			destroyWindow("ROI");
    			image0.copyTo(imageR);
    			
    			flag = 0;
    		}
    	}
    	
    	imshow("image",imageR);
    	
    	if(waitKey(15)==27)
    	return Rect();
    	
    	image0.copyTo(imageR);
    	
     }	
     
     b = box;
    
    return b;
}

static void onMouseSelect( int event, int x, int y, int, void* )
{
    switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			{
				if(sel)
				point = Point2f((float)x,(float)y);
				break;
			}		
		case CV_EVENT_LBUTTONDOWN:
			{
				point = Point2f((float)x,(float)y);
				sel = true;
				break;
			}
		case CV_EVENT_LBUTTONUP:
			{
				sel = false;
				point = Point2f((float)x,(float)y);
				draw_circle(imageC,point,10);
				cnt++;
				flagc=1;
				break;
			}
		}
}

Point2f* selectPoint(Mat src)
{
    Point2f* points;
    points = (Point2f*)malloc(sizeof(Point2f)*4);
    char c;
    
    if( src.empty() )
    {
       printf("Image empty\n");
        return NULL;
    }
    
    src.copyTo(imageC);

    namedWindow( "SelectPoints", 1 );
    cnt = 0;
    setMouseCallback( "SelectPoints", onMouseSelect, 0 );
    
    while(cnt<5)
    {	
  
    	if(flagc==1)
    	{
    	
    		imshow("SelectPoints",imageC);
    		c = waitKey(0);
    		if(c=='c')
    		{
    			points[cnt-1] = point; 
    			printf("\ncnt:%d\n",cnt);
    			if(cnt == 4)
    			break;
    		}
    		else
    		{
    			cnt--;
    			printf("\ncnt:%d",cnt);
    			src.copyTo(imageC);
    			
    		}
    		flagc = 0;
            cout<<"point selection done"<<endl;
    	}
    	
    	imshow("SelectPoints",imageC);
    	
    	if(waitKey(15)==27)
    	    return NULL;
    	
    	
    	//src.copyTo(imageC);
    	
     }	
    printf("\nReturning\n");
    return points;

}

Mat getTransformMat( Point2f* srcq, Mat image )
{
	Point2f srcQuad[4],dstQuad[4];
	Mat warp_perspective;
	
	srcQuad[0] = srcq[0];
	srcQuad[1] = srcq[1];
	srcQuad[2] = srcq[2];
	srcQuad[3] = srcq[3];

	dstQuad[0] = Point2f(192.0/5.0,0.0);
	dstQuad[1] = Point2f(3008.0/5.0,0.0);
	dstQuad[2] = Point2f(3008.0/5.0,float(image.rows));
	dstQuad[3] = Point2f(192.0/5.0,float(image.rows));
	
	warp_perspective= getPerspectiveTransform(srcQuad,dstQuad);
	//warpPerspective(src, warped, warp_perspective, Size(640,480));

	return warp_perspective;
	
}

double m[3][3] = { {1.1657005628894042e+03, 0., 6.3950000000000000e+02}, { 0.,
1.1657005628894042e+03, 5.1150000000000000e+02} , { 0., 0., 1. }};
Mat cameraMatrix = cv::Mat(3, 3, CV_64F, m);

double n[5] = {-2.1954871319622427e-01, 1.5530276870537740e-01,
-1.9835716995890589e-03, -2.0422084872251836e-03,
2.5442808467893572e-02 };
Mat distCoeffs = cv::Mat(5, 1, CV_64F, n);

Mat map1,map2,map3,map4;


Mat radial_error_correction(Mat src)
{
	Mat view, rview;
	view = src.clone();
        
    Size imageSize = Size(src.cols, src.rows);
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),imageSize, CV_32FC1, map1, map2);

    remap(view, rview, map1, map2, INTER_LINEAR);
    map3 = Mat::zeros(map1.size(),CV_32FC1);
    map4 = Mat::zeros(map1.size(),CV_32FC1);
    cout << "map1cols" << map1.cols << ' ' << map1.rows << endl;
    for(int x=0;x<map1.cols;x++)
    {
       	for(int y=0;y<map1.rows;y++)
       	{
       		if(map2.at<float>(y,x)>=0 && map1.at<float>(y,x)>=0 && map1.at<float>(y,x)<map1.cols && map2.at<float>(y,x)<map2.rows)
       		{
//                circle( rview, Point((int)map1.at<float>(y,x),(int)map2.at<float>(y,x)), 1, CV_RGB( 255, 255, 255 ), 1, 8, 0 );
       			map3.at<float>((int)map2.at<float>(y,x),(int)map1.at<float>(y,x)) = x;
       			map4.at<float>((int)map2.at<float>(y,x),(int)map1.at<float>(y,x)) = y;
       		}
       	}
    }
    medianBlur(map3,map3,3);
    medianBlur(map4,map4,3);

	return rview;
}

Mat warp_matrix;

void find_perspective_transform(Point2f srcQuad[4])
{
    Point2f dstQuad[4];

    dstQuad[0] = Point2f(1280.*15./250.,0);
	dstQuad[1] = Point2f(1280.*235./250.,0);
	dstQuad[2] = Point2f(1280.*15./250.,1024);
	dstQuad[3] = Point2f(1280.*235./250.,1024);
			
	warp_matrix= getPerspectiveTransform(srcQuad,dstQuad);
}

Point find_transformed_point(Point src)
{
	Point temp1 = Point(map3.at<float>(src.y,src.x),map4.at<float>(src.y,src.x));
	Point warp_dst;

    warp_dst.x = (warp_matrix.at<double>(0,0)*temp1.x + warp_matrix.at<double>(0,1)*temp1.y + warp_matrix.at<double>(0,2))/(warp_matrix.at<double>(2,0)*temp1.x + warp_matrix.at<double>(2,1)*temp1.y + warp_matrix.at<double>(2,2));
            
    warp_dst.y = (warp_matrix.at<double>(1,0)*temp1.x + warp_matrix.at<double>(1,1)*temp1.y + warp_matrix.at<double>(1,2))/(warp_matrix.at<double>(2,0)*temp1.x + warp_matrix.at<double>(2,1)*temp1.y + warp_matrix.at<double>(2,2));
    
    return warp_dst;
}

Point find_inv_transformed_point(Point src)
{
	Point warp_dst;

    Mat warp_matrix_inv = warp_matrix.inv();
    warp_dst.x = (warp_matrix_inv.at<double>(0,0)*src.x + warp_matrix_inv.at<double>(0,1)*src.y + warp_matrix_inv.at<double>(0,2))/(warp_matrix_inv.at<double>(2,0)*src.x + warp_matrix_inv.at<double>(2,1)*src.y + warp_matrix_inv.at<double>(2,2));
            
    warp_dst.y = (warp_matrix_inv.at<double>(1,0)*src.x +warp_matrix_inv.at<double>(1,1)*src.y + warp_matrix_inv.at<double>(1,2))/(warp_matrix_inv.at<double>(2,0)*src.x + warp_matrix_inv.at<double>(2,1)*src.y + warp_matrix_inv.at<double>(2,2));
    
	Point sph_inv =
        Point(map1.at<float>(warp_dst.y,warp_dst.x),map2.at<float>(warp_dst.y,warp_dst.x));
    
    return sph_inv;
}
