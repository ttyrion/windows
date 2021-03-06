#include "stdafx.h"
#include "SubWindow.h"
#include <sstream>


CSubWindow::CSubWindow(int id)
{
    id_ = id;
}


CSubWindow::~CSubWindow()
{
}

LPCTSTR CSubWindow::GetWindowClassName() const {
    return L"TypeApp SubWindow";
}

void CSubWindow::SetTitle(const std::wstring& title) {
    if (title_) {
        title_->SetText(title.c_str());
    }
}

LRESULT CSubWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    BOOL handled = FALSE;
    LRESULT result = 0;
    switch (uMsg)
    {
        case WM_CREATE: {
            result = OnCreate(uMsg, wParam, lParam, handled);
        }
            break;
        case WM_NCHITTEST: {
            result = OnNcHitTest(uMsg, wParam, lParam, handled);
        }
            break;
        case WM_PAINT: {
            int m = id_;
            int n = m;
            break;
        }
        case WM_MOVE: {
            if (id_ % 2 == 0) {
                RECT rect = { 0 };
                //不是GetWindowRect
                ::GetClientRect(m_hWnd, &rect);
                //w and h are the width-height of the the rounded corner, which may be a ellipse
                HRGN rgn = ::CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, rect.right - rect.left, rect.bottom - rect.top);
                ::SetWindowRgn(m_hWnd, rgn, TRUE);
                ::DeleteObject(rgn);
            }
            break;
        }
        default:
            break;
    }

    if (handled) {
        return result;
    }

    if (pntm_.MessageHandler(uMsg, wParam, lParam, result)) {
        return result;
    }

    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CSubWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled) {
    handled = TRUE;
    pntm_.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI *root = builder.Create(L"Sub.xml", 0, NULL, &pntm_, NULL);
    if (root) {
        pntm_.AttachDialog(root);
        FindSubCtrls();
    }

    return 0;
}


LRESULT CSubWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled) {
    handled = TRUE;
    if (caption_) {
        short x = GET_X_LPARAM(lParam);
        short y = GET_Y_LPARAM(lParam);
        POINT pt = { x,y };

        int height = caption_->GetFixedHeight();
        RECT rect = { 0 };
        ::GetWindowRect(m_hWnd, &rect);
        rect.bottom = rect.top + height;
        if (::PtInRect(&rect, pt)) {
            //FindControl的pt须是相对窗口的坐标
            pt.x -= rect.left;
            pt.y -= rect.top;
            CControlUI* ctrl = pntm_.FindControl(pt);
            if (ctrl) {
                std::wstring class_name = ctrl->GetClass();
                if (class_name == DUI_CTR_BUTTON) {
                    return HTCLIENT;
                }
            }

            return HTCAPTION;
        }
    }

    return HTCLIENT;
}

void CSubWindow::FindSubCtrls() {
    close_ = static_cast<CButtonUI*>(pntm_.FindControl(L"close"));
    if (close_) {
        close_->Subscribe(CEventSets::EventClick, MakeDelegate(this, &CSubWindow::OnCloseClicked));
    }

    caption_ = static_cast<CHorizontalLayoutUI*>(pntm_.FindControl(L"caption"));
    title_ = static_cast<CLabelUI*>(pntm_.FindControl(L"title"));
    if (title_) {
        std::wstringstream stream;
        stream << L"Sub:" << id_;
        title_->SetText(stream.str().c_str());
    }
}

bool CSubWindow::OnCloseClicked(void *param) {
    Close();

    return true;
}