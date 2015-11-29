/**
 *	�ļ���Minesweeper.cpp
 *	MinesweeperԴ�ļ�
 *
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	���˼���¦����ʦ
 *	2015-11-28
 */


#include "stdafx.h"
#include "Minesweeper.h"
#include "core.h"
#include "io.h"
#include <time.h>
#include <MMSystem.h>

#pragma comment(lib, "WINMM.LIB")

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Option(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Statistic(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Win(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Lose(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINESWEEPER));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MINESWEEPER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//���öԻ��򴰿ڹ��̺���
INT_PTR CALLBACK Option(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static int tRow, tCol, tCount, tFlag;
	static int oldUserDefined[3], userDefined[3];//�Զ���ֵ
	static bool user = false;//�Ƿ�ѡ���Զ���
	static bool flag = true;//ĳ���༭���Ƿ��д
	HWND hStatic;

	switch (message)
	{
	case WM_INITDIALOG:

		//��ʼ����ѡ��ť
		if (optionFlag & OF_DRAW)
			CheckRadioButton(hDlg, IDC_CHECK1, IDC_CHECK1, IDC_CHECK1);
		if (optionFlag & OF_SOUND)
			CheckRadioButton(hDlg, IDC_CHECK2, IDC_CHECK2, IDC_CHECK2);
		if (optionFlag & OF_PROM)
			CheckRadioButton(hDlg, IDC_CHECK3, IDC_CHECK3, IDC_CHECK3);
		if (optionFlag & OF_STARTATSAVE)
			CheckRadioButton(hDlg, IDC_CHECK4, IDC_CHECK4, IDC_CHECK4);
		if (optionFlag & OF_SAVE)
			CheckRadioButton(hDlg, IDC_CHECK5, IDC_CHECK5, IDC_CHECK5);
		if (optionFlag & OF_UNKNOW)
			CheckRadioButton(hDlg, IDC_CHECK6, IDC_CHECK6, IDC_CHECK6);

		//��ʼ���Ѷȵ�ѡ��ť
		if ((optionFlag & 0x03) == OF_USERDEFINED)
		{
			user = true;
			CheckRadioButton(hDlg, IDC_USERDEFINED, IDC_USERDEFINED, IDC_USERDEFINED);
		}
		else if ((optionFlag & 0x03) == OF_SENIOR)
			CheckRadioButton(hDlg, IDC_SENIOR, IDC_SENIOR, IDC_SENIOR);
		else if ((optionFlag & 0x03) == OF_MEDIATE)
			CheckRadioButton(hDlg, IDC_MEDIATE, IDC_MEDIATE, IDC_MEDIATE);
		else if ((optionFlag & 0x03) == OF_JUNIOR)
			CheckRadioButton(hDlg, IDC_JUNIOR, IDC_JUNIOR, IDC_JUNIOR);

		//���Զ�������Զ�������
		if (!user)
		{
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
		}
		//��ȡ�Զ���ֵ����ʾ
		readProfileUserdefined(oldUserDefined);
		userDefined[0] = oldUserDefined[0];
		userDefined[1] = oldUserDefined[1];
		userDefined[2] = oldUserDefined[2];
		flag = false;
		SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, oldUserDefined[0], TRUE);
		flag = true;
		flag = false;
		SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, oldUserDefined[1], TRUE);
		flag = true;
		flag = false;
		SetDlgItemInt(hDlg, IDC_EDIT_COUNT, oldUserDefined[2], TRUE);
		flag = true;

		//�����ʼֵ
		tRow = row;
		tCol = col;
		tCount = count;
		tFlag = optionFlag;
		break;
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (user)//�Զ���
			{
				//��ȡ������Զ���ֵ
				tRow = GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, FALSE, TRUE);
				userDefined[0] = tRow;
				tCol = GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, FALSE, TRUE);
				userDefined[1] = tCol;
				tCount = GetDlgItemInt(hDlg, IDC_EDIT_COUNT, FALSE, TRUE);
				userDefined[2] = tCount;
				//��ȷ�Լ��
				if (tRow < 9 || tRow > 24 || tCol < 9 || tCol > 30 || tCount < 10 || tCount > 668)
				{
					if (tRow < 9 || tRow > 24)
					{
						tRow = 9;
						userDefined[0] = tRow;
						SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, tRow, TRUE);
					}
					if (tCol < 9 || tCol > 30)
					{
						tCol = 9;
						userDefined[1] = tCol;
						SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, tCol, TRUE);
					}
					if (tCount < 10 || tCount > 668)
					{
						tCount = 10;
						userDefined[2] = tCount;
						SetDlgItemInt(hDlg, IDC_EDIT_COUNT, tCount, TRUE);
					}
					break;
				}
				//����ĳЩֵ�����ñ�־Ϊ��Ӧ�ȼ�
				if (tRow == 9 && tCol == 9 && tCount == 10)
				{
					tFlag = (tFlag & 0xFC) + OF_JUNIOR;
					user = false;
				}
				else if (tRow == 16 && tCol == 16 && tCount == 40)
				{
					tFlag = (tFlag & 0xFC) + OF_MEDIATE;
					user = false;
				}
				else if (tRow == 16 && tCol == 30 && tCount == 99)
				{
					tFlag = (tFlag & 0xFC) + OF_SENIOR;
					user = false;
				}
			}
			//�Զ���ֵ���ʼ�Զ���ֵ����ͬ��д�������ļ�
			if (userDefined[0] != oldUserDefined[0] || userDefined[1] != oldUserDefined[1] || userDefined[2] != oldUserDefined[2])
				writeProfileUserdefined(userDefined);
			
			//�����������ʼ��ͬ
			if (row != tRow || col != tCol || count != tCount)
			{
				if (firstLButtonDown)
				{
					//������Ϣ��
					switch (MessageBox(hDlg, TEXT("��Щ���ò���Ӧ���ڽ����е���Ϸ!��ϣ����ʲô?\n\n�˳����������ÿ�ʼ����Ϸ(Y)\n����ͳ��ʱ��Ϊ��!\n\n������Ϸ(N)\n�����û�Ӧ������һ����Ϸ!"), TEXT("�Ѹ�����Ϸ����"), MB_YESNOCANCEL))
					{
					case IDYES://�������ÿ�ʼ����Ϸ
						row = tRow;
						col = tCol;
						count = tCount;
						if ((optionFlag & 0x03) < 3)
							changeStatistic((optionFlag & 0x03), -1);//��Ϊ��
						optionFlag = tFlag;
						EndDialog(hDlg, 1);//����1
						break;
					case IDNO://������Ӧ������һ����Ϸ
						tempOption[0] = tRow;
						tempOption[1] = tCol;
						tempOption[2] = tCount;
						tempOption[3] = tFlag;
						optionFlag = (tFlag & 0xFC) + (optionFlag & 0x03);
						EndDialog(hDlg, 2);//����2
						break;
					}
				}
				else//δ��ʼ��Ϸֱ��Ӧ��������
				{
					row = tRow;
					col = tCol;
					count = tCount;
					optionFlag = tFlag;
					EndDialog(hDlg, 1);//����1
				}
			}
			else
			{
				optionFlag = tFlag;
				EndDialog(hDlg, 0);
			}
			return (INT_PTR)TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return (INT_PTR)TRUE;
			break;
		case IDC_JUNIOR:
			user = false;
			//�����Զ�������
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
			//���ñ�־Ϊ��Ӧ�ȼ�
			tFlag = (tFlag & 0xFC) + OF_JUNIOR;
			tRow = 9;
			tCol = 9;
			tCount = 10;
			break;
		case IDC_MEDIATE:
			user = false;
			//�����Զ�������
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
			//���ñ�־Ϊ��Ӧ�ȼ�
			tFlag = (tFlag & 0xFC) + OF_MEDIATE;
			tRow = 16;
			tCol = 16;
			tCount = 40;
			break;
		case IDC_SENIOR:
			user = false;
			//�����Զ�������
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
			//���ñ�־Ϊ��Ӧ�ȼ�
			tFlag = (tFlag & 0xFC) + OF_SENIOR;
			tRow = 16;
			tCol = 30;
			tCount = 99;
			break;
		case IDC_USERDEFINED:
			user = true;
			//�����Զ�������
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 1);
			//���ñ�־Ϊ�Զ���
			tFlag = (tFlag & 0xFC) + OF_USERDEFINED;
			break;
		case IDC_CHECK1:
			tFlag ^= OF_DRAW;
			break;
		case IDC_CHECK2:
			tFlag ^= OF_SOUND;
			break;
		case IDC_CHECK3:
			tFlag ^= OF_PROM;
			break;
		case IDC_CHECK4:
			tFlag ^= OF_STARTATSAVE;
			break;
		case IDC_CHECK5:
			tFlag ^= OF_SAVE;
			break;
		case IDC_CHECK6:
			tFlag ^= OF_UNKNOW;
			break;
		case IDC_EDIT_HEIGHT:
			if (flag && user)
			{
				tRow = GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, FALSE, TRUE);
				userDefined[0] = tRow;
				//������������Զ���ֵ����ȷ��
				if (tCol < 9 || tCol > 30)
				{
					tCol = 9;
					userDefined[1] = tCol;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, tCol, TRUE);
					flag = true;
				}
				if (tCount < 10 || tCount > 668)
				{
					tCount = 10;
					userDefined[2] = tCount;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_COUNT, tCount, TRUE);
					flag = true;
				}
			}
			break;
		case IDC_EDIT_WIDTH:
			if (flag && user)
			{
				tCol = GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, FALSE, TRUE);
				userDefined[1] = tCol;
				//������������Զ���ֵ����ȷ��
				if (tRow < 9 || tRow > 24)
				{
					tRow = 9;
					userDefined[0] = tRow;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, tRow, TRUE);
					flag = true;
				}
				if (tCount < 10 || tCount > 668)
				{
					tCount = 10;
					userDefined[2] = tCount;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_COUNT, tCount, TRUE);
					flag = true;
				}
			}
			break;
		case IDC_EDIT_COUNT:
			if (flag && user)
			{
				tCount = GetDlgItemInt(hDlg, IDC_EDIT_COUNT, FALSE, TRUE);
				userDefined[2] = tCount;
				//������������Զ���ֵ����ȷ��
				if (tRow < 9 || tRow > 24)
				{
					tRow = 9;
					userDefined[0] = tRow;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, tRow, TRUE);
					flag = true;
				}
				if (tCol < 9 || tCol > 30)
				{
					tCol = 9;
					userDefined[1] = tCol;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, tCol, TRUE);
					flag = true;
				}
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	}
	return (INT_PTR)FALSE;
}

