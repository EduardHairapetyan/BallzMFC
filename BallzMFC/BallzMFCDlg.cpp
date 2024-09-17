
// BallzMFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BallzMFC.h"
#include "BallzMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBallzMFCDlg dialog



CBallzMFCDlg::CBallzMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BALLZMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBallzMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBallzMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()  // Ensure this is included
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CBallzMFCDlg::OnNMThemeChangedScrollbar1)
END_MESSAGE_MAP()



// CBallzMFCDlg message handlers

BOOL CBallzMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Initialize ball position and speed
	m_BallRadius = 20;
	m_BallRect.SetRect(50, 50, 50 + m_BallRadius * 2, 50 + m_BallRadius * 2);
	m_SpeedX = 5;
	m_SpeedY = 4;

	// Set up the timer for ball movement
	m_TimerID = SetTimer(1, 16, nullptr);

	// Create the custom slider
	CRect sliderRect(0, 10, 470, 30); // Example size and position
	m_Slider.Create(nullptr, _T("Slider"), WS_CHILD | WS_VISIBLE | WS_BORDER, sliderRect, this, 1001);
	m_Slider.SetRange(0, 100);
	m_Slider.SetPos(50);
	m_Slider.SetThumbSize(80, 20);

	return TRUE;
}



void CBallzMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	static bool ballBouncedFromSlider = false;

	if (nIDEvent == 1)
	{
		OutputDebugString(_T("OnTimer triggered!\n"));

		// Move the ball
		m_BallRect.OffsetRect(m_SpeedX, m_SpeedY);

		// Bounce off the walls
		CRect clientRect;
		GetClientRect(&clientRect);

		if (m_BallRect.left <= clientRect.left || m_BallRect.right >= clientRect.right)
		{
			m_SpeedX = -m_SpeedX; // Reverse X direction
		}

		// Check if ball bounces from top and hasn't bounced from the slider
		if (m_BallRect.top <= clientRect.top)
		{
			if (!ballBouncedFromSlider)
			{
				// Game over condition
				KillTimer(m_TimerID); // Stop the game timer
				MessageBox(_T("Game Over!"), _T("BallzMFC"), MB_OK | MB_ICONINFORMATION);
				return; // Exit the timer event
			}
			else
			{
				m_SpeedY = -m_SpeedY; // Reverse Y direction if it bounced from the slider
				ballBouncedFromSlider = false; // Reset flag after bouncing from top
			}
		}

		// Check for collision with the bottom
		if (m_BallRect.bottom >= clientRect.bottom)
		{
			m_SpeedY = -m_SpeedY; // Reverse Y direction
		}

		// Check for collision with the slider when moving upwards
		if (m_SpeedY < 0) // Only check if the ball is moving upwards
		{
			CRect sliderRect;
			m_Slider.GetWindowRect(&sliderRect);  // Get the slider's position in screen coordinates
			ScreenToClient(&sliderRect);  // Convert slider's position to client coordinates

			CRect thumbRect;
			m_Slider.GetThumbRect(thumbRect);  // Get the slider's thumb position and size

			// Check if the ball is hitting the thumb of the slider
			if (m_BallRect.top <= thumbRect.bottom &&
				(m_BallRect.right + m_BallRect.left) / 2 >= thumbRect.left &&
				(m_BallRect.right + m_BallRect.left) / 2 <= thumbRect.right)
			{
				// Ball has touched the bottom of the slider, reverse Y direction
				m_SpeedY = -m_SpeedY;
				ballBouncedFromSlider = true; // Set flag that ball has bounced from the slider

				// Move the ball below the slider to avoid multiple hits
				m_BallRect.OffsetRect(0, 10);
			}
		}

		// Redraw the ball
		Invalidate();  // Trigger a repaint
	}

	CDialogEx::OnTimer(nIDEvent);
}

// BallzMFCDlg.cpp
void CBallzMFCDlg::OnDestroy()
{
	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID); // Stop the timer
		m_TimerID = 0;
	}

	CDialogEx::OnDestroy();
}


void CBallzMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBallzMFCDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Get the client rectangle
	CRect clientRect;
	GetClientRect(&clientRect);

	// Create a memory device context compatible with the screen DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	// Create a bitmap compatible with the client area
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());

	// Select the bitmap into the memory DC
	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);

	// Fill the background
	memDC.FillSolidRect(&clientRect, RGB(255, 255, 255));  // White background

	// Set up the brush and pen for the ball
	CBrush ballBrush(RGB(255, 0, 0));  // Red ball
	CBrush* pOldBrush = memDC.SelectObject(&ballBrush);

	// Draw the ball in the memory DC
	memDC.Ellipse(m_BallRect);

	// Copy the memory DC (off-screen) to the screen DC
	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memDC, 0, 0, SRCCOPY);

	// Clean up
	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldBitmap);
}





// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBallzMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBallzMFCDlg::OnNMCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CBallzMFCDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CBallzMFCDlg::OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
