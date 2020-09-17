
// ViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define M_E        2.71828182845904523536
#define M_PI 3.14159265358979323846
#include <cmath>
using namespace std;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
void DisplayHistogram(int* Hist);
unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal) {
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
}
void CViewerDlg::DisplayImage(Mat DispMat, bool bInOut) {
	if (bInOut) {
		GetDlgItem(IDC_PICTURE_IN)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicOri.GetDC();
		m_PicOri.GetClientRect(&rect);
	}
	else {
		GetDlgItem(IDC_PICTURE_OUT)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicProc.GetDC();
		m_PicProc.GetClientRect(&rect);
	}
	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;
	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}
void CViewerDlg::DisplayBitmap(CDC *pDC, CRect rect, IplImage *DispIplImage) {
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DispIplImage->width;
	bitmapInfo.bmiHeader.biHeight = DispIplImage->height;

	if (DispIplImage->nChannels == 3) {
		IplImage* ReverseImg = cvCreateImage(CvSize(DispIplImage->width, DispIplImage->height), 8, 3);
		for (int h = 0; h < DispIplImage->height; h++) {
			for (int w = 0; w < DispIplImage->width; w++) {
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w * 3];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 1] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 2] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w * 3 + 2];
			}
		}
		bitmapInfo.bmiHeader.biBitCount = DispIplImage->depth*DispIplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, DispIplImage->width, DispIplImage->height, ReverseImg->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		cvReleaseImage(&ReverseImg);
	}
	else {
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 1);
		for (int h = 0; h < DispIplImage->height; h++) {
			for (int w = 0; w < DispIplImage->width; w++) {
				ReverseImg->imageData[h*DispIplImage->widthStep + w] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w];
			}
		}
		IplImage *tmp = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);
		bitmapInfo.bmiHeader.biBitCount = tmp->depth*tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);
	}
}
void CViewerDlg::ResizeImage(IplImage *src, IplImage **dst, CRect *rect) {
	int ndst_width = 0, ndst_height = 0;
	if (src->width > src->height) {
		ndst_width = rect->Width();
		ndst_height = (src->height*rect->Width()) / src->width;
	}
	else {
		ndst_width = (src->width*rect->Height()) / src->height;
		ndst_height = rect->Height();
	}
	(*dst) = cvCreateImage(cvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);
	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConvertgrayBtnClick();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_CONVERT_CONVERTGRAY, &CAboutDlg::OnConvertgrayBtnClick)
END_MESSAGE_MAP()


// CViewerDlg 대화 상자



CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_IN, m_PicOri);
	DDX_Control(pDX, IDC_PICTURE_OUT, m_PicProc);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_FILEOPEN, &CViewerDlg::OnFileOpenBtnClick)
	ON_COMMAND(ID_FILE_FILECLOSE, &CViewerDlg::OnFileCloseBtnClick)
	ON_COMMAND(ID_CONVERT_CONVERTGRAY, &CViewerDlg::OnConvertConvertgray)
	ON_COMMAND(ID_EDIT_SCALING, &CViewerDlg::OnEditScalingClick)
	ON_COMMAND(ID_EDIT_ROTATION, &CViewerDlg::OnEditRotationClick)
	ON_COMMAND(ID_HISTOGRAM_GETHISTOGRAM, &CViewerDlg::OnHistogramGetClick)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &CViewerDlg::OnHistogramEqualizationClicked)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &CViewerDlg::OnHistogramMatchingClicked)
	ON_COMMAND(ID_EDIT_FILTER, &CViewerDlg::OnEditFilterClicked)
	ON_COMMAND(ID_NOISEREDUCTION_ADDNOISE, &CViewerDlg::OnNoisereductionAddnoiseClicked)
	ON_COMMAND(ID_NOISEFILTERING_MEANFILTER, &CViewerDlg::OnNoisefilteringMeanfilterClicked)
	ON_COMMAND(ID_NOISEFILTERING_ORDER, &CViewerDlg::OnNoisefilteringOrderClicked)
//	ON_BN_CLICKED(IDC_GETPSNRBUTTON, &CViewerDlg::OnBnClickedGetpsnrbutton)
	ON_COMMAND(ID_NOISEREDUCTION_GETPSNR, &CViewerDlg::OnGetpsnr)
END_MESSAGE_MAP()


