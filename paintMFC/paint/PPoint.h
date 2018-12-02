#pragma once
#include "BasicPaintType.h"
class PPoint :
	public BasicPaintType
{	
public:
	void mouseDown(CPoint &, CpaintDoc* doc, unsigned long color = 0, int width = 1) override;
	void mouseMove(CPoint & ,CpaintDoc* doc, unsigned long color = 0, int width = 1) override;
	void mouseUp(CPoint &, CpaintDoc* doc, unsigned long color = 0, int width = 1) override;
	void Draw(CDC* pDC,  const figure& Figire) override;
	PPoint(unsigned long, int);
	~PPoint();
};

