#include "stdafx.h"
#include "ddARToolkitPlus.h"

#include "ARToolKitPlus/TrackerMultiMarker.h"



ARToolKitPlus::TrackerMultiMarker *tracker;
ARToolKitPlus::ARMultiMarkerInfoT *multiMarker;

//class MyLogger : public ARToolKitPlus::Logger
//{
//    void artLog(const char* nStr)
//    {
//        cout <<nStr;
//    }
//};

//static MyLogger logger;

ddARToolkitPlus::ddARToolkitPlus() {
	multiMarkerLoaded = false;
}

ddARToolkitPlus::~ddARToolkitPlus() {
	// This must be static so don't delete it when using multiple instances
	// TODO: Keep count of instances, and delete final one.
	//if( tracker != NULL ) { delete tracker; }
}


//--------------------------------------------------
void ddARToolkitPlus::setup(int w, int h) {
	// load std. ARToolKit camera file	
	// These need to be in the data folder
	setup(w, h, "data/Logitech_Notebook_Pro.cal", "data/markerboard_480-499.cfg");
}

//--------------------------------------------------
void ddARToolkitPlus::setup(int w, int h, string camParamFile, string multiFile, int maxImagePatterns, int pattWidth, int pattHeight, int pattSamples, int maxLoadPatterns){
	
	width = w;
	height = h;	
	useBCH = true;
	markerWidth = 40.0;
	halfMarkerWidth = markerWidth/2;
	c[0] = 0;
	c[1] = 0;
	
	// ----------------------------------  AR TK+ STUFF - ripped from the single marker demo app
    
	// create a tracker that does:
    //  - "pattWidth" x "pattHeight" sized marker images (6x6 required for binary markers)
    //  - samples at a maximum of 6x6
    //  - works with luminance (gray) images
    //  - can load a maximum of "maxLoadPatterns" non-binary pattern
    //  - can detect a maximum of "maxImagePatterns" patterns in one image
    tracker = new ARToolKitPlus::TrackerMultiMarker (width, height, maxImagePatterns, pattWidth, pattHeight, pattSamples, maxLoadPatterns);
//	const char* description = tracker->getDescription();
//	printf("ARToolKitPlus compile-time information:\n%s\n\n", description);
	
    // set a logger so we can output error messages
//    tracker->setLogger(&logger);
    //  - works with luminance (gray) images
	tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);	

	tracker->setImageProcessingMode(ARToolKitPlus::IMAGE_FULL_RES);
	
	// Initialize a multimarker tracker with
	// Camera and marker files
	// & near and far clipping values for the OpenGL projection matrix

	
    if( !tracker->init( camParamFile.c_str(),
						multiFile.c_str(),
						1.0f, 1000.0f) )	
	{

		printf("ERROR: init() failed\n");
		delete tracker;
	
		return;
	}
	tracker->getCamera()->printSettings();
    // define size of the marker
    //tracker->setPatternWidth(80); // I'm not sure how to define the size with multimarkers since it doesnt seem to have this option.
	
	// the marker in the BCH test image has a thin border...
    tracker->setBorderWidth(useBCH ? 0.125f : 0.250f);
    //tracker->setBorderWidth(0.250f);	
	
    // set a threshold. alternatively we could also activate automatic thresholding
    tracker->setThreshold(85);

	// let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
	//tracker->setUndistortionMode(ARToolKitPlus::UNDIST_STD);
	
    // RPP is more robust than ARToolKit's standard pose estimator
    tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);
	
    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);
	//tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);
	
	//tracker->activateVignettingCompensation(true);
	
	tracker->setUseDetectLite(false);
	
	setupHomoSrc();
	
}

//--------------------------------------------------
int ddARToolkitPlus::update(unsigned char *pixels) {
	return tracker->calc(pixels);
}

//--------------------------------------------------
void ddARToolkitPlus::draw() {
	draw(0, 0, width, height);
}