//ͳ����Ϣ�Ի��򴰿ڹ��̺���
INT_PTR CALLBACK Statistic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	int valueCount[6], bestTime[5], date[5];//ͳ����Ϣ���� gameCount, winCount, winRate, mostWin, mostLost, nowCount
	static int level;//�ȼ�
	static HWND hList;//�б���
	TCHAR *pLevel[3] = { TEXT("����"), TEXT("�м�"), TEXT("�߼�") };

	//���ڼ��������ʱ��
	tm pTime;
	time_t pTime_t;
	char timeArray[30];
	TCHAR value[30];
	TCHAR best[42];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:	
		level = (optionFlag & 0x03) % 3;
		
		//��ʼ���б�
		hList = GetDlgItem(hDlg, IDC_LIST);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pLevel[0]);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pLevel[1]);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pLevel[2]);
		SendMessage(hList, LB_SETCURSEL, level, 0);

		//��ʼ��ͳ����Ϣ
		readProfileStatistic(section[level], valueCount, bestTime, date);
		wsprintf(value, TEXT("%d%%"), valueCount[2]);
		SetDlgItemInt(hDlg, IDC_STATIC1, valueCount[0], TRUE);
		SetDlgItemInt(hDlg, IDC_STATIC2, valueCount[1], TRUE);
		SetDlgItemText(hDlg, IDC_STATIC3, value);
		SetDlgItemInt(hDlg, IDC_STATIC4, valueCount[3], TRUE);
		SetDlgItemInt(hDlg, IDC_STATIC5, valueCount[4], TRUE);
		SetDlgItemInt(hDlg, IDC_STATIC6, valueCount[5], TRUE);

		//��ʼ�����ʱ��
		for (int k = 0; bestTime[k] > 0; k++)
		{
			pTime_t = (time_t)date[k];
			localtime_s(&pTime, &pTime_t);
			asctime_s(timeArray, &pTime);

			ZeroMemory(best, 42);
			wsprintf(best, TEXT("%d\t"), bestTime[k] / 10);
			CharToTchar(timeArray, value);
			_tcscat_s(best, value);

			SetDlgItemText(hDlg, IDC_TIME1 + k, best);
		}
		return (INT_PTR)TRUE;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_RETURN:
			//����ͳ����Ϣ
			writeProfileStatistic(section[level], valueCount, bestTime, date, 0);

			//����ͳ����ϢΪ0
			SetDlgItemInt(hDlg, IDC_STATIC1, 0, TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC2, 0, TRUE);
			SetDlgItemText(hDlg, IDC_STATIC3, TEXT("0%"));
			SetDlgItemInt(hDlg, IDC_STATIC4, 0, TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC5, 0, TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC6, 0, TRUE);
			for (int i = 0; i < 5; i++)
				SetDlgItemText(hDlg, IDC_TIME1 + i, TEXT(""));
			return (INT_PTR)TRUE;
			break;
		case IDC_LIST:
			level = SendMessage(hList, LB_GETCURSEL, 0, 0);

			//��ȡ����ʾͳ����Ϣ
			readProfileStatistic(section[level], valueCount, bestTime, date);
			wsprintf(value, TEXT("%d%%"), valueCount[2]);
			SetDlgItemInt(hDlg, IDC_STATIC1, valueCount[0], TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC2, valueCount[1], TRUE);
			SetDlgItemText(hDlg, IDC_STATIC3, value);
			SetDlgItemInt(hDlg, IDC_STATIC4, valueCount[3], TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC5, valueCount[4], TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC6, valueCount[5], TRUE);

			for (int k = 0; k < 5; k++)
				SetDlgItemText(hDlg, IDC_TIME1 + k, TEXT(""));
			for (int k = 0; bestTime[k] > 0 && k < 5; k++)
			{
				pTime_t = (time_t)date[k];
				localtime_s(&pTime, &pTime_t);
				asctime_s(timeArray, &pTime);

				ZeroMemory(best, 42);
				wsprintf(best, TEXT("%d\t"), bestTime[k] / 10);
				CharToTchar(timeArray, value);
				_tcscat_s(best, value);

				SetDlgItemText(hDlg, IDC_TIME1 + k, best);
			}
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//Ӯ����Ϸ�Ի��򴰿ڹ��̺���
INT_PTR CALLBACK Win(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR* Static[] = { TEXT("�����õ�ʱ��������Ѷȼ�������ʱ��!"), TEXT("ʱ�䣺"), TEXT("���ʱ�䣺"), TEXT("��"), TEXT("���ڣ�"), TEXT("������Ϸ��"), TEXT("��ʤ��Ϸ��"), TEXT("�ٷֱȣ�") };
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	static int level;//�ȼ�
	int valueCount[6], bestTime[5], date[5];//ͳ����Ϣ����

	//���ڼ��������ʱ��
	TCHAR value[30], temp[30];
	tm pTime;
	time_t pTime_t;
	char timeArray[30];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if ((level = optionFlag & 0x03) < 3)//�����Զ���
		{
			//��ȡͳ����Ϣ
			readProfileStatistic(section[level], valueCount, bestTime, date);
			
			//�������ʱ����ʾ"�����õ�ʱ��������Ѷȼ�������ʱ��!"
			if (timeCount == bestTime[0] / 10)
				SetDlgItemText(hDlg, IDC_STATIC0, Static[0]);

			//���²���ʾ����ͳ����Ϣ
			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[1]);
			wsprintf(value, TEXT("%d"), timeCount);
			_tcscat_s(temp, value);
			_tcscat_s(temp, Static[3]);
			SetDlgItemText(hDlg, IDC_STATIC1, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[2]);
			wsprintf(value, TEXT("%d"), bestTime[0] / 10);
			_tcscat_s(temp, value);
			_tcscat_s(temp, Static[3]);
			SetDlgItemText(hDlg, IDC_STATIC2, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[4]);
			pTime_t = (time_t)date[0];
			localtime_s(&pTime, &pTime_t);
			asctime_s(timeArray, &pTime);
			CharToTchar(timeArray, value);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC3, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[5]);
			wsprintf(value, TEXT("%d"), valueCount[0]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC4, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[6]);
			wsprintf(value, TEXT("%d"), valueCount[1]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC5, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[7]);
			wsprintf(value, TEXT("%d%%"), valueCount[2]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC6, temp);

		}

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:case IDC_NEWGAME://����һ��
			EndDialog(hDlg, 2);//����2
			return (INT_PTR)TRUE;
			break;
		case IDC_EXIT://�˳�
			EndDialog(hDlg, 0);//����0
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//��Ϸʧ�ܶԻ��򴰿ڹ��̺���
INT_PTR CALLBACK Lose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR* Static[] = { TEXT("ʱ�䣺"), TEXT("��"), TEXT("������Ϸ��"), TEXT("��ʤ��Ϸ��"), TEXT("�ٷֱȣ�") };
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	static int level;//�ȼ�
	int valueCount[6], bestTime[5], date[5];//ͳ����Ϣ����
	TCHAR value[30], temp[30];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if ((level = optionFlag & 0x03) < 3)//�����Զ���
		{
			//��ȡͳ����Ϣ
			readProfileStatistic(section[level], valueCount, bestTime, date);

			//���²���ʾ����ͳ����Ϣ
			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[0]);
			wsprintf(value, TEXT("%d"), timeCount);
			_tcscat_s(temp, value);
			_tcscat_s(temp, Static[1]);
			SetDlgItemText(hDlg, IDC_STATIC1, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[2]);
			wsprintf(value, TEXT("%d"), valueCount[0]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC2, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[3]);
			wsprintf(value, TEXT("%d"), valueCount[1]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC3, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[4]);
			wsprintf(value, TEXT("%d%%"), valueCount[2]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC4, temp);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:case IDC_NEWGAME://����һ��
			EndDialog(hDlg, 2);//����2
			return (INT_PTR)TRUE;
			break;
		case IDC_RETURN://���¿�ʼ�����Ϸ
			EndDialog(hDlg, 1);//����1
			return (INT_PTR)TRUE;
			break;
		case IDC_EXIT://�˳�
			EndDialog(hDlg, 0);//����0
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//���ڶԻ��򴰿ڹ��̺���
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;			//��ͼĿ����
	HDC hdcMem;			//��ͼԴ���, ����Minesweeperͼ
	HDC hdcMem_Frame;	//��ͼԴ���, ����Frameͼ

	int m, n;
	TCHAR value[11];


	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case ID_NEWGAME://����Ϸ
			hdc = GetDC(hWnd);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);

			if (wmEvent == 0 || wmEvent == 1)
			{
				if (firstLButtonDown)
				{
					switch (MessageBox(hWnd, TEXT("��Ϸ���ڽ��� , ��ϣ����ʲô?\n\n�˳�����ʼ����Ϸ(Y)\n���ֽ���Ϊʧ�ܼ���ͳ����Ϣ!\n\n���¿�ʼ�����Ϸ(N)\n���ֽ���Ϊʧ�ܼ���ͳ����Ϣ!"), TEXT("����Ϸ"), MB_YESNOCANCEL))
					{
					case IDYES:
						if ((optionFlag & 0x03) < 3)
							changeStatistic((optionFlag & 0x03), -1);
						wmEvent = 3;
						break;
					case IDNO:
						if ((optionFlag & 0x03) < 3)
							changeStatistic((optionFlag & 0x03), -1);
						wmEvent = 2;
						break;
					case IDCANCEL:
						break;
					}
				}
				else
					wmEvent = 3;
			}

			if (wmEvent == 2)//���¿�ʼ�����Ϸ
			{
				startAtSave = false;
				firstPaint = false;
				firstLButtonDown = true;
				timeFlag = true;
				InitInterface(row, col, count, 1, hWnd, hdc, hdcMem);
			}
			else if (wmEvent == 3)//�����ÿ�ʼ����Ϸ
			{
				startAtSave = false;
				firstPaint = false;
				firstLButtonDown = false;
				if (tempOption[0] >= 9 && tempOption[0] <= 24 && tempOption[1] >= 9 && tempOption[1] <= 30 && tempOption[2] >= 10 && tempOption[2] <= 668 && tempOption[3] >= 0 && tempOption[3] <= 0xFF)
				{
					row = tempOption[0];
					col = tempOption[1];
					count = tempOption[2];
					optionFlag = tempOption[3];
					tempOption[0] = -1;
					tempOption[1] = -1;
					tempOption[2] = -1;
					tempOption[3] = -1;
					//���´��ڴ�С
					MoveWindow(hWnd, wndX, wndY, wndScale * col + gap.left + gap.right + 2 * DEFAULT_XFRAME, wndScale * row + gap.top + gap.bottom + 2 * DEFAULT_YFRAME + DEFAULT_YTOP, TRUE);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else
					InitInterface(row, col, count, 0, hWnd, hdc, hdcMem);
			}

			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
			break;
		case ID_STATISTICS://ͳ����Ϣ
			DialogBox(hInst, MAKEINTRESOURCE(IDD_STATISTIC), hWnd, Statistic);
			break;
		case ID_OPTION://����
			hdc = GetDC(hWnd);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);

			switch (DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTION), hWnd, Option))
			{
			case 1://�����ÿ�ʼ����Ϸ
				startAtSave = false;
				firstPaint = false;
				firstLButtonDown = false;
				MoveWindow(hWnd, wndX, wndY, wndScale * col + gap.left + gap.right + 2 * DEFAULT_XFRAME, wndScale * row + gap.top + gap.bottom + 2 * DEFAULT_YFRAME + DEFAULT_YTOP, TRUE);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			writeProfileBasic();

			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
			break;
		case IDM_ABOUT: //����
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT: //�˳�
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;


	case WM_CREATE:
		//��ʼ��ȫ�ֱ���
		saveFlags = NULL;
		mapInfo = NULL;
		hasSave = false;
		startAtSave = false;
		firstPaint = false;
		firstLButtonDown = false;
		hasSpeed = false;
		tempOption[0] = -1;
		tempOption[1] = -1;
		tempOption[2] = -1;
		tempOption[3] = -1;

		//��ȡ�����ļ�
		readProfileBasic();
		readProfileSave();

		//�ƶ�������
		MoveWindow(hWnd, wndX, wndY, wndWidth, wndHeight, TRUE);

		//��ʼ����ͼԴ���
		hBitmap_Frame = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FRAME));
		GetObject(hBitmap_Frame, sizeof(BITMAP), &bitmap_Frame);
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MINESWEEPER));
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		//lpvBits�������Minesweeperͼrgbֵ
		lpvBits = new char[bitmap.bmWidthBytes * bitmap.bmHeight];
		GetBitmapBits(hBitmap, bitmap.bmWidthBytes * bitmap.bmHeight, lpvBits);

		//��ʼ������ͼrgbֵ����
		pBrick = new unsigned int[INITIAL_BMWDITH * INITIAL_BMHEIGHT];
		//��ʼ������ͼ���
		hBitmapT = CreateBitmap(INITIAL_BMWDITH, INITIAL_BMHEIGHT, 1, 32, pBrick);
		break;


	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hdcMem_Frame = CreateCompatibleDC(hdc);
		SelectObject(hdcMem_Frame, hBitmap_Frame);

		//�������Ľ���ͼ
		StretchBlt(hdc, 0, 0, gap.left, gap.top, hdcMem_Frame, 0, 0, DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col, 0, gap.right, gap.top, hdcMem_Frame, 571, 0, DEFAULT_GAP_RIGHT, DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, 0, gap.top + wndScale * row, gap.left, gap.bottom, hdcMem_Frame, 0, 319, DEFAULT_GAP_LEFT, DEFAULT_GAP_BOTTOM, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col, gap.top + wndScale * row, gap.right, gap.bottom, hdcMem_Frame, 571, 319, DEFAULT_GAP_RIGHT, DEFAULT_GAP_BOTTOM, SRCCOPY);

		//����������������ͼ
		StretchBlt(hdc, gap.left, 0, wndScale * col, gap.top, hdcMem_Frame, DEFAULT_GAP_LEFT, 0, 571 - DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, 0, gap.top, gap.left, wndScale * row, hdcMem_Frame, 0, DEFAULT_GAP_TOP, DEFAULT_GAP_LEFT, 319 - DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col, gap.top, gap.right, wndScale * row, hdcMem_Frame, 571, DEFAULT_GAP_TOP, DEFAULT_GAP_RIGHT, 319 - DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, gap.left, gap.top + wndScale * row, wndScale * col, gap.bottom, hdcMem_Frame, DEFAULT_GAP_LEFT, 319, 571 - DEFAULT_GAP_LEFT, DEFAULT_GAP_BOTTOM, SRCCOPY);

		//ʱ�����������ͼ
		StretchBlt(hdc, gap.left + (int)((col - 12.5) / 7 * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(25.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(25.0 / INITIAL_BMWDITH * wndScale), (int)(25.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP, 25, 25, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col + (int)((12.5 - col) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(25.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(25.0 / INITIAL_BMWDITH * wndScale), (int)(25.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP + 25, 25, 25, SRCCOPY);
		StretchBlt(hdc, gap.left + (int)((col - 12.5 + 12) / 7 * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(40.0 / INITIAL_BMWDITH * wndScale), (int)(22.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT + 25, DEFAULT_GAP_TOP, 40, 22, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col + (int)((12.5 - col - 17.5) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(40.0 / INITIAL_BMWDITH * wndScale), (int)(22.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT + 25, DEFAULT_GAP_TOP, 40, 22, SRCCOPY);

		DeleteDC(hdcMem_Frame);

		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmap);

		if (hasSave)//���ڱ������Ϸ
		{
			//��MF_BRICKDOWN��
			for (m = 0; m < row; m++)
				for (n = 0; n < col; n++)
					LightShadow(hdc, m, n, hdcMem, 4, 0);

			//��ʾ�Ի���
			if ((optionFlag & OF_STARTATSAVE) || MessageBox(hWnd, TEXT("��������������Ϸ��?"), TEXT("�ҵ��������Ϸ"), MB_YESNO) == IDYES)
			{
				//�Ա������Ϸ��ʼ
				startAtSave = true;
				InitMapInfo(seed, firstLButtonNo, saveFlags, hWnd, hdc);
			}
			else
			{
				//������Ϸ��ʼ
				startAtSave = false;
				InitInterface(row, col, count, 0, hWnd, hdc, hdcMem);
			}
			hasSave = false;
		}
		else
		{
			//������Ϸ��ʼ
			startAtSave = false;
			InitInterface(row, col, count, 0, hWnd, hdc, hdcMem);
		}

		//�ػ���ͼ
		for (m = 0; m < row; m++)
			for (n = 0; n < col; n++)
				LightShadow(hdc, m, n, hdcMem, (mapInfo[n + m * col] & 0x00FF), (mapInfo[n + m * col] & 0x0F00) / 0x0100);

		//��ʾʱ������
		showTime(timeCount, hdc);
		showMime(flagCount, hdc);

		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
		break;


	case WM_LBUTTONDOWN:
		//������״̬�ṹ��
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONDOWN);
		if (wParam & MK_RBUTTON)
		{
			//˫������
			mouseState.mType = WM_LBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_LBUTTONDBLCLK);
		}
		else
		{
			mouseState.mType = WM_LBUTTONDOWN;
			OnMouseClick(hWnd, mouseState, WM_LBUTTONDOWN);
		}
		break;


	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);

		//˫�����������
		if (mouseState.mType == WM_RBUTTONDBLCLK)
			break;

		int lastType;
		lastType = mouseState.mType;
		//������״̬�ṹ��
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONUP);
		//˫�����¹����в���ʼ���������־����
		if (lastType == WM_LBUTTONDOWN && mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row && mapInfo[mouseState.mCol + mouseState.mRow * col] != 0xF006)
			InitMapInfo((unsigned int)time(NULL), mouseState.mCol + mouseState.mRow * col, NULL, hWnd, hdc);

		//������ʱ��
		if (timeFlag)
		{
			SetTimer(hWnd, ID_TIMER, 1000, NULL);
			timeFlag = false;
		}

		if (wParam & MK_RBUTTON)
		{
			//˫������
			mouseState.mType = WM_RBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_RBUTTONDBLCLK);
		}
		else
		{
			OnMouseClick(hWnd, mouseState, WM_LBUTTONUP);
		}
		ReleaseDC(hWnd, hdc);
		break;


	case WM_RBUTTONDOWN:
		//������״̬�ṹ��
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_RBUTTONDOWN);
		if (wParam & MK_LBUTTON)
		{
			//˫������
			mouseState.mType = WM_LBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_LBUTTONDBLCLK);
		}
		else
		{
			OnMouseClick(hWnd, mouseState, WM_RBUTTONDOWN);
		}
		break;


	case WM_RBUTTONUP:
		//˫�����������
		if (mouseState.mType == WM_RBUTTONDBLCLK)
			break;

		//������״̬�ṹ��
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_RBUTTONUP);
		if (wParam & MK_LBUTTON)
		{
			//˫������
			mouseState.mType = WM_RBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_RBUTTONDBLCLK);
		}
		else
		{
			OnMouseClick(hWnd, mouseState, WM_RBUTTONUP);
		}
		break;


	case WM_MOUSEMOVE:
		//������״̬�ṹ��
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_MOUSEMOVE);
		if (wParam & MK_LBUTTON)
		{
			if (wParam & MK_RBUTTON)
			{
				//˫������
				mouseState.mType = WM_LBUTTONDBLCLK;
				OnMouseClick(hWnd, mouseState, WM_MOUSEMOVE);
			}
			else
			{
				//�������
				mouseState.mType = WM_LBUTTONDOWN;
				OnMouseClick(hWnd, mouseState, WM_MOUSEMOVE);
			}
		}
		else
		{
			//��δ���»�ֻ�����Ҽ�
			OnMouseClick(hWnd, mouseState, WM_MOUSEMOVE);
		}
		break;


	case WM_TIMER:
		hdc = GetDC(hWnd);
		if (timeCount < 999)
			timeCount++;
		showTime(timeCount, hdc);
		ReleaseDC(hWnd, hdc);
		break;


	case WM_GETMINMAXINFO://���ƴ�����С���
		MINMAXINFO *mminfo;
		mminfo = (PMINMAXINFO)lParam;
		mminfo->ptMinTrackSize.x = INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT + 2 * DEFAULT_XFRAME;// (GetSystemMetrics(SM_CXFRAME) + 2) * 2 + GetSystemMetrics(SM_CXBORDER) * 4;;
		mminfo->ptMinTrackSize.y = INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;// (GetSystemMetrics(SM_CYFRAME) + 2) * 2 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYBORDER) * 4;
		break;


	case WM_MOVE:
		//�ƶ����λ�ü��������ļ�
		wndX = LOWORD(lParam) - DEFAULT_XFRAME;
		wsprintf(value, TEXT("%d"), wndX);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDX"), value, PROFILE);
		wndY = HIWORD(lParam) - DEFAULT_YFRAME - DEFAULT_YTOP;
		wsprintf(value, TEXT("%d"), wndY);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDY"), value, PROFILE);
		break;


	case WM_SIZING://�ȱ����Ŵ���С����
		RECT* pRect;
		pRect = (RECT*)lParam;
		float kWidth, kHeight;
		//�Ŵ����
		kWidth = (float)((*pRect).right - (*pRect).left) / (float)(INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT + 2 * DEFAULT_XFRAME);
		kHeight = (float)((*pRect).bottom - (*pRect).top) / (float)(INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP);
		switch (wParam)
		{
		case WMSZ_RIGHT:case WMSZ_LEFT:case WMSZ_BOTTOMRIGHT:case WMSZ_BOTTOMLEFT:
			(*pRect).bottom = (*pRect).top + (int)(kWidth * (INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP));
			wndScale = (int)(kWidth * INITIAL_BMWDITH);
			break;
		case WMSZ_TOP:case WMSZ_BOTTOM:
			(*pRect).right = (*pRect).left + (int)(kHeight * (INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT + 2 * DEFAULT_XFRAME));
			wndScale = (int)(kHeight * INITIAL_BMWDITH);
			break;
		case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
			(*pRect).top = (*pRect).bottom - (int)(kWidth * (INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP));
			wndScale = (int)(kWidth * INITIAL_BMWDITH);
			break;
		}
		//���������ڿ��
		wndWidth = (*pRect).right - (*pRect).left;
		wndHeight = (*pRect).bottom - (*pRect).top;
		//���������ⱳ�������
		gap.top = (wndHeight - 2 * DEFAULT_YFRAME - DEFAULT_YTOP - wndScale * row) / 2;
		gap.bottom = wndHeight - 2 * DEFAULT_YFRAME - DEFAULT_YTOP - wndScale * row - gap.top;
		gap.right = (wndWidth - DEFAULT_XFRAME - wndScale * col) / 2;
		gap.left = wndWidth - DEFAULT_XFRAME - wndScale * col - gap.right;

		//���������ļ�
		wsprintf(value, TEXT("%d"), wndScale);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDSCALE"), value, PROFILE);
		break;


	case WM_SIZE:
		static int lastfwSizeType;

		switch (wParam)
		{
		case SIZE_MINIMIZED://��С��
			KillTimer(hWnd, ID_TIMER);
			lastfwSizeType = SIZE_MINIMIZED;
			break;
		case SIZE_MAXIMIZED://���
			//���������ڿ��
			wndWidth = LOWORD(lParam) + DEFAULT_XFRAME;
			wndHeight = HIWORD(lParam) + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;
			wndScale = min((int)(LOWORD(lParam) / (float)(INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT) * INITIAL_BMWDITH),
				(int)(HIWORD(lParam) / (float)(INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM) * INITIAL_BMWDITH));
			//���������ⱳ�������
			gap.top = (HIWORD(lParam) - wndScale * row) / 2;
			gap.bottom = HIWORD(lParam) - wndScale * row - gap.top;
			gap.right = (LOWORD(lParam) - wndScale * col) / 2;
			gap.left = LOWORD(lParam) - wndScale * col - gap.right;
			//����С�������������ʱ��
			if (lastfwSizeType == SIZE_MINIMIZED && firstLButtonDown)
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
			lastfwSizeType = SIZE_MAXIMIZED;
			break;
		case SIZE_RESTORED://��ԭ
			if (lastfwSizeType == SIZE_MAXIMIZED)
			{
				//���������ڿ��
				wndWidth = LOWORD(lParam) + DEFAULT_XFRAME;
				wndHeight = HIWORD(lParam) + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;
				wndScale = min((int)(LOWORD(lParam) / (float)(INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT) * INITIAL_BMWDITH),
					(int)(HIWORD(lParam) / (float)(INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM) * INITIAL_BMWDITH));
				//���������ⱳ�������
				gap.top = (HIWORD(lParam) - wndScale * row) / 2;
				gap.bottom = HIWORD(lParam) - wndScale * row - gap.top;
				gap.right = (LOWORD(lParam) - wndScale * col) / 2;
				gap.left = LOWORD(lParam) - wndScale * col - gap.right;
			}
			//������ʱ��
			if (lastfwSizeType == SIZE_MINIMIZED && firstLButtonDown)
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
			lastfwSizeType = SIZE_RESTORED;
			break;
		default:
			/*bug	�������ƶ����������Ե���ұ�Ե��ϵͳ�Ŵ�, 
					û���¿ͻ���, �����¿ͻ���, ���ڸı������ڴ�С, 
					���������ƶ����ʱ, ��ʱ����ֿ��ػ�λ�ô���*/
			break;
		}
		break;


	case WM_CLOSE:
		int temp;
		//�����Ի���
		if ((!startAtSave && firstLButtonDown) || flagCount != 0)
		{
			if ((optionFlag & OF_SAVE) || (temp = MessageBox(hWnd, TEXT("��Ϸ���ڽ��� , ��ϣ��������Ϸ��?"), TEXT("�˳���Ϸ"), MB_YESNOCANCEL)) == IDYES)
			{
				//������Ϸ
				writeProfileSave();
				DestroyWindow(hWnd);
			}
			else if (temp == IDNO)
			{
				//��������Ϸ
				DestroyWindow(hWnd);
				break;
			}
		}
		else
		{
			DestroyWindow(hWnd);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

tagMouseState getMouseState(int mXpos, int mYpos, int mType) {
	tagMouseState mouseState;
	//�������������
	if (mXpos >= gap.left && mXpos <= gap.left + wndScale * col
		&& mYpos >= gap.top && mYpos <= gap.top + wndScale * row)
	{
		//���������ڿ��λ��
		mouseState.mCol = (mXpos - gap.left) / wndScale;
		mouseState.mRow = (mYpos - gap.top) / wndScale;
	}
	else
	{
		mouseState.mCol = -1;
		mouseState.mRow = -1;
	}
	mouseState.mType = mType;//��갴������
	return mouseState;
}

bool OnMouseClick(HWND hWnd, tagMouseState mouseState, int nIDMessage) {
	static tagMouseState lastMouseState;//��������״̬�ṹ��
	bool msgFlag = false;
	int no, temp1;

	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);

	switch (nIDMessage)
	{
	case WM_LBUTTONDOWN://�������
		buttonDown(mouseState, hWnd, hdc, hdcMem);//�鰴��
		lastMouseState = mouseState;
		break;

	case WM_LBUTTONUP://�������
		buttonUp(mouseState, hWnd, hdc, hdcMem, MF_LIGHTFLAG);//��ˢ��
		lastMouseState = mouseState;
		break;

	case WM_RBUTTONDOWN://�Ҽ�����
		buttonChange(mouseState, hWnd, hdc, hdcMem);//��ı�
		lastMouseState = mouseState;
		break;

	case WM_RBUTTONUP://�Ҽ�����
		break;

	case WM_LBUTTONDBLCLK://˫������
		nineButtonDown(mouseState, hWnd, hdc, hdcMem);//�Ÿ��鰴��
		lastMouseState = mouseState;
		break;

	case WM_RBUTTONDBLCLK://˫������
		if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
		{
			no = mouseState.mCol + mouseState.mRow * col;

			if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 == 0)//���µĿ鲻������
				nineButtonRestore(mouseState, hWnd, hdc, hdcMem);//�Ÿ��黹ԭ
			else if ((mapInfo[no] & 0x0F00) == 0x0100)//���µĿ�������
			{
				if (dbButtonClickFlag(no, mapInfo, row, col) != mapInfo[no] / 0x1000)//��Χ����������������
				{
					nineButtonRestore(mouseState, hWnd, hdc, hdcMem);//�Ÿ��黹ԭ
					if (optionFlag & OF_SOUND)
						PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);//��������
					//������������������
					for (int i = 0; (optionFlag & OF_DRAW) && i < 2; i++)
					{
						Sleep(150);
						LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, (mapInfo[no] & 0x00FF) + 9, (mapInfo[no] & 0x0F00) / 0x0100);
						Sleep(150);
						LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
					}
				}
				else if ((temp1 = dbButtonClickMine(no, mapInfo, row, col)) == 0)//����λ����ȷ
					nineButtonUp(mouseState, hWnd, hdc, hdcMem);//�Ÿ���ˢ��
				else//����λ�ò���ȷ
				{
					nineButtonRestore(mouseState, hWnd, hdc, hdcMem);//�Ÿ��黹ԭ
					no += temp1;
					if ((optionFlag & 0x03) < 3)
						changeStatistic((optionFlag & 0x03), -1);//��Ϸʧ��
					mineBomb(no / col, no % col, hWnd, hdc, hdcMem);//�ױ�ըЧ��
					showLose(hWnd);//��ʾʧ�ܶԻ���
				}
			}
		}
		lastMouseState = mouseState;
		break;

	case WM_MOUSEMOVE://����ƶ�
		switch (mouseState.mType)
		{
		case WM_MOUSEMOVE://��δ����
			if (lastMouseState.mCol != mouseState.mCol || lastMouseState.mRow != mouseState.mRow)
			{
				buttonRestore(lastMouseState, hWnd, hdc, hdcMem);//�黹ԭ
				buttonLight(mouseState, hWnd, hdc, hdcMem);//�����
			}
			lastMouseState = mouseState;
			break;

		case WM_LBUTTONDOWN://�������
			if (lastMouseState.mCol != mouseState.mCol || lastMouseState.mRow != mouseState.mRow)
			{
				buttonRestore(lastMouseState, hWnd, hdc, hdcMem);//�黹ԭ
				buttonDown(mouseState, hWnd, hdc, hdcMem);//�鰴��
			}
			lastMouseState = mouseState;
			break;

		case WM_LBUTTONDBLCLK://˫������
			if (lastMouseState.mCol != mouseState.mCol || lastMouseState.mRow != mouseState.mRow)
			{
				nineButtonRestore(lastMouseState, hWnd, hdc, hdcMem);//�Ÿ��黹ԭ
				nineButtonDown(mouseState, hWnd, hdc, hdcMem);//�Ÿ��鰴��
			}
			lastMouseState = mouseState;
			break;
		}
		break;
	}


	DeleteDC(hdcMem);
	ReleaseDC(hWnd, hdc);

	return true;
}