// CViewerDlg 메시지 처리기

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CViewerDlg::OnFileOpenBtnClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE);
	if (isImageOpened) {
		if (ch_in_1 != NULL) {
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL) {
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}
	if (dlg.DoModal() == IDOK) {
		if (dlg.GetFileExt() != "jpg" && dlg.GetFileExt() != "JPG" && dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "yuv") {
			MessageBox("JPG 또는 YUV, Gray 파일이 아닙니다.");
			return;
		}
		InpFileName = dlg.GetFileTitle();
		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG") {
			Ori_Img = imread((const char*)dlg.GetPathName());
			nHeight_in = Ori_Img.rows;
			nWidth_in = Ori_Img.cols;
			if (Ori_Img.channels() == 3) {
				ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						ch_in_1[h][w] = RGB[2];
						ch_in_2[h][w] = RGB[1];
						ch_in_3[h][w] = RGB[0];
					}
				}
				nFormat = FORMAT_RGB;
			}
			else {
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						ch_in_gray[h][w] = Ori_Img.at<unsigned char>(h, w);
					}
					nFormat = FORMAT_GRAY;
				}
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
		else {
			CRawInfoDlg RawInfoDlg;
			if (RawInfoDlg.DoModal() == IDOK) {
				nHeight_in = RawInfoDlg.GetRawHeight();
				nWidth_in = RawInfoDlg.GetRawWidth();
				nFormat = RawInfoDlg.GetRawFormat();
				if (nHeight_in == 0 || nWidth_in == 0) {
					MessageBox("값을 잘못 입력하였습니다.");
					return;
				}
				FILE *fp_in;
				fopen_s(&fp_in, (const char*)dlg.GetPathName(), "rb");
				if (nFormat == FORMAT_GRAY) {
					ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_gray[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
						}
					}
				}
				else if (nFormat == FORMAT_YUV444) {
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_1[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_2[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_3[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC3);
					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Vec3b BGR;
							BGR[2] = static_cast<unsigned char>(ch_in_1[h][w] + 1.402*(ch_in_3[h][w]));
							BGR[1] = static_cast<unsigned char>(ch_in_1[h][w] - 0.344*(ch_in_2[h][w]) - 0.714*(ch_in_3[h][w]));
							BGR[0] = static_cast<unsigned char>(ch_in_1[h][w] + 1.772*(ch_in_2[h][w]));
							Ori_Img.at<Vec3b>(h, w) = BGR;
						}
					}
				}
				else {
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					ch_in_3 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_1[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in / 2; h++) {
						fread(ch_in_2[h], sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
					for (int h = 0; h < nHeight_in / 2; h++) {
						fread(ch_in_3[h], sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC3);
					for (int h = 0; h < nHeight_in / 2; h++) {
						for (int w = 0; w < nWidth_in / 2; w++) {
							for (int y = 0; y < 2; y++) {
								for (int x = 0; x < 2; x++) {
									Vec3b BGR;
									BGR[2] = static_cast<unsigned char>(ch_in_1[h * 2 + y][w * 2 + x] + 1.402*(ch_in_3[h][w]));
									BGR[1] = static_cast<unsigned char>(ch_in_1[h * 2 + y][w * 2 + x] - 0.344*(ch_in_2[h][w]) - 0.714*(ch_in_3[h][w]));
									BGR[0] = static_cast<unsigned char>(ch_in_1[h * 2 + y][w * 2 + x] + 1.772*(ch_in_2[h][w]));
									Ori_Img.at<Vec3b>(h * 2 + y, w * 2 + x) = BGR;
								}
							}
						}
					}
				}
				fclose(fp_in);
				isImageOpened = true;
				DisplayImage(Ori_Img, true);
			}
		}
	}
}


void CViewerDlg::OnFileCloseBtnClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened) {
		if (ch_in_1 != NULL) {
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL) {
			if (nFormat == FORMAT_YUV420) {
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL) {
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

void CViewerDlg::OnConvertConvertgray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened) {
		if (nFormat == FORMAT_GRAY) {
			MessageBox("입력 영상이 회색조 영상입니다.");
			return;
		}
		if (nFormat == FORMAT_YUV444 || nFormat == FORMAT_YUV420) {
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = ch_in_1[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			return;
		}
		if (nFormat == FORMAT_RGB) {
			if (ch_in_gray == NULL) {
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
			}
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					ch_in_gray[h][w] = saturate_cast<unsigned char>(0.299*ch_in_1[h][w] + 0.587*ch_in_2[h][w] + 0.114*ch_in_3[h][w]);
				}
			}
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			return;
		}
	}
	else {
		MessageBox("불러온 영상이 없습니다.");
	}
}
void CAboutDlg::OnConvertgrayBtnClick() {

}

