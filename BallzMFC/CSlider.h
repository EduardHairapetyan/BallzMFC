#pragma once
#include "pch.h"
#include <afxwin.h>

class CMyCustomSlider : public CWnd
{
public:
    CMyCustomSlider();

    void SetRange(int min, int max);
    void SetPos(int pos);
    int GetPos() const;
    void SetThumbSize(int width, int height);
    void GetThumbRect(CRect& thumbRect);

protected:
    virtual void PreSubclassWindow();
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()

private:
    void UpdateThumbRect();
    int m_minRange;
    int m_maxRange;
    int m_pos;
    int m_thumbWidth;
    int m_thumbHeight;
    CRect m_thumbRect;
};
