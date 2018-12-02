
// paintDoc.h : interface of the CpaintDoc class
//


#pragma once

#include <vector>


class CpaintDoc : public CDocument
{
protected: // create from serialization only
	CpaintDoc() noexcept;
	DECLARE_DYNCREATE(CpaintDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CpaintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void addPoint(const CPoint& point);
	const std::vector<CPoint>& getVector();
protected:

	std::vector<CPoint> m_points;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
