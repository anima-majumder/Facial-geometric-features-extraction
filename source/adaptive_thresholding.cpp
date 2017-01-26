


# include "adaptive_thresholding.h"


void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w)
{
   
  uchar * data;
  int height, width, channels, step, dimn;
  float h_data;
 
  float sum=0;
  mean=0;
 

  // get the image data
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data    = (uchar*)img->imageData;
 
  float hue_h_arr[width][height];
  printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

  for(int i=0;i< width;i++)
    {
      for(int j=0;j<height;j++)
	{
	  h_data=(float)data[j*step + i*channels+ 0 ];
	 
	  //	  cout<<"sudo hue_data :"<<h_data<<endl;

	  sum=sum+h_data;
	  hue_h_arr[i][j]=h_data;
	  //  std::cout<<hue_h_arr<<std::endl;
	}
    }

  dimn=height*width;
  mean=sum/dimn;
  cout<<"mean is "<<mean<<endl;
  //calculate local gradient

  local_mean_st_dev( img, mean, thres,k_w);
  //completed properly
  return;
}//end of adaptive threshold function
//======================================================================================//

void local_mean_st_dev(IplImage* img, float global_mean, IplImage* thres_img, int k_w)
{
  cvSmooth(img, img, CV_GAUSSIAN,  3, 0, 0, 0); //smooth the image
  uchar * data1;
  int height, width, channels, step;
  // dimn;
  float img_data, img_w_data, local_mean;// img_w_data is the windowed data
  // get the image data
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data1    = (uchar*)img->imageData;
  //Obtain the local mean and local standard deviation of an image. It is done as follows
  //For a window size dxd extract the ROI image of size dxd.
  //check for the boundary of the image, i.e, if i-d/2 <0 then padd with zero. Also if i+d/2>width pad with zero.
 
  //Get each pixel value within the dxd region. Obtain local mean
 

  // perform thresholding
  int i,j,m,n, x1,x2,y1, y2;
  // set the window size
  int d=7;//window size can give 7, 15, 9, 5, 3 // windows 7x7 gives better result than others.
  int s2=d/2;
  int count=0;
  float sum_local, stand_dev=0.0,diff_sq=0.0,thres_value;
  for (i=0; i<width; i++)
    {
      for (j=0; j<height; j++)
	{
		

	  img_data =(float)data1[j*step + i*channels+ 0 ];
			
	  // set the SxS region
	  x1=i-s2; x2=i+s2;
	  y1=j-s2; y2=j+s2;
	  sum_local=0.0;// this sum is locally calculated

			// check the border
	  if (x1 < 0) x1 = 0;
	  if (x2 >= width) x2 = width-1;
	  if (y1 < 0) y1 = 0;
	  if (y2 >= height) y2 = height-1;
			
	  count = (x2-x1)*(y2-y1);

	  //m and n are the local window along x is m and along y is n
	  for(m=x1;m<x2;m++)
	    {

	      for (n=y1;n<y2;n++)
		{

		  img_w_data =(float)data1[n*step + m*channels+ 0 ];
		  sum_local=sum_local+img_w_data;

		  //  cout<<"see"<<"for m="<<m<<"and n="<<n<<"is"<<img_w_data<<endl;
		}
	      //    cout<<"itr i="<<j<<endl;

	    }

	  //	cout<<"itr i:"<<i<<endl;

	  local_mean=sum_local/count;//mean of the windowed image

	  //	  cout<<"local mean is ="<<local_mean<<endl;
	  //	  cout<<"count is:"<<count<<endl;

	  for (m=x1;m<x2;m++)
	    {
	      for(n=y1;n<y2;n++)
		{
		  img_w_data =(float)data1[n*step + m*channels+ 0 ];
		  diff_sq=(local_mean-img_w_data)*(local_mean-img_w_data);

		  stand_dev=stand_dev+diff_sq;// cumulative standard deviation 
		}

	    }

	  stand_dev=stand_dev/count;// Standard deviation of a single pixel is given by this
	  //	  cout<<"standard deviation is:"<<stand_dev<<endl;
	 
	  // Modified  Niblacks algorithm ***********************************************
	  // thres_value=global_mean+0.2*stand_dev;//actual one where 0.1 is the constant value 
	  
	  // Original  Niblacks algorithm ***********************************************
	  //  thres_value=local_mean-0.1*stand_dev;//actual one where 0.1 is the constant value 
	  
	   //===============================================================================
	   //   cout<<"threshold value is:"<<thres_value<<endl;
	  // thres_value=global_mean-150*stand_dev;// modified one


	  //	thres_value=local_mean-0.2*stand_dev;// doesn't work

	   //Sauvola_s algorithm mean(x,y)+(1-k(1-(sigma_sq/R)))
	  float k_l=0.5;// for sauvola_s algo
	  int R=128;// for sauvola_s algo
	  thres_value=global_mean+(1-k_l*(1-stand_dev/R))+k_w;//remove this k_w= 5
	  //*********************************************************end of sauvola_s thresholding algorithm

	  if (img_data<thres_value)//set the image value to be zero else set the image value to be 255
			
	    cvSetReal2D(thres_img, j, i,0 );

	  else
	    cvSetReal2D(thres_img, j, i,255 );//threshold image 
	}
    }

  std::cout<<"thresholding successfully completed"<<endl;

  return;
}  //end of the function for obtaining local mean gradient thresholding algorithm










// void adaptive_threshold(IplImage* img,float& mean, IplImage* thres, int k_w)
// {
   
//   uchar * data;
//   int height, width, channels, step, dimn;
//   float h_data;
 
//   float sum=0;
//   mean=0;
 

