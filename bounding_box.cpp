#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>
#include <utility>

using namespace std;
using namespace cv;

int size_x,size_y;
Mat image,image_grey;
float bound_height,bound_width;


int main( int argc, char** argv )
{
  	image = imread( argv[1], 1 );
  	
  	if( argc != 2 || !image.data )
  	{
      	printf( "No image data \n" );
      	return -1;
  	}
  	
  	cvtColor(image, image_grey, CV_RGB2GRAY);
  	namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  
  	Size s = image_grey.size();
  	size_x = s.height;
  	size_y = s.width;
  	int pixel_count=0;
  	int min_x=size_x,min_y=size_y,max_x=0,max_y=0;
  	
  	for(int x=0;x<size_x;x++)
  	{
		for(int y=0;y<size_y;y++)
		{
		    image_grey.at<uchar>(x,y)=(image_grey.at<uchar>(x,y)==255?255:0);
		    
		    if(!image_grey.at<uchar>(x,y))
		    {
		    	pixel_count++;
		        min_x=min(min_x,x);
		        max_x=max(max_x,x);
		        min_y=min(min_y,y);
		        max_y=max(max_y,y);
		    }
    	}
  	}
  	
  	bound_height=max_x-min_x+1;
  	bound_width=max_y-min_y+1;
  	
  	long double AR=bound_width/bound_height;
  	
  	waitKey(0);

  	return 0;
}
