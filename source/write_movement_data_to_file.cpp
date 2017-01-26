#include<stdio.h>
#include <cv.h>


int write_movement_data_to_file(char* filename,  CvPoint2D32f* movement_array, const int inp_dimn=4 )

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
  for(int i=0; i<inp_dimn; i++)
    {
     
      int a1=movement_array[i].x;
      int b1=movement_array[i].y;

 
      fprintf(ofp, "%d \t %d \t", a1, b1 );
    
    }
  fprintf(ofp, " \n" );
  fclose(ofp);

  
 

  return 0;
}