//   // get the image data
//   height    = img->height;
//   width     = img->width;
//   step      = img->widthStep;
//   channels  = img->nChannels;
//   data    = (uchar*)img->imageData;
 
//   float hue_h_arr[width][height];
//   printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

//   for(int i=0;i< width;i++)
//     {
//       for(int j=0;j<height;j++)
// 	{
// 	  h_data=(float)data[j*step + i*channels+ 0 ];
	 
// 	  //	  cout<<"sudo hue_data :"<<h_data<<endl;

// 	  sum=sum+h_data;
// 	  hue_h_arr[i][j]=h_data;
	 
// 	}
//     }

//   dimn=height*width;
//   mean=sum/dimn;
//   cout<<"mean is "<<mean<<endl;
//   //calculate local gradient

//   local_mean_st_dev( img, mean, thres,k_w);
//   //completed properly
//   return;
// }//end of adaptive threshold function
// //======================================================================================//
// void local_mean_st_dev(IplImage* img, float global_mean, IplImage* thres_img, int k_w)
// {
//   cvSmooth(img, img, CV_GAUSSIAN,  3, 0, 0, 0); //smooth the image
//   uchar * data1;
//   int height, width, channels, step, dimn;
//   float img_data, img_w_data, local_mean;// img_w_data is the windowed data
//   // get the image data
//   height    = img->height;
//   width     = img->width;
//   step      = img->widthStep;
//   channels  = img->nChannels;
//   data1    = (uchar*)img->imageData;
//   //Obtain the local mean and local standard deviation of an image. It is done as follows
//   //For a window size dxd extract the ROI image of size dxd.
//   //check for the boundary of the image, i.e, if i-d/2 <0 then padd with zero. Also if i+d/2>width pad with zero.
 
//   //Get each pixel value within the dxd region. Obtain local mean
 

//   // perform thresholding
//   int i,j,m,n, x1,x2,y1, y2;
//   // set the window size
//   int d=7;//window size can give 7, 15, 9, 5, 3 // windows 7x7 gives better result than others.
//   int s2=d/2;
//   int count=0;
//   float sum_local, stand_dev=0.0,diff_sq=0.0,thres_value;
//   for (i=0; i<width; i++)
//     {
//       for (j=0; j<height; j++)
// 	{
		

// 	  img_data =(float)data1[j*step + i*channels+ 0 ];
			
// 	  // set the SxS region
// 	  x1=i-s2; x2=i+s2;
// 	  y1=j-s2; y2=j+s2;
// 	  sum_local=0.0;// this sum is locally calculated

// 			// check the border
// 	  if (x1 < 0) x1 = 0;
// 	  if (x2 >= width) x2 = width-1;
// 	  if (y1 < 0) y1 = 0;
// 	  if (y2 >= height) y2 = height-1;
			
// 	  count = (x2-x1)*(y2-y1);

// 	  //m and n are the local window along x is m and along y is n
// 	  for(m=x1;m<x2;m++)
// 	    {

// 	      for (n=y1;n<y2;n++)
// 		{

// 		  img_w_data =(float)data1[n*step + m*channels+ 0 ];
// 		  sum_local=sum_local+img_w_data;

// 		  //  cout<<"see"<<"for m="<<m<<"and n="<<n<<"is"<<img_w_data<<endl;
// 		}
// 	      //    cout<<"itr i="<<j<<endl;

// 	    }

// 	  //	cout<<"itr i:"<<i<<endl;

// 	  local_mean=sum_local/count;//mean of the windowed image

// 	  //	  cout<<"local mean is ="<<local_mean<<endl;
// 	  //	  cout<<"count is:"<<count<<endl;

// 	  for (m=x1;m<x2;m++)
// 	    {
// 	      for(n=y1;n<y2;n++)
// 		{
// 		  img_w_data =(float)data1[n*step + m*channels+ 0 ];
// 		  diff_sq=(local_mean-img_w_data)*(local_mean-img_w_data);

// 		  stand_dev=stand_dev+diff_sq;// cumulative standard deviation 
// 		}

// 	    }

// 	  stand_dev=stand_dev/count;// Standard deviation of a single pixel is given by this
// 	  //	  cout<<"standard deviation is:"<<stand_dev<<endl;
	 
// 	  // Modified  Niblacks algorithm ***********************************************
// 	  // thres_value=global_mean+0.2*stand_dev;//actual one where 0.1 is the constant value 
	  
// 	  // Original  Niblacks algorithm ***********************************************
// 	  //  thres_value=local_mean-0.1*stand_dev;//actual one where 0.1 is the constant value 
	  
// 	   //===============================================================================
// 	   //   cout<<"threshold value is:"<<thres_value<<endl;
// 	  // thres_value=global_mean-150*stand_dev;// modified one


// 	  //	thres_value=local_mean-0.2*stand_dev;// doesn't work

// 	   //Sauvola_s algorithm mean(x,y)+(1-k(1-(sigma_sq/R)))
// 	  float k_l=0.5;// for sauvola_s algo
// 	  int R=128;// for sauvola_s algo
// 	  thres_value=global_mean+(1-k_l*(1-stand_dev/R))+k_w;//remove this k_w= 5
// 	  //*********************************************************end of sauvola_s thresholding algorithm

// 	  if (img_data<thres_value)//set the image value to be zero else set the image value to be 255
			
// 	    cvSetReal2D(thres_img, j, i,0 );

// 	  else
// 	    cvSetReal2D(thres_img, j, i,255 );//threshold image 
// 	}
//     }

//   //  cout<<"successfully completed"<<endl;

//   return;
// }  //end of the function for obtaining local mean gradient thresholding algorithm

