
// IMP_KSH_200721Doc.cpp: CIMPKSH200721Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IMP_KSH_200721.h"
#endif

#include "IMP_KSH_200721Doc.h"
#include "CDownSampleDlg.h"
#include "CUpSampleDig.h"
#include "CQuantizationDlg.h" // 대화상자 사용을 위한 헤더 선언
#include "math.h" // 수학 함수 사용을 위한 헤더 선언
#include "CStressTransformDlg.h"
#include "CConstantDlg.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIMPKSH200721Doc

IMPLEMENT_DYNCREATE(CIMPKSH200721Doc, CDocument)

BEGIN_MESSAGE_MAP(CIMPKSH200721Doc, CDocument)
END_MESSAGE_MAP()


// CIMPKSH200721Doc 생성/소멸

CIMPKSH200721Doc::CIMPKSH200721Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CIMPKSH200721Doc::~CIMPKSH200721Doc()
{
}

BOOL CIMPKSH200721Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CIMPKSH200721Doc serialization

void CIMPKSH200721Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CIMPKSH200721Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CIMPKSH200721Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CIMPKSH200721Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CIMPKSH200721Doc 진단

#ifdef _DEBUG
void CIMPKSH200721Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIMPKSH200721Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIMPKSH200721Doc 명령


BOOL CIMPKSH200721Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	CFile file;
	file.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	if (file.GetLength() == 256 * 256)
		m_height = m_width = 256;
	else if (file.GetLength() == 512 * 512)
		m_height = m_width = 512;
	else if (file.GetLength() == 640 * 480)
	{
		m_width = 640;
		m_height = 480;
	}
	m_size = m_height * m_width;
	m_InputImage = new unsigned char[m_size];
	for (int i = 0; i < m_size; i++)
		m_InputImage[i] = 255;
	file.Read(m_InputImage, m_size);
	file.Close();
	return TRUE;
}


BOOL CIMPKSH200721Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFile File; // 파일 객체 선언
	CFileDialog SaveDlg(FALSE, L"raw", NULL, OFN_HIDEREADONLY);
	// raw 파일을 다른 이름으로 저장하기를 위한 대화상자 객체 선언
	if (SaveDlg.DoModal() == IDOK) {
		// DoModal 멤버 함수에서 저장하기 수행
		File.Open(SaveDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite); // 파일 열기
		File.Write(m_InputImage, m_size); // 파일 쓰기
		File.Close(); // 파일 닫기
	}
	return TRUE;
}


void CIMPKSH200721Doc::OnDownSampling()
{
	int i, j;
	CDownSampleDlg dlg;
	if (dlg.DoModal() == IDOK) // 대화상자의 활성화 여부
	{
		m_Re_height = m_height / dlg.m_DownSampleRate;
		// 축소 영상의 세로 길이를 계산
		m_Re_width = m_width / dlg.m_DownSampleRate;
		// 축소 영상의 가로 길이를 계산
		m_Re_size = m_Re_height * m_Re_width;
		// 축소 영상의 크기를 계산
		m_OutputImage = new unsigned char[m_Re_size];
		// 축소 영상을 위한 메모리 할당
		for (i = 0; i < m_Re_height; i++) {
			for (j = 0; j < m_Re_width; j++) {
				m_OutputImage[i * m_Re_width + j]
					= m_InputImage[(i * dlg.m_DownSampleRate * m_width) + dlg.m_DownSampleRate * j];
				// 축소 영상을 생성
			}
		}
	}
}


void CIMPKSH200721Doc::OnUpSampling()
{
	int x, x2, y, y2;
	double rate1, rate2;
	CUpSampleDig dlg;
	if (dlg.DoModal() == IDOK) { // DoModal 대화상자의 활성화 여부
		m_Re_height = m_height * dlg.m_UpSampleRate;
		// 확대 영상의 세로 길이 계산
		m_Re_width = m_width * dlg.m_UpSampleRate;
		// 확대 영상의 가로 길이 계산
		m_Re_size = m_Re_height * m_Re_width;
		// 확대 영상의 크기 계산
		m_OutputImage = new unsigned char[m_Re_size];
		// 확대 영상을 위한 메모리 할당
		for (x = 0; x < m_Re_size; x++)
			m_OutputImage[x] = 0; // 초기화
		for (y = 0; y < m_height; y++) {
			for (x = 0; x < m_width; x++) {
				m_OutputImage[y * dlg.m_UpSampleRate * m_Re_width + dlg.m_UpSampleRate * x] = m_InputImage[y * m_width + x];
			} // 재배치하여 영상 확대
		}

		for (y = 0; y < m_Re_height - dlg.m_UpSampleRate; y+=dlg.m_UpSampleRate)
		{
			for (x = 0; x < m_Re_width - dlg.m_UpSampleRate; x+=dlg.m_UpSampleRate)
			{
				for (x2 = 1; x2 < dlg.m_UpSampleRate; x2++)
				{
					m_OutputImage[y * m_Re_width + x + x2]
						= m_OutputImage[y * m_Re_width + x] * (dlg.m_UpSampleRate - x2) / dlg.m_UpSampleRate
						+ m_OutputImage[y * m_Re_width + x + dlg.m_UpSampleRate] * x2 / dlg.m_UpSampleRate;
					m_OutputImage[(y+dlg.m_UpSampleRate) * m_Re_width + x + x2]
						= m_OutputImage[(y + dlg.m_UpSampleRate) * m_Re_width + x] * (dlg.m_UpSampleRate - x2) / dlg.m_UpSampleRate
						+ m_OutputImage[(y + dlg.m_UpSampleRate) * m_Re_width + x + dlg.m_UpSampleRate] * x2 / dlg.m_UpSampleRate;
				}
				for (y2 = 1; y2 < dlg.m_UpSampleRate; y2++)
				{
					rate1 = (double)(dlg.m_UpSampleRate - y2) / dlg.m_UpSampleRate;
					rate2 = (double)y2 / dlg.m_UpSampleRate;
					for (x2 = 0; x2 <= dlg.m_UpSampleRate; x2++)
					{
						m_OutputImage[(y+y2) * m_Re_width + x + x2] = (double)m_OutputImage[y * m_Re_width + x + x2] * rate1 + (double)m_OutputImage[(y + dlg.m_UpSampleRate) * m_Re_width + x + x2] * rate2;
					}
				}
			}
		}
	}
}

