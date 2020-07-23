
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
	else if (file.GetLength() == 128*128)
		m_height = m_width = 128;
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
				m_OutputImage[i * m_Re_width + j] = m_InputImage[(i * dlg.m_DownSampleRate * m_width) + dlg.m_DownSampleRate * j];
				// 축소 영상을 생성
			}
		}
	}
}


void CIMPKSH200721Doc::OnUpSampling()
{
	int x, y, x1, x2, y1, y2;
	double rate1, rate2, rate3, rate4;
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
		memset(m_OutputImage, 0, sizeof(m_OutputImage));
		for (y = 0; y < m_height; y++) {
			for (x = 0; x < m_width; x++) {
				m_OutputImage[y * dlg.m_UpSampleRate * m_Re_width + dlg.m_UpSampleRate * x] = m_InputImage[y * m_width + x];
			} // 재배치하여 영상 확대
		}

		for (y = 0; y <= m_Re_height - dlg.m_UpSampleRate; y++)
		{
			for (x = 0; x <= m_Re_width - dlg.m_UpSampleRate; x++)
			{
				x1 = (x / dlg.m_UpSampleRate) * dlg.m_UpSampleRate;
				x2 = x1 + dlg.m_UpSampleRate;
				y1 = (y / dlg.m_UpSampleRate) * dlg.m_UpSampleRate;
				y2 = y1 + dlg.m_UpSampleRate;
				rate2 = (double)(x-x1) / dlg.m_UpSampleRate;
				rate1 = 1 - rate2;
				rate4 = (double)(y-y1) / dlg.m_UpSampleRate;
				rate3 = 1 - rate4;
				m_OutputImage[y * m_Re_width + x] = (unsigned char)(rate3 * (rate1 * m_OutputImage[y1 * m_Re_width + x1] + rate2 * m_OutputImage[y1 * m_Re_width + x2])
					+ rate4 * (rate1 * m_OutputImage[y2 * m_Re_width + x1] + rate2 * m_OutputImage[y2 * m_Re_width + x2]));
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
				m_OutputImage[i] = 255;
			else
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
				m_OutputImage[i] = 0;
			else
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
				m_OutputImage[i] = 255;
			else if (temp < 0)
				m_OutputImage[i] = 0;
			else
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
				m_OutputImage[i] = 255;
			else if (temp < 0)			
				m_OutputImage[i] = 0;		
			else
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
				m_OutputImage[i] = 255;
			else if (temp < 0)
				m_OutputImage[i] = 0;
			else
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
				m_OutputImage[i] = 255;
			else if (temp < 0)
				m_OutputImage[i] = 0;
			else
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
				m_OutputImage[i] = 255;
			else if (temp < 0)
				m_OutputImage[i] = 0;
			else
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
		m_OutputImage[i] = 255 - m_InputImage[i];
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
				m_OutputImage[i] = 255;
			else if (temp < 0)
				m_OutputImage[i] = 0;
			else
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

void CIMPKSH200721Doc::OnEndInSearch()
{
	unsigned char MIN{ 255 }, MAX{ 0 };
	int i;
	
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_size;

	for (i = 0; i < m_size; i++)
	{
		if (m_InputImage[i] < MIN)
			MIN = m_InputImage[i];
		if (m_InputImage[i] > MAX)
			MAX = m_InputImage[i];
	}

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_size; i++)
	{
		if (m_InputImage[i] >= MAX)
			m_OutputImage[i] = 255;
		else if (m_InputImage[i] <= MIN)
			m_OutputImage[i] = 0;
		else
			m_OutputImage[i] = (m_InputImage[i] - MIN) * 255 / (MAX - MIN);
	}
}

void CIMPKSH200721Doc::OnHistogram()
{
	int x, y;
	double MAX{ 0 }, MIN{ INT_MAX };
	
	m_Re_width = 256;
	m_Re_height = 256;
	m_Re_size = 256 * 256;

	memset(m_HIST, 0, sizeof(m_HIST));

	for (x = 0; x < m_Re_size; x++)
		m_HIST[(int)m_InputImage[x]]++;
	for (x = 0; x < 256; x++)
	{
		if (m_HIST[x] < MIN)
			MIN = m_HIST[x];
		if (m_HIST[x] > MAX)
			MAX = m_HIST[x];
	}
	for (x = 0; x < 256; x++)
		m_Scale_HIST[x] = (m_HIST[x] - MIN) * 255 / (MAX - MIN);
	
	m_OutputImage = new unsigned char[m_Re_size + 256 * 20];
	memset(m_OutputImage, 255, sizeof(m_OutputImage) + 256 * 20);
	
	for (x = 0; x < 256; x++)
		for (y = 0; y < m_Scale_HIST[x]; y++)
			m_OutputImage[(255-y) * m_Re_width + x] = 0;
	for(y=m_Re_height+5;y< m_Re_height+20;y++)
		for (x = 0; x < 256; x++)
			m_OutputImage[y * m_Re_width + x] = x;

	m_Re_height += 20;
	m_Re_size = m_Re_width * m_Re_height;	
}

void CIMPKSH200721Doc::OnHistoEqual()
{
	int x, y;

	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size=m_size;

	memset(m_HIST, 0, sizeof(m_HIST));

	for (x = 0; x < m_Re_size; x++)
		m_HIST[(int)m_InputImage[x]]++;

	m_Sum_Of_HIST[0] = m_HIST[0];

	for (x = 1; x < 256; x++)
		m_Sum_Of_HIST[x] = m_Sum_Of_HIST[x - 1] + m_HIST[x];

	m_OutputImage = new unsigned char[m_Re_size];

	for (x = 0; x < m_Re_size; x++)
		m_OutputImage[x] = (unsigned char)(m_Sum_Of_HIST[(int)m_InputImage[x]] * 255 / m_Re_size);
}

void CIMPKSH200721Doc::OnHistoSpec()
{
	int i, value, Dvalue, top, bottom, DADD;
	unsigned char* m_DTEMP, m_Sum_Of_ScHIST[256], m_TABLE[256];
	unsigned char LOW, HIGH, * m_Org_Temp;
	double m_DHIST[256], m_Sum_Of_DHIST[256], DIF;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];
	m_Org_Temp = new unsigned char[m_size];
	m_DTEMP = new unsigned char[m_size];

	CFile File;
	CFileDialog OpenDlg(TRUE);
	// 원하는 히스토그램이 있는 영상을 입력받음
	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetPathName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_size) {
			File.Read(m_DTEMP, m_size);
			File.Close();
		}
		else {
			AfxMessageBox(L"Image size not matched");
			// 같은 크기의 영상을 대상으로 함
			return;
		}
	}
	// 초기화
	memset(m_HIST, 0, sizeof(m_HIST));
	memset(m_DHIST, 0, sizeof(m_DHIST));
	memset(m_TABLE, 0, sizeof(m_TABLE));
	// 빈도 수 조사
	for (i = 0; i < m_size; i++) {
		m_HIST[(int)m_InputImage[i]]++;
		m_DHIST[(int)m_DTEMP[i]]++;
	}
	// 누적 히스토그램 조사
	m_Sum_Of_HIST[0] = m_HIST[0];
	m_Sum_Of_DHIST[0] = m_DHIST[0];
	for (i = 1; i < 256; i++) {
		m_Sum_Of_HIST[i] = m_Sum_Of_HIST[i-1]+ m_HIST[i];
		m_Sum_Of_DHIST[i] = m_Sum_Of_DHIST[i-1] + m_DHIST[i];
	}
	// 원본 영상의 평활화
	for (i = 0; i < m_size; i++)
		m_Org_Temp[i] = (unsigned char)(m_Sum_Of_HIST[(int)m_InputImage[i]] * 255 / m_size);
	// 누적 히스토그램에서 최소값과 최대값 지정
	DIF = m_Sum_Of_DHIST[255] - m_Sum_Of_DHIST[0];
	// 원하는 영상을 평활화
	for (i = 0; i < 256; i++)
		m_Sum_Of_ScHIST[i] = (unsigned char)((m_Sum_Of_DHIST[i] - m_Sum_Of_DHIST[0]) * 255 / DIF);
	// 룩업테이블을 이용한 명세화
	for(top = 255, bottom = top - 1 ; bottom > -1 ; bottom--)
	{
		for (i = m_Sum_Of_ScHIST[bottom]; i <= m_Sum_Of_ScHIST[top]; i++)
			m_TABLE[i] = top;
		top = bottom;
	}
	for (i = 0; i < m_size; i++) 
		m_OutputImage[i] = m_TABLE[(int)m_Org_Temp[i]];
}

