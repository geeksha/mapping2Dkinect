#include "testApp.h"

const int width = 640;
const int height = 480;
const int bpp = 3;
const int totalPixels = width * height;
const int totalBytes = totalPixels * bpp;

//--------------------------------------------------------------
void testApp::setup(){

camWidth = width;	// try to grab at this size.
camHeight = height;

vidGrabber.setVerbose(true);
vidGrabber.initGrabber(camWidth,camHeight);
videoTexture.allocate(camWidth,camHeight, GL_RGB);

ofSetLogLevel(OF_LOG_VERBOSE);

//mesh.setMode(OF_PRIMITIVE_POINTS)
#ifdef KINECT
// enable depth->video image calibration
//kinect.setRegistration(true);

kinect.init(true);
//kinect.init(true); // shows infrared instead of RGB video image
//kinect.init(false, false); // disable video image (faster fps)

kinect.open();	// opens first available kinect
//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #

// zero the tilt on startup
angle = 0;
kinect.setCameraTiltAngle(angle);

#endif

colorImg.allocate(width, height);
grayImage.allocate(width, height);
grayThreshNear.allocate(width, height);
grayThreshFar.allocate(width, height);

nearThreshold = 230;
farThreshold = 70;
bThreshWithOpenCV = true;

ofSetFrameRate(60);

outputPixels = new unsigned char[totalBytes];
pixels = colorImg.getPixels();
pix = grayImage.getPixels();

// start from the front
bDrawPointCloud = false;
}

void testApp::update() {

ofBackground(100, 100, 100);

kinect.update();

// there is a new frame and we are connected
if(kinect.isFrameNew()) {

// load grayscale depth image from the kinect source
grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

// we do two thresholds - one for the far plane and one for the near plane
// we then do a cvAnd to get the pixels which are a union of the two thresholds
if(bThreshWithOpenCV) {
grayThreshNear = grayImage;
grayThreshFar = grayImage;
grayThreshNear.threshold(nearThreshold, true);
grayThreshFar.threshold(farThreshold);
cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
} else {

// or we do it ourselves - show people how they can work with the pixels
pix = grayImage.getPixels();

int numPixels = grayImage.getWidth() * grayImage.getHeight();
for(int i = 0; i < numPixels; i++) {
if(pix[i] < nearThreshold && pix[i] > farThreshold) {
pix[i] = 255;
} else {
pix[i] = 0;
}
}
}

// update the cv images
grayImage.flagImageChanged();

// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
// also, find holes is set to true so we will get interior contours as well....
contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
}


    vidGrabber.update();
bool bNewFrame = vidGrabber.isFrameNew();

if (bNewFrame)
{
        colorImg.setFromPixels(vidGrabber.getPixels(), width, height);
   // mesh.addIndex();workshop reconstrucción volumetrica todo o relativo a clase ofMesh
//pix = colorImg.getPixels();
//floatImage = grayImage;
//floatImage /= 255.0;
//floatImage.scaleIntoMe(colorImage);
//floatImage *= colorImage;

//for (int i = 0; i < totalPixels; i++){

// if (pix[i] == 255) {
//  outputPixels[i] = pixels[i];

//  }

//  }
// rgb masking
for(int i = 0; i < totalPixels; i++)
{
int rgb_i = i * bpp;

if (pix[i] == 255)
{
outputPixels[rgb_i] = pixels[rgb_i];
outputPixels[rgb_i + 1] = pixels[rgb_i + 1];
outputPixels[rgb_i + 2] = pixels[rgb_i + 2];
}
else
{
outputPixels[rgb_i] = 0;
outputPixels[rgb_i + 1] = 0;
outputPixels[rgb_i + 2] = 0;
}
}

videoTexture.loadData (outputPixels, width, height, GL_RGB);
}

}

//--------------------------------------------------------------
void testApp::draw()
{
videoTexture.draw(0,0);
//getTextureRef().bind();
//mesh.draw();
//getTextureRef().unbind();
//glEnable(GL_DEPTH_TEST);
//glDisable(GL_DEPTH_TEST);
}