void CViewerDlg::OnEditScalingClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (ImgScalingDlg.DoModal() == IDOK) {
			int nInterpolationType = ImgScalingDlg.GetInterpType();
			double dScaleRate = ImgScalingDlg.GetScaleRate();
			if (dScaleRate < 1 || dScaleRate >2) {
				MessageBox("스케일링 범위가 잘못되었습니다.");
				return;
			}
			int nHeight_Scale = nHeight_in*dScaleRate;
			int nWidth_Scale = nWidth_in*dScaleRate;

			unsigned char** Out = MemAlloc2D(nHeight_Scale, nWidth_Scale, 0);
			for (int h = 0; h < nHeight_Scale; h++) {
			
				for (int w = 0; w < nWidth_Scale; w++) {
					double h_Ori = h / dScaleRate;
					double w_Ori = w / dScaleRate;
					if (isInsideBoundary(nHeight_Scale, nWidth_Scale, h_Ori, w_Ori)) {
						if (nInterpolationType == BILINEAR) {
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
						}
						else {
							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
						}
					}
				}
			}
			Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			Proc_Img = Mat(nHeight_Scale, nWidth_Scale, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}
			for (int h = 0; h < nHeight_Scale; h++) {
				for (int w = 0; w < nWidth_Scale; w++) {
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}
			imshow("Original Image", Ori_Img);
			imshow("Scaled Image", Proc_Img);
			waitKey(0);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}
bool CViewerDlg::isInsideBoundary(int nHeight, int nWidth, double h, double w) {
	if (h >= 0 && w >= 0 && h < nHeight && w < nWidth) {
		return true;
	}
	else return false;
}
unsigned char CViewerDlg::BilinearInterpolation(unsigned char **In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt) {
	unsigned char result;		//변환된 좌표에있는 픽셀값
	
	int h = ceil(h_Cvt);
	int w = ceil(w_Cvt);
	
	//먼저, 경계선 픽셀값 설정
	if (h_Cvt == 0 && w_Cvt == 0) {
		result = In[0][0];
	}
	else if (h_Cvt == 0 && w_Cvt < nWidth_Ori - 1) {
		result = (w - w_Cvt)*In[0][w - 1] + (1 + w_Cvt - w)*In[0][w];
	}
	else if (h_Cvt == 0 && w_Cvt >= nWidth_Ori - 1) {
		result = In[0][nWidth_Ori - 1];
	}
	else if (w_Cvt == 0 && h_Cvt < nHeight_Ori - 1) {
		result = (h - h_Cvt)*In[h - 1][0] + (1 + h_Cvt - h)*In[h][0];
	}
	else if (w_Cvt == 0 && h_Cvt >= nHeight_Ori - 1) {
		result = In[nHeight_Ori - 1][0];
	}

	else if (w_Cvt >= nWidth_Ori - 1 && h_Cvt < nHeight_Ori - 1) {
		result = (h - h_Cvt)*In[h - 1][nWidth_Ori - 1] + (1 + h_Cvt - h)*In[h][nWidth_Ori - 1];
	}
	else if (h_Cvt >= nHeight_Ori - 1 && w_Cvt < nWidth_Ori - 1) {
		result = (w - w_Cvt)*In[nHeight_Ori - 1][w - 1] + (1 + w_Cvt - w)*In[nHeight_Ori - 1][w];
	}
	else if (h_Cvt >= nHeight_Ori - 1 && w_Cvt >= nWidth_Ori - 1) {
		result = In[nHeight_Ori - 1][nWidth_Ori - 1];
	}

	//이제 가운데를 채워넣는다.
	else {
		unsigned char p1 = (w - w_Cvt)*In[h - 1][w - 1] + (1 + w_Cvt - w)*In[h - 1][w];
		unsigned char p2 = (w - w_Cvt)*In[h][w - 1] + (1 + w_Cvt - w)*In[h][w];
		result = (h - h_Cvt)*p1 + (1 + h_Cvt - h)*p2;
	}
	
	return result;
}
unsigned char CViewerDlg::BicubicInterpolation(unsigned char **In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt) {
	unsigned char result = 0;
	int h = ceil(h_Cvt);
	int w = ceil(w_Cvt);

	double a[4] = { 0,0,0,0 };
	double matrix[4][4] = { {-0.166667, 0.5, -0.5, 0.166667},
	{0.5,-1,0.5,0},
	{-0.333333, -0.5, 1, -0.166667},
	{0,1,0,0} };	//연산할 행렬
	
	if (h_Cvt == 0 && w_Cvt == 0) {
		result = In[0][0];
	}
	else if (h_Cvt == 0 && w_Cvt <= 1) {
		result = In[0][w];
	}
	else if (h_Cvt == 0 && w_Cvt > 1 && w_Cvt < nWidth_Ori - 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				
				a[i] += matrix[i][j] * In[0][w - 2 + j];
			}
		}
		for (int i = 3; i >= 0; i--) {
			result += pow(1 + w_Cvt - w, i)*a[3 - i];
		}
	}

	else if (w_Cvt == 0 && h_Cvt <= 1) {
		result = In[h][0];
	}
	else if (w_Cvt == 0 && h_Cvt > 1 && h_Cvt < nHeight_Ori - 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				a[i] += matrix[i][j] * In[h - 2 + j][0];
			}
		}
		for (int i = 3; i >= 0; i--) {
			result += pow(1 + h_Cvt - h, i)*a[3 - i];
		}
	}
	else if (w_Cvt >= nWidth_Ori - 2 && h_Cvt < nHeight_Ori - 2) {
		result = In[h][nWidth_Ori - 1];
	}
	else if (h_Cvt >= nHeight_Ori - 2 && w_Cvt < nWidth_Ori - 2) {
		result = In[nHeight_Ori - 1][w];
	}
	else if(h_Cvt >= nHeight_Ori -2 && w_Cvt >= nWidth_Ori-2) {
		result = In[nHeight_Ori - 1][nWidth_Ori - 1];
	}
	else if (h_Cvt <= 1 || w_Cvt <= 1) {
		result = In[h][w];
	}
	//이젠 내부
	else {
		double x[4] = { 0,0,0,0 };
		for (int index_x = 0; index_x < 4; index_x++) {
			double a_2[4] = { 0,0,0,0 };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					a_2[i] += matrix[i][j] * In[h - 2 + index_x][w - 2 + j];
				}
			}
			for (int i = 3; i >= 0; i--) {
				x[index_x] += pow(1 + w_Cvt - w, i)*a_2[3 - i];
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				a[i] += matrix[i][j] * x[j];
			}
		}
		for (int i = 3; i >= 0; i--) {
			result += pow(1 + h_Cvt - h, i)*a[3 - i];
		}
	}
	
	return result;
}

