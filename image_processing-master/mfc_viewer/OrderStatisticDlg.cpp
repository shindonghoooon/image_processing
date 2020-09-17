// OrderStatisticDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "OrderStatisticDlg.h"
#include "afxdialogex.h"


// COrderStatisticDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COrderStatisticDlg, CDialogEx)

COrderStatisticDlg::COrderStatisticDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATISTICDIALOG, pParent)
	, m_radio_osf(0)
{

}

COrderStatisticDlg::~COrderStatisticDlg()
{
}

void COrderStatisticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MEDIAN, m_radio_osf);
	DDX_Control(pDX, IDC_EDIT_MINSIZE, Edit_FilterSize1);
	DDX_Control(pDX, IDC_EDIT_MAXSIZE, Edit_FilterSize2);
}


BEGIN_MESSAGE_MAP(COrderStatisticDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OKOSFBUTTON, &COrderStatisticDlg::OnBnClickedOkosfbutton)
	ON_BN_CLICKED(IDC_CANCELOSFBUTTON, &COrderStatisticDlg::OnBnClickedCancelosfbutton)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MEDIAN, IDC_RADIO_ADAPTIVEMEDIAN, OnRadioClicked)
END_MESSAGE_MAP()


// COrderStatisticDlg 메시지 처리기입니다.


void COrderStatisticDlg::OnBnClickedOkosfbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp1, strTemp2;
	Edit_FilterSize1.GetWindowTextA(strTemp1);
	Edit_FilterSize2.GetWindowTextA(strTemp2);
	CDialog::OnOK();
}


void COrderStatisticDlg::OnBnClickedCancelosfbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void COrderStatisticDlg::OnRadioClicked(UINT msg) {
	UpdateData(TRUE);
	switch (m_radio_osf) {
	case 0:
		nOSFType = 4;
		break;
	case 1:
		nOSFType = 5;
		break;
	}
}
int COrderStatisticDlg::GetFilterSize1() { return nFilterSize1; }
int COrderStatisticDlg::GetFilterSize2() { return nFilterSize2; }
int COrderStatisticDlg::GetOSFType() { return nOSFType; }
