
// RtspVlcPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RtspVlcPlayer.h"
#include "RtspVlcPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRtspVlcPlayerDlg �Ի���



CRtspVlcPlayerDlg::CRtspVlcPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRtspVlcPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRtspVlcPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRtspVlcPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PlLAY, &CRtspVlcPlayerDlg::OnBnClickedButtonPllay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CRtspVlcPlayerDlg::OnBnClickedButtonStop)
	ON_MESSAGE(WM_INVADE_HOG_MESSAGE, OnInvadeHogMessage)//�Զ�����Ϣ
END_MESSAGE_MAP()


// CRtspVlcPlayerDlg ��Ϣ�������

BOOL CRtspVlcPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	canSendData = true;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRtspVlcPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRtspVlcPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRtspVlcPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT ThreadRtspPlay(LPVOID pParam)
{
		CRtspVlcPlayerDlg *mDlg = (CRtspVlcPlayerDlg *)pParam;
		//CWinThread* pThread=AfxBeginThread(ThreadRtspPlay, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		//pThread->ResumeThread();
		int videoStream;
		int frameFinished;
		int numBytes;
		uint8_t * buffer;
		AVPacket * packet;
		AVFrame * pFrameRGB;
		AVFrame * pFrame;
		AVCodec * pCodec;
		AVCodecContext * pCodecCtx;
		AVFormatContext * pFormatCtx;
		char* filename = "rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp";
		//char* filename = "rtsp://115.29.239.238/000000001201.sdp";
		//char* filename = "rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
		//char* filename = "D:\\motiondetect\\test.avi";
		av_register_all();//ע�����
		avformat_network_init();//֧��������

		pFormatCtx = avformat_alloc_context(); //��ʼ��AVFormatContext, ����һ���ڴ棬������Ƶ������Ϣ

		if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0)
		{
			AfxMessageBox(L"open  input  file  faile !");
			return -1;
		}
		TRACE(L"\n open  input  file !");

		if (avformat_find_stream_info(pFormatCtx, NULL) < 0)//��������Ϣ
		{
			TRACE("Couldn't find stream information.\n");
			return -1;
		}

		for (int i = 0; i < pFormatCtx->nb_streams; i++)//��ȡ��Ƶ��ID
		{
			if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				videoStream = i;
				break;
			}
		}
		if (videoStream == -1)
		{
			AfxMessageBox(L"could  not find video stream !");
			return -1;
		}
		TRACE(L"find video stream !");

		pCodecCtx = pFormatCtx->streams[videoStream]->codec;
		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);//���ҽ�����
		if (pCodec == NULL)
		{
			AfxMessageBox(L"could not find decoder");
			return -1;
		}
		TRACE(L"find decoder");

		if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)//�򿪽�����
		{
			AfxMessageBox(L"could not open decoder");
			return -1;
		}
		TRACE(L"open decoder");

		av_dump_format(pFormatCtx, 0, filename, 0);

		pFrame = av_frame_alloc();//avcodec_alloc_frame //�洢�����AVFrame
		pFrameRGB = av_frame_alloc(); //�洢ת����AVFrame��ΪʲôҪת�������Ľ��ͣ�
		packet = (AVPacket *)av_malloc(sizeof(AVPacket));
		if (pFrameRGB == NULL)
		{
			AfxMessageBox(L"open mem faile");
			return -1;
		}

		numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);//����AVFrame�����ڴ�

		buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

		avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);////���AVFrame
		
		IplImage* motion = 0;
		while (av_read_frame(pFormatCtx, packet) >= 0)
		{
			if (packet->stream_index == videoStream)
			{
				avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);

				if (frameFinished)
				{
					struct SwsContext * img_convert_ctx;

					img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
					if (img_convert_ctx == NULL)
					{
						AfxMessageBox(L"Cannot initialize the conversion context !");
						return -1;
					}

					static uchar *p = NULL;

					p = pFrame->data[1];
					pFrame->data[1] = pFrame->data[2];
					pFrame->data[2] = p;

					sws_scale(img_convert_ctx,
						pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
						pFrameRGB->data, pFrameRGB->linesize);

					if (img_convert_ctx)
					{
						sws_freeContext(img_convert_ctx);
						img_convert_ctx = NULL;
					}

					IplImage * Image;
					CvSize Size = cvSize(pCodecCtx->width, pCodecCtx->height);
					Image = cvCreateImage(Size, IPL_DEPTH_8U, 3);
					memcpy(Image->imageData, buffer, pCodecCtx->width*pCodecCtx->height * 24 / 8);
					Image->widthStep = pCodecCtx->width * 3;
					Image->origin = 0;
					//cvShowImage("video", Image);
					if (Image)//��ȡ�����ж�motion�Ƿ�Ϊ�� 
					{
						if (!motion)
						{
							motion = cvCreateImage(cvSize(Image->width, Image->height), 8, 1); //����motion֡����λ��һͨ�� 
							cvZero(motion); //�����motion 
							motion->origin = Image->origin; //�ڴ�洢��˳���ȡ����֡��ͬ 
						}
					}
					mDlg->mInvade.update_mhi(Image, motion, 60);//������ʷͼ�� 
					//mDlg->mHogInvade.HogImage(Image);
					mDlg->DrawPicToHDC(Image, IDC_PIC);
					//mDlg->UpdateData(FALSE);

					cvReleaseImage(&Image);
				}
			}
		}
		return 0;
}


void CRtspVlcPlayerDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img, 1);

	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}


void CRtspVlcPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CDialogEx::OnTimer(nIDEvent);
	if (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index == videoStream)
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);

			if (frameFinished)
			{
				struct SwsContext * img_convert_ctx;

				img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
				if (img_convert_ctx == NULL)
				{
					AfxMessageBox(L"Cannot initialize the conversion context !");
					return;
				}

				static uchar *p = NULL;

				p = pFrame->data[1];
				pFrame->data[1] = pFrame->data[2];
				pFrame->data[2] = p;

				sws_scale(img_convert_ctx,
					pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				if (img_convert_ctx)
				{
					sws_freeContext(img_convert_ctx);
					img_convert_ctx = NULL;
				}

				IplImage * Image;
				CvSize Size = cvSize(pCodecCtx->width, pCodecCtx->height);
				Image = cvCreateImage(Size, IPL_DEPTH_8U, 3);
				memcpy(Image->imageData, buffer, pCodecCtx->width*pCodecCtx->height * 24 / 8);
				Image->widthStep = pCodecCtx->width * 3;
				Image->origin = 0;
				//cvShowImage("video", Image);
				DrawPicToHDC(Image, IDC_PIC);
				UpdateData(FALSE);

				cvReleaseImage(&Image);
			}
		}
	}
}


void CRtspVlcPlayerDlg::OnBnClickedButtonStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
}