void InitInterface(int tRow, int tCol, int tCount, int uType, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (!firstPaint)//δ���ų�ʼ������Ч��
	{
		//��ʼ����������
		row = tRow;
		col = tCol;
		count = tCount;
		brickNumber = row * col;

		//ֹͣ��ʱ��, ���ò���ʾʱ�����
		KillTimer(hWnd, ID_TIMER);
		timeCount = DEFAULT_TIMECOUNT;
		showTime(timeCount, hdc);

		//���ú�����, ��ʼ����, ��ʾ����
		flagCount = 0;
		brickCount = brickNumber;
		showMime(flagCount, hdc);


		unsigned short int *temp;
		temp = new unsigned short int[brickNumber];
		if (uType == 0)//���²���
		{
			//�����������־����
			if (mapInfo != NULL)
				delete[] mapInfo;
			mapInfo = new unsigned short int[brickNumber];
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = MF_NOINIT | MF_BRICKDOWN;
		}
		else if (uType == 1)//�����²���
		{
			for (int i = 0; i < brickNumber; i++)
			{
				//���沢�����������־����
				temp[i] = (mapInfo[i] & 0xF000) + MF_BRICK;
				mapInfo[i] = MF_NOINIT | MF_BRICKDOWN;
			}
		}

		//���ñ���Ŀ���ͼ��Ϣ����
		if (saveFlags != NULL)
			delete[] saveFlags;
		saveFlags = new TCHAR[brickNumber + 1];

		//��MF_BRICKDOWN��
		for (int m = 0; m < row; m++)
			for (int n = 0; n < col; n++)
				LightShadow(hdc, m, n, hdcMem, 3, 0);

		if (uType == 0)//���²���
		{
			//��ʼ���������־����
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = MF_INIT;
		}
		if (uType == 1)//�����²���
		{
			//��ԭ�������־����
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = temp[i];
		}
		delete[] temp;


		int *ptemp1 = getFirstPaintNo(row, col);//��ʼ��������ͼ����������
		if ((optionFlag & OF_DRAW) && (optionFlag & OF_SOUND))
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);//��������
		hasSpeed = false;//����Ч����ʱ���Ʊ�־
		for (int i = 0; i < brickNumber; i++)
		{
			LightShadow(hdc, ptemp1[i] / col, ptemp1[i] % col, hdcMem, 4, 0);
			speed((int)(1100.0 / (row * col)), NULL, 0);//Ч����ʱ����
			//bug	WM_LBUTTONDOWNֹͣ����, WM_LBUTTONUP��Ϣ���ʼ��mapInfo
			//bug	���е��ʵ�һ�����л�bug

			/*if (optionFlag & OF_DRAW)
				Sleep((int)(1100.0 / (row * col)));*/
		}
		//PlaySound(NULL, NULL, SND_ASYNC);
		delete[] ptemp1;

		firstPaint = true;
	}
}

