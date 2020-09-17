#pragma once


// CMeanFilterDlg ��ȭ �����Դϴ�.

class CMeanFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMeanFilterDlg)
private:
	
	int nFilterSize;
	int nMeanFilterType;
	int Q;
	CEdit Edit_FilterSize;
	CEdit Edit_Q;

public:
	CMeanFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMeanFilterDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOkbutton6();
	afx_msg void OnBnClickedCancelbutton6();
	afx_msg void OnRadioClicked(UINT msg);
	int GetFilterSize();
	int GetMeanFilterType();
	int GetQ();
	int m_radio_mf;
};
