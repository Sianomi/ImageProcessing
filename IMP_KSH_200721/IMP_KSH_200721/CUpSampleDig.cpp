// CUpSampleDig.cpp: 구현 파일
//

#include "pch.h"
#include "IMP_KSH_200721.h"
#include "CUpSampleDig.h"
#include "afxdialogex.h"


// CUpSampleDig 대화 상자

IMPLEMENT_DYNAMIC(CUpSampleDig, CDialog)

CUpSampleDig::CUpSampleDig(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
	, m_UpSampleRate(0)
{

}

CUpSampleDig::~CUpSampleDig()
{
}

void CUpSampleDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_UpSampleRate);
}


BEGIN_MESSAGE_MAP(CUpSampleDig, CDialog)
END_MESSAGE_MAP()


// CUpSampleDig 메시지 처리기
