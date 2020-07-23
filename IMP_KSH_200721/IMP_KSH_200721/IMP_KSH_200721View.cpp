
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
	ON_COMMAND(ID_DOWN_SAMPLING, &CIMPKSH200721View::OnDownSampling)
	ON_COMMAND(ID_UP_SAMPLING, &CIMPKSH200721View::OnUpSampling)
	ON_COMMAND(ID_QUANTIZATION, &CIMPKSH200721View::OnQuantization)
	ON_COMMAND(ID_SUM_CONSTANT, &CIMPKSH200721View::OnSumConstant)
	ON_COMMAND(ID_SUB_CONSTANT, &CIMPKSH200721View::OnSubConstant)
	ON_COMMAND(ID_MUL_CONSTANT, &CIMPKSH200721View::OnMulConstant)
	ON_COMMAND(ID_DIV_CONSTANT, &CIMPKSH200721View::OnDivConstant)
	ON_COMMAND(ID_AND_OPERATE, &CIMPKSH200721View::OnAndOperate)
	ON_COMMAND(ID_OR_OPERATE, &CIMPKSH200721View::OnOrOperate)
	ON_COMMAND(ID_XOR_OPERATE, &CIMPKSH200721View::OnXorOperate)
	ON_COMMAND(ID_NEGA_TRANSFORM, &CIMPKSH200721View::OnNegaTransform)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CIMPKSH200721View::OnGammaCorrection)
	ON_COMMAND(ID_BINARIZATION, &CIMPKSH200721View::OnBinarization)
	ON_COMMAND(ID_STRESS_TRANSFORM,&CIMPKSH200721View::OnStressTransform)
	ON_COMMAND(ID_HISTO_STRETCH,&CIMPKSH200721View::OnHistoStretch)
	ON_COMMAND(ID_END_IN_SEARCH, &CIMPKSH200721View::OnEndInSearch)
	ON_COMMAND(ID_HISTOGRAM, &CIMPKSH200721View::OnHistogram)
	ON_COMMAND(ID_HISTO_EQUAL,&CIMPKSH200721View::OnHistoEqual)
	ON_COMMAND(ID_HISTO_SPEC,&CIMPKSH200721View::OnHistoSpec)
	ON_COMMAND(ID_EMBOSSING,&CIMPKSH200721View::OnEmbossing)
	ON_COMMAND(ID_BLURR, &CIMPKSH200721View::OnBlurr)
	ON_COMMAND(ID_GAUSSIAN_FILTER, &CIMPKSH200721View::OnGaussianFilter)
	ON_COMMAND(ID_SHARPENING, &CIMPKSH200721View::OnSharpening)
	ON_COMMAND(ID_HPF_SHARP, &CIMPKSH200721View::OnHpfSharp)
	ON_COMMAND(ID_LPF_SHARP, &CIMPKSH200721View::OnLpfSharp)
	ON_COMMAND(ID_DIFF_OPERATOR_HOR, &CIMPKSH200721View::OnDiffOperatorHor)
	ON_COMMAND(ID_HOMOGEN_OPERATOR,&CIMPKSH200721View::OnHomogenOperator)
	ON_COMMAND(ID_LAPLACIAN,&CIMPKSH200721View::OnLaplacian)
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


void CIMPKSH200721View::OnDownSampling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CIMPKSH200721Doc* pDoc = GetDocument(); // Doc 클래스 참조
	ASSERT_VALID(pDoc);
	pDoc->OnDownSampling(); // Doc 클래스에 OnDownSampling 함수 호출
	Invalidate(TRUE); // 화면 갱신
}


void CIMPKSH200721View::OnUpSampling()
{
	// TODO: Add your command handler code here
	CIMPKSH200721Doc* pDoc = GetDocument(); // Doc 클래스 참조
	ASSERT_VALID(pDoc);
	pDoc->OnUpSampling(); // Doc 클래스에 OnUpSampling 함수 호출
	Invalidate(TRUE); // 화면 갱신

}


void CIMPKSH200721View::OnQuantization()
{
	CIMPKSH200721Doc* pDoc = GetDocument(); // Doc 클래스 참조
	ASSERT_VALID(pDoc);
	pDoc->OnQuantization(); // Doc 클래스에 OnQuantization 함수 호출
	Invalidate(TRUE); // 화면 갱신

}

void CIMPKSH200721View::OnSumConstant()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSumConstant();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnSubConstant()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSubConstant();
	Invalidate(TRUE);
}
void CIMPKSH200721View::OnMulConstant()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnMulConstant();
	Invalidate(TRUE);
}
void CIMPKSH200721View::OnDivConstant()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDivConstant();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnAndOperate()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnAndOperate();
	Invalidate(TRUE);
}
void CIMPKSH200721View::OnOrOperate()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOrOperate();
	Invalidate(TRUE);
}
void CIMPKSH200721View::OnXorOperate()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnXorOperate();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnNegaTransform()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnNegaTransform();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnGammaCorrection()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGammaCorrection();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnBinarization()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBinarizaion();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnStressTransform()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnStressTransform();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnHistoStretch()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistoStretch();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnEndInSearch()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEndInSearch();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnHistogram()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogram();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnHistoEqual()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistoEqual();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnHistoSpec()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistoSpec();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnEmbossing()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnEmbossing();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnBlurr()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnBlurr();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnGaussianFilter()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnGaussianFilter();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnSharpening()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnSharpening();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnHpfSharp()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHpfSharp();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnLpfSharp()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLpfSharp();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnDiffOperatorHor()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnDiffOperatorHor();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnHomogenOperator()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHomogenOperator();
	Invalidate(TRUE);
}

void CIMPKSH200721View::OnLaplacian()
{
	CIMPKSH200721Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnLaplacian();
	Invalidate(TRUE);
}
