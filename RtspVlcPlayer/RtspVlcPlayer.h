
// RtspVlcPlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRtspVlcPlayerApp: 
// �йش����ʵ�֣������ RtspVlcPlayer.cpp
//

class CRtspVlcPlayerApp : public CWinApp
{
public:
	CRtspVlcPlayerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRtspVlcPlayerApp theApp;