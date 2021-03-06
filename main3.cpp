#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <exception>
#include <unistd.h>

using namespace std;
using namespace cv;

void detectaFaceDesenhaRetangulo(Mat& imagem, CascadeClassifier& cascade, double scale);
string cascadeName;

int main(int argc, const char** argv)
{
    Mat image = imread("rose-blue-flower-rose-blooms-67636.jpeg", CV_LOAD_IMAGE_UNCHANGED);
    resize(image,image,Size(70,70));
    VideoCapture capture;
    Mat frame;
    CascadeClassifier cascade;
    double scale;

    cascadeName       = "/home/sandro/Downloads/opencv-3.4.0/data/haarcascades/haarcascade_frontalface_alt.xml";
    scale = 1;

    if(!cascade.load(cascadeName))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

	try {
		if(!capture.open(0))
		    cout << "Capture from camera #0 didn't work" << endl;
    }
	catch(std::exception& e)
	{
		std::cout << " Excecao capturada open: " << e.what() << std::endl;
	}

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;

        for(;;)
        {
			try {
	            capture >> frame;
	            Mat newFrame=frame.clone();
                int cx = (newFrame.cols - 70) / 2;
			}
			catch (cv::Exception& e)
			{
				std::cout << " Excecao2 capturada frame: " << e.what() << std::endl;
				usleep(1000000);
				continue;
			}
			catch (std::exception& e)
			{
				std::cout << " Excecao3 capturada frame: " << e.what() << std::endl;
				usleep(1000000);
				continue;
			}

            if( frame.empty() )
                break;


		/*for (int m=0 ; m<400 ; m++) {
			//unsigned char * p = frame.ptr(m, m);
			p[0] = 255;
			p[1] = 0;
			p[2] = 0;
		}*/

            detectaFaceDesenhaRetangulo(frame, cascade, scale);

            char c = (char)waitKey(10);
            if(c == 27 || c == 'q' || c == 'Q')
                break;
        }
    }

    return 0;
}

void detectaFaceDesenhaRetangulo(Mat& imagem,CascadeClassifier& cascade, double scale)
{
    vector<Rect> faces;
    Scalar color = Scalar(255,0,0);
    Mat gray, smallimagem;

    cvtColor( imagem, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallimagem, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallimagem, smallimagem );

    cascade.detectMultiScale( smallimagem, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );

    for (size_t i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        Point center;

		printf("xy face = %d x %d\n", r.x, r.y);

        rectangle(imagem, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                   cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                   color, 3, 8, 0);

    }
    imshow("Jogo da Serpente", imagem);
}