void ddARToolkitPlus::draw(int x, int y) {
	draw(x, y, width, height);
}
/*
void ddARToolkitPlus::draw(int x, int y, int w, int h) {
	
	float scaleX = (float)w / width;
	float scaleY = (float)h / height;
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(scaleX, scaleY, 1);
	ofSetLineWidth(1);
	
	int numDetected = tracker->getNumDetectedMarkers();
	
	for(int i=0; i<numDetected; i++) {
		
		ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(i);
		
		// Draw the center point
		ofFill();
		ofSetColor(255, 0, 255 );
		ofRect(marker.pos[0]-1, marker.pos[1]-1, 2, 2);
		
		// Draw the inner rectangle
		ofNoFill();
		ofSetColor(255, 255, 0 );		
		ofBeginShape();
		ofVertex(marker.vertex[0][0], marker.vertex[0][1]);
		ofVertex(marker.vertex[1][0], marker.vertex[1][1]);
		ofVertex(marker.vertex[2][0], marker.vertex[2][1]);
		ofVertex(marker.vertex[3][0], marker.vertex[3][1]);
		ofEndShape(true);
		
		// Draw the marker ID
		ofSetColor( 255, 255, 255 );	
		ofDrawBitmapString(ofToString(marker.id), marker.pos[0], marker.pos[1]);
		
		// Draw the outer rectangle
//		dhVector center(marker.pos[0], marker.pos[1]);
//		ofNoFill();
//		ofSetColor(255, 0, 0 );		
//		ofBeginShape();
//		for (int j=0; j<4; j++) {
//			dhVector corner(marker.vertex[j][0], marker.vertex[j][1]);
//			corner -= center;
//			corner *= BORDER_SCALE;
//			corner += center;
//			ofVertex(corner.x, corner.y);
//		}
//		ofEndShape(true);
		
	}
	
	glPopMatrix();
	
}
*/
//--------------------------------------------------
void ddARToolkitPlus::applyProjectionMatrix(int viewportWidth, int viewportHeight){
	glViewport(0, 0, viewportWidth, viewportHeight );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf(tracker->getProjectionMatrix());
	
}

//--------------------------------------------------
void ddARToolkitPlus::applyProjectionMatrix(){
	glViewport(0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf(tracker->getProjectionMatrix());
	
}

void ddARToolkitPlus::applyModelMatrix(int markerIndex) {

	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	
	getTransMat( &marker, c, m34 );
	
	// Convert from ARTK matrix to OpenGL format
	m[0] = m34[0][0];
	m[1] = m34[1][0];	
	m[2] = m34[2][0];		
	m[3] = 0;			

	m[4] = m34[0][1];
	m[5] = m34[1][1];	
	m[6] = m34[2][1];		
	m[7] = 0;	

	m[8] = m34[0][2];
	m[9] = m34[1][2];	
	m[10] = m34[2][2];		
	m[11] = 0;	
	
	m[12] = m34[0][3];
	m[13] = m34[1][3];	
	m[14] = m34[2][3];		
	m[15] = 1;	
		
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( m );
	
}

dhMat ddARToolkitPlus::getMatrix(int markerIndex) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	
	getTransMat( &marker, c, m34 );

	dhMat matrix;
	matrix.v[0] = m34[0][0];
	matrix.v[1] = m34[1][0];
	matrix.v[2] = m34[2][0];
	matrix.v[3] = 0;

	matrix.v[4] = m34[0][1];
	matrix.v[5] = m34[1][1];
	matrix.v[6] = m34[2][1];
	matrix.v[7] = 0;

	matrix.v[8] = m34[0][2];
	matrix.v[9] = m34[1][2];
	matrix.v[10] = m34[2][2];
	matrix.v[11] = 0;

	matrix.v[12] = m34[0][3];
	matrix.v[13] = m34[1][3];
	matrix.v[14] = m34[2][3];
	matrix.v[15] = 1;
		
	return matrix;
}

