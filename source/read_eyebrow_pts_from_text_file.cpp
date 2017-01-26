#include<stdio.h>
#include <cv.h>
int read_eyebrow_pts_from_text_file(char* filename, CvPoint &eye_brow_left, CvPoint &eye_brow_upper_mid, CvPoint &eye_brow_lower_mid, CvPoint &eye_brow_right )

{



  FILE *ofp;

 int a1=eye_brow_left.x;
 int b1=eye_brow_left.y;

 int a2=eye_brow_upper_mid.x;
 int b2=eye_brow_upper_mid.y;

 int a3=eye_brow_lower_mid.x;
 int b3=eye_brow_lower_mid.y;

 int a4=eye_brow_right.x;
 int b4=eye_brow_right.y;

 ofp = fopen(filename, "r+");

 fscanf(ofp, "%d \t %d \t %d \t %d \t %d \t %d \t %d \t %d  \n", a1, b1, a2, b2, a3, b3, a4, b4 );




 fclose(ofp);


return 0;
}
