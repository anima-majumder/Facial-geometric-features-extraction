
//note **** it is observed that the data is reading in BGR format***********//

  #include "segmented_data.h"

void segmented_data(int &count, IplImage* inp_img )
{
  //  IplImage* inp_img=cvLoadImage("./lip.jpeg",1);
//Get data of the images of each plane.
  // This part of the code is used to obtain the sudo hue plane of the image. 
  if (inp_img==NULL)
    std::cout<<"image not found in segmentated data file "<<std::endl;
  uchar * data;
  int height, width, channels, step;
  float r_data, g_data, b_data;
  CvScalar s;
  // float hue_rg, hue_rg_smallest=0, hue_rg_largest=0;
  // int sudo_hue;
  float get_range=190;
  // get the image data
  height    = inp_img->height;
  width     = inp_img->width;
  step      = inp_img->widthStep;
  channels  = inp_img->nChannels;
  data    = (uchar*)inp_img->imageData;
  //  CvScalar s;
  // float hue_rg_arr[width][height];
  printf("Processing a %dx%d image with %d channels\n",height,width,channels); 
 
  IplImage* r_plane = cvCreateImage( cvGetSize(inp_img), 8, 1 );
  IplImage* g_plane = cvCreateImage( cvGetSize(inp_img), 8, 1 );
  IplImage* b_plane = cvCreateImage( cvGetSize(inp_img), 8, 1 );

 cvSplit(inp_img, r_plane,g_plane,b_plane, 0);

 count=0;//initialize the count to zero;


  for(int i=0;i< width;i++)
    {
      for(int j=0;j<height;j++)
	{
	  r_data=(float)data[j*step + i*channels+ 0 ];
	  //  cout<<"red data are :"<<r_data<<endl;


	  g_data=(float)data[j*step + i*channels+ 1 ];
	  // cout<<"green data are :"<<g_data<<endl;
	  b_data=(float)data[j*step + i*channels+ 2 ];
	  // calculate the  hue_rg value of the image as follows
	  //	  std::cout<<"r data=\t"<<r_data<<"g data=\t"<<g_data<<"b data=\t"<<b_data<<std::endl;
	  if((r_data>get_range && g_data>get_range && b_data>get_range)||(r_data<80 && g_data<80 && b_data<80))

	    {
	      //	 std::cout<<"inside if condn r data=\t"<<r_data<<"g data=\t"<<g_data<<"b data=\t"<<b_data<<std::endl;
	   
	      count++;//increase the count  if condition is satisfies

	 s.val[0]=110;//150
	 s.val[1]=105;//150
	 s.val[2]=170;//230
	    
	  cvSetReal2D(r_plane, j, i,s.val[0] );
	  cvSetReal2D(g_plane, j, i,s.val[1] );
	  cvSetReal2D(b_plane, j, i,s.val[2] );


	    }


	}
    }

 
  cvMerge(r_plane,g_plane,b_plane,0,inp_img);
  cvSaveImage("lip.jpeg",inp_img);

  cvReleaseImage (&r_plane);
  cvReleaseImage (&g_plane);
  cvReleaseImage (&b_plane);
  //cvReleaseImage (&inp_img);
return;
}
