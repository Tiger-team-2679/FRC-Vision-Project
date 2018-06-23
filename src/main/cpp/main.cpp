#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>

using namespace std;
using namespace cv;

void fastBlur(Mat image, int radius) {
	if (radius<1) return;
	int w = image.cols;
	int h = image.rows;
	unsigned char * pix = (unsigned char*)(image.data);
	int wm=w-1;
	int hm=h-1;
	int wh=w*h;
	int div=radius+radius+1;
	unsigned char *r=new unsigned char[wh];
	unsigned char *g=new unsigned char[wh];
	unsigned char *b=new unsigned char[wh];
	int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
	int *vMIN = new int[MAX(w,h)];
	int *vMAX = new int[MAX(w,h)];

	unsigned char *dv=new unsigned char[256*div];
	for (i=0;i<256*div;i++) dv[i]=(i/div);

	yw=yi=0;

	for (y=0;y<h;y++){
		rsum=gsum=bsum=0;
		for(i=-radius;i<=radius;i++){
			p = (yi + MIN(wm, MAX(i,0))) * 3;
			rsum += pix[p];
			gsum += pix[p+1];
			bsum += pix[p+2];
		}
		for (x=0;x<w;x++){

			r[yi]=dv[rsum];
			g[yi]=dv[gsum];
			b[yi]=dv[bsum];

			if(y==0){
				vMIN[x]=MIN(x+radius+1,wm);
				vMAX[x]=MAX(x-radius,0);
			}
			p1 = (yw+vMIN[x])*3;
			p2 = (yw+vMAX[x])*3;

			rsum += pix[p1] - pix[p2];
			gsum += pix[p1+1] - pix[p2+1];
			bsum += pix[p1+2] - pix[p2+2];

			yi++;
		}
		yw+=w;
	}

	for (x=0;x<w;x++){
		rsum=gsum=bsum=0;
		yp=-radius*w;
		for(i=-radius;i<=radius;i++){
			yi=MAX(0,yp)+x;
			rsum+=r[yi];
			gsum+=g[yi];
			bsum+=b[yi];
			yp+=w;
		}
		yi=x;
		for (y=0;y<h;y++){
			pix[yi*3] = dv[rsum];
			pix[yi*3 + 1] = dv[gsum];
			pix[yi*3 + 2] = dv[bsum];
			if(x==0){
				vMIN[y]=MIN(y+radius+1,hm)*w;
				vMAX[y]=MAX(y-radius,0)*w;
			}
			p1=x+vMIN[y];
			p2=x+vMAX[y];

			rsum+=r[p1]-r[p2];
			gsum+=g[p1]-g[p2];
			bsum+=b[p1]-b[p2];

			yi+=w;
		}	
	}

	delete r;
	delete g;
	delete b;

	delete vMIN;
	delete vMAX;
	delete dv;
}

int main(int, char**)
{
	clock_t time0;
    clock_t time1;
	Mat frame;
	Mat threshold;
	
    VideoCapture cap(0); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
        return -1;
	
    while(1)
    {
		cap >> frame;
		time0 = clock();  
		fastBlur(frame, 1);
		inRange(frame, Scalar(110,50,50), Scalar(130,255,255), threshold);
		time1 = clock();  
		printf("inRange time is: %d\n", time1-time0);
		imshow("frame", frame);
        if(waitKey(27) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