void CViewerDlg::OnEditRotationClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (RotationDlg.DoModal() == IDOK) {
			int nInterpolationType = RotationDlg.GetInterpolationType();
			int nAngle = RotationDlg.GetAngle();
			if (nAngle < 0 || nAngle> 360) {
				MessageBox("회전 범위가 잘못되었습니다.");
				return;
			}
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
			int nHeight_2 = nHeight_in / 2;
			int nWidth_2 = nWidth_in / 2;

			double h_Rotate = 0;
			double w_Rotate = 0;
			double ang = nAngle*(M_PI / 180);

			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {

					//좌표계산한뒤,
					h_Rotate = nHeight_2 + cos(ang)*(h - nHeight_2) - sin(ang)*(w - nWidth_2);
					w_Rotate = nWidth_2 + sin(ang)*(h - nHeight_2) + cos(ang)*(w - nWidth_2);
					///////
					if (isInsideBoundary(nHeight_in, nWidth_in, h_Rotate, w_Rotate)) {
						if (nInterpolationType == BILINEAR) {
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
						else {
							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
					}
				}
			}
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			MemFree2D(Out, nHeight_in);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}


void CViewerDlg::OnHistogramGetClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (Hist == NULL) {
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}
		int nMax = 0;
		for (int n = 0; n < 256; n++) {
			if (nMax < Hist[n]) {
				nMax = Hist[n];
			}
		}
		double dNormalizeFactor = 255.0 / nMax;
		Mat HistDisp = Mat(256, 256, CV_8UC1);
		HistDisp = Scalar::all(0);
		for (int w = 0; w < 256; w++) {
			int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
			for (int h = 255; h > 255 - nNormalizedValue; h--) {
				HistDisp.at<unsigned char>(h, w) = 255;
			}
		}
		imshow("Hitogram of Input Image", HistDisp);
		waitKey(0);
	}
}


