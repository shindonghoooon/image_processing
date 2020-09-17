// ImgScalingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "ImgScalingDlg.h"
#include "afxdialogex.h"


// CImgScalingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CImgScalingDlg, CDialogEx)

CImgScalingDlg::CImgScalingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_radio_interp(0)
{

}

CImgScalingDlg::~CImgScalingDlg()
{
}

void CImgScalingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_Bilinear, m_radio_interp);
	DDX_Control(pDX, IDC_EDIT_SCALE, m_EditScaleRate);
}


BEGIN_MESSAGE_MAP(CImgScalingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OKBUTTON2, &CImgScalingDlg::OnBnClickedOkbutton2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_Bilinear, IDC_RADIO_BICUBIC, OnRadioClicked)
END_MESSAGE_MAP()


// CImgScalingDlg 메시지 처리기입니다.


void CImgScalingDlg::OnBnClickedOkbutton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString cstrTemp;
	m_EditScaleRate.GetWindowTextA(cstrTemp);
	dScaleRate = atof(cstrTemp);
	CDialog::OnOK();
}
void CImgScalingDlg::OnRadioClicked(UINT msg) {
	UpdateData(TRUE);
	switch (m_radio_interp) {
	case 0:
		nInterpolationType = 0;
		break;
	case 1:
		nInterpolationType = 1;
		break;
	}
}

double CImgScalingDlg::GetScaleRate() { return dScaleRate; }
int CImgScalingDlg::GetInterpType() { return nInterpolationType; }
