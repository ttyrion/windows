#pragma once
#include "..\..\Dui\UIlib.h"

using namespace DuiLib;

class CSubWindow : public CWindowWnd
{
public:
    CSubWindow(int id);
    ~CSubWindow();

protected:
    virtual LPCTSTR GetWindowClassName() const override;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    void SetTitle(const std::wstring& title);
private:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& handled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &handled);
    void FindSubCtrls();
    bool OnCloseClicked(void *param);

    CPaintManagerUI pntm_;
    CButtonUI *close_ = nullptr;
    CHorizontalLayoutUI *caption_ = nullptr;
    CLabelUI* title_ = nullptr;
    int id_ = 0;
};

