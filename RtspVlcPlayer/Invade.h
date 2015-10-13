#pragma once
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "opencv2/opencv.hpp"
//#include "opencv/cvaux.hpp"//��������ͷ�ļ�

class Invade
{
public:
	Invade();
	~Invade();
public:
	double TrianglArea(int x1, int y1, int x2, int y2, int x3, int y3);
	double SquArea(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void detect_face(IplImage* dst_img, int* count);

	// various tracking parameters (in seconds)���ٵĲ���(��λΪ��)
// 	const double MHI_DURATION = 0.5;
// 	const double MAX_TIME_DELTA = 0.5;
// 	const double MIN_TIME_DELTA = 0.05;
	const double MHI_DURATION = 1;//�˶����ٵ�������ʱ�� 
	const double MAX_TIME_DELTA = 1;////���ʱ������
	const double MIN_TIME_DELTA = 0.05;//��Сʱ������0.05s
	const int    N = 3;
	const int CONTOUR_MAX_AERA = 100;


	// ring image buffer Ȧ��ͼ�񻺳�
	IplImage **buf = 0;
	int last = 0;
	int nFrmnum = 1;
	// ��ʱͼ�����
	IplImage *mhi = 0;							 // MHI: �˶���ʷͼ��

	//CvFilter filter = CV_GAUSSIAN_5x5;      
	int filter = CV_GAUSSIAN_5x5;          //����5x5�ĸ�˹�˲�ģ��
	CvConnectedComp *cur_comp, min_comp; //���Ӳ���
	CvConnectedComp comp;
	CvMemStorage *storage;					//�ڴ�洢��
	CvPoint pt[4];									//��ά����ϵ�µĵ㣬����Ϊ���� ��ͨ����0��Ϊԭ�㣬��x�����y����
	//�����ĸ���Ȥ�������
	IplImage* dst_img1 = 0;                    //�������Ȥ����ͼ��ָ��
	//CvMemStorage *stor1;
	//CvSeq *cont1;
	IplImage *sub_buy = 0;
	//IplImage* sub_buy=0;
	//  ������
	//  img �C ������Ƶ֡
	//  dst �C �����
public:
	void update_mhi(IplImage* img, IplImage* dst, int diff_threshold);
	//void update_mhiEx(IplImage* img, IplImage* dst, int diff_threshold);
};