UINT ThreadRtspBaiscPlay(LPVOID pParam)
{
	AVFormatContext	*pFormatCtx;
	int i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec	*pCodec;
	//char filepath[] = "nwn.mp4";
	//char rtspUrl[] = "rtsp://192.168.11.19/live0.264";
	//char rtspUrl[] = "rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
	char rtspUrl[] = "rtsp://211.139.194.251:554/live/2/13E6330A31193128/5iLd2iNl5nQ2s8r8.sdp";
	av_register_all();//ע�����
	avformat_network_init();//֧��������
	pFormatCtx = avformat_alloc_context();//��ʼ��AVFormatContext
	if (avformat_open_input(&pFormatCtx,/*filepath*/rtspUrl, NULL, NULL) != 0){//���ļ�
		AfxMessageBox(L"�޷�������\n");
		return -1;
	}
	if (avformat_find_stream_info(pFormatCtx,NULL) < 0)//��������Ϣ
	{
		AfxMessageBox(L"���ܹ��ҵ�����Ϣ\n");
		return -1;
	}
	videoindex = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++) //��ȡ��Ƶ��ID
	if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		videoindex = i;
		break;
	}
	if (videoindex == -1)
	{
		TRACE("Didn't find a video stream.\n");
		return -1;
	}
	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);//���ҽ�����
	if (pCodec == NULL)
	{
		TRACE("Codec not found.\n");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)//�򿪽�����
	{
		TRACE("Could not open codec.\n");
		return -1;
	}
	AVFrame	*pFrame, *pFrameYUV;
	pFrame = av_frame_alloc();//�洢�����AVFrame
	pFrameYUV = av_frame_alloc();//�洢ת����AVFrame��ΪʲôҪת�������Ľ��ͣ�
	uint8_t *out_buffer;
	out_buffer = new uint8_t[avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height)];//����AVFrame�����ڴ�
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);//���AVFrame

	
	int ret, got_picture;
	static struct SwsContext *img_convert_ctx;
	int y_size = pCodecCtx->width * pCodecCtx->height;

	
	AVPacket *packet = (AVPacket *)malloc(sizeof(AVPacket));//�洢����ǰ���ݰ�AVPacket
	av_new_packet(packet, y_size);
	//���һ����Ϣ-----------------------------
	printf("�ļ���Ϣ-----------------------------------------\n");
	av_dump_format(pFormatCtx, 0, rtspUrl, 0);
	printf("-------------------------------------------------\n");
	//------------------------------
	while (av_read_frame(pFormatCtx, packet) >= 0)//ѭ����ȡѹ�����ݰ�AVPacket
	{
		if (packet->stream_index == videoindex)
		{
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);//���롣����ΪAVPacket�����ΪAVFrame
			if (ret < 0)
			{
				printf("�������\n");
				return -1;
			}
			if (got_picture)
			{
				//���ظ�ʽת����pFrameת��ΪpFrameYUV��
				img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
				sws_freeContext(img_convert_ctx);

			}
		}
		av_free_packet(packet);

		
		
	}


	delete[] out_buffer;
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}
/*
UINT ThreadOnePicDealPlay(LPVOID pParam)
{
	CRtspVlcPlayerDlg *mDlg = (CRtspVlcPlayerDlg *)pParam;

	IplImage * Image = mDlg->sendImage;

	//mDlg->mInvade.update_mhi(Image, motion, 60);//������ʷͼ�� 
	mDlg->mHogInvade.HogImage(Image);
	mDlg->DrawPicToHDC(Image, IDC_PIC);
	//mDlg->UpdateData(FALSE);
	mDlg->canSendData = true;
	//cvReleaseImage(&Image);

	return 0;
}
*/
UINT ThreadRtspNoDealPlay(LPVOID pParam)
{
	CRtspVlcPlayerDlg *mDlg = (CRtspVlcPlayerDlg *)pParam;
	//CWinThread* pThread=AfxBeginThread(ThreadRtspPlay, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	//pThread->ResumeThread();
	int videoStream;
	int frameFinished;
	int numBytes;
	uint8_t * buffer;
	AVPacket * packet;
	AVFrame * pFrameRGB;
	AVFrame * pFrame;
	AVCodec * pCodec;
	AVCodecContext * pCodecCtx;
	AVFormatContext * pFormatCtx;
	char* filename = "rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp";
	//char* filename = "rtsp://115.29.239.238/000000001201.sdp";
	//char* filename = "rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
	//char* filename = "D:\\motiondetect\\test.avi";
	av_register_all();//ע�����
	avformat_network_init();//֧��������

	pFormatCtx = avformat_alloc_context(); //��ʼ��AVFormatContext, ����һ���ڴ棬������Ƶ������Ϣ

	if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0)
	{
		AfxMessageBox(L"open  input  file  faile !");
		return -1;
	}
	TRACE(L"\n open  input  file !");

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)//��������Ϣ
	{
		TRACE("Couldn't find stream information.\n");
		return -1;
	}

	for (int i = 0; i < pFormatCtx->nb_streams; i++)//��ȡ��Ƶ��ID
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1)
	{
		AfxMessageBox(L"could  not find video stream !");
		return -1;
	}
	TRACE(L"find video stream !");

	pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);//���ҽ�����
	if (pCodec == NULL)
	{
		AfxMessageBox(L"could not find decoder");
		return -1;
	}
	TRACE(L"find decoder");

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)//�򿪽�����
	{
		AfxMessageBox(L"could not open decoder");
		return -1;
	}
	TRACE(L"open decoder");

	av_dump_format(pFormatCtx, 0, filename, 0);

	pFrame = av_frame_alloc();//avcodec_alloc_frame //�洢�����AVFrame
	pFrameRGB = av_frame_alloc(); //�洢ת����AVFrame��ΪʲôҪת�������Ľ��ͣ�
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	if (pFrameRGB == NULL)
	{
		AfxMessageBox(L"open mem faile");
		return -1;
	}

	numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);//����AVFrame�����ڴ�

	buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);////���AVFrame

	IplImage* motion = 0;
	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index == videoStream)
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);

			if (frameFinished)
			{
				struct SwsContext * img_convert_ctx;

				img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
				if (img_convert_ctx == NULL)
				{
					AfxMessageBox(L"Cannot initialize the conversion context !");
					return -1;
				}

				static uchar *p = NULL;

				p = pFrame->data[1];
				pFrame->data[1] = pFrame->data[2];
				pFrame->data[2] = p;

				sws_scale(img_convert_ctx,
					pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameRGB->data, pFrameRGB->linesize);

				if (img_convert_ctx)
				{
					sws_freeContext(img_convert_ctx);
					img_convert_ctx = NULL;
				}

				IplImage * Image;
				CvSize Size = cvSize(pCodecCtx->width, pCodecCtx->height);
				Image = cvCreateImage(Size, IPL_DEPTH_8U, 3);
				memcpy(Image->imageData, buffer, pCodecCtx->width*pCodecCtx->height * 24 / 8);
				Image->widthStep = pCodecCtx->width * 3;
				Image->origin = 0;
				//cvShowImage("video", Image);
				if (Image)//��ȡ�����ж�motion�Ƿ�Ϊ�� 
				{
					if (!motion)
					{
						motion = cvCreateImage(cvSize(Image->width, Image->height), 8, 1); //����motion֡����λ��һͨ�� 
						cvZero(motion); //�����motion 
						motion->origin = Image->origin; //�ڴ�洢��˳���ȡ����֡��ͬ 
					}
				}
				//mDlg->mInvade.update_mhi(Image, motion, 60);//������ʷͼ�� 
				//mDlg->mHogInvade.HogImage(Image);
				//if (mDlg->canSendData)
				//{
					//mDlg->canSendData = false;
					//mDlg->sendImage = *Image;
					//mDlg->SendMessage(WM_INVADE_HOG_MESSAGE, 0, 0);
				//}
				
				mDlg->DrawPicToHDC(Image, IDC_RTSP);
				//mDlg->UpdateData(FALSE);

				cvReleaseImage(&Image);
			}
		}
	}
	return 0;
}



void CRtspVlcPlayerDlg::OnBnClickedButtonPllay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CWinThread* pThread0 = AfxBeginThread(ThreadRtspPlay, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	//pThread0->ResumeThread();

	CWinThread* pThread1 = AfxBeginThread(ThreadRtspNoDealPlay, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	pThread1->ResumeThread();

}

LRESULT CRtspVlcPlayerDlg::OnInvadeHogMessage(WPARAM wParam, LPARAM lParam)
{
	//TODO  
	IplImage  Image = sendImage;

	//mDlg->mInvade.update_mhi(Image, motion, 60);//������ʷͼ�� 
	mHogInvade.HogImage(&Image);
	DrawPicToHDC(&Image, IDC_PIC);
	//mDlg->UpdateData(FALSE);
	canSendData = true;
	//cvReleaseImage(&Image);
	return 0;
}