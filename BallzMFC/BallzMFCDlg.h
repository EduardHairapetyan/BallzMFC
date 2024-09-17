// BallzMFCDlg.h

// Ensure you include necessary headers
#include <afxcmn.h>  // For CSliderCtrl
#include "CSlider.h"

class CBallzMFCDlg : public CDialogEx
{
    // Construction
public:
    CBallzMFCDlg(CWnd* pParent = nullptr);  // standard constructor

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BALLZMFC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

    // Implementation
protected:
    HICON m_hIcon;
    UINT_PTR m_TimerID;
    CRect m_BallRect;
    int m_BallRadius;
    int m_SpeedX;
    int m_SpeedY;
    CMyCustomSlider m_Slider;

    // Slider control for bounce effect
    CSliderCtrl m_SliderBounce;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnNMCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult);
};


