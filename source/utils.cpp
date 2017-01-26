
// #include "cv.h"
// #include "highgui.h"

#include "utils.h"

void show_image(IplImage* img,unsigned int const wait_ms )

{
  // unsigned int const wait_ms=10;

  cvNamedWindow("image", CV_WINDOW_AUTOSIZE);

  cvShowImage("image",img);

  cvWaitKey(wait_ms);

  cvDestroyWindow("image");



}


void draw_contour(IplImage* img, CvSeq* contours)
{
  
  if( contours )


cvDrawContours(
 img,
 contours,
cvScalarAll(255),
cvScalarAll(255),
-1,

 1,
 8,
 cvPoint(0,0)
);

 
  else
    {
      std::cout<<"no contours"<<std::endl;
      return;
    }
   

 
  return;

}

//************end of the function draw contour****************//


//find largest contour*****************//

// void largest_contour_detect(CvSeq* contour, CvSeq* largest_contour)// this function is not working
// {
//    if( contour )//find the largest contour
//      {
//       double area, max=0;
//       for( ; contour != 0; contour = contour->h_next ){
//       	area = cvContourArea( contour , CV_WHOLE_SEQ );

//       	if (area > max ) 
//       	  {max = area;
	  
//       	    largest_contour=contour;

//       	  }
   
    
//       }

//      }

//       return;
//      }

  




 //end of the function to find largest contour


// void draw_circle(IplImage* img, CvPoint eyecentroid)
// {
//   cvCircle( img, eyecentroid, 4, CV_RGB(255,0,0), 0.5, CV_AA, 0 );
//   return;
// }



//******************************************//


double vertical_projection(IplImage* img)
{


// find the vertical projection
  std::vector <double> v_vl_proj; // holds the column sum values    
double max_vl_proj = 0; // holds the maximum value    
for( int i=0;i<img->width;++i )
{
    CvMat col;
    CvScalar col_sum;
    cvGetCol( img, &col, i ); // get individual columns    
    col_sum = cvSum( &col ); // find the sum of ith column
    v_vl_proj.push_back( col_sum.val[0] ); // push back to vector
    if( col_sum.val[0]>max_vl_proj )    max_vl_proj = col_sum.val[0];    
}



 return max_vl_proj;


  }

//*******end of vertical projection funct*************//

double horizontal_projection(IplImage* img)
{


// find the vertical projection
  std::vector <double> v_hl_proj; // holds the row sum values    
double max_hl_proj = 0.0; // holds the maximum value    
for( int i=0;i<img->height;++i )
{
    CvMat row;
    CvScalar row_sum;
    cvGetRow( img, &row, i ); // get individual rows    
    row_sum = cvSum( &row ); // find the sum of ith row
    v_hl_proj.push_back( row_sum.val[0] ); // push back to vector
    if( row_sum.val[0]>max_hl_proj )    max_hl_proj = row_sum.val[0];    
}



 return max_hl_proj;


  }
//*********** end of horizontal projection function***********//




void invert_img(IplImage* img){
int height,width,step,channels;
  uchar *data;
  int i,j,k;
 // get the image data
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
  printf("Processing a %dx%d image with %d channels with step %d\n",height,width,channels,step);
 

// invert the image
  for(i=0;i<height;i++)
     for(j=0;j<width;j++)
        for(k=0;k<channels;k++)  //loop to read for each channel
           data[i*step+j*channels+k]=255-data[i*step+j*channels+k];    //inverting the image

// cvNamedWindow("inverted_img", CV_WINDOW_AUTOSIZE);
//   // show the image
//   cvShowImage("inverted_img", img );
 
//   // wait for a key
//   cvWaitKey(0);

//  cvDestroyWindow("inverted_img");

  return;
}


//************end of function invert image *******************//




int write_scalar_data_to_file(char* filename,  const double data )

{

  // int i;
  // float f;
  FILE *ofp;


  // printf("Please enter an integer number\n");
  // scanf("%d", &i);
  // printf("Please enter a floating point number\n");
  // scanf("%f", &f);

  //ofp = fopen("exercise3.txt", "a");
 


  // int len=sizeof(movement_array/sizeof(int));

  // cout<<"size of array="<< len;
 ofp = fopen(filename, "a");
 
    
      fprintf(ofp, "%lf \t", data );
    
   
  fprintf(ofp, " \n" );
  fclose(ofp);

  
 

  return 0;
}
//**************************************//