void InitMapInfo(unsigned int tSeed, int tFirstLButtonNo, TCHAR *saveFlags, HWND hWnd, HDC hdc)
{
	if (!firstLButtonDown)//��Ϸδ��ʼ
	{
		if (!firstPaint)//δ���ų�ʼ��Ч��
		{
			//ֹͣ��ʱ��, ��ʾʱ�����
			KillTimer(hWnd, ID_TIMER);
			showTime(timeCount, hdc);

			//���ú�����, ��ʼ����, ��ʾ����
			flagCount = 0;
			brickCount = brickNumber;
			showMime(flagCount, hdc);

			//�����������־����
			if (mapInfo != NULL)
				delete[] mapInfo;
			mapInfo = new unsigned short int[brickNumber];
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = MF_INIT;

			firstPaint = true;
		}

		//��ʼ���������־����, �������
		seed = tSeed;
		firstLButtonNo = tFirstLButtonNo;
		initMineCount(mapInfo, seed, firstLButtonNo, row, col, count);
		if (saveFlags != NULL) {
			save2map(mapInfo, saveFlags, brickNumber);//�ӱ������Ϸ�����������־����
			//�����������ʼ����
			for (int i = 0; i < brickNumber; i++)
			{
				if ((mapInfo[i] & 0x0F00) == 0x0000 && (mapInfo[i] & 0x00FF) / 3 == 2)
					flagCount++;
				if ((mapInfo[i] & 0x0F00) != 0x0000 || (mapInfo[i] & 0x00FF) / 3 != 1)
					brickCount--;
			}
		}

		SetTimer(hWnd, ID_TIMER, 1000, NULL);//������ʱ��

		firstLButtonDown = true;
	}
}


