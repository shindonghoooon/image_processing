// FilterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// CFilterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_radio_Filtertype(0)
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_AVERAGE, m_radio_Filtertype);
	DDX_Control(pDX, IDC_EDIT_FILTERSIZE, m_EditFilterSize);
	DDX_Control(pDX, IDC_EDIT_GAUSSIAN, m_EditSigma);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OKBUTTON4, &CFilterDlg::OnBnClickedOkbutton4)
	ON_BN_CLICKED(IDC_CANCELBUTTON4, &CFilterDlg::OnBnClickedCancelbutton4)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_AVERAGE, IDC_RADIO_SMOOTH, OnClickedRadio)
END_MESSAGE_MAP()


// CFilterDlg �޽��� ó�����Դϴ�.


void CFilterDlg::OnBnClickedOkbutton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString cstrTemp1, cstrTemp2;
	m_EditFilterSize.GetWindowTextA(cstrTemp1);
	nFilterSize = atoi(cstrTemp1);

	m_EditSigma.GetWindowTextA(cstrTemp2);
	dSigma = atof(cstrTemp2);

	CDialog::OnOK();
}
void CFilterDlg::OnBnClickedCancelbutton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnCancel();
}
void CFilterDlg::OnClickedRadio(UINT msg) {
	UpdateData(TRUE);
	switch (m_radio_Filtertype) {
	case 0:
		nFilterType = 0;
		break;
	case 1:
		nFilterType = 3;
		break;
	case 2:
		nFilterType = 2;
		break;
	case 3:
		nFilterType = 1;
		break;
	}
}
int CFilterDlg::GetFilterSize() { return nFilterSize; }
int CFilterDlg::GetFilterType() { return nFilterType; }
double CFilterDlg::GetSigma() { return dSigma; }