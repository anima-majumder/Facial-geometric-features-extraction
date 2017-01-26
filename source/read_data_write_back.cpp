
#include "constants.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include<cvaux.h>



int read_data_write_back()
{

  
 
  CvPoint2D32f* eyebrow_data= new CvPoint2D32f[8];
  double* eye1_proj_r= new double[1];
  double* eye2_proj_r= new double[1];
  CvPoint2D32f* lip_data= new CvPoint2D32f[4];

  std::ifstream febr;//eyebrow_data
  open2read(febr,"eyebrow_movement_array.txt");

  std::ifstream fe1r;//eye1 projection data
  open2read(fe1r,"eye1_projection_ratio.txt");
  
  std::ifstream fe2r;//eye2 proj data
  open2read(fe2r,"eye2_projection_ratio.txt");
  
 std::ifstream flr;//lip data
 open2read(flr,"lip_feature_points.txt"); 

  CvPoint2D32f* au_data= new CvPoint2D32f[13];//at the ninth index eye_proj data will be there

  std::ofstream fw;//eyebrow_data
   
  //  open2write(fw,"au_data.txt");
  //*************************************************//
  //open to write the file in append mode 
  fw.open("au_data.txt", std::ios_base::out | std::ios::app);
  if(fw== NULL)
    {
      std::cout<<"err in opening file to write "<<std::endl;
      return(0);
    }

 
  //******************************************************//

  //read all the data from different files, combine all and write back all the data into a single file
  while (!fe1r.eof() && !fe1r.eof() && !fe2r.eof() && !flr.eof())
      //for(int i=0; i<10; i++)
      {

    
	for (int i=0; i<8; i++)
	  {
	  febr >> eyebrow_data[i].x>>eyebrow_data[i].y;
	  au_data[i].x=eyebrow_data[i].x;
	  au_data[i].y=eyebrow_data[i].y;
	  }


	fe1r>>eye1_proj_r[0]; 
	au_data[8].x=eye1_proj_r[0]*0.1;


	fe2r>>eye2_proj_r[0];
	au_data[8].y=eye2_proj_r[0]*0.1;



	for (int i=0; i<4; i++)
	  {
	  flr>>lip_data[i].x>>lip_data[i].y;
	  au_data[i+9].x=lip_data[i].x;
	  au_data[i+9].y=lip_data[i].y;
	 
	  }


	//	write all the data to a new file 
	
    

	for (int i=0; i<13; i++)
	  {
	    // std::cout<<"\t"<<au_data[i].x <<"\t"<<au_data[i].y; 

	    fw<<au_data[i].x<<"\t"<<au_data[i].y<<"\t";
	  }
	fw<<"\n";
	//	std::cout<<std::endl;




 }
 

  

 

 
  delete []eyebrow_data;
  delete []eye1_proj_r;
  delete []eye2_proj_r;
  delete []lip_data;
  delete []au_data;
  
  //===============================
 
  febr.close();
  fe1r.close();
  fe2r.close();
  flr.close();
  fw.close();
  return(0);
}
