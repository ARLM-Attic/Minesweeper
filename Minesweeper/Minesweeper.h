/**
 *	�ļ���Minesweeper.h
 *	Minesweeperͷ�ļ�
 *
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	���˼���¦����ʦ
 *	2015-11-28
 */


#pragma once
#include "resource.h"


//*****************************************************************************
/*Basic Flag*/

//Ĭ�������ļ�
#define PROFILE				TEXT(".\\Minesweeper.ini")

//Ĭ������������������
#define DEFAULT_ROW			9
#define DEFAULT_COL			9
#define DEFAULT_COUNT		10

//���������Ͻ�����x,y
//����-1��ʾ��Ļ����
#define DEFAULT_WNDX		-1
#define DEFAULT_WNDY		-1

//�Ŵ��� = wndScale / DEFAULT_WNDSCALE
//�Ŵ�������,Ĭ����ͼ����
#define DEFAULT_WNDSCALE	18

//��ʱ����ʼֵ
#define DEFAULT_TIMECOUNT	0

//Ĭ�������ⱳ���������
#define DEFAULT_GAP_TOP		30
#define DEFAULT_GAP_BOTTOM	32
#define DEFAULT_GAP_LEFT	30
#define DEFAULT_GAP_RIGHT	29

//�����ڿ�
#define DEFAULT_XFRAME		((GetSystemMetrics(SM_CXFRAME) + 2) + GetSystemMetrics(SM_CXBORDER) * 2)
//�����ڸ� = DEFAULT_YFRAME + DEFAULT_YTOP
#define DEFAULT_YFRAME		((GetSystemMetrics(SM_CYFRAME) + 2) + GetSystemMetrics(SM_CYBORDER) * 2)
#define DEFAULT_YTOP		(GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU))

//Ĭ�ϴ���ֵ
#define DEFAULT_ERROR_INT	-2
#define DEFAULT_ERROR_STR	TEXT("")

//Ĭ����ͼ���
#define INITIAL_BMWDITH		18
#define INITIAL_BMHEIGHT	18

//��ʱ��ID
#define ID_TIMER			1
//*****************************************************************************


//*****************************************************************************
/*�������־
//0xabcd
//a		�ױ�־
//		0-8		��Χ����
//		C		��
//		F		δ��ʼ��
//bcd	���־
//b		
//		0-7		Minesweeperͼ��b+1��
//cd			Minesweeperͼ��cd+1��*/

//Minesweeperͼ��1��
#define MF_UNKNOWDOWN		0x0000
#define MF_UNKNOW			0x0001
#define MF_LIGHTUNKNOW		0x0002
#define MF_BRICKDOWN		0x0003
#define MF_BRICK			0x0004
#define MF_LIGHTBRICK		0x0005
#define MF_FLAGDOWN			0x0006
#define MF_FLAG				0x0007
#define MF_LIGHTFLAG		0x0008

//Minesweeperͼ��2��
#define MF_ZERO				0x0100
#define MF_NUMBER(N)		(MF_ZERO + N)
#define MF_CROSSNUMBER(N)	(MF_ZERO + 0x0009 + N)

#define MF_NOMINE			0x0200
#define MF_MINEUNKNOW		0x0300
#define MF_MINEBRICK		0x0400
#define MF_MINEFLAG			0x0500
#define MF_REDMINEUNKNOW	0x0600
#define MF_REDMINEBRICK		0x0700

//�ױ�־0xa000
#define MF_ZEROMINE			0x0000
#define MF_MINENUMBER(N)	(MF_ZEROMINE + N * 0x1000)
#define MF_MINE				0xC000
#define MF_NOINIT			0xF000
#define MF_INIT				(MF_NOINIT | MF_BRICK)
//*****************************************************************************


//*****************************************************************************
/*���öԻ����־
//0x00-0xFF*/
#define OF_DRAW				0x80
#define OF_SOUND			0x40
#define OF_PROM				0x20
#define OF_STARTATSAVE		0x10
#define OF_SAVE				0x08
#define OF_UNKNOW			0x04
#define OF_USERDEFINED		0x03
#define OF_SENIOR			0x02
#define OF_MEDIATE			0x01
#define OF_JUNIOR			0x00
#define DEFAULT_OPTIONFLAG	(OF_DRAW | OF_SOUND | OF_PROM | OF_UNKNOW | OF_JUNIOR)
//*****************************************************************************


