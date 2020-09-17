// PSNRDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer.h"
#include "PSNRDlg.h"
#include "afxdialogex.h"


// CPSNRDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPSNRDlg, CDialogEx)

CPSNRDlg::CPSNRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PSNR, pParent)
{

}

CPSNRDlg::~CPSNRDlg()
{
}

void CPSNRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ORIIMAGE, Edit_File_Ori);
	DDX_Control(pDX, IDC_EDIT_RESIMAGE, Edit_File_Dst);
	DDX_Control(pDX, IDC_EDIT_GETPSNR, Edit_PSNR);
}


BEGIN_MESSAGE_MAP(CPSNRDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ORIBUTTON, &CPSNRDlg::OnBnClickedOributton)
	ON_BN_CLICKED(IDC_RESTBUTTON, &CPSNRDlg::OnBnClickedRestbutton)
//	ON_BN_CLICKED(IDC_GETPSNRBUTTON, &CPSNRDlg::OnBnClickedGetpsnrbutton)
	ON_BN_CLICKED(IDC_PSNRCLOSE, &CPSNRDlg::OnBnClickedPsnrclose)
//	ON_BN_CLICKED(IDC_PSNRBUTTON, &CPSNRDlg::OnBnClickedPsnrbutton)
END_MESSAGE_MAP()


// CPSNRDlg �޽��� ó�����Դϴ�.


void CPSNRDlg::OnBnClickedOributton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(TRUE);
	CRawInfoDlg RawDlg;
	if (Ori != NULL) {
		MemFree2D(Ori, nHeight_Ori);
	}
	if (dlg.DoModal() == IDOK) {
		if (dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "RAW") {
			MessageBox("���� Ȯ���ڰ� raw������ �ƴմϴ�.");
			return;
		}
		FILE* fp;
		fopen_s(&fp, dlg.GetFileName(), "rb");
		if (RawDlg.DoModal() == IDOK) {
			nHeight_Ori = RawDlg.GetRawHeight();
			nWidth_Ori = RawDlg.GetRawWidth();
			if (nHeight_Ori == 0 || nWidth_Ori == 0 || RawDlg.GetRawFormat() != 3) {
				MessageBox("�Է°��� �߸��Ǿ����ϴ�.");
				return;
			}
			Edit_File_Ori.SetWindowTextA(dlg.GetFileTitle());
			Ori = MemAlloc2D(nHeight_Ori, nWidth_Ori, 0);
			for (int h = 0; h < nHeight_Ori; h++) {
				fread(Ori[h], sizeof(unsigned char), nWidth_Ori, fp);
			}
		}
		fclose(fp);
	}
}


void CPSNRDlg::OnBnClickedRestbutton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(TRUE);
	CRawInfoDlg RawDlg;
	if (Dst != NULL) {
		MemFree2D(Dst, nHeight_Dst);
	}
	if (dlg.DoModal() == IDOK) {
		if (dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "RAW") {
			MessageBox("���� Ȯ���ڰ� raw ������ �ƴմϴ�.");
			return;
		}
		FILE *fp;
		fopen_s(&fp, dlg.GetFileName(), "rb");
		if (RawDlg.DoModal() == IDOK) {
			nHeight_Dst = RawDlg.GetRawHeight();
			nWidth_Dst = RawDlg.GetRawWidth();
			if (nHeight_Dst == 0 || nWidth_Dst == 0 || RawDlg.GetRawFormat() != 3) {
				MessageBox("�Է� ���� �߸��Ǿ����ϴ�.");
				return;
			}
			Edit_File_Dst.SetWindowTextA(dlg.GetFileTitle());
			Dst = MemAlloc2D(nHeight_Ori, nWidth_Ori, 0);
			for (int h = 0; h < nHeight_Dst; h++) {
				fread(Ori[h], sizeof(unsigned char), nWidth_Dst, fp);
			}
		}
		fclose(fp);
	}
}

/*
void CPSNRDlg::OnBnClickedGetpsnrbutton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Ori == NULL || Dst == NULL) {
		MessageBox("���� ����� �� ������ �ҷ����� �ʾҽ��ϴ�.");
		return;
	}
	if (nHeight_Ori != nHeight_Dst || nWidth_Ori != nWidth_Dst) {
		MessageBox("���� ����� �� ������ ũ�Ⱑ �ٸ��ϴ�.");
		return;
	}
	CString strTemp;
	double dPSNR = GetPSNR(Ori, Dst, nHeight_Ori, nWidth_Ori);
	strTemp.Format(_T("%f"), dPSNR);
	Edit_PSNR.SetWindowTextA(strTemp);
}
*/

void CPSNRDlg::OnBnClickedPsnrclose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Ori != NULL) {
		MemFree2D(Ori, nHeight_Ori);
	}
	if (Dst != NULL) {
		MemFree2D(Dst, nWidth_Dst);
	}
	CDialog::OnCancel();
}
double CPSNRDlg::GetPSNR(unsigned char** Ori, unsigned char** Dst, int nHeight, int nWidth) {

}

void CPSNRDlg::OnBnClickedPsnrbutton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
