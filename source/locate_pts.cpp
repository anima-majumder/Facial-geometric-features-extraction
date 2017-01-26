#include "locate_pts.h"

void locate_pts(int const contour_count, CvPoint* contour_array, CvPoint & rect_center, const float & constraint_height,  CvPoint &upper_mid,CvPoint &lower_mid, CvPoint &left,CvPoint &right, const bool &flag_constraint )


{
  //min x value and corresponding y value is for left  pt. whereas max x value and corresponding y value is for right  pt.	//once max and min x coordinate values are found, obtain the corresponding y values. 	  
  int max_x=contour_array[0].x, max_y=contour_array[0].y;
  int min_x=contour_array[0].x, min_y=contour_array[0].y;
  for (int j=1; j<contour_count; j++)
    {
      //********** if lip pt--detection--- if diff between array pt and lip center is within the range boundbox.height/5 then find the maximum and minimum pts************************//
      if(flag_constraint)
	{
	  if(abs(contour_array[j].y-rect_center.y)<=constraint_height)//constraint is added here
	    {
 
	      if (max_x<contour_array[j].x)
		{
		  max_x=contour_array[j].x;
		  max_y=contour_array[j].y;
		}//max_x and corresponding  value are obtained here

	      if(min_x>contour_array[j].x)
		{
		  min_x=contour_array[j].x;
		  min_y=contour_array[j].y;
		}// min_x and corresponding value are obtained here

	    }

	}


      else 
	{
				 
	  if (max_x<contour_array[j].x)
	    {
	      max_x=contour_array[j].x;
	      max_y=contour_array[j].y;
	    }//max_x and corresponding  value are obtained here

	  if(min_x>contour_array[j].x)
	    {
	      min_x=contour_array[j].x;
	      min_y=contour_array[j].y;
	    }// min_x and corresponding value are obtained here
	}
			    
    }
  left.x=min_x;
  left.y=min_y;
  right.x=max_x;
  right.y=max_y;

  //to obtain the middle points of ... obtain the average of two extreme x points. then find the nearest x value for the avg x value and obtain corresponding upper and lower y coordinate values. 
  int mid_x=abs((left.x+right.x)/2);
  int mid_y1=0, mid_y2=0;
    
  // //******************************************************************//
  int diff_x=0, y_diff;
  int small_x=1000, index=0, index2=0;
  int large_y_diff=0;
  for(int l=0; l<contour_count; l++)
    {
      diff_x=abs(mid_x - contour_array[l].x);
      cout<<"diff data"<<"\t"<<diff_x<<endl;
      if(small_x>diff_x)
  	{
  	  small_x=diff_x;
  	  index=l;
  	}
      	 
    }
  // cout<<"smallest data"<<small_x<<"\t"<<"index"<<"\t"<<index<<endl;

  mid_x=contour_array[index].x;
  mid_y1=contour_array[index].y;


  for (int k=0; k<contour_count; k++)
    {
	
      //find out the contour point for which diff between mid_x and near contour points. Then obtain the smallest difference and index of that. find corresponding contour coordinate value. // This part is to be done**************************************//

      if (contour_array[k].x==mid_x || abs(mid_x-contour_array[k].x<=1))//&& abs(mid_y1-contour_array[k].y)>2)
	{
	  // mid_x=contour_array[k].x;
	  // if(mid_y1==0)
	  //   mid_y1=contour_array[k].y;
	  // else

	  if (contour_array[k].y!=contour_array[index].y)
	    {

	      y_diff=abs(contour_array[k].y-contour_array[index].y);
		 
	      if(y_diff>large_y_diff)
		{
		  large_y_diff=y_diff;
		  index2=k;
		}


	      else
		{
		  large_y_diff=large_y_diff;

		  index2=index2;
		}

	    }
 

	}


     

    }
  //}


  mid_y2=contour_array[index2].y;


  upper_mid.x=mid_x;
  lower_mid.x=mid_x;
  if(mid_y1>mid_y2)
    {
      upper_mid.y=mid_y2;
      lower_mid.y=mid_y1;
    }
  else
    {
      upper_mid.y=mid_y1;
      lower_mid.y=mid_y2;
    }

  

  return;
}//end of the function 
//*****************************************************//