dhMat ddARToolkitPlus::getGLMatrix(int markerIndex) 
{
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	
	getTransMat( &marker, c, m34 );

	// OpenGL Order

	dhMat matrix;
	matrix.v[0] = m34[0][0];
	matrix.v[1] = m34[1][0];
	matrix.v[2] = m34[2][0];
	matrix.v[3] = 0;

	matrix.v[4] = m34[0][1];
	matrix.v[5] = m34[1][1];
	matrix.v[6] = m34[2][1];
	matrix.v[7] = 0;

	matrix.v[8] = m34[0][2];
	matrix.v[9] = m34[1][2];
	matrix.v[10] = m34[2][2];
	matrix.v[11] = 0;

	matrix.v[12] = m34[0][3];
	matrix.v[13] = m34[1][3];
	matrix.v[14] = m34[2][3];
	matrix.v[15] = 1;
		
	/*
	(m34[0][0], m34[1][0], m34[2][0], 0,
	m34[0][1], m34[1][1], m34[2][1], 0,
	m34[0][2], m34[1][2], m34[2][2], 0,
	m34[0][3], m34[1][3], m34[2][3], 1);
	*/
	return matrix;
}

dhMat ddARToolkitPlus::getHomography(int markerIndex) {
	vector<dhVector> corners;
	getDetectedMarkerOrderedBorderCorners(markerIndex, corners);
	return findHomography(homoSrc, corners);
}

dhMat ddARToolkitPlus::getHomography(int markerIndex, vector<dhVector> &src) {
	vector<dhVector> corners;
	getDetectedMarkerOrderedBorderCorners(markerIndex, corners);
	return findHomography(src, corners);
}


dhVector ddARToolkitPlus::getTranslation(int markerIndex) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	
	getTransMat( &marker, c, m34 );
	
	dhVector trans(m34[0][3], m34[1][3], m34[2][3]);
	return trans;
}

dhMat ddARToolkitPlus::getOrientationMatrix(int markerIndex) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);

	getTransMat( &marker, c, m34 );

	dhMat matrix;
	matrix.v[0] = m34[0][0];
	matrix.v[1] = m34[1][0];
	matrix.v[2] = m34[2][0];
	matrix.v[3] = 0;

	matrix.v[4] = m34[0][1];
	matrix.v[5] = m34[1][1];
	matrix.v[6] = m34[2][1];
	matrix.v[7] = 0;

	matrix.v[8] = m34[0][2];
	matrix.v[9] = m34[1][2];
	matrix.v[10] = m34[2][2];
	matrix.v[11] = 0;

	matrix.v[12] = 0;
	matrix.v[13] = 0;
	matrix.v[14] = 0;
	matrix.v[15] = 1;
	/*
	dhMat matrix(m34[0][0], m34[0][1], m34[0][2], 0,
						m34[1][0], m34[1][1], m34[1][2], 0,
						m34[2][0], m34[2][1], m34[2][2], 0,
						0, 0, 0, 1);
						*/
	return matrix;
}