void CIMPKSH200721Doc::OnQuantization()
{
	CQuantizationDlg dlg;
	if (dlg.DoModal() == IDOK)
		// 양자화 비트 수를 결정하는 대화상자의 활성화 여부
	{
		int i, j, value, LEVEL;
		double HIGH, * TEMP;
		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_Re_height * m_Re_width;
		m_OutputImage = new unsigned char[m_Re_size];
		// 양자화 처리된 영상을 출력하기 위한 메모리 할당
		TEMP = new double[m_size];
		// 입력 영상 크기(m_size)와 동일한 메모리 할당
		LEVEL = 256; // 입력 영상의 양자화 단계(28=256)
		HIGH = 256.;
		value = (int)pow(2, dlg.m_QuantBit);
		int temp = LEVEL / value;
		// 양자화 단계 결정(예 : 24=16)
		for (i = 0; i < m_size; i++) {
			//for (j = 0; j < value; j++) {
			//	if (m_InputImage[i] >= (LEVEL / value) * j && m_InputImage[i] < (LEVEL / value) * (j + 1)) {
			//		TEMP[i] = (double)(HIGH / value) * j; // 양자화 수행
			//		break;
			//	}
			//}
			TEMP[i] = (int)(((double)m_InputImage[i]+0.5) / temp) * temp;
		}
		for (i = 0; i < m_size; i++) 
		{
			m_OutputImage[i] = (unsigned char)TEMP[i];
			// 결과 영상 생성
		}
	}
}

void CIMPKSH200721Doc::OnSumConstant()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			double temp = m_InputImage[i] + dlg.m_Constant;
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}

void CIMPKSH200721Doc::OnSubConstant()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			double temp = m_InputImage[i] - dlg.m_Constant;
			if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}

void CIMPKSH200721Doc::OnMulConstant()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			double temp = m_InputImage[i] * dlg.m_Constant;
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}

void CIMPKSH200721Doc::OnDivConstant()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			double temp = m_InputImage[i] / dlg.m_Constant;
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}

void CIMPKSH200721Doc::OnAndOperate()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] & (unsigned char)dlg.m_Constant;
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}
void CIMPKSH200721Doc::OnOrOperate()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] | (unsigned char)dlg.m_Constant;
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}
void CIMPKSH200721Doc::OnXorOperate()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] ^ (unsigned char)dlg.m_Constant;
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}

void CIMPKSH200721Doc::OnNegaTransform()
{
	m_OutputImage = new unsigned char[m_size];

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_size;

	for (int i = 0; i < m_Re_size; i++)
	{
		m_OutputImage[i] = 255 - m_InputImage[i];
	}
}

void CIMPKSH200721Doc::OnGammaCorrection()
{
	CConstantDlg dlg;
	m_OutputImage = new unsigned char[m_size];

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
		{
			double temp = pow((double)m_InputImage[i], 1/dlg.m_Constant);
			if (temp > 255)
			{
				m_OutputImage[i] = 255;
				continue;
			}
			else if (temp < 0)
			{
				m_OutputImage[i] = 0;
				continue;
			}
			m_OutputImage[i] = (unsigned char)temp;
		}
	}
}

void CIMPKSH200721Doc::OnBinarizaion()
{
	CConstantDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
			m_OutputImage[i] = (m_InputImage[i] >= dlg.m_Constant) ? 255 : 0;
	}
}

void CIMPKSH200721Doc::OnStressTransform()
{
	CStressTransformDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_OutputImage = new unsigned char[m_size];

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_size;

		for (int i = 0; i < m_Re_size; i++)
			m_OutputImage[i] = (m_InputImage[i] >= dlg.m_StartPoint && m_InputImage[i] < dlg.m_EndPoint) ? 255 : m_InputImage[i];
	}
}

void CIMPKSH200721Doc::OnHistoStretch()
{
	unsigned char LOW, HIGH, MAX, MIN;
	int i, temp;
	
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_size;

	m_OutputImage = new unsigned char[m_Re_size];

	MAX = LOW = 0;
	MIN = HIGH = 255;

	for (i = 0; i < m_Re_size; i++)
	{
		if (m_InputImage[i] < MIN)
			MIN = m_InputImage[i];
		if (m_InputImage[i] > MAX)
			MAX = m_InputImage[i];
	}

	temp = MAX - MIN;

	for (i = 0; i < m_Re_size; i++)
		m_OutputImage[i] = (unsigned char)(((double)m_InputImage[i] - MIN) * HIGH / temp);
}