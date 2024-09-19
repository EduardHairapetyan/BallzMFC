#include "pch.h"
#include "CSlider.h"
#include "afxdialogex.h"

// Define the thumb size constants
const int THUMB_WIDTH = 80;
const int THUMB_HEIGHT = 20;

CSlider::CSlider()
    : m_minRange(0), m_maxRange(100), m_pos(0), m_thumbWidth(20), m_thumbHeight(20)
{
}

void CSlider::PreSubclassWindow()
{
    CWnd::PreSubclassWindow();
}

void CSlider::OnPaint()
{
    CPaintDC dc(this);
    CBrush brush(RGB(0, 0, 255)); // Blue thumb
    dc.SelectObject(&brush);
    dc.Rectangle(m_thumbRect);
}

void CSlider::OnMouseMove(UINT nFlags, CPoint point)
{
    if (nFlags & MK_LBUTTON)
    {
        // Get the client rectangle of the slider
        CRect clientRect;
        GetClientRect(&clientRect);  // Fill clientRect with the slider's dimensions

        // Update the slider's position based on the mouse position
        int newPos = (point.x - m_thumbWidth / 2) * (m_maxRange - m_minRange) / (clientRect.Width() - m_thumbWidth) + m_minRange;
        SetPos(newPos);  // Set the new slider position
    }

    CWnd::OnMouseMove(nFlags, point);
}


void CSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonDown(nFlags, point);
    // Check if the mouse click is within the thumb's rectangle
    if (m_thumbRect.PtInRect(point))
    {
        SetCapture(); // Capture mouse events to handle dragging
    }
}

void CSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture(); // Release mouse capture
    CWnd::OnLButtonUp(nFlags, point);
}

void CSlider::SetRange(int min, int max)
{
    m_minRange = min;
    m_maxRange = max;
    UpdateThumbRect();
    Invalidate();
}


void CSlider::SetPos(int pos)
{
    if (pos < m_minRange) pos = m_minRange;
    if (pos > m_maxRange) pos = m_maxRange;
    m_pos = pos;
    UpdateThumbRect();
    Invalidate();
}

int CSlider::GetPos() const
{
    return m_pos;
}

// In CSlider.cpp

void CSlider::GetThumbRect(CRect& thumbRect)
{
    // Get the slider's client rectangle (control's total dimensions)
    CRect clientRect;
    GetClientRect(&clientRect);

    // Calculate the thumb's horizontal position
    int thumbLeft = (m_pos - m_minRange) * (clientRect.Width() - m_thumbWidth) / (m_maxRange - m_minRange);

    // Set the thumb's rectangle
    thumbRect.left = thumbLeft;
    thumbRect.right = thumbLeft + m_thumbWidth;
    thumbRect.top = 0;
    thumbRect.bottom = m_thumbHeight;
}

void CSlider::UpdateThumbRect()
{
    CRect thumbRect;
    GetThumbRect(thumbRect);

    // Set the thumb rectangle
    m_thumbRect = thumbRect;
}

void CSlider::SetThumbSize(int width, int height)
{
    m_thumbWidth = width;
    m_thumbHeight = height;
    UpdateThumbRect();
    Invalidate();
}


BEGIN_MESSAGE_MAP(CSlider, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()