void LightShadow(HDC hdcDest, int rowDest, int colDest, HDC hdcSrc, int rowSrc, int colSrc)
{
	if ((rowSrc % 3 == 1 && colSrc == 0) || (rowSrc == 6 && colSrc == 0) ||  (rowSrc == 0 && colSrc >= 3 && colSrc <= 7))//��Ч��
	{
		int r, g, b, dr, dg, db, start;//Դrgbֵ, ��ͼrgbֵ����, ��ͼ������ʼλ��
		double xi, xj;//����
		xi = (double)rowDest / row * 24;
		xj = (double)colDest / col * 30;

		//������ͼrgbֵ����
		dr = (int)DR(xj, xi);
		dg = (int)DG(xj, xi);
		db = (int)DB(xj, xi);

		//��ͼ������ʼλ��
		start = rowSrc * INITIAL_BMHEIGHT * bitmap.bmWidth + colSrc * INITIAL_BMWDITH;

		for (int i = 0; i < INITIAL_BMHEIGHT; i++)
		{
			for (int j = 0; j < INITIAL_BMWDITH; j++)
			{
				//������ͼrgbֵ
				r = dr + ((unsigned int*)lpvBits)[start + i * bitmap.bmWidth + j] / 0x010000;
				g = dg + (((unsigned int*)lpvBits)[start + i * bitmap.bmWidth + j] & 0x00FF00) / 0x000100;
				b = db + ((unsigned int*)lpvBits)[start + i * bitmap.bmWidth + j] & 0x0000FF;

				//�޶���Χ
				r = min(max(r, 0), 255);
				g = min(max(g, 0), 255);
				b = min(max(b, 0), 255);

				//�ϲ�����ɫֵ����������
				pBrick[i * INITIAL_BMWDITH + j] = r * 0x010000 + g * 0x000100 + b;
			}
		}
		HDC hdcMem1;
		hdcMem1 = CreateCompatibleDC(hdcDest);

		//����ɫ���鸳������ͼ���
		SetBitmapBits(hBitmapT, INITIAL_BMWDITH * INITIAL_BMHEIGHT * 4, (LPVOID)pBrick);
		SelectObject(hdcMem1, hBitmapT);

		//�ÿ���ͼ�����ͼ
		StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, wndScale, hdcMem1, 0, 0, INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);

		DeleteDC(hdcMem1);
	}
	else if (rowSrc % 3 == 2 && colSrc == 0)//����ֱ����ͼ
	{
		StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * colSrc, INITIAL_BMHEIGHT * rowSrc, INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
	}
	else//ӰЧ��
	{
		//����ͼ
		StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * colSrc, INITIAL_BMHEIGHT * rowSrc, INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
		
		/*Ӱ��־	Ӱ��λ��
			0		��
			1		��
			2		��
			3		�Ϻ���*/
		int shadowFlag = 0x0;
		int temp, temp1;

		//�ж�Ӱ��־
		if (rowDest == 0)
			shadowFlag = (shadowFlag & 0x2) + 0x1;
		else
		{
			temp = mapInfo[(rowDest - 1) * col + colDest];
			if (((temp & 0x0F00) == 0x0000 && (temp & 0x00FF) != 0 && (temp & 0x00FF) != 3) || ((temp & 0x00FF) == 0 && (temp & 0x0F00) >= 0x0300 && (temp & 0x0F00)  <= 0x0700))
				shadowFlag = (shadowFlag & 0x2) + 0x1;
		}
		if (colDest == 0)
			shadowFlag = (shadowFlag & 0x1) + 0x2;
		else
		{
			temp = mapInfo[rowDest * col + colDest - 1];
			if (((temp & 0x0F00) == 0x0000 && (temp & 0x00FF) != 0 && (temp & 0x00FF) != 3) || ((temp & 0x00FF) == 0 && (temp & 0x0F00) >= 0x0300 && (temp & 0x0F00)  <= 0x0700))
				shadowFlag = (shadowFlag & 0x1) + 0x2;
		}

		//ӰЧ����ͼ
		if (shadowFlag == 1)
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 1, INITIAL_BMWDITH, 3, SRCCOPY);
		if (shadowFlag == 2)
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, (int)(wndScale * 3.0 / 18), wndScale, hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 2, 3, INITIAL_BMHEIGHT, SRCCOPY);
		if (shadowFlag == 3)
		{
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 3, INITIAL_BMWDITH, 3, SRCCOPY);
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest + (int)(wndScale * 3.0 / 18), (int)(wndScale * 3.0 / 18), wndScale - (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 3 + 3, 3, INITIAL_BMHEIGHT - 3, SRCCOPY);
		}

		//�ı��·����ӰЧ��
		if (rowDest < row - 1)
		{
			temp = mapInfo[(rowDest + 1) * col + colDest];
			if ((temp & 0x0FFF) == MF_BRICKDOWN || (temp & 0x0F00) == 0x0100 || (temp & 0x0F00) == 0x0200 || ((temp & 0x0F00) >= 0x0300 && (temp & 0x0F00) <= 0x0700 && (temp & 0x00FF) != 0x0000))
			{
				StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * (rowDest + 1), wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * ((temp & 0x0F00) / 0x0100), INITIAL_BMHEIGHT * (temp & 0x00FF), INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
				if (colDest != 0)
				{
					temp1 = mapInfo[(rowDest + 1) * col + colDest - 1];
					if (((temp1 & 0xF000) != MF_NOINIT) && (((temp1 & 0x0F00) == 0x0000 && (temp1 & 0x00FF) != 0 && (temp1 & 0x00FF) != 3) || ((temp1 & 0x00FF) == 0 && (temp1 & 0x0F00) >= 0x0300 && (temp1 & 0x0F00) <= 0x0700)))
						StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * (rowDest + 1), (int)(wndScale * 3.0 / 18), wndScale, hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 2, 3, INITIAL_BMHEIGHT, SRCCOPY);
				}
				else
					StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * (rowDest + 1), (int)(wndScale * 3.0 / 18), wndScale, hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 2, 3, INITIAL_BMHEIGHT, SRCCOPY);
			}
		}
		//�ı��ҷ����ӰЧ��
		if (colDest < col - 1)
		{
			temp = mapInfo[rowDest * col + colDest + 1];
			if ((temp & 0x0FFF) == MF_BRICKDOWN || (temp & 0x0F00) == 0x0100 || (temp & 0x0F00) == 0x0200 || ((temp & 0x0F00) >= 0x0300 && (temp & 0x0F00) <= 0x0700 && (temp & 0x00FF) != 0x0000))
			{
				StretchBlt(hdcDest, gap.left + wndScale * (colDest + 1), gap.top + wndScale * rowDest, wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * ((temp & 0x0F00) / 0x0100), INITIAL_BMHEIGHT * (temp & 0x00FF), INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
				if (rowDest != 0)
				{
					temp1 = mapInfo[(rowDest - 1) * col + colDest + 1];
					if (((temp1 & 0xF000) != MF_NOINIT) && (((temp1 & 0x0F00) == 0x0000 && (temp1 & 0x00FF) != 0 && (temp1 & 0x00FF) != 3) || ((temp1 & 0x00FF) == 0 && (temp1 & 0x0F00) >= 0x0300 && (temp1 & 0x0F00) <= 0x0700)))
						StretchBlt(hdcDest, gap.left + wndScale * (colDest + 1), gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 1, INITIAL_BMWDITH, 3, SRCCOPY);
				}
				else
					StretchBlt(hdcDest, gap.left + wndScale * (colDest + 1), gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 1, INITIAL_BMWDITH, 3, SRCCOPY);
			}
		}
	}
}