//*****************************************************************************
/*��ӰЧ��rgbֵ�������㺯��
//win7ɨ���Ͻ�ͼ, ÿ����ȡһ�����Rֵ�����Gֵ�����Bֵ, Ȼ���������ֵ�ֱ������κ�����ά���*/
#define DR(x, y)			(213.1031 - 4.1249*x + 1.0402*y - 0.0397*x*x - 0.6143*y*y - 0.0707*x*y + 0.0023*x*x*x + 0.0166*y*y*y + 0.0007*x*x*y + 0.0077*x*y*y - 113.6045)
#define DG(x, y)			(234.0942 + 0.3211*x + 7.4699*y - 0.2486*x*x - 1.0551*y*y - 0.4910*x*y + 0.0048*x*x*x + 0.0241*y*y*y + 0.0070*x*x*y + 0.0175*x*y*y - 154.3656)
#define DB(x, y)			(239.0961 + 0.8147*x + 2.2898*y - 0.0085*x*x - 0.1965*y*y - 0.1034*x*y - 0.000063*x*x*x + 0.0024*y*y*y + 0.0008*x*x*y + 0.0025*x*y*y - 215.1669)
//*****************************************************************************


//*****************************************************************************
/*ȫ�ֱ���*/

//���״̬�ṹ��
struct tagMouseState {
	int mCol;		//λ����������
	int mRow;		//λ����������
	int mType;		/*��갴������
						WM_LBUTTONDOWN			�������
						WM_LBUTTONUP			�������
						WM_RBUTTONDOWN			�Ҽ�����
						WM_RBUTTONUP			�Ҽ�����
						WM_LBUTTONDBLCLK		˫�����¿�ʼ
						WM_RBUTTONDBLCLK		˫�����½���
						WM_MOUSEMOVE			����ƶ�*/
} mouseState;

//�����ⱳ�������߽ṹ��
struct tagGap {
	int bottom;		//�����߶�
	int left;		//��߿��
	int right;		//�ұ߿��
	int top;		//�ײ��߶�
} gap;

//bitmap���
//hBitmap			Minesweeper.bmp
//hBitmap_Frame		Frame.bmp
HBITMAP hBitmap, hBitmap_Frame;
BITMAP bitmap, bitmap_Frame;

LPVOID lpvBits;				//Minesweeperͼrgbֵ����
HBITMAP hBitmapT;			//����ͼ���
unsigned int *pBrick;		//����ͼrgbֵ����

int row;					//��������
int col;					//��������
int count;					//��������
int wndX;					//���������Ͻ�����x
int wndY;					//���������Ͻ�����y
int wndScale;				//����ͼ���� = �Ŵ��� * DEFAULT_WNDSCALE
int optionFlag;				//���öԻ����־

//�������
unsigned int seed;			//�������, ʱ���
int firstLButtonNo;			//��һ��������µĿ����
int timeCount;				//��ʱ��ʱ�����
TCHAR *saveFlags;			//����Ŀ���ͼ��Ϣ����

int brickNumber;			//��������
int wndWidth;				//�����ڿ�
int wndHeight;				//�����ڸ�

unsigned short int *mapInfo;//�������־����

int flagCount;				//�������
int brickCount;				//��ʼ�����

//������־
bool hasSave;				//�Ƿ���ڱ������Ϸ
bool startAtSave;			//�Ƿ��ɱ������Ϸ��ʼ
bool firstPaint;			//��ʼ��Ч���Ƿ��Ѳ���
bool firstLButtonDown;		//����Ƿ��ѵ�һ�ΰ��£���Ϸ�Ƿ��ѿ�ʼ
bool hasSpeed;				//Ч����ʱ�����Ƿ������
bool timeFlag;				//���Կ��Ƽ�ʱ��, �����Ƿ���С��

int tempOption[4];			//������ʱ�����������������������öԻ����־
//*****************************************************************************


/*
������״̬�ṹ��
������
	mXpos			����ڿͻ�����ˮƽ����
	mYpos			����ڿͻ����Ĵ�ֱ����
	mType			��갴������
����ֵ��
	mouseState		���״̬�ṹ��
*/
tagMouseState getMouseState(int mXpos, int mYpos, int mType);

