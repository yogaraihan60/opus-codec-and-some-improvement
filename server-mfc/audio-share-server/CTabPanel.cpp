#include "pch.h"
#include "CTabPanel.h"
#include <resource.h>

IMPLEMENT_DYNAMIC(CTabPanel, CDialogEx)

CTabPanel::CTabPanel(UINT nIDTemplate, UINT nIDTitle, CWnd* pParent)
    : CDialogEx(nIDTemplate, pParent)
{
    (void)m_strTitle.LoadStringW(nIDTitle);
}

CTabPanel::~CTabPanel()
{
}

BOOL CTabPanel::Create()
{
    return CDialogEx::Create(m_lpszTemplateName, m_pParentWnd);
}


BEGIN_MESSAGE_MAP(CTabPanel, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CTabPanel::PostNcDestroy()
{
    delete this;
}

BOOL CTabPanel::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetBackgroundColor(RGB(32, 32, 32));
    return TRUE;
}


HBRUSH CTabPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // Dark Mode Colors
    static COLORREF darkColor = RGB(32, 32, 32);
    static HBRUSH hbrDark = CreateSolidBrush(darkColor);

    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetTextColor(RGB(240, 240, 240)); // White text
        pDC->SetBkColor(darkColor);
        return hbrDark;
    }

    return hbr;
}
