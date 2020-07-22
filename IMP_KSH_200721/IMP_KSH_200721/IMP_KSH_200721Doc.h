
// IMP_KSH_200721Doc.h: CIMPKSH200721Doc 클래스의 인터페이스
//


#pragma once


class CIMPKSH200721Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CIMPKSH200721Doc() noexcept;
	DECLARE_DYNCREATE(CIMPKSH200721Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CIMPKSH200721Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char* m_InputImage;
	int m_width, m_height, m_size;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnDownSampling();
	unsigned char* m_OutputImage;
	int m_Re_width, m_Re_height, m_Re_size;
	void OnUpSampling();
	void OnQuantization();
	void OnSumConstant();
	void OnSubConstant();
	void OnMulConstant();
	void OnDivConstant();
	void OnAndOperate();
	void OnOrOperate();
	void OnXorOperate();
	void OnNegaTransform();
	void OnGammaCorrection();
	void OnBinarizaion();
	void OnStressTransform();
	void OnHistoStretch();
};