/*
����¼�������
������
	hWnd			�����ھ��
	mouseState		���״̬�ṹ��
	nIDMessage		�Զ�������¼���Ϣ, ����ֱ����ϵͳ����Ϣ�������Զ�����Ϣ
					WM_LBUTTONDOWN			�������
					WM_LBUTTONUP			�������
					WM_RBUTTONDOWN			�Ҽ�����
					WM_RBUTTONUP			�Ҽ�����
					WM_LBUTTONDBLCLK		˫�����¿�ʼ
					WM_RBUTTONDBLCLK		˫�����½���
					WM_MOUSEMOVE			����ƶ�
											WM_MOUSEMOVE			˫����δ����
											WM_LBUTTONDOWN			�������
											WM_LBUTTONDBLCLK		˫������
����ֵ��
	true
*/
bool OnMouseClick(HWND hWnd, tagMouseState mouseState, int nIDMessage);


/*
��ʼ������ȫ�ֱ���, ʵ�ֳ�ʼ������Ч��
������
	tRow			����Ϸ��������
	tCol			����Ϸ��������
	tCount			����Ϸ��������
	uType			�Ƿ����¿�ʼ���̵���Ϸ
					0				����Ϸ
					1				���¿�ʼ���̵���Ϸ
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void InitInterface(int tRow, int tCol, int tCount, int uType, HWND hWnd, HDC hdc, HDC hdcMem);

/*
��ʼ���������־����
������
	tSeed			����Ϸ�������
	tFirstLButtonNo	����Ϸ��һ��������µĿ����
	saveFlags		����Ŀ���ͼ��Ϣ����
	hWnd			�����ھ��
	hdc				��ͼĿ����
�޷���ֵ
*/
void InitMapInfo(unsigned int tSeed, int tFirstLButtonNo, TCHAR *saveFlags, HWND hWnd, HDC hdc);


/*
ʵ�ֹ�ӰЧ����ͼ
������
	hdcDest			��ͼĿ����
	rowDest			Ŀ�������
	colDest			Ŀ�������
	hdcSrc			��ͼԴ���������Minesweeperͼ
	rowSrc			Դ�����
	colSrc			Դ�����
�޷���ֵ
*/
void LightShadow(HDC hdcDest, int rowDest, int colDest, HDC hdcSrc, int rowSrc, int colSrc);


/*
��ʾ��ʱ��
������
	timeCount		ʱ�����
	hdc				��ʾĿ����
�޷���ֵ
*/
void showTime(int timeCount, HDC hdc);

/*
��ʾ����
������
	flagCount		�������
	hdc				��ʾĿ����
�޷���ֵ
*/
void showMime(int flagCount, HDC hdc);

/*
�����Ϸʤ��
������
	hWnd			�����ھ��
	hdc				��ʾĿ����
�޷���ֵ
*/
void checkWin(HWND hWnd, HDC hdc);

/*
��ʾ��Ϸʧ��
������
	hWnd			�����ھ��
�޷���ֵ
*/
void showLose(HWND hWnd);


/*
�㵽����Ϊ��Ŀ�, �忪��Χ�Ŀ�
������
	rowDest			�㵽��������
	colDest			�㵽��������
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
����ֵ��
	1+number		1+�忪�Ŀ���
*/
int zeroDevelop(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem);

/*
�㵽��, ��ըЧ��
������
	rowDest			�㵽��������
	colDest			�㵽��������
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void mineBomb(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem);

/*
���Ƹ�������Ч����ͼ�����ʱ
������
	dwMilliseconds	Ч����ͼ��Ļ�����ʱ
	timeCount		��ʱ��������
	startNo			����������ʼ���
�޷���ֵ
*/
void speed(DWORD dwMilliseconds, int *timeCount, int startNo);


/*
�鰴��
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void buttonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
�Ÿ��鰴��
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void nineButtonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
�黹ԭ
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void buttonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
�Ÿ��黹ԭ
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void nineButtonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
��ˢ��, �ɰ���״̬��Ϊ���ֻ���
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
	redFlagUp		������ͼ�Ƿ����
					MF_LIGHTFLAG			����
					MF_FLAG					������
�޷���ֵ
*/
void buttonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem, int redFlagUp);

/*
�Ÿ���ˢ��
������
mouState		���״̬�ṹ��
hWnd			�����ھ��
hdc				��ͼĿ����
hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void nineButtonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
�����
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void buttonLight(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
��ı�
������
	mouState		���״̬�ṹ��
	hWnd			�����ھ��
	hdc				��ͼĿ����
	hdcMem			��ͼԴ���������Minesweeperͼ
�޷���ֵ
*/
void buttonChange(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);