void showTime(int timeCount, HDC hdc)
{
	TCHAR value[10];
	wsprintf(value, TEXT(" %d "), timeCount);
	//��ʾ����
	//SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(50, 90, 160));
	SetTextColor(hdc, RGB(196, 218, 235));

	//λ�þ���
	RECT rect;
	rect.top = gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.bottom = gap.top + wndScale * row + (gap.bottom + (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.left = gap.left + (int)((col - 12.5 + 12) / 7 * wndScale);
	rect.right = gap.left + (int)((col - 12.5 + 12) / 7 * wndScale + 40.0 / INITIAL_BMWDITH * wndScale);

	//��ʾʱ��
	DrawText(hdc, value, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void showMime(int flagCount, HDC hdc)
{
	TCHAR value[10];
	wsprintf(value, TEXT(" %d "), count - flagCount);
	//��ʾ����
	//SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(50, 90, 160));
	SetTextColor(hdc, RGB(196, 218, 235));

	//λ�þ���
	RECT rect;
	rect.top = gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.bottom = gap.top + wndScale * row + (gap.bottom + (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.left = gap.left + wndScale * col + (int)((12.5 - col - 17.5) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale);
	rect.right = gap.left + wndScale * col + (int)((12.5 - col - 17.5) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale + 40.0 / INITIAL_BMWDITH * wndScale);

	//��ʾ����
	DrawText(hdc, value, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void checkWin(HWND hWnd, HDC hdc)
{
	if (flagCount + brickCount == count)//�ж���Ϸʤ��
	{
		KillTimer(hWnd, ID_TIMER);
		showMime(count, hdc);
		if ((optionFlag & 0x03) < 3)
			changeStatistic((optionFlag & 0x03), 1);//��Ϸʤ��
		//��ʾʤ���Ի���
		switch (DialogBox(hInst, MAKEINTRESOURCE(IDD_WIN), hWnd, Win))
		{
		case 0://�˳�
			DestroyWindow(hWnd);
			break;
		case 2://����һ��
			PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_NEWGAME, 3), 0);
			break;
		}
	}
}

void showLose(HWND hWnd)
{
	//��ʾʧ�ܶԻ���
	switch (DialogBox(hInst, MAKEINTRESOURCE(IDD_LOSE), hWnd, Lose))
	{
	case 0://�˳�
		DestroyWindow(hWnd);
		break;
	case 1://���¿�ʼ�����Ϸ
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_NEWGAME, 2), 0);
		break;
	case 2://����Ϸ
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_NEWGAME, 3), 0);
		break;
	}
}


int zeroDevelop(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem)
{
	int number = 0;//�忪�Ŀ���
	int no = colDest + rowDest * col;//�㵽�Ŀ����
	mapInfo[no] = (mapInfo[no] & 0xF000) + MF_ZERO;
	LightShadow(hdc, rowDest, colDest, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);

	int *ptemp1 = getZeroDevelopNo(no, mapInfo, row, col);//�忪�����������
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);//��������

	//�忪����ͼ
	for (int i = 0; ptemp1[i] != -1 && i < brickNumber; i++)
	{
		if ((mapInfo[ptemp1[i]] & 0x0F00) != 0x0100 && ((mapInfo[ptemp1[i]] & 0x0F00) != 0x0000 || (mapInfo[ptemp1[i]] & 0x00FF) / 3 != 2))
		{
			mapInfo[ptemp1[i]] = (mapInfo[ptemp1[i]] & 0xF000) + MF_NUMBER(mapInfo[ptemp1[i]] / 0x1000);
			LightShadow(hdc, ptemp1[i] / col, ptemp1[i] % col, hdcMem, mapInfo[ptemp1[i]] & 0x00FF, (mapInfo[ptemp1[i]] & 0x0F00) / 0x0100);
			if (optionFlag & OF_DRAW)
				Sleep(1);//
			number++;
		}
	}
	return 1 + number;
}

void mineBomb(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem)
{
	KillTimer(hWnd, ID_TIMER);//ֹͣ��ʱ��
	int no = colDest + rowDest * col;//�㵽�Ŀ����

	//��ʾ������
	for (int i = 0; i < brickNumber; i++)
	{
		if ((mapInfo[i] & 0xF000) == MF_MINE || ((mapInfo[i] & 0x0F00) == 0x0000 && (mapInfo[i] & 0x00FF) / 3 == 2))
		{
			mapInfo[i] = (mapInfo[i] & 0xF000) + ((mapInfo[i] & 0x00FF) / 3 + (mapInfo[i] / 0x1000) / 9 * 3) * 0x0100;
			LightShadow(hdc, i / col, i % col, hdcMem, mapInfo[i] & 0x00FF, (mapInfo[i] & 0x0F00) / 0x0100);
		}
	}//

	mapInfo[no] += 0x0300;
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);//���ſ�ʼ��ը����
	//�㵽���ױ�ըЧ��
	for (int i = 1; i <= 25; i++)
	{
		mapInfo[no]++;
		LightShadow(hdc, rowDest, colDest, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		if (optionFlag & OF_DRAW)
			Sleep(23);
	}

	int *ptemp1 = getMineDevelopNo(no, mapInfo, 25, row, col, count);//�ױ�ը��ͼ����������
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);//ѭ�����ű�ը����

	hasSpeed = false;//����Ч����ʱ���Ʊ�־
	//�����ױ�ըЧ��
	for (int i = 0; ptemp1[i] > -1 && i < count * 25; i++)//
	{
		mapInfo[ptemp1[i]] = (mapInfo[ptemp1[i]] & 0xFF00) + ptemp1[i + count * 25];
		LightShadow(hdc, ptemp1[i] / col, ptemp1[i] % col, hdcMem, mapInfo[ptemp1[i]] & 0x00FF, (mapInfo[ptemp1[i]] & 0x0F00) / 0x0100);
		speed(150, ptemp1, 2 * count * 25);
	}
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);//���ű�ը��������
}