dhQuat ddARToolkitPlus::getOrientationQuaternion(int markerIndex) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	
	getTransMat( &marker, c, m34 );
	
	dhMat matrix;
	matrix.v[0] = m34[0][0];
	matrix.v[1] = m34[1][0];
	matrix.v[2] = m34[2][0];
	matrix.v[3] = 0;

	matrix.v[4] = m34[0][1];
	matrix.v[5] = m34[1][1];
	matrix.v[6] = m34[2][1];
	matrix.v[7] = 0;

	matrix.v[8] = m34[0][2];
	matrix.v[9] = m34[1][2];
	matrix.v[10] = m34[2][2];
	matrix.v[11] = 0;

	matrix.v[12] = 0;
	matrix.v[13] = 0;
	matrix.v[14] = 0;
	matrix.v[15] = 1;

	/*
	dhMat matrix(m34[0][0], m34[0][1], m34[0][2], 0,
						m34[1][0], m34[1][1], m34[1][2], 0,
						m34[2][0], m34[2][1], m34[2][2], 0,
						0, 0, 0, 1);
						*/
	return matrix.getRotate();
}
/*
void ddARToolkitPlus::getTranslationAndOrientation(int markerIndex, dhVector &translation, dhMat &orientation) {
	
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);

	getTransMat( &marker, c, m34 );
	
	// Translation
	translation.set(m34[0][3], m34[1][3], m34[2][3]);
	
	// Orientation
	orientation.set(m34[0][0], m34[0][1], m34[0][2], 0,
					m34[1][0], m34[1][1], m34[1][2], 0,
					m34[2][0], m34[2][1], m34[2][2], 0,
					0, 0, 0, 1);
}

dhVector ddARToolkitPlus::getCameraPosition(int markerIndex)  {

	// Translation
	dhVector trans;
	// Orientation
	dhMat orient;
	
	getTranslationAndOrientation(markerIndex, trans, orient);
	
	// Transpose of the camera orientation
	dhMat tOrient = dhMat::getTransposedOf(orient);
	// Negative of the transpose
	dhMat negtOrient = tOrient;
	negtOrient.scale(-1, -1, -1);
	
	// Camera Location
	// Location of the camera relative to the marker
	// z appears up
	dhVector loc = negtOrient * trans;
	
	return loc;
}
*/
void ddARToolkitPlus::getMultiMarkerTranslationAndOrientation(dhVector &translation, dhMat &orientation) {

	const ARToolKitPlus::ARMultiMarkerInfoT *multiMarkerConst = tracker->getMultiMarkerConfig();
	if(multiMarkerConst != NULL) {
		// Create a copy of the ARMultiMarkerInfoT struct
		ARToolKitPlus::ARMultiMarkerInfoT mm;
		size_t mmSize = sizeof(ARToolKitPlus::ARMultiMarkerInfoT);
		memcpy(&mm, multiMarkerConst, mmSize);
		
		// Copy and pass in the markers
		int numberOfMarkers = tracker->getNumDetectedMarkers();
#ifdef TARGET_WIN32
		ARToolKitPlus::ARMarkerInfo *marker = new ARToolKitPlus::ARMarkerInfo[numberOfMarkers];
#else
		ARToolKitPlus::ARMarkerInfo marker[numberOfMarkers];
#endif
		for (int i=0; i<numberOfMarkers; i++) {
			marker[i] = tracker->getDetectedMarker(i);
		}
		float result = tracker->rppMultiGetTransMat(marker, numberOfMarkers, &mm);
		
		// Check for error - yes this does occur
		if(result < 0 || result >= INT_MAX) {
			tracker->arMultiGetTransMat(marker, numberOfMarkers, &mm);
			//ofLog(OF_LOG_VERBOSE, "RPP failed on multimarker");	
		} 
		
		// Translation
		translation.set(mm.trans[0][3], mm.trans[1][3], mm.trans[2][3]);		
		// Orientation
		orientation.set(mm.trans[0][0], mm.trans[0][1], mm.trans[0][2], 0,
						mm.trans[1][0], mm.trans[1][1], mm.trans[1][2], 0,
						mm.trans[2][0], mm.trans[2][1], mm.trans[2][2], 0,
						0, 0, 0, 1);
#ifdef TARGET_WIN32
		free(marker);
#endif
	} else {
		;//ofLog(OF_LOG_VERBOSE, "MultiMarkerConfig file NULL");
	}

}

bool ddARToolkitPlus::loadMultiMarkerFile(string filename) {

	string fullFilePath = filename;////ofToDataPath(filename);
	if(multiMarkerLoaded) {
		tracker->arMultiFreeConfig(multiMarker);		
	}
	multiMarker = tracker->arMultiReadConfigFile(fullFilePath.c_str());
	
	if(multiMarker==NULL) {
		multiMarkerLoaded = false;
		return false;
	} else {
		multiMarkerLoaded = true;
		return true;
	}
	
}


//--------------------------------------------------
void ddARToolkitPlus::setThreshold(int threshold) {
	tracker->activateAutoThreshold(false);
	tracker->setThreshold(threshold);
}

void ddARToolkitPlus::activateAutoThreshold(bool state) {
	tracker->activateAutoThreshold(state);
}

void ddARToolkitPlus::setMarkerWidth(float mm) {
	markerWidth = mm;
	halfMarkerWidth = markerWidth/2;
	setupHomoSrc();
}

void ddARToolkitPlus::setupHomoSrc() {
	
	homoSrc.clear();
	homoSrc.push_back(dhVector(-halfMarkerWidth, -halfMarkerWidth));
	homoSrc.push_back(dhVector(halfMarkerWidth, -halfMarkerWidth));		
	homoSrc.push_back(dhVector(halfMarkerWidth, halfMarkerWidth));		
	homoSrc.push_back(dhVector(-halfMarkerWidth, halfMarkerWidth));
	
}

