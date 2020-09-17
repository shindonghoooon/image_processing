#pragma once


// COrderStatisticDlg ��ȭ �����Դϴ�.

class COrderStatisticDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrderStatisticDlg)
private:
	int nFilterSize1;
	int nFilterSize2;
	int nOSFType;
	CEdit Edit_FilterSize1;
	CEdit Edit_FilterSize2;
public:
	COrderStatisticDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COrderStatisticDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATISTICDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_osf;
	afx_msg void OnBnClickedOkosfbutton();
	afx_msg void OnBnClickedCancelosfbutton();
	afx_msg void OnRadioClicked(UINT msg);

	int GetFilterSize1();
	int GetFilterSize2();
	int GetOSFType();
};