void speed(DWORD dwMilliseconds, int *timeCount, int startNo)
{
	MSG msg;
	static bool msgFlag = false;
	static int temp1 = 0, i = 0;

	if (!hasSpeed)
	{
		msgFlag = false;
		temp1 = 0;
		i = 0;
		hasSpeed = true;
	}

	//�����Ϣֹͣ����Ч��
	if ((optionFlag & OF_DRAW) && (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) || (msg.message != WM_LBUTTONDOWN && msg.message != WM_RBUTTONDOWN)))
	{
		if (!msgFlag)//û��WM_LBUTTONDOWN��WM_RBUTTONDOWN��Ϣ
		{
			if (temp1 == 0)//��ʱ����ֵ
			{
				Sleep(dwMilliseconds);///
				if (timeCount != NULL)
					temp1 = timeCount[i + startNo] / 2;//�����һ����ʱ����ֵ
			}
			else
				temp1--;
		}
	}
	else
	{
		msgFlag = true;
		PlaySound(NULL, NULL, SND_ASYNC);//ֹͣ������������
		//PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	}
	i++;
}


void buttonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 != 0)//û�а���
		{
			mapInfo[no] = (mapInfo[no] & 0xF000) + (mapInfo[no] & 0x00FF) / 3 * 3;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
	}
}

void nineButtonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	tagMouseState tempMouseState;
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		for (int i = -1; i <= 1; i++)
		{
			tempMouseState.mCol = mouseState.mCol + i;
			for (int j = -1; j <= 1; j++)
			{
				tempMouseState.mRow = mouseState.mRow + j;
				buttonDown(tempMouseState, hWnd, hdc, hdcMem);
			}
		}
	}
}

void buttonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 != 1)
		{
			mapInfo[no] = (mapInfo[no] & 0xF000) + (mapInfo[no] & 0x00FF) / 3 * 3 + 1;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
	}
}

void nineButtonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	tagMouseState tempMouseState;
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		for (int i = -1; i <= 1; i++)
		{
			tempMouseState.mCol = mouseState.mCol + i;
			for (int j = -1; j <= 1; j++)
			{
				tempMouseState.mRow = mouseState.mRow + j;
				buttonRestore(tempMouseState, hWnd, hdc, hdcMem);
			}
		}
	}
}

void buttonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem, int redFlagUp)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) / 3 == 2)//������ͼ
		{
			mapInfo[no] = (mapInfo[no] & 0xF000) + redFlagUp;// MF_LIGHTFLAG;// MF_FLAG;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
		else if ((mapInfo[no] & 0x0FFF) == MF_UNKNOWDOWN || (mapInfo[no] & 0x0FFF) == MF_BRICKDOWN)
		{
			if ((mapInfo[no] & 0xF000) == MF_ZEROMINE)//����Χû����
			{
				brickCount -= zeroDevelop(mouseState.mRow, mouseState.mCol, hWnd, hdc, hdcMem);//�忪û���׵Ŀ�
				checkWin(hWnd, hdc);//�����Ϸ�Ƿ�ʤ��
			}
			else if ((mapInfo[no] / 0x1000) / 9 == 0)//����
			{
				mapInfo[no] = (mapInfo[no] & 0xF000) + MF_NUMBER(mapInfo[no] / 0x1000);
				LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
				brickCount--;//��ʼ���1
				checkWin(hWnd, hdc);//�����Ϸ�Ƿ�ʤ��
			}
			else if ((mapInfo[no] & 0xF000) == MF_MINE)
			{
				if ((optionFlag & 0x03) < 3)
					changeStatistic((optionFlag & 0x03), -1);//��Ϸʧ��
				mineBomb(mouseState.mRow, mouseState.mCol, hWnd, hdc, hdcMem);//�ױ�ըЧ��
				showLose(hWnd);//��ʾʧ�ܶԻ���
			}
		}
	}
}

void nineButtonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	tagMouseState tempMouseState;
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		for (int i = -1; i <= 1; i++)
		{
			tempMouseState.mCol = mouseState.mCol + i;
			for (int j = -1; j <= 1; j++)
			{
				tempMouseState.mRow = mouseState.mRow + j;
				buttonUp(tempMouseState, hWnd, hdc, hdcMem, MF_FLAG);
			}
		}
	}
}

void buttonLight(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 == 1)
		{
			mapInfo[no]++;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
	}
}

void buttonChange(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 != 0)
		{
			if ((mapInfo[no] & 0x00FF) / 3 == 1)
			{
				flagCount++;
				brickCount--;
			}
			else if ((mapInfo[no] & 0x00FF) / 3 == 2)
			{
				flagCount--;
				brickCount++;
			}
			mapInfo[no] = (mapInfo[no] & 0xF000) + ((mapInfo[no] & 0x00FF) / 3 * 3 + 5) % 9;
			if (!(optionFlag & OF_UNKNOW) && (mapInfo[no] & 0x00FF) / 3 == 0)
				mapInfo[no] = (mapInfo[no] & 0xF000) + (mapInfo[no] & 0x00FF) + 3;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
			showMime(flagCount, hdc);//��ʾ����
		}
	}
}