//--------------------------------------------------
int ddARToolkitPlus::getNumDetectedMarkers() {
	return tracker->getNumDetectedMarkers();
}

int ddARToolkitPlus::getMarkerIndex(int markerID) {
	int numDetected = tracker->getNumDetectedMarkers();
	for(int i=0; i<numDetected; i++) {
		ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(i);
		if(marker.id == markerID) {
			return i;
		}
	}
	return -1;
}

int ddARToolkitPlus::getMarkerID(int markerIndex) {
	int numDetected = tracker->getNumDetectedMarkers();
	if (markerIndex < 0 || markerIndex >= numDetected) {
		return -1;
	}
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	return marker.id;
}

//--------------------------------------------------
dhVector ddARToolkitPlus::getDetectedMarkerCenter(int markerIndex) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	return dhVector(marker.pos[0], marker.pos[1]);
}

int ddARToolkitPlus::getDetectedMarkerDirection(int markerIndex) {
	return tracker->getDetectedMarker(markerIndex).dir;
}

void ddARToolkitPlus::getDetectedMarkerCorners(int markerIndex, vector<dhVector> &corners) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	corners.clear();
	corners.push_back(dhVector(marker.vertex[0][0], marker.vertex[0][1]));
	corners.push_back(dhVector(marker.vertex[1][0], marker.vertex[1][1]));
	corners.push_back(dhVector(marker.vertex[2][0], marker.vertex[2][1]));
	corners.push_back(dhVector(marker.vertex[3][0], marker.vertex[3][1]));
}

void ddARToolkitPlus::getDetectedMarkerOrderedCorners(int markerIndex, vector<dhVector> &corners) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	corners.clear();
	
	int index = 4 - marker.dir;
	for (int i=0; i<4; i++) {		
		corners.push_back(dhVector(marker.vertex[index][0], marker.vertex[index][1]));
		index++;
		if(index >= 4) {
			index = 0;
		}
	}	
}

void ddARToolkitPlus::getDetectedMarkerBorderCorners(int markerIndex, vector<dhVector> &corners) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	corners.clear();
	corners.push_back(dhVector(marker.vertex[0][0], marker.vertex[0][1]));
	corners.push_back(dhVector(marker.vertex[1][0], marker.vertex[1][1]));
	corners.push_back(dhVector(marker.vertex[2][0], marker.vertex[2][1]));
	corners.push_back(dhVector(marker.vertex[3][0], marker.vertex[3][1]));
	
	dhVector center(marker.pos[0], marker.pos[1]);
	for (int j=0; j<4; j++) {
		corners[j] -= center;
		corners[j] *= BORDER_SCALE;
		corners[j] += center;
	}
}

void ddARToolkitPlus::getDetectedMarkerOrderedBorderCorners(int markerIndex, vector<dhVector> &corners) {
	ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(markerIndex);
	corners.clear();
	
	int index = 4 - marker.dir;
	for (int i=0; i<4; i++) {		
		if(index >= 4) {
			index = 0;
		}		
		corners.push_back(dhVector(marker.vertex[index][0], marker.vertex[index][1]));
		index++;
	}
		
	dhVector center(marker.pos[0], marker.pos[1]);
	for (int j=0; j<4; j++) {
		corners[j] -= center;
		corners[j] *= BORDER_SCALE;
		corners[j] += center;
	}
}

void ddARToolkitPlus::getTransMat(ARToolKitPlus::ARMarkerInfo *marker_info, float center[2], float conv[3][4]) {
	float result = tracker->rppGetTransMat( marker_info, center, markerWidth, conv );
	
	// Check for error - yes this does occur
	if(result < 0 || result >= INT_MAX) {
		//ofLog(OF_LOG_VERBOSE, "RPP failed on marker " + ofToString(marker_info->id));	
		// Use standard pose estimation
		tracker->arGetTransMat( marker_info, center, markerWidth, conv );
	}
}