void CIMPKSH200721Doc::OnEmbossing()
{
	int i, j;
	double EmboMask[3][3] = { {-1., 0., 0.}, {0., 0., 0.}, {0., 0., 1.} };
	// 마스크 선택
	// double EmboMask[3][3] = {{0., 0., 0.}, {0., 1., 0.}, {0., 0., 0.}};
	// double EmboMask[3][3] = {{1., 1., 1.}, {1., -8.,1.}, {1., 1., 1.}};
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, EmboMask);
	// OnMaskProcess 함수를 호출하여 회선 처리를 한다.
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	// 정규화 함수를 사용할 때
	// 회선 처리 결과나 정규화 처리 결과는 2차원 배열 값이 되므로
	// 2차원 배열을 1차원 배열로 바꾸어 출력하도록 한다.
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}

double** CIMPKSH200721Doc::OnMaskProcess(unsigned char* Target, double Mask[3][3])
{
	int i, j, n, m;
	double** tempInputImage, ** tempOutputImage, S = 0.0;
	tempInputImage = Image2DMem(m_height + 2, m_width + 2);
	// 입력 값을 위한 메모리 할당
	tempOutputImage = Image2DMem(m_height, m_width);
	// 출력 값을 위한 메모리 할당
	// 1차원 입력 영상의 값을 2차원 배열에 할당한다.
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInputImage[i + 1][j + 1]
				= (double)Target[i * m_width + j];
		}
	}
	// 회선연산
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					S += Mask[n][m] * tempInputImage[i + n][j + m];
				}
			} // 회선 마스크의 크기 만큼 이동하면서 값을 누적
			tempOutputImage[i][j] = S; // 누적된 값을 출력 메모리에 저장
			S = 0.0; // 다음 블록으로 이동하면 누적 값을 초기화
		}
	}
	return tempOutputImage; // 결과 값 반환
}
double** CIMPKSH200721Doc::OnScale(double** Target, int height, int width)
{ // 정규화를 위한 함수
	int i, j;
	double min, max;
	min = max = Target[0][0];
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (Target[i][j] <= min)
				min = Target[i][j];
		}
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (Target[i][j] >= max)
				max = Target[i][j];
		}
	}
	max = max - min;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			Target[i][j] = (Target[i][j] - min) * (255. / max);
		}
	}
	return Target;
}
double** CIMPKSH200721Doc::Image2DMem(int height, int width)
{
	double** temp;
	int i, j;
	temp = new double* [height];
	for (i = 0; i < height; i++) {
		temp[i] = new double[width];
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			temp[i][j] = 0.0;
		}
	} // 할당된 2차원 메모리를 초기화
	return temp;
}

