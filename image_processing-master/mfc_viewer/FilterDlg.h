#pragma once


// CFilterDlg ��ȭ �����Դϴ�.

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFilterDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif
private:
	CStatic m_EditFilterSize;
	CStatic m_EditSigma;

	int nFilterSize;
	int nFilterType;
	double dSigma;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_Filtertype;
	afx_msg void OnBnClickedOkbutton4();
	afx_msg void OnBnClickedCancelbutton4();
	afx_msg void OnClickedRadio(UINT msg);

	int GetFilterSize();
	int GetFilterType();
	double GetSigma();
};
