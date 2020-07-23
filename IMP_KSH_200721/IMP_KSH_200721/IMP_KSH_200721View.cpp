
// IMP_KSH_200721View.cpp: CIMPKSH200721View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IMP_KSH_200721.h"
#endif

#include "IMP_KSH_200721Doc.h"
#include "IMP_KSH_200721View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIMPKSH200721View

IMPLEMENT_DYNCREATE(CIMPKSH200721View, CView)

BEGIN_MESSAGE_MAP(CIMPKSH200721View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIMPKSH200721View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND_RANGE(ID_DOWN_SAMPLING, ID_LAPLACIAN, &CIMPKSH200721View::OnSelect)
END_MESSAGE_MAP()

// CIMPKSH200721View 생성/소멸

CIMPKSH200721View::CIMPKSH200721View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CIMPKSH200721View::~CIMPKSH200721View()
{
}

BOOL CIMPKSH200721View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CIMPKSH200721View 그리기

void CIMPKSH200721View::OnDraw(CDC* pDC)
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	ASSERT_VALID(pDoc);
	int i, j;
	unsigned char pixel;
	for (int i = 0; i < pDoc->m_height; i++)
		for (int j = 0; j < pDoc->m_width; j++)
		{
			pixel = pDoc->m_InputImage[i * pDoc->m_width + j];
			pDC->SetPixel(j + 5, i + 5, RGB(pixel, pixel, pixel));
		}
	// 축소된 영상 출력
	for (i = 0; i < pDoc->m_Re_height; i++) {
		for (j = 0; j < pDoc->m_Re_width; j++) {
			pixel = pDoc->m_OutputImage[i * pDoc->m_Re_width + j];
			pDC->SetPixel(j + pDoc->m_width + 10, i + 5, RGB(pixel, pixel, pixel));
		}
	}
}


// CIMPKSH200721View 인쇄

void CIMPKSH200721View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIMPKSH200721View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CIMPKSH200721View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CIMPKSH200721View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CIMPKSH200721View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIMPKSH200721View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIMPKSH200721View 진단

#ifdef _DEBUG
void CIMPKSH200721View::AssertValid() const
{
	CView::AssertValid();
}

void CIMPKSH200721View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIMPKSH200721Doc* CIMPKSH200721View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIMPKSH200721Doc)));
	return (CIMPKSH200721Doc*)m_pDocument;
}
#endif //_DEBUG


// CIMPKSH200721View 메시지 처리기


void CIMPKSH200721View::OnSelect(UINT UID)
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	switch (UID)
	{
		case ID_DOWN_SAMPLING:
			pDoc->OnDownSampling();
			break;
		case ID_UP_SAMPLING:
			pDoc->OnUpSampling();
			break;
		case ID_QUANTIZATION:
			pDoc->OnQuantization();
			break;
		case ID_SUM_CONSTANT:
			pDoc->OnSumConstant();
			break;
		case ID_SUB_CONSTANT:
			pDoc->OnSubConstant();
			break;
		case ID_MUL_CONSTANT:
			pDoc->OnMulConstant();
			break;
		case ID_DIV_CONSTANT:
			pDoc->OnDivConstant();
			break;
		case ID_AND_OPERATE:
			pDoc->OnAndOperate();
			break;
		case ID_OR_OPERATE:
			pDoc->OnOrOperate();
			break;
		case ID_XOR_OPERATE:
			pDoc->OnXorOperate();
			break;
		case ID_NEGA_TRANSFORM:
			pDoc->OnNegaTransform();
			break;
		case ID_GAMMA_CORRECTION:
			pDoc->OnGammaCorrection();
			break;
		case ID_BINARIZATION:
			pDoc->OnBinarizaion();
			break;
		case ID_STRESS_TRANSFORM:
			pDoc->OnStressTransform();
			break;
		case ID_HISTO_STRETCH:
			pDoc->OnHistoStretch();
			break;
		case ID_END_IN_SEARCH:
			pDoc->OnEndInSearch();
			break;
		case ID_HISTOGRAM:
			pDoc->OnHistogram();
			break;
		case ID_HISTO_EQUAL:
			pDoc->OnHistoEqual();
			break;
		case ID_HISTO_SPEC:
			pDoc->OnHistoSpec();
			break;
		case ID_EMBOSSING:
			pDoc->OnEmbossing();
			break;
		case ID_BLURR:
			pDoc->OnBlurr();
			break;
		case ID_GAUSSIAN_FILTER:
			pDoc->OnGaussianFilter();
			break;
		case ID_SHARPENING:
			pDoc->OnSharpening();
			break;
		case ID_HPF_SHARP:
			pDoc->OnHpfSharp();
			break;
		case ID_LPF_SHARP:
			pDoc->OnLpfSharp();
			break;
		case ID_DIFF_OPERATOR_HOR:
			pDoc->OnDiffOperatorHor();
			break;
		case ID_HOMOGEN_OPERATOR:
			pDoc->OnHomogenOperator();
			break;
		case ID_LAPLACIAN:
			pDoc->OnLaplacian();
			break;
	}
	Invalidate(TRUE);
}