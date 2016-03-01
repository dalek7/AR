#include "./include/FlyCapture2.h"
#include "../CvvImage.h"
using namespace FlyCapture2;

#include "../Elapsed.h"
#include "../ddCVMacro.h"
#include "../ttMath.h"

#include "../FileProc.h"

class PGRCamera
{
	public:

		//static const int camWidth  = 1280;//1280;//2080
		//static const int camHeight = 1024;//1552;

		int camWidth;//1280;//2080
		int camHeight;//1552;

		IplImage* mCamImage;

		Camera*   mCamera;
		CvvImage cimg;

		CElapsed m_tm;
		float t0;
		int cnt1;

		float tf[30];
//		ofImage*  mCamImage;

		PGRCamera()
		{
			camWidth  = 1280;//1280;//2080
			camHeight = 1024;//1552;

			mCamImage = NULL;


			t0 = 0;
			cnt1 = 0;
			for(int i=0; i<10; i++)
			{
				tf[i] = 0;
			}
		}

		~PGRCamera()
		{
			mCamera->StopCapture();
			mCamera->Disconnect();

			delete mCamera;
//			delete mCamImage;

			if(mCamImage)
				cvReleaseImage(&mCamImage);
		}

		void catchError(Error error) 
		{
			if(error != PGRERROR_OK)
				error.PrintErrorTrace();
		}

		void printInfo(const CameraInfo& cameraInfo) 
		{
			printf(
				"\n*** CAMERA INFORMATION ***\n"
				"Serial number - %u\n"
				"Camera model - %s\n"
				"Camera vendor - %s\n"
				"Sensor - %s\n"
				"Resolution - %s\n"
				"Firmware version - %s\n"
				"Firmware build time - %s\n\n",
				cameraInfo.serialNumber,
				cameraInfo.modelName,
				cameraInfo.vendorName,
				cameraInfo.sensorInfo,
				cameraInfo.sensorResolution,
				cameraInfo.firmwareVersion,
				cameraInfo.firmwareBuildTime);
		}

		void printBuildInfo()
		{
			FC2Version fc2Version;
			Utilities::GetLibraryVersion( &fc2Version );
			char version[128];
			sprintf(version,"FlyCapture2 library version: %d.%d.%d.%d\n",fc2Version.major, fc2Version.minor, fc2Version.type, fc2Version.build );  
				
			printf( "%s", version );

			char timeStamp[512];
			sprintf( timeStamp, "Application build date: %s %s\n\n", __DATE__, __TIME__ );

			printf( "%s", timeStamp );

		}

		void printFormat7Capabilities(Format7Info fmt7Info)
		{
			 printf(
					"Max image pixels: (%u, %u)\n"
					"Image Unit size: (%u, %u)\n"
					"Offset Unit size: (%u, %u)\n"
					"Pixel format bitfield: 0x%08x\n",
					fmt7Info.maxWidth,
					fmt7Info.maxHeight,
					fmt7Info.imageHStepSize,
					fmt7Info.imageVStepSize,
					fmt7Info.offsetHStepSize,
					fmt7Info.offsetVStepSize,
					fmt7Info.pixelFormatBitField );

		}