void CViewerDlg::OnHistogramEqualizationClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (Hist == NULL) {
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}
		double Hist_CDF[256] = { 0.0 };
		int out[256] = { 0.0 };
		Mat Eq_Img = Mat(256, 256, CV_8UC1);

		Hist_CDF[0] = (double)Hist[0] / (nHeight_in*nWidth_in);
		//out[(int)round(Hist_CDF[0] * 255)] = Hist[0];
		for (int i = 1; i < 256; i++) {
			Hist_CDF[i] = (double)Hist[i] / (nHeight_in*nWidth_in) + Hist_CDF[i - 1];
		//	out[(int)round(Hist_CDF[i] * 255)] = Hist[i];
		}
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Eq_Img.at<unsigned char>(h, w) = (int)round(Hist_CDF[ch_in_gray[h][w]]*255);
			}
		}
		//Displat Result
		int* Hist_Eq = new int[256];
		memset(Hist_Eq, 0, sizeof(int) * 256);
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist_Eq[Eq_Img.at<unsigned char>(h, w)]++;
			}
		}
		DisplayImage(Eq_Img, false);
		DisplayHistogram(Hist);
		DisplayHistogram(Hist_Eq);

		delete[] Hist_Eq;
	}
}
void DisplayHistogram(int* Hist) {
	int nMax = 0;
	for (int n = 0; n < 256; n++) {
		if (nMax < Hist[n]) {
			nMax = Hist[n];
		}
	}
	double dNormalizeFactor = 255.0 / nMax;
	Mat HistDisp = Mat(256, 256, CV_8UC1);
	HistDisp = Scalar::all(0);

	for (int w = 0; w < 256; w++) {
		int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
		for (int h = 255; h > 255 - nNormalizedValue; h--) {
			HistDisp.at<unsigned char>(h, w) = 255;
		}
	}
	imshow("Histogram", HistDisp);
	waitKey(0);
}

void CViewerDlg::OnHistogramMatchingClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (Hist == NULL) {
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}
		Mat Matching(256, 256, CV_8UC1), RefImg(nHeight_in, nWidth_in, CV_8UC1);
		int Hist_Ref[256] = { 0 };
		int Hist_Mat[256] = { 0 };
		float Hist_CDF[256], Hist_CDF_Ref[256];
		Matching = Scalar::all(0);
		RefImg = Scalar::all(0);
		srand(GetTickCount());
		for (int h = 0; h < RefImg.rows; h++) {
			for (int w = 0; w < RefImg.cols; w++) {
				RefImg.at<unsigned char>(h, w) = (unsigned char)rand() % 255;
				Hist_Ref[RefImg.at<unsigned char>(h, w)]++;
			}
		}
		Hist_CDF[0] = (double)Hist[0] / (nHeight_in*nWidth_in);
		Hist_CDF_Ref[0] = (double)Hist_Ref[0] / (nHeight_in*nWidth_in);
		for (int n = 1; n < 256; n++) {
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in*nWidth_in) + Hist_CDF[n - 1];
			Hist_CDF_Ref[n] = (double)Hist_Ref[n] / (nHeight_in*nWidth_in) + Hist_CDF_Ref[n - 1];
		}
		for (int h = 0; h < Matching.rows; h++) {
			for (int w = 0; w < Matching.cols; w++) {
				for (int i = 0; i < 256; i++) {
					if (Hist_CDF[ch_in_gray[h][w]] == Hist_CDF_Ref[i]) {
						Matching.at<unsigned char>(h, w) = i;
						Hist_Mat[i]++;
						break;
					}
					else if ((Hist_CDF[ch_in_gray[h][w]] >= Hist_CDF_Ref[i - 1] && Hist_CDF[ch_in_gray[h][w]] <= Hist_CDF_Ref[i + 1])) {
						Matching.at<unsigned char>(h, w) = i;
						Hist_Mat[i]++;
						break;
					}
				}
			}
		}
		DisplayImage(Matching, false);
		DisplayHistogram(Hist_Ref);
		DisplayHistogram(Hist_Mat);
	}
}

unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);
	
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}
void CViewerDlg::OnEditFilterClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		CFilterDlg FilterDlg;
		unsigned char** Pad, **Out;
		Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
		if (FilterDlg.DoModal() == IDOK) {
			int nFilterType = FilterDlg.GetFilterType();
			int nFilterSize = FilterDlg.GetFilterSize();
			double dSigma = FilterDlg.GetSigma();
			switch (nFilterType) {
			case FILTER_AVERAGE: {
				if (nFilterSize >= 3) {
					if (nFilterSize % 2 == 0) {
						nFilterSize++;
					}
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					AverageFilter(Pad, Out, nHeight_in, nWidth_in, nFilterSize);
					MemFree2D(Pad, nHeight_in + (int)(nFilterSize / 2) * 2);
				}
				else MessageBox("Filter Size가 잘못되었습니다. Filter Size는 3 이상의 정수값을 가져야 합니다.");
				break;
			}
			case FILTER_SMOOTH: {
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				SmoothFilter(Pad, Out, nHeight_in, nWidth_in);
				MemFree2D(Pad, nHeight_in + 2);
				break;
			}
			case FILTER_SHARPEN: {
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				SharpenFilter(Pad, Out, nHeight_in, nWidth_in);
				MemFree2D(Pad, nHeight_in + 2);
				break;
			}
			case FILTER_GAUSSIAN: {
				if (nFilterSize >= 3) {
					if (nFilterSize % 2 == 0) {
						nFilterSize++;
					}
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					GaussianFilter(Pad, Out, nHeight_in, nWidth_in, nFilterSize, dSigma);
					MemFree2D(Pad, nHeight_in + (int)(nFilterSize / 2) + 2);
				}
				else MessageBox("Filter Size가 잘못되었습니다. Filter size는 3 이상의 정수값을 가져야 합니다.");
				break;
			}
			}
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			MemFree2D(Out, nHeight_in);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상포맷이 회색조 영상이 아닙니다.");
	}
}
void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int pad_size = (nFilterSize / 2);
	double **AverageF = new double*[nFilterSize];
	for (int i = 0; i < nFilterSize; i++) {
		AverageF[i] = new double[nFilterSize];
		for (int j = 0; j < nFilterSize; j++)
			AverageF[i][j] = (1.0) / (nFilterSize*nFilterSize);
	}
	
	for (int i = pad_size; i < nHeight + pad_size; i++) {
		for (int j = pad_size; j < nWidth + pad_size; j++) {
			double ftemp = 0;
			for (int mask_x = -pad_size; mask_x <= pad_size; mask_x++) {
				for (int mask_y = -pad_size; mask_y <= pad_size; mask_y++) {
					ftemp += (In_Pad[i + mask_x][j + mask_y] * AverageF[mask_x + pad_size][mask_y + pad_size]);
				}
			}
			Out[i - pad_size][j - pad_size] = (unsigned char)ftemp;
		}
	}
}
void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	int pad_size = 1;
	double SmoothF[3][3] = { {1.0 / 16, 2.0 / 16, 1.0 / 16},
	{2.0 / 16, 4.0 / 16, 2.0 / 16},
	{1.0 / 16, 2.0 / 16, 1.0 / 16} };
	for (int i = pad_size; i < nHeight + pad_size; i++) {
		for (int j = pad_size; j < nWidth + pad_size; j++) {
			double ftemp = 0;
			for (int mask_x = -pad_size; mask_x <= pad_size; mask_x++) {
				for (int mask_y = -pad_size; mask_y <= pad_size; mask_y++) {
					ftemp += (In_Pad[i + mask_x][j + mask_y] * SmoothF[mask_x + pad_size][mask_y + pad_size]);
				}
			}
			Out[i - pad_size][j - pad_size] = (unsigned char)ftemp;
		}
	}
}
void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	int pad_size = 1;
	double SharpenF[3][3] =
	{ {0.0, -1.0, 0.0},
	{-1.0, 5.0, -1.0},
	{0.0, -1.0, 0.0} };
	for (int i = pad_size; i < nHeight + pad_size; i++) {
		for (int j = pad_size; j < nWidth + pad_size; j++) {
			double ftemp = 0;
			for (int mask_x = -pad_size; mask_x <= pad_size; mask_x++) {
				for (int mask_y = -pad_size; mask_y <= pad_size; mask_y++) {
					ftemp += (In_Pad[i + mask_x][j + mask_y] * SharpenF[mask_x + pad_size][mask_y + pad_size]);

				}
			}
			if (ftemp < 0)
				ftemp = 0;
			else if (ftemp > 255)
				ftemp = 255;
			Out[i - pad_size][j - pad_size] = (unsigned char)ftemp;
		}
	}
}
void GaussianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, double dSigma) {
	int pad_size = (nFilterSize / 2);
	double sum = 0;
	double **GaussianF = new double*[nFilterSize];
	for (int i = 0; i < nFilterSize; i++) {
		GaussianF[i] = new double[nFilterSize];
		int h = i-pad_size;
		for (int j = 0; j < nFilterSize; j++) {
			int w = j-pad_size;
			GaussianF[i][j] = pow(M_E, -(pow(h, 2) + pow(w, 2)) / (2 * pow(dSigma, 2))) / (dSigma*sqrt(2 * M_PI));
			sum += GaussianF[i][j];
		}
	}
	for (int i = 0; i < nFilterSize; i++) {
		for (int j = 0; j < nFilterSize; j++) {
			GaussianF[i][j] /= sum;
		}
	}
	for (int i = pad_size; i < nHeight + pad_size; i++) {
		for (int j = pad_size; j < nWidth + pad_size; j++) {
			double ftemp = 0;
			for (int mask_x = -pad_size; mask_x <= pad_size; mask_x++) {
				for (int mask_y = -pad_size; mask_y <= pad_size; mask_y++) {
					ftemp += (In_Pad[i + mask_x][j + mask_y] * GaussianF[mask_x + pad_size][mask_y + pad_size]);
				}
			}
			if (ftemp < 0)
				ftemp = 0;
			else if (ftemp > 255)
				ftemp = 255;
			Out[i - pad_size][j - pad_size] = (unsigned char)ftemp;
		}
	}

}
Mat GrayToMat(unsigned char** Img, int nHeight, int nWidth) {
	Mat rtn = Mat(nHeight, nWidth, CV_8UC1);
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			rtn.at<unsigned char>(h, w) = Img[h][w];
		}
	}
	return rtn;
}

