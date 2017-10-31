
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>
#include <utility>
#include <string.h>
using namespace std;
using namespace cv;

#define pi (22/7)

int size_x,size_y;
Mat image,image_grey;
float bound_height,bound_width;

int h=0,b=0,length=0,height=0;
int **binaryImage;


//-------------------------------------------------------------------------------

void load_image()
{
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<b;j++)
		{
			binaryImage[i][j]=(image_grey.at<uchar>(i,j)==255 ? 1:0);
		}
	}

return;
}


void print_skeleton_image()
{

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<b;j++)
		{
			cout<<binaryImage[i][j];
		}
	}

return;
}

int find_B_P1(int x, int y)
{
int count=0; 

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(i==1&&j==1)
				continue;
			else
			{
				if((binaryImage[i+x][j+y]==1))
				count++;
			}
		}
	}

return count;

}

int isvalid(int x,int y)
{
	int valid=1;

	if(x<0||y<0||x>height||y>length)
	valid=0;
	
	return valid;
}

int find_A_P1(int x, int y)
{
    int count = 0;
        
		//p2 p3
        
		if(isvalid(x+1,y-1) && binaryImage[y-1][x]==0 && binaryImage[y-1][x+1]==1 )
		count++;
        
		//p3 p4
        
		if(isvalid(x+1,y-1) && binaryImage[y-1][x+1]==0 && binaryImage[y][x+1]==1)
        count++;
        
        //p4 p5
        
		if(isvalid(x+1,y+1) && binaryImage[y][x+1]== 0 && binaryImage[y+1][x+1]==1)
        count++;
       
        //p5 p6
        
		if(isvalid(x+1,y+1) && binaryImage[y+1][x+1]==0 && binaryImage[y+1][x]==1)
        count++;
       
        //p6 p7
        
		if(isvalid(x-1,y+1) && binaryImage[y+1][x]==0 && binaryImage[y+1][x-1]==1)
        count++;
        
        //p7 p8
        
		if(isvalid(x-1,y+1) && binaryImage[y+1][x-1]== 0 && binaryImage[y][x-1]==1)
        count++;
        
        //p8 p9
        
		if(isvalid(x-1,y-1) && binaryImage[y][x-1]==0 && binaryImage[y-1][x-1]==1)
        count++;
        
		//p9 p2
        
		if(isvalid(x-1,y-1) &&binaryImage[y-1][x-1]==0 && binaryImage[y-1][x]==1)
        count++;
        
return count;

}

int condition1(int x, int y)
{
	int b=find_B_P1(x,y);
	
	if(b>=2 && b<=6)
		return 1;
	else
		return 0;
}

int condition2(int x, int y)
{
	if(find_A_P1(x,y)==1)
		return 1;
	else
		return 0;
}

int condition3(int x,int y)
{
	if(binaryImage[y-1][x] * binaryImage[y][x + 1] * binaryImage[y][x-1] == 0 || find_A_P1(y-1, x) != 1)
		return 1;
	else
		return 0;
}

int condition4(int x, int y)
{	
	if(binaryImage[y-1][x] * binaryImage[y][x+1] * binaryImage[y+1][x] == 0 || find_A_P1(y,x+1) != 1)
		return 1;
	else
		return 0;
}

void hilditch( )
{
	/*
	Conditions to be satisified to change a pixel p1 from black to white. (i.e deleting the pixel)

	condition 1  :   2 < = B(p1) < = 6
	condition 2  :   A(p1)=1
	condition 3  :   p2.p4.p8=0 or A(p2)!= 1
	condition 4  :   p2.p4.p6=0 or A(p4)!= 1
	*/

	bool hasChange;
        
		do
		{
		hasChange = false;
        
		for (int i = 1; i + 1 < height; i++)
		{
                for (int j = 1; j + 1 < length; j++)
				{
					if (binaryImage[i][j]==1 && condition1(i,j) && condition2(i,j) && condition3(i,j) && condition4(i,j))
					{
                        binaryImage[i][j] = 0;
                        hasChange = true;
                    }
                }
            }
        }
		while (hasChange);

		return;
}

