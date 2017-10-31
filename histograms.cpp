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
	

