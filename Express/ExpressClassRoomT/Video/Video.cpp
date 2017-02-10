#include "Video.h"


CVideoWnd::CVideoWnd():m_pOwner(NULL)
{

}


CVideoUI::CVideoUI(void) :m_pwindows(NULL), MediaPlayer(NULL)
{
}

CVideoUI::~CVideoUI(void)
{
	if (MediaPlayer)
		ILivePlayer::ReleaseInstance(MediaPlayer);
}

LPCTSTR CVideoWnd::GetWindowClassName() const
{
	return _T("CVideoWnd");
}
void CVideoWnd::Init(CVideoUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcpos=CalPos();
	RECT rcwnd;
	::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(),&rcwnd);
	rcpos.left+=rcwnd.left;
	rcpos.right+=rcwnd.left;
	rcpos.top+=rcwnd.top;
	rcpos.bottom+=rcwnd.top;
	Create(m_pOwner->GetManager()->GetPaintWindow(),NULL,WS_CHILD|WS_EX_TOPMOST | ES_AUTOHSCROLL | WS_VISIBLE,0,rcpos);
	::ShowWindow(m_hWnd,SW_SHOW);
}
LRESULT CVideoWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ERASEBKGND)
	{
		PainBk();
	}
	return CWindowWnd::HandleMessage(uMsg,wParam,lParam);
}

RECT CVideoWnd::CalPos()
{
	return m_pOwner->GetPos();
}
void CVideoWnd::PainBk()
{
	CDuiRect rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	PAINTSTRUCT    ps;
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
	::SelectObject(hMemDC, hBitmap);
	CRenderEngine::DrawColor(hMemDC, rcClient, 0x0);
	//CRenderEngine::DrawColor(hMemDC, rcClient, 0xffff0000);

	BitBlt(hDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hMemDC, 0, 0, SRCCOPY);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	::EndPaint(m_hWnd, &ps);
}

/**************************************************/
void CVideoUI::SetVisible(bool bVisible )
{
	__super::SetVisible(bVisible);
	ShowWindow(getHwnd(),bVisible);
}
void CVideoUI::SetPos(RECT rc)
{
	__super::SetPos(rc);
	if(m_pwindows)
	{
		RECT rcpos=m_pwindows->CalPos();
		::SetWindowPos(m_pwindows->GetHWND(),NULL,rcpos.left,rcpos.top,rcpos.right-rcpos.left,rcpos.bottom-rcpos.top,SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
HWND CVideoUI::getHwnd()
{
	if(!m_pwindows)
	{
		m_pwindows = new CVideoWnd();
		ASSERT(m_pwindows);
		m_pwindows->Init(this);
	}
	return m_pwindows->GetHWND();
}
void CVideoUI::Init()
{
	if(!m_pwindows)
	{
		m_pwindows = new CVideoWnd();
		ASSERT(m_pwindows);
		m_pwindows->Init(this);

		if (getHwnd())
		{
			MediaPlayer = ILivePlayer::GetInstance();
			MediaPlayer->SetHWND(getHwnd());
		}
	}	
}