void CViewerDlg::OnNoisereductionAddnoiseClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAddNoiseDlg AddNoiseDlg;
	BYTE** NoiseImg = MemAlloc2D(nHeight_in, nWidth_in, 0);
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (AddNoiseDlg.DoModal() == IDOK) {
			CString OutFileName = InpFileName;
			float Value_1 = AddNoiseDlg.GetVal_1();
			float Value_2 = AddNoiseDlg.GetVal_2();
			int nNoiseType = AddNoiseDlg.GetNoiseType();
			if (nNoiseType == NOISE_GN) {
				if (Value_2 < 1) {
					MessageBox("표준편차 값이 잘못되었습니다. 표준편차 값은 1 이상이여야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputGaussianNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "_GN.raw";
			}
			else {
				if (Value_1 <= 0 && Value_2 <= 0) {
					MessageBox("두개의 확률 값 중 적어도 하나는 0보다 커야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputSaltPepperNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "SPN.raw";
			}
			FILE *fp;
			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(NoiseImg[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
			MemFree2D(NoiseImg, nHeight_in);
		}
	}
}
void InputGaussianNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fMean, float fStdev) {
	float fTemp = 0, fPDF[256] = { 0.0f };
	GetGaussianPDF(fPDF, 256, fMean, fStdev);
	srand(GetTickCount());
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			fTemp = (float)In[h][w] + GetNoise(fPDF, 256);
			Out[h][w] = static_cast<unsigned char>(fTemp);
		}
	}
}
void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev) {
	int n;
	int Center = nLength / 2;
	float x;
	for (n = 0; n < nLength; n++) {
		x = (float)(n - Center);
		EmptyPDF[n] = (1 / ((float)sqrt(2 * M_PI)*fStDev))*exp(-pow(x - fMean, 2) / (2 * fStDev*fStDev));
	}
}
float GetNoise(float* PDF, int nLength) {
	int n;
	int Center = nLength / 2;
	float fRand, fComp, fTemp = 0;
	float x = 0, fDiff;
	float* CDF = new float[nLength];
	CDF[0] = 0;
	fRand = (float)rand() / (RAND_MAX + 1);
	for (n = 1; n < nLength; n++) {
		CDF[n] = (PDF[n] + PDF[n - 1]) / 2 + CDF[n - 1];
		fDiff = fRand - CDF[n];
		if (fDiff < 0) {
			x = ((float)n - Center);
			break;
		}
	}
	fComp = (fRand - CDF[n - 1]) / (CDF[n] - CDF[n - 1]);
	delete[] CDF;
	return x + fComp;
}
void InputSaltPepperNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fSProb, float fPProb) {
	float Low = fSProb;
	float High = 1.0f - fPProb;
	float fRand;
	srand(GetTickCount());
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			fRand = ((float)rand() / RAND_MAX);
			if (fRand < Low) {
				Out[h][w] = 255;
			}
			else if (fRand > High) {
				Out[h][w] = 0;
			}
			else
				Out[h][w] = In[h][w];
		}
	}
}