void CIMPKSH200721Doc::OnBlurr()
{
	int i, j;
	double BlurrMask[3][3] = { {1. / 9., 1. / 9., 1. / 9.},
	{1. / 9., 1. / 9., 1. / 9.}, {1. / 9., 1. / 9., 1. / 9.} };
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, BlurrMask);
	// 블러링 처리
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	// 정규화
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}

void CIMPKSH200721Doc::OnGaussianFilter()
{
	int i, j;
	double GaussianMask[3][3] = { {1. / 16., 1. / 8., 1. / 16.},
	{1. / 8., 1. / 4., 1. / 8.}, {1. / 16., 1. / 8., 1. / 16.} };
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, GaussianMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}

void CIMPKSH200721Doc::OnSharpening()
{
	int i, j;
	//double SharpeningMask[3][3] = {{-1., -1., -1.},
	//	{-1., 9., -1.}, { -1., -1., -1. }};
	double SharpeningMask[3][3] = { {0., -1., 0.}, {-1., 5.,
	-1.}, {0., -1., 0.} };
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, SharpeningMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}

void CIMPKSH200721Doc::OnHpfSharp()
{
	int i, j;
	double HpfSharpMask[3][3] = { {-1. / 9., -1. / 9., -1. / 9.},
	{-1. / 9., 8. / 9., -1. / 9.}, {-1. / 9., -1. / 9., -1. / 9.} };
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, HpfSharpMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}

