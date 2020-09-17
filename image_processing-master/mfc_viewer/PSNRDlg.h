#pragma once
#include "RawInfoDlg.h"
#include "ViewerDlg.h"

// CPSNRDlg 대화 상자입니다.
/*unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal) {
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++) {
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char)*nWidth);
	}
	return rtn;
}
void MemFree2D(unsigned char** Mem, int nHeight) {
	for (int n = 0; n < nHeight; n++) {
		delete[] Mem[n];
	}
	delete[] Mem;
}*/
class CPSNRDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPSNRDlg)
private:
	unsigned char** Ori, **Dst;
	int nHeight_Ori, nWidth_Ori;
	int nHeight_Dst, nWidth_Dst;

	CEdit Edit_File_Ori, Edit_File_Dst, Edit_PSNR;
	double GetPSNR(unsigned char** Ori, unsigned char** Dst, int nHeight, int nWidth);
public:
	CPSNRDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPSNRDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PSNR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOributton();
	afx_msg void OnBnClickedRestbutton();
	//afx_msg void OnBnClickedGetpsnrbutton();
	afx_msg void OnBnClickedPsnrclose();
	afx_msg void OnBnClickedPsnrbutton();
};
