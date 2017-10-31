#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>
#include <utility>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define pi (22/7)

int size_x,size_y;
Mat image,image_grey;
float bound_height,bound_width;
ofstream wrt;
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
			wrt<<binaryImage[i][j];
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
	string img=".jpeg";
	int cnt=0;
	//mkdir("out", S_IRWXU | S_IRWXG | S_IROTH |S_IXOTH);
	chdir(argv[1]);
	vector<long double> naa,ar,hh,vh,ca,ts1,ts2,ts3,sf1,sf2,sf3,sf4,sf5,sf6,lrx,lry,rlx,rly,tbx,tby,btx,bty,ep;
	vector<pair<long double,long double> > c1,c2;
	int flag=0;
	while(1){
	  	cnt++;
	  	string file;
	  	stringstream ss;
	  	ss<<cnt;ss>>file;
	  	string ff=file+".txt";
	  	file+=img;
	  	
	  	image = imread( file.c_str(), 1 );
	  	if(!image.data )
	  	{
		  	printf( "No image data \n" );
		  	break;
	  	}
	  	wrt.open(ff.c_str());
	  	flag=1;
	  	
		size_x=0,size_y=0;
		bound_height=0,bound_width=0;
		h=0,b=0,length=0,height=0;
	  	
	  	cvtColor(image, image_grey, CV_RGB2GRAY);
	  	namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
	  	namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );
		
	  	//imshow( "Original Image", image_grey );
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
	  	
	  	wrt<<"Height 		: "<<size_x<<"\n";
	  	wrt<<"Width 		: "<<size_y<<"\n";
	  	wrt<<"Pixel count	: "<<pixel_count<<"\n";
	  	wrt<<"Total pixels	: "<<size_x * size_y<<"\n";
	  	
	  	wrt<<"Bounding Box Height	: "<<bound_height<<"\t\tMin_x	: "<<min_x<<"\tMax_x	: "<<max_x<<"\n";
	  	wrt<<"Bounding Box Width	: "<<bound_width<<"\t\tMin_y	: "<<min_y<<"\tMax_y	: "<<max_y<<"\n";
	  	wrt<<"Bounding Box Area 	: "<<area<<"\n";
	  	
	  	normalised_area=pixel_count/area;  	// (area of signature/ area of bounding box)
	  	
	  	wrt<<"Feature 1      : Normalised area               : "<<normalised_area<<"\n";
	  	naa.push_back(normalised_area);
	  
	  	long double AR=bound_width/bound_height;
	  	wrt<<"Feature 2      : Aspect Ratio                  : "<<AR<<"\n";
	    ar.push_back(AR);
	  
	  	int hor_histo=0;
	  	int ver_histo=0;
	  	long double histo_row,histo_col;
	  	
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
	
	  	wrt<<"Feature 3      : horizontal histogram          : "<<histo_row<<"\n";
	  	wrt<<"Feature 4      : vertical histogram            : "<<histo_col<<"\n";
	  	hh.push_back(histo_row);
	  	vh.push_back(histo_col);
	  	long double centroid_x1,centroid_y1,centroid_x2,centroid_y2;
	  	centroid_x1=(long double)sum_x1/(long double)p_cnt1;
	  	centroid_y1=(long double)sum_y1/(long double)p_cnt1;
	  	centroid_x2=(long double)sum_x2/(long double)p_cnt2;
	  	centroid_y2=(long double)sum_y2/(long double)p_cnt2;
	  
	  
	  	long double hyp=sqrt(((centroid_x1-centroid_x2)*(centroid_x1-centroid_x2))+((centroid_y1-centroid_y2)*(centroid_y1-centroid_y2)));
	  	long double height=abs(centroid_x1-centroid_x2);
	  	long double c_angle=asin(height/hyp)/pi;  
	  
	  	c_angle+=(long double)0.5;
	  	wrt<<"Feature 5a     : Centroid 1                    : "<<centroid_y1<<" "<<centroid_x1<<"\n";
	  	wrt<<"Feature 5b     : Centroid 2                    : "<<centroid_y2<<" "<<centroid_x2<<"\n";
	  	wrt<<"Feature 5c     : Centroid angle                : "<<c_angle<<"\n";
		c1.push_back(make_pair(centroid_y1,centroid_x1));
		c2.push_back(make_pair(centroid_y2,centroid_x2));
	  	long double tri_surface_1=na(min_y,min_y+bound_width/3,0);
	  	long double tri_surface_2=na(min_y+bound_width/3+1,min_y+(bound_width/3)*2,0);
	  	long double tri_surface_3=na(min_y+((bound_width/3)*2)+1,max_y,0);
	  	wrt<<"Feature 6      : tri_surface 1                 : "<<tri_surface_1<<"\n";
	  	wrt<<"Feature 7      : tri_surface 2                 : "<<tri_surface_2<<"\n";
	  	wrt<<"Feature 8      : tri_surface 3                 : "<<tri_surface_3<<"\n";

		ts1.push_back(tri_surface_1);
		ts2.push_back(tri_surface_2);
		ts3.push_back(tri_surface_3);
	  	long double six_fold_1=na(min_y,min_y+bound_width/3,1);
	  	long double six_fold_2=na(min_y,min_y+bound_width/3,2);
	  	long double six_fold_3=na(min_y+bound_width/3+1,min_y+(bound_width/3)*2,1);
	  	long double six_fold_4=na(min_y+bound_width/3+1,min_y+(bound_width/3)*2,2);
	  	long double six_fold_5=na(min_y+((bound_width/3)*2)+1,max_y,1);
	  	long double six_fold_6=na(min_y+((bound_width/3)*2)+1,max_y,2);
	  	wrt<<"Feature 9      : six_fold 1                    : "<<six_fold_1<<"\n";
	  	wrt<<"Feature 10     : six_fold 2                    : "<<six_fold_2<<"\n";
	  	wrt<<"Feature 11     : six_fold 3                    : "<<six_fold_3<<"\n";
	  	wrt<<"Feature 12     : six_fold 4                    : "<<six_fold_4<<"\n";
	  	wrt<<"Feature 13     : six_fold 5                    : "<<six_fold_5<<"\n";
	  	wrt<<"Feature 14     : six_fold 6                    : "<<six_fold_6<<"\n";
		
		sf1.push_back(six_fold_1);
		sf2.push_back(six_fold_2);
		sf3.push_back(six_fold_3);
		sf4.push_back(six_fold_4);
		sf5.push_back(six_fold_5);
		sf6.push_back(six_fold_6);

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

	  	/*wrt<<xr1<<" "<<xr2<<" "<<xr3<<" "<<xr4<<"\n";
	  	wrt<<yr1<<" "<<yr2<<" "<<yr3<<" "<<yr4<<"\n";*/

	  	wrt<<"Feature 15     : left-right(x)                 : "<<xr1<<"\n";
	  	wrt<<"Feature 16     : left-right(y)                 : "<<yr1<<"\n";
	  	wrt<<"Feature 17     : right-left(x)                 : "<<xr2<<"\n";
	  	wrt<<"Feature 18     : right-left(y)                 : "<<yr2<<"\n";
	  	wrt<<"Feature 19     : top-bottom(x)                 : "<<xr3<<"\n";
	  	wrt<<"Feature 20     : top-bottom(y)                 : "<<yr3<<"\n";
	  	wrt<<"Feature 21     : bottom-top(x)                 : "<<xr4<<"\n";
	  	wrt<<"Feature 22     : bottom-top(y)                 : "<<yr4<<"\n";
		
		lrx.push_back(xr1);
		lry.push_back(yr1);
		rlx.push_back(xr2);
		rly.push_back(yr2);
		tbx.push_back(xr3);
		tby.push_back(yr3);
		btx.push_back(xr4);
		bty.push_back(yr4);
	
		binaryImage=new int* [size_x+10];

		for(int i=0;i<size_x+10;i++)
			binaryImage[i]=new int[size_y+10];

		b=size_y;
		h=size_x;
		length=size_y;
		height=size_x;
		load_image( ); 
		//wrt<<"Image before Skeletonization \n";	  
		hilditch( );
		//wrt<<"Image after Skeletonization \n";
		//print_skeleton_image();
		int edge_points=number_of_edge_points();
	  	wrt<<"Feature 23     : edge_points                   : "<<edge_points<<"\n";
	  	
	  	ep.push_back(edge_points);
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
			
	  	//imshow( "Display Image", image_grey );
	  	//int next;
	  	//cin>>next;
	  	wrt.close();
  	}
  	if(!flag)
  		return 0;
  	wrt.open("final.txt");
  	long double avg[30]={0};
  	for(int i=0;i<(int)naa.size();i++){
  		avg[1]+=naa[i];
  	}
  	avg[1]/=naa.size();
  	for(int i=0;i<(int)ar.size();i++){
  		avg[2]+=ar[i];
  	}
  	avg[2]/=ar.size();;
  	for(int i=0;i<(int)hh.size();i++){
  		avg[3]+=hh[i];
  	}
  	avg[3]/=hh.size();
  	for(int i=0;i<(int)vh.size();i++){
  		avg[4]+=vh[i];
  	}
  	avg[4]/=vh.size();
  	for(int i=0;i<(int)c1.size();i++){
  		avg[5]+=c1[i].first;
  		avg[6]+=c1[i].second;
  	}
  	avg[5]/=c1.size();
  	avg[6]/=c1.size();
  	for(int i=0;i<(int)c2.size();i++){
  		avg[7]+=c2[i].first;
  		avg[8]+=c2[i].second;
  	}
  	avg[7]/=c2.size();
  	avg[8]/=c2.size();
  	for(int i=0;i<(int)ca.size();i++){
  		avg[9]+=ca[i];
  	}
  	avg[9]/=ca.size();
  	for(int i=0;i<(int)ts1.size();i++){
  		avg[10]+=ts1[i];
  	}
  	avg[10]/=ts1.size();
  	for(int i=0;i<(int)ts2.size();i++){
  		avg[11]+=ts2[i];
  	}
  	avg[11]/=ts2.size();
  	for(int i=0;i<(int)ts3.size();i++){
  		avg[12]+=ts3[i];
  	}
  	avg[12]/=ts3.size();
  	for(int i=0;i<(int)sf1.size();i++){
  		avg[13]+=sf1[i];
  	}
  	avg[13]/=sf1.size();
  	for(int i=0;i<(int)sf2.size();i++){
  		avg[14]+=sf2[i];
  	}
  	avg[14]/=sf2.size();
  	for(int i=0;i<(int)sf3.size();i++){
  		avg[15]+=sf3[i];
  	}
  	avg[15]/=sf3.size();
  	for(int i=0;i<(int)sf4.size();i++){
  		avg[16]+=sf4[i];
  	}
  	avg[16]/=sf4.size();
  	for(int i=0;i<(int)sf5.size();i++){
  		avg[17]+=sf5[i];
  	}
  	avg[17]/=sf5.size();
  	for(int i=0;i<(int)sf6.size();i++){
  		avg[18]+=sf6[i];
  	}
  	avg[18]/=sf6.size();
  	for(int i=0;i<(int)lrx.size();i++){
  		avg[19]+=lrx[i];
  	}
  	avg[19]/=lrx.size();
  	for(int i=0;i<(int)lry.size();i++){
  		avg[20]+=lry[i];
  	}
  	avg[20]/=lry.size();
  	for(int i=0;i<(int)rlx.size();i++){
  		avg[21]+=rlx[i];
  	}
  	avg[21]/=rlx.size();
  	for(int i=0;i<(int)rly.size();i++){
  		avg[22]+=rly[i];
  	}
  	avg[22]/=rly.size();
  	for(int i=0;i<(int)tbx.size();i++){
  		avg[23]+=tbx[i];
  	}
  	avg[23]/=tbx.size();
  	for(int i=0;i<(int)tby.size();i++){
  		avg[24]+=tby[i];
  	}
  	avg[24]=tby.size();
  	for(int i=0;i<(int)btx.size();i++){
  		avg[25]+=btx[i];
  	}
  	avg[25]/=btx.size();
  	for(int i=0;i<(int)bty.size();i++){
  		avg[26]+=bty[i];
  	}
  	avg[26]/=bty.size();
  	for(int i=0;i<(int)ep.size();i++){
  		avg[27]+=ep[i];
  	}
  	avg[27]/=ep.size();
  	for(int i=1;i<28;i++){
  		wrt<<avg[i];
  		if(i==5 or i==7){
  			i++;
  			wrt<<" "<<avg[i];
  		}
  		wrt<<"\n";
  	}
  	wrt.close();
  	waitKey(0);

  	return 0;
}