void CViewerDlg::OnNoisefilteringMeanfilterClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMeanFilterDlg MFDlg;
	if (isImageOpened == true && nFormat == FORMAT_GRAY) {
		if (MFDlg.DoModal() == IDOK) {
			int nFilterSize = MFDlg.GetFilterSize();
			int nFilterType = MFDlg.GetMeanFilterType();
			if(nFilterSize < 3) {
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}
			if (nFilterSize % 2 == 0) {
				nFilterSize++;
			}
			CString OutFileName = InpFileName;
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
			switch (nFilterType) {
			case NF_ARITHMETIC:
				ArithmeticMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_AMF.raw";
				break;
			case NF_GEOMETRIC:
				GeometricMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_GMF.raw";
				break;
			case NF_HARMONIC:
				HarmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_HMF.raw";
				break;
			case NF_CONTRAHARMONIC:
				int Q = MFDlg.GetQ();
				ContraharmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, Q);
				OutFileName += "_CHMF.raw";
				break;
			}
			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);
			FILE *fp;
			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
		}
	}
}
void ArithmeticMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int nTemp, nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp = 0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					nTemp += Img_Pad[h + m][w + n];
				}
			}
			Out[h][w] = static_cast<unsigned char>(nTemp / (nFilterSize*nFilterSize));
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void GeometricMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	double nTemp;
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp = 1.0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					nTemp *= Img_Pad[h + m][w + n];
				}
			}
			
			Out[h][w] = static_cast<unsigned char>(pow(nTemp, 1.0 / (nFilterSize*nFilterSize)));
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void HarmonicMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	double nTemp;
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp = 0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					nTemp += 1.0 / Img_Pad[h + m][w + n];
				}
			}
			Out[h][w] = static_cast<unsigned char>((nFilterSize*nFilterSize) / nTemp);
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void ContraharmonicMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, int Q) {
	int nTemp1, nTemp2, result;
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp1 = 0;
			nTemp2 = 0;
			result = 0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					nTemp1 += pow(Img_Pad[h + m][w + n], Q + 1);
					nTemp2 += pow(Img_Pad[h + m][w + n], Q);
				}
			}
			result = nTemp1 / nTemp2;
			if (result > 255)
				result = 255;
			else if (result < 0)
				result = 0;
			Out[h][w] = static_cast<unsigned char>(result);
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}

void CViewerDlg::OnNoisefilteringOrderClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COrderStatisticDlg OSFDlg;
	if (isImageOpened == true && nFormat == FORMAT_GRAY) {
		if (OSFDlg.DoModal() == IDOK) {
			int nFilterSize = OSFDlg.GetFilterSize1();
			int nFilterType = OSFDlg.GetOSFType();
			if (nFilterSize < 3) {
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}
			if (nFilterSize % 2 == 0) {
				nFilterSize++;
			}
			CString OutFileName = InpFileName;
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
			switch (nFilterSize) {
			case NF_MEDIAN:
				//MedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_MEDF.raw";
				break;
			case NF_ADAPTIVEMEDIAN:
				int MaxFilterSize = OSFDlg.GetFilterSize2();
				//AdaptiveMedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, MaxFilterSize);
				OutFileName += "_AMEDF.raw";
				break;
			}
			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);
			FILE* fp;
			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
		}
	}
}

void CViewerDlg::OnGetpsnr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPSNRDlg PSNRDlg;
	PSNRDlg.DoModal();
}