void CIMPKSH200721Doc::OnLpfSharp()
{
	CConstantDlg dlg; // 상수를 입력받으려고 대화상자 선언
	int i, j, alpha;
	double LpfSharpMask[3][3] = { {1. / 9., 1. / 9., 1. / 9.},
	{1. / 9., 1. / 9., 1. / 9.}, {1. / 9., 1. / 9., 1. / 9.} };
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	if (dlg.DoModal() == IDOK) {
		alpha = (int)dlg.m_Constant;
		// 대화상자를 이용하여 상수를 입력받는다.
	}
	m_tempImage = OnMaskProcess(m_InputImage, LpfSharpMask);
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (alpha * m_InputImage
				[i * m_width + j]) - (unsigned char)m_tempImage[i][j];
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}

void CIMPKSH200721Doc::OnDiffOperatorHor()
{
	int i, j;
	double DiffHorMask[3][3]
		= { {0., -1., 0.}, {0., 1., 0.}, {0., 0., 0.} };
	// 수평 필터 선택
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, DiffHorMask);
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}

void CIMPKSH200721Doc::OnHomogenOperator()
{
	int i, j, n, m;
	double max, ** tempOutputImage;
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = Image2DMem(m_height + 2, m_width + 2);
	tempOutputImage = Image2DMem(m_Re_height, m_Re_width);
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i + 1][j + 1] = (double)m_InputImage[i * m_width + j];
		}
	}
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			max = 0.0; // 블록이 이동할 때마다 최대값 초기화
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					if (DoubleABS(m_tempImage[i + 1][j + 1] -
						m_tempImage[i + n][j + m]) >= max)
						// 블록의 가운데 값 - 블록의 주변 픽셀 값의 절대 값
						// 중에서 최대값을 찾는다.
						max = DoubleABS(m_tempImage[i + 1]
							[j + 1] - m_tempImage[i + n][j + m]);
				}
			}
			tempOutputImage[i][j] = max; // 찾은 최대값을 출력 값으로 지정
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (tempOutputImage[i][j] > 255.)
				tempOutputImage[i][j] = 255.;
			if (tempOutputImage[i][j] < 0.)
				tempOutputImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++)
		for (j = 0; j < m_Re_width; j++)
			m_OutputImage[i * m_Re_width + j] = (unsigned char)tempOutputImage[i][j];
}

double CIMPKSH200721Doc::DoubleABS(double X)
{
	return X >= 0 ? X: -X;
}

void CIMPKSH200721Doc::OnLaplacian()
{
	int i, j;
	double LaplacianMask[3][3] = { {0., 1., 0.}, {1., -4., 1.}, {0., 1., 0.} };
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, LaplacianMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}

