// OrderStatisticDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer.h"
#include "OrderStatisticDlg.h"
#include "afxdialogex.h"


// COrderStatisticDlg ��ȭ �����Դϴ�.

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


// COrderStatisticDlg �޽��� ó�����Դϴ�.


void COrderStatisticDlg::OnBnClickedOkosfbutton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp1, strTemp2;
	Edit_FilterSize1.GetWindowTextA(strTemp1);
	Edit_FilterSize2.GetWindowTextA(strTemp2);
	CDialog::OnOK();
}


void COrderStatisticDlg::OnBnClickedCancelosfbutton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
