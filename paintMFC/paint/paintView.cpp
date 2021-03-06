
// paintView.cpp : implementation of the CpaintView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "paint.h"
#endif

#include "paintDoc.h"
#include "paintView.h"
#include <memory>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpaintView

IMPLEMENT_DYNCREATE(CpaintView, CView)

BEGIN_MESSAGE_MAP(CpaintView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_SETCOLOR_RED, &CpaintView::OnSetcolorRed)
ON_COMMAND(ID_SETCOLOR_GREEN, &CpaintView::OnSetcolorGreen)
ON_COMMAND(ID_SETCOLOR_BLUE, &CpaintView::OnSetcolorBlue)
ON_COMMAND(ID_SETCOLOR_BLACK, &CpaintView::OnSetcolorBlack)
ON_COMMAND(ID_SETWIDTH_1, &CpaintView::OnSetwidth1)
ON_COMMAND(ID_SETWIDTH_3, &CpaintView::OnSetwidth3)
ON_COMMAND(ID_SETWIDTH_5, &CpaintView::OnSetwidth5)
ON_COMMAND(ID_SETTYPE_POINT, &CpaintView::OnSettypePoint)
ON_COMMAND(ID_SETTYPE_LINE, &CpaintView::OnSettypeLine)
ON_COMMAND(ID_SETTYPE_RECTANGLE, &CpaintView::OnSettypeRectangle)
ON_COMMAND(ID_SETTYPE_ELLIPSE, &CpaintView::OnSettypeEllipse)
ON_COMMAND(ID_SETTYPE_ERASER, &CpaintView::OnSettypeEraser)
END_MESSAGE_MAP()

// CpaintView construction/destruction

CpaintView::CpaintView() noexcept
{
	// TODO: add construction code here
	baseFigure = factory.createFigureInstance(L"PPoint");
}

CpaintView::~CpaintView()
{

}

BOOL CpaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	

	return CView::PreCreateWindow(cs);
}

// CpaintView drawing


void CpaintView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
	CpaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	std::unique_ptr<BasicPaintType> tmpFigure;

	std::vector<figure> figureVector = pDoc->getVector();

	if (figureVector.size() > 0)
	{
		for(auto i = 0; i < figureVector.size(); i++)
		{
			tmpFigure = factory.createFigureInstance(figureVector[i].figureName);
			CPen myPen(PS_SOLID, figureVector[i].width, figureVector[i].color);
			pDC->SelectObject(&myPen);
			tmpFigure->Draw(pDC, figureVector[i]);
			
		}
	}
}


// CpaintView printing

BOOL CpaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CpaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CpaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CpaintView::drawFigure(const figure& curState,const CPoint& point)
{
	CClientDC pDC(this);

	CPen myPen(PS_SOLID, curState.width, curState.color);
	pDC.SelectObject(&myPen);
	baseFigure->Draw(&pDC, curState);
}


// CpaintView diagnostics

#ifdef _DEBUG
void CpaintView::AssertValid() const
{
	CView::AssertValid();
}

void CpaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpaintDoc* CpaintView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpaintDoc)));
	return (CpaintDoc*)m_pDocument;
}
#endif //_DEBUG


// CpaintView message handlers


void CpaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	drawingInProgress = true;

	CpaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	baseFigure->mouseDown(point, pDoc,penColor,penWidth);

	CView::OnLButtonDown(nFlags, point);
}


void CpaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CpaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	drawingInProgress = false;
	baseFigure->mouseUp(point, pDoc);
	drawFigure(baseFigure->GetFigure(), point);

	CView::OnLButtonUp(nFlags, point);
}


void CpaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CpaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	figure curState = baseFigure->GetFigure();
	if (drawingInProgress && baseFigure->mouseMove(point, pDoc))
	{
		curState.secP = point;
		drawFigure(curState, point);
	}

	CView::OnMouseMove(nFlags, point);
}


void CpaintView::OnSetcolorRed()
{
	penColor = RGB(255,0,0);
	// TODO: Add your command handler code here
}


void CpaintView::OnSetcolorGreen()
{
	// TODO: Add your command handler code here
	penColor = RGB(0, 255, 0);
}


void CpaintView::OnSetcolorBlue()
{
	// TODO: Add your command handler code here
	penColor = RGB(0, 0, 255);

}


void CpaintView::OnSetcolorBlack()
{
	// TODO: Add your command handler code here
	penColor = RGB(0, 0, 0);
}


void CpaintView::OnSetwidth1()
{
	// TODO: Add your command handler code here
	penWidth = 1;
}

void CpaintView::OnSetwidth3()
{
	// TODO: Add your command handler code here
	penWidth = 3;
}

void CpaintView::OnSetwidth5()
{
	penWidth = 5;
}


void CpaintView::OnSettypePoint()
{
	baseFigure = factory.createFigureInstance(L"PPoint");
	// TODO: Add your command handler code here
}


void CpaintView::OnSettypeLine()
{
	baseFigure = factory.createFigureInstance(L"PLine");
	// TODO: Add your command handler code here
}


void CpaintView::OnSettypeRectangle()
{
	baseFigure = factory.createFigureInstance(L"PRectangle");
	// TODO: Add your command handler code here
}


void CpaintView::OnSettypeEllipse()
{
	baseFigure = factory.createFigureInstance(L"PEllipse");
	// TODO: Add your command handler code here
}


void CpaintView::OnSettypeEraser()
{
	baseFigure = factory.createFigureInstance(L"PEraser");
	// TODO: Add your command handler code here
}
