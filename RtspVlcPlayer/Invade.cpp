#include "stdafx.h"
#include "Invade.h"


Invade::Invade()
{
}


Invade::~Invade()
{
}

// ������ 
// img - ������Ƶ֡ // dst - ����� 
void Invade::update_mhi(IplImage* img, IplImage* dst, int diff_threshold)
{
	double timestamp = clock() / 100.; // get current time in seconds ʱ��� 
	CvSize size = cvSize(img->width, img->height); // get current frame size���õ���ǰ֡�ĳߴ� 
	int i, idx1, idx2;
	IplImage* silh;
	IplImage* pyr = cvCreateImage(cvSize((size.width & -2) / 2, (size.height & -2) / 2), 8, 1);
	CvMemStorage *stor;
	CvSeq *cont;

	/*�Ƚ������ݵĳ�ʼ��*/
	if (!mhi || mhi->width != size.width || mhi->height != size.height)
	{
		if (buf == 0) //����û�г�ʼ��������ڴ���� 
		{
			buf = (IplImage**)malloc(N*sizeof(buf[0]));
			memset(buf, 0, N*sizeof(buf[0]));
		}

		for (i = 0; i < N; i++)
		{
			cvReleaseImage(&buf[i]);
			buf[i] = cvCreateImage(size, IPL_DEPTH_8U, 1);
			cvZero(buf[i]);// clear Buffer Frame at the beginning 
		}
		cvReleaseImage(&mhi);
		mhi = cvCreateImage(size, IPL_DEPTH_32F, 1);
		cvZero(mhi); // clear MHI at the beginning 
	} // end of if(mhi) 

	/*����ǰҪ�����֡ת��Ϊ�Ҷȷŵ�buffer�����һ֡��*/
	cvCvtColor(img, buf[last], CV_BGR2GRAY); // convert frame to grayscale 

	/*�趨֡�����*/
	idx1 = last;
	idx2 = (last + 1) % N; // index of (last - (N-1))th frame 
	last = idx2;

	// ��֡�� 
	silh = buf[idx2];//��ֵ��ָ��idx2 
	cvAbsDiff(buf[idx1], buf[idx2], silh); // get difference between frames 

	// �Բ�ͼ������ֵ�� 
	cvThreshold(silh, silh, 50, 255, CV_THRESH_BINARY); //threshold it,��ֵ�� 

	//ȥ����ʱ��Ӱ���Ը����˶���ʷͼ��
	cvUpdateMotionHistory(silh, mhi, timestamp, MHI_DURATION); // update MHI 

	cvConvert(mhi, dst);//��mhiת��Ϊdst,dst=mhi 

	// ��ֵ�˲�������С������ 
	cvSmooth(dst, dst, CV_MEDIAN, 3, 0, 0, 0);

	cvPyrDown(dst, pyr, CV_GAUSSIAN_5x5);// ���²�����ȥ��������ͼ����ԭͼ����ķ�֮һ 
	cvDilate(pyr, pyr, 0, 1); // �����Ͳ���������Ŀ��Ĳ������ն� 
	cvPyrUp(pyr, dst, CV_GAUSSIAN_5x5);// ���ϲ������ָ�ͼ��ͼ����ԭͼ����ı� 

	// ����ĳ���������ҵ����� 
	// Create dynamic structure and sequence. 
	stor = cvCreateMemStorage(0);
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor);

	// �ҵ��������� 
	cvFindContours(dst, stor, &cont, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// ֱ��ʹ��CONTOUR�еľ����������� 
	for (; cont; cont = cont->h_next)
	{
		CvRect r = ((CvContour*)cont)->rect;
		if (r.height * r.width > CONTOUR_MAX_AERA) // ���С�ķ��������� 
		{
			cvRectangle(img, cvPoint(r.x, r.y),
				cvPoint(r.x + r.width, r.y + r.height),
				CV_RGB(255, 0, 0), 1, CV_AA, 0);
		}
	}
	// free memory 
	cvReleaseMemStorage(&stor);
	cvReleaseImage(&pyr);
}

/*
void Invade::update_mhiEx(IplImage* img, IplImage* dst, int diff_threshold)
{
	int i, idx1, idx2;
	IplImage* silh;
	CvSeq *cont;
	CvCapture*pCapture = NULL;
	CvMemStorage *stor;
	//���ļ�USB����ͷ������Ƶ֡:
//	pCapture = cvCaptureFromAVI();
	//����ǰҪ�����֡ת��Ϊ�Ҷȷŵ�buffer�����һ֡��
	cvCvtColor(img, buf[last], CV_BGR2GRAY); // convert frame to grayscale 
	//�趨֡�����
	idx1 = last;
	idx2 = (last + 1) % N; // index of (last - (N-1))th frame 
	last = idx2;
	//���ڸ�˹ͳ�ƹ��Ʊ���ģ��
	CvGaussBGModel*bg_model = NULL;//�����˹����ģ�� : 
	bg_model = (CvGaussBGModel*)cvCreateGaussianBGModel(buf[idx1], 0);//ʹ�õ�һ֡buf[idx1]������˹����ģ��:
	cvUpdateBGStatModel(buf[idx1], (CvBGStatModel*)bg_model);//ʹ�õ�ǰ֡���¸�˹����ģ��:
	//����˹����ģ�͵ı���ͼ���Ƶ����ȶ���ı���ͼ��pBkImg�У�
	cvCopy(bg_model->background, dst, 0);
	//ʹ��˲ʱ��ַ���ò������˶�����ĵ�ǰ֡�������±�������ǰ֡buf[idx1]��ǰһ֡buf[idx2]�����, ������ڲ��ͼ��diff�У�����ֵ��:
	silh = buf[idx2];
	cvAbsDiff(buf[idx1], buf[idx2], silh);
	//�Բ�ͼ������ֵ��
	cvThreshold(silh, silh, 30, 255, CV_THRESH_BINARY);
	cvSmooth(dst, dst, CV_MEDIAN, 3, 0, 0, 0);
	//����ĳ���������ҵ�����
	//Createdynamicstructureandsequence.

	stor=cvCreateMemStorage(0);
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor);
	//�ҵ���������
	cvFindContours(dst, stor, &cont, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	//ֱ��ʹ��CONTOUR�еľ�����������
	for (; cont; cont = cont->h_next)
	{
		CvRect r = ((CvContour*)cont)->rect;
		if (r.height*r.width > CONTOUR_MAX_AERA)//���С�ķ�������
		{
			cvRectangle(img, cvPoint(r.x, r.y), cvPoint(r.x + r.width, r.y + r.height),
				CV_RGB(255, 255, 0), 1, CV_AA, 0);
		}
	}
}
*/