int is_edge_point(int x, int y)
{
	int count=0;

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(i==1&&j==1)
				continue;
			if((binaryImage[i+x][j+y]==1))
			count++;
		}
	}

	if(count==1)
		return 1;
	else
		return 0;
}

int number_of_edge_points()
{
	int count=0;

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<b;j++)
		{
			if(is_edge_point(i,j)==1)
			count++;
		}
	}
	
	return count;
}

// Normalised Area Function including centroid
// Normalised Area = (area of signature / area of bounding box)  

long double na(int my,int mmy,int ret)
{
	long double sum_x=0;
	long double p_cnt=0;
	int pixel_count=0;
	int min_x=size_x,min_y=size_y,max_x=0,max_y=0;
	
	for(int x=0;x<=size_x;x++)
	{
		for(int y=my;y<=mmy;y++)
		{
			if(!image_grey.at<uchar>(x,y))
		    {
		    	pixel_count++;
		        min_x=min(min_x,x);
		        max_x=max(max_x,x);
		        min_y=min(min_y,y);
		        max_y=max(max_y,y);
		        sum_x+=x;
		        p_cnt++;
		    }
		}
	  }
	  
	float bound_height=max_x-min_x+1;
	float bound_width=max_y-min_y+1;
	
	long double normalised_area;
	long double area=(bound_height)*(bound_width);
	
	normalised_area=pixel_count/area;			// (area of signature/ area of bounding box)
	
	long double centroid_x=sum_x/p_cnt;
	long double area11=0,area12=0;
	
	area11=(centroid_x-min_x+1)*(max_y-min_y+1);
	area12=(max_x-centroid_x)*(max_y-min_y+1);
	
	if(ret==0)
    	return normalised_area;
    if(ret==1)
    	return area11;
    	
  	return area12;
}



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
  	namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );
    
  	imshow( "Original Image", image_grey );
  	Size s = image_grey.size();
  	size_x = s.height;
  	size_y = s.width;
  	int pixel_count=0;
  	int min_x=size_x,min_y=size_y,max_x=0,max_y=0;
  	
  	// Bounding Box
  	for(int x=0;x<size_x;x++)
  	{
		for(int y=0;y<size_y;y++)
		{
		    image_grey.at<uchar>(x,y)=(image_grey.at<uchar>(x,y)<=125 ? 0:255);
		    
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
  	
  	long double normalised_area;
  	long double area=(bound_height)*(bound_width);
  	
  	cout<<"Height 		: "<<size_x<<"\n";
  	cout<<"Width 		: "<<size_y<<"\n";
  	cout<<"Pixel count	: "<<pixel_count<<"\n";
  	cout<<"Total pixels	: "<<size_x * size_y<<"\n";
  	
  	cout<<"Bounding Box Height	: "<<bound_height<<"\t\tMin_x	: "<<min_x<<"\tMax_x	: "<<max_x<<"\n";
  	cout<<"Bounding Box Width	: "<<bound_width<<"\t\tMin_y	: "<<min_y<<"\tMax_y	: "<<max_y<<"\n";
  	cout<<"Bounding Box Area 	: "<<area<<"\n";
  	
  	normalised_area=pixel_count/area;  	// (area of signature/ area of bounding box)
  	
  	cout<<"Feature 1      : Normalised area               : "<<normalised_area<<"\n";
  
  	long double AR=bound_width/bound_height;
  	cout<<"Feature 2      : Aspect Ratio                  : "<<AR<<"\n";
  
  
  	int hor_histo=0;
  	int ver_histo=0;
  	int histo_row,histo_col;
  	
  	int sum_x1=0,sum_x2=0,sum_y1=0,sum_y2=0;
  	int p_cnt1=0,p_cnt2=0;
  
  	for(int x=min_x;x<=max_x;x++)
  	{
    	int cnt=0;
    	
    	for(int y=min_y;y<=max_y;y++)
    	{
		    if(!image_grey.at<uchar>(x,y))
		    {
		        cnt++;
		        if(y<(max_y-min_y)/2)
		        {
		                sum_x1+=x;
		                sum_y1+=y;
		                p_cnt1++;
		        }
		        
		        else
		        {
		            sum_x2+=x;
		            sum_y2+=y;
		            p_cnt2++;
		        }
		    }
    	}
    
    	if(hor_histo<cnt)
    	{
		    hor_histo=cnt;
		    histo_row=x;
		}
  	}
  
  	for(int y=min_y;y<=max_y;y++)
  	{
		int cnt=0;

		for(int x=min_x;x<=max_x;x++)
		{
		    if(!image_grey.at<uchar>(x,y))
		    {    
		        cnt++;
		    }
		}
		if(ver_histo<cnt)
		{
		    ver_histo=cnt;
		    histo_col=y;
		}
	}
	
  	cout<<"Feature 3      : horizontal histogram          : "<<histo_row<<"\n";
  	cout<<"Feature 4      : vertical histogram            : "<<histo_col<<"\n";
  
  	long double centroid_x1,centroid_y1,centroid_x2,centroid_y2;
  	centroid_x1=(long double)sum_x1/(long double)p_cnt1;
  	centroid_y1=(long double)sum_y1/(long double)p_cnt1;
  	centroid_x2=(long double)sum_x2/(long double)p_cnt2;
  	centroid_y2=(long double)sum_y2/(long double)p_cnt2;
  
  
  	long double hyp=sqrt(((centroid_x1-centroid_x2)*(centroid_x1-centroid_x2))+((centroid_y1-centroid_y2)*(centroid_y1-centroid_y2)));
  	long double height=abs(centroid_x1-centroid_x2);
  	long double c_angle=asin(height/hyp)/pi;  
  
  	c_angle+=(long double)0.5;
  	cout<<"Feature 5a     : Centroid 1                    : "<<centroid_y1<<" "<<centroid_x1<<"\n";
  	cout<<"Feature 5b     : Centroid 2                    : "<<centroid_y2<<" "<<centroid_x2<<"\n";
  	cout<<"Feature 5c     : Centroid angle                : "<<c_angle<<"\n";

  	long double tri_surface_1=na(min_y,min_y+bound_width/3,0);
  	long double tri_surface_2=na(min_y+bound_width/3+1,min_y+(bound_width/3)*2,0);
  	long double tri_surface_3=na(min_y+((bound_width/3)*2)+1,max_y,0);
  	cout<<"Feature 6      : tri_surface 1                 : "<<tri_surface_1<<"\n";
  	cout<<"Feature 7      : tri_surface 2                 : "<<tri_surface_2<<"\n";
  	cout<<"Feature 8      : tri_surface 3                 : "<<tri_surface_3<<"\n";

  	long double six_fold_1=na(min_y,min_y+bound_width/3,1);
  	long double six_fold_2=na(min_y,min_y+bound_width/3,2);
  	long double six_fold_3=na(min_y+bound_width/3+1,min_y+(bound_width/3)*2,1);
  	long double six_fold_4=na(min_y+bound_width/3+1,min_y+(bound_width/3)*2,2);
  	long double six_fold_5=na(min_y+((bound_width/3)*2)+1,max_y,1);
  	long double six_fold_6=na(min_y+((bound_width/3)*2)+1,max_y,2);
  	cout<<"Feature 9      : six_fold 1                    : "<<six_fold_1<<"\n";
  	cout<<"Feature 10     : six_fold 2                    : "<<six_fold_2<<"\n";
  	cout<<"Feature 11     : six_fold 3                    : "<<six_fold_3<<"\n";
  	cout<<"Feature 12     : six_fold 4                    : "<<six_fold_4<<"\n";
  	cout<<"Feature 13     : six_fold 5                    : "<<six_fold_5<<"\n";
  	cout<<"Feature 14     : six_fold 6                    : "<<six_fold_6<<"\n";


	double xr1=0,yr1=0,xr2=0,yr2=0,xr3=0,yr3=0,xr4=0,yr4=0;
	int cnt=0;
	for(int x=0;x<size_x;x++)
  	{
		for(int y=0;y<size_y-1;y++)
		{
		    if(image_grey.at<uchar>(x,y)!=(image_grey.at<uchar>(x,y+1))){
		    		xr1+=x;
		    		yr1+=y;
		    		cnt++;
		    }
    	}
  	}	
  	xr1/=(cnt*size_x);
  	yr1/=(cnt*size_y);

  	cnt=0;
  	for(int x=0;x<size_x;x++)
  	{
		for(int y=size_y-1;y>=1;y--)
		{
		    if(image_grey.at<uchar>(x,y)!=(image_grey.at<uchar>(x,y-1))){
		    		xr2+=x;
		    		yr2+=y;
		    		cnt++;
		    }
    	}
  	}	
  	xr2/=(cnt*size_x);
  	yr2/=(cnt*size_y);

  	cnt=0;
  	for(int y=0;y<size_y;y++)
  	{
		for(int x=0;x<size_x-1;x++)
		{
		    if(image_grey.at<uchar>(x,y)!=(image_grey.at<uchar>(x+1,y))){
		    		xr3+=x;
		    		yr3+=y;
		    		cnt++;
		    }
    	}
  	}		
  	xr3/=(cnt*size_x);
  	yr3/=(cnt*size_y);

  	cnt=0;
  	for(int y=0;y<size_y;y++)
  	{
		for(int x=size_x-1;x>=1;x--)
		{
		    if(image_grey.at<uchar>(x,y)!=(image_grey.at<uchar>(x-1,y))){
		    		xr4+=x;
		    		yr4+=y;
		    		cnt++;
		    }
    	}
  	}		
  	xr4/=(cnt*size_x);
  	yr4/=(cnt*size_y);

  	/*cout<<xr1<<" "<<xr2<<" "<<xr3<<" "<<xr4<<"\n";
  	cout<<yr1<<" "<<yr2<<" "<<yr3<<" "<<yr4<<"\n";*/

  	cout<<"Feature 15     : left-right(x)                 : "<<xr1<<"\n";
  	cout<<"Feature 16     : left-right(y)                 : "<<yr1<<"\n";
  	cout<<"Feature 17     : right-left(x)                 : "<<xr2<<"\n";
  	cout<<"Feature 18     : right-left(y)                 : "<<yr2<<"\n";
  	cout<<"Feature 19     : top-bottom(x)                 : "<<xr3<<"\n";
  	cout<<"Feature 20     : top-bottom(y)                 : "<<yr3<<"\n";
  	cout<<"Feature 21     : bottom-top(x)                 : "<<xr4<<"\n";
  	cout<<"Feature 22     : bottom-top(y)                 : "<<yr4<<"\n";
	
	binaryImage=new int* [size_x+10];

	for(int i=0;i<size_x+10;i++)
		binaryImage[i]=new int[size_y+10];

	b=size_y;
	h=size_x;
	length=size_y;
	height=size_x;
	load_image( ); 
	//cout<<"Image before Skeletonization \n";	  
	hilditch( );
	//cout<<"Image after Skeletonization \n";
	//print_skeleton_image();
	  	
  	cout<<"Feature 23     : edge_points                   : "<<number_of_edge_points()<<"\n";
  	
  	
	for(int y=min_y;y<=max_y;y++)
	{
		for(int x = min_x;x<=max_x;x++)
			if(y==min_y || y==max_y || x==min_x || x==max_x)
			image_grey.at<uchar>(x,y)=0;
	}
	
	for(int y=min_y;y<=max_y;y++)
	{
		image_grey.at<uchar>(histo_row,y)=0;
	}
	
	for(int x = min_x;x<=max_x;x++)
	{
		image_grey.at<uchar>(x,histo_col)=0;
	}
	
  	imshow( "Display Image", image_grey );
  	waitKey(0);

  	return 0;
}

