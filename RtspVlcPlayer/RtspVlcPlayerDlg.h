
// RtspVlcPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "CvvImage.h"
#include "Invade.h"
#include "HogInvade.h"
#ifndef INT64_C
#define INT64_C(c) (c##LL)
#define UINT64_C(c) (c##LL)
#endif

#ifdef __cplusplus
extern "C"{
#endif

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/pixfmt.h"
#include "libavfilter/avfilter.h"


#ifdef __cplusplus
}

#endif
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 

#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")

#define WM_INVADE_HOG_MESSAGE (WM_USER+100)  
// CRtspVlcPlayerDlg �Ի���
class CRtspVlcPlayerDlg : public CDialogEx
{
// ����
public:
	CRtspVlcPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RTSPVLCPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPllay();
public:
	void DrawPicToHDC(IplImage *img, UINT ID);

	char * filename;
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();
	Invade mInvade;
	HogInvade mHogInvade;
	bool canSendData;
	IplImage  sendImage;

public:
	afx_msg LRESULT OnInvadeHogMessage(WPARAM wParam, LPARAM lParam);//��һ����������ӵ�  
};
