#include "stdafx.h"
#include "HogInvade.h"


HogInvade::HogInvade()
{
}


HogInvade::~HogInvade()
{
}

void HogInvade::HogImage(IplImage* img)
{
	//cv::Mat image = cv::imread("test.bmp");
	cv::Mat image(img, 0);
	if (image.empty())
	{
		std::cout << "read image failed" << std::endl;
	}


	// 1. ����HOG����  
	cv::HOGDescriptor hog; // ����Ĭ�ϲ���  
	//cv::HOGDescriptor hog(cv::Size(48, 96), cv::Size(16, 16), cv::Size(8, 8), cv::Size(8, 8), 9, 1, -1, cv::HOGDescriptor::L2Hys, 0.2, true, cv::HOGDescriptor::DEFAULT_NLEVELS);

	// 2. ����SVM������  
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());   // �����Ѿ�ѵ���õ����˼�������  

	// 3. �ڲ���ͼ���ϼ����������  
	std::vector<cv::Rect> regions;
	hog.detectMultiScale(image, regions, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 1);

	// ��ʾ  
	for (size_t i = 0; i < regions.size(); i++)
	{
		cv::rectangle(image, regions[i], cv::Scalar(0, 0, 255), 2);
	}

	*img = IplImage(image);
}