		void setup(int w, int h) 
		{

			camWidth  = w;//1280;//2080
			camHeight = h;//1552;
			setup() ;

		}
		void setup() 
		{
			printBuildInfo();

			BusManager busMgr;

			unsigned int numCameras;

			catchError(busMgr.GetNumOfCameras(&numCameras));

			printf("Number of cameras detected: %u\n",numCameras);

			if(numCameras < 1)
			{
				printf("Insufficient number of cameras... exiting\n");
				return;
			}

			//Init Camera
			const Mode fmt7Mode = MODE_0;
			const PixelFormat fmt7PixFmt = PIXEL_FORMAT_MONO8;

			Camera* camera = new Camera();

			PGRGuid		guid;
			CameraInfo  camInfo;
			Format7Info fmt7Info;

			//Get First Connected Camera	
			catchError(busMgr.GetCameraFromIndex(0,&guid));
			
			//Connect to Camera
			catchError(camera->Connect(&guid));
			
			//Get Camera Information
			catchError(camera->GetCameraInfo(&camInfo));
			printInfo(camInfo);

			//Query Format 7 Modes
			bool supported;
			fmt7Info.mode = fmt7Mode;
			catchError(camera->GetFormat7Info(&fmt7Info,&supported));
			printFormat7Capabilities(fmt7Info);

			if ( (fmt7PixFmt & fmt7Info.pixelFormatBitField) == 0 )
			{
				// Pixel format not supported!
				printf("Pixel format is not supported\n");
				return;
			}

			Format7ImageSettings fmt7ImageSettings;

			fmt7ImageSettings.mode			= fmt7Mode;
			fmt7ImageSettings.offsetX		= 0;
			fmt7ImageSettings.offsetY		= 0;
			fmt7ImageSettings.width			= fmt7Info.maxWidth;
			fmt7ImageSettings.height		= fmt7Info.maxHeight;
			fmt7ImageSettings.pixelFormat	= fmt7PixFmt;

			bool valid;
			Format7PacketInfo fmt7PacketInfo;

			//Validate Format 7 Settings
			catchError(camera->ValidateFormat7Settings(&fmt7ImageSettings,&valid,&fmt7PacketInfo));

			if(!valid)
			{
				printf("Format7 Settings are not Valid\n");
			}

			//Set Settings to Camera
			catchError(camera->SetFormat7Configuration(&fmt7ImageSettings,fmt7PacketInfo.recommendedBytesPerPacket));

			//Start Capture
			catchError(camera->StartCapture());

			mCamImage = cvCreateImage( cvSize(camWidth,camHeight), IPL_DEPTH_8U, 1);

			mCamera	   = camera;

			t0 = m_tm.End();

/*
			//Allocate Image
			ofImage* image = new ofImage();
			image->allocate(camWidth,camHeight,OF_IMAGE_GRAYSCALE);//OF_IMAGE_GRAYSCALE//OF_IMAGE_COLOR
				
			//Init Camera & Image
			
			mCamImage  = image;

*/
		}

		void update() 
		{
			float t1 = m_tm.End();
			float dt = t1 - t0;
			ShiftWithNew<float>(tf,dt,30);
			int i;
			float sum1=0;
			for(i=0; i<30; i++)
			{
				sum1 += tf[i];
			}
			
			sum1 /= 30.0;
			t0 = t1;
			char txt1[255];
			sprintf(txt1, "%d, %.1f fps", cnt1, 1/sum1 );


			///

			Camera& camera = *mCamera;

			Image rawImage;
			catchError(camera.RetrieveBuffer(&rawImage));
			memmove(mCamImage->imageData,rawImage.GetData(),camWidth*camHeight);

			IplImage *img1 = cvCreateImage( cvSize(800,640), IPL_DEPTH_8U, 1);

			cvResize(mCamImage, img1);


			cvText(img1, txt1, CPoint(10,20));

			cimg.CopyOf(img1);

			cvReleaseImage(&img1);
			
			

			

			

			//cvText(mCamImage, txt1, CPoint(10,20));

			if(0)
			if(cnt1%30 ==0)
			{


				CString fb;

				FileProc::MakeFileNameWithDateTime(&fb);


				CString fndata;

				fndata.Format("../_out/SC_%s.png", fb);

				cimg.Save(fndata);

			}
/*			
			

			
			

			//Flip Image
			//mCamImage->mirror(true,false);
			mCamImage->mirror(false,false);

			mCamImage->update();
			*/

			cnt1++;
		}

		void draw(CPaintDC &dc) 
		{
			
			if(cimg.GetImage())
			{

				//CDC *pDC2 = m_tex_view.GetDC();
				//cimg.DrawToHDC(dc.GetSafeHdc(), CRect(0,0,1024,768));//dc->

				cimg.DrawToHDC(dc.GetSafeHdc(), CRect(0,0,800,640));//dc->
				//ReleaseDC(pDC2);

				

			}


			//mCamImage->draw(0,0);
		}
/*
		ofImage* getCamImage()
		{
			return mCamImage;
		}
*/
};