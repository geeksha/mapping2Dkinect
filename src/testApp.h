#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#define KINECT

#ifdef KINECT
#include "ofxKinect.h"
#endif

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

class testApp : public ofBaseApp {
public:

void setup();
void update();
void draw();

void drawPointCloud();

#ifdef KINECT
ofxKinect kinect;
#endif
ofxCvColorImage colorImg;

ofxCvGrayscaleImage grayImage; // grayscale depth image
ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

ofxCvContourFinder contourFinder;

bool bThreshWithOpenCV;
bool bDrawPointCloud;

int nearThreshold;
int farThreshold;

int angle;

// used for viewing the point cloud
ofEasyCam easyCam;


ofVideoGrabber vidGrabber;
unsigned char * outputPixels;
ofTexture	videoTexture;
int camWidth;
int camHeight;
unsigned char *     pix;
unsigned char *     pixels;
int                 rgb_i;
float               floatImage;
float               colorImage;

};

