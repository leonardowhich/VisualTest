// VisualTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VisualTest.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
time_t		T1		= time(NULL),
			T2		= time(NULL);
struct tm	*time1	= localtime(&T1),
			*time2	= localtime(&T2);
Tag			TagArr[200];
Tag			SelectedTags[200];
string		TagName[200];

//FILE		*frw;
//char		*HCombo;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    TimeWindow(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	TagWindow(HWND , UINT, WPARAM, LPARAM);
int GetTagByName(Tag TagAll[], Tag &TagResponse, char*, int);
int	WriteFindValues(int, Tag TagAll[], FILE*, time_t, time_t, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VISUALTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VISUALTEST));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VISUALTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_VISUALTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_VISUALTEST));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 950, 450, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hCompatibleDC;
	PAINTSTRUCT ps;
	HANDLE hbitmap, holdbitmap;
	RECT rect;
	BITMAP bitmap;	
    switch (message)
    {
    case WM_COMMAND:
        {

            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_MY_HELLO:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_TAGS), hWnd, TagWindow);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            /*PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);*/
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			hbitmap = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, rect.right - rect.left, rect.bottom - rect.top, LR_DEFAULTCOLOR);
			//hbitmap = LoadImage(NULL, L"LOGO.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			GetObject(hbitmap, sizeof(BITMAP), &bitmap);
			hCompatibleDC = CreateCompatibleDC(hdc);
			holdbitmap = SelectObject(hCompatibleDC, hbitmap);			
			StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hCompatibleDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			SelectObject(hCompatibleDC, holdbitmap);
			DeleteObject(hbitmap);
			DeleteDC(hCompatibleDC);
			EndPaint(hWnd, &ps);
			return 0;
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

// Message handler for about box.
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
INT_PTR CALLBACK TimeWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SetDlgItemTextA(hDlg,IDC_TEXT, TagArr[0].Path.c_str());
		//HCombo = GetDlgItem(hDlg, IDC_COMBO);
		for (int i = 0; i <20; i++)
		{
			Tag CurTag = TagArr[i];
			SendDlgItemMessageA(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)CurTag.Name.c_str());
		}
		/*for (int i = 0; i < 5; i++)
		{
			
			//SendMessage(HCombo, CB_ADDSTRING, 0, (LPARAM));
		}*/

	}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_LIST2:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				char str[255];
				int i = SendDlgItemMessageA(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
				SendDlgItemMessageA(hDlg, IDC_LIST2, LB_GETTEXT, i, (LPARAM)str);
				Tag CurrentTag;
				//Tag* pCT;
				GetTagByName(TagArr, CurrentTag, str, 200);
				/*if (GetTagByName(SelectedTags, dTag, str, 200)) 
				{
				
				}*/
				SelectedTags[0] = CurrentTag;
				SetDlgItemTextA(hDlg, IDC_TEXT, CurrentTag.Description.c_str());
			}
		}
		break;
		case IDOK:
		{
			
			time2->tm_year = GetDlgItemInt(hDlg, IDC_YEAR2, 0, 0)-1900;
			time2->tm_mon = GetDlgItemInt(hDlg, IDC_MON2, 0, 0)-1;
			time2->tm_mday = GetDlgItemInt(hDlg, IDC_DAY2, 0, 0);
			time2->tm_hour = GetDlgItemInt(hDlg, IDC_HOUR2, 0, 0);
			time2->tm_min = GetDlgItemInt(hDlg, IDC_MIN2, 0, 0);
			time2->tm_sec = GetDlgItemInt(hDlg, IDC_SEC2, 0, 0);
			T2 = mktime(time2);
			time1->tm_year = GetDlgItemInt(hDlg, IDC_YEAR1, 0, 0)-1900;
			time1->tm_mon = GetDlgItemInt(hDlg, IDC_MON1, 0, 0)-1;
			time1->tm_mday = GetDlgItemInt(hDlg, IDC_DAY1, 0, 0);
			time1->tm_hour = GetDlgItemInt(hDlg, IDC_HOUR1, 0, 0);
			time1->tm_min = GetDlgItemInt(hDlg, IDC_MIN1, 0, 0);
			time1->tm_sec = GetDlgItemInt(hDlg, IDC_SEC1, 0, 0);
			T1 = mktime(time1);
			int step = GetDlgItemInt(hDlg, IDC_STEP, 0, 0);
			if (T1 == -1 || T2 == -1|| step==0)
			{
				SetDlgItemTextA(hDlg, IDC_TEXT, "Неверный формат времени или дата ранее 1.2.1970");
				break;
			}
			int d = (int) difftime(T2,T1);						
			FILE *fw;
			char FileName[30];
			GetDlgItemTextA(hDlg, IDC_FILENAME, FileName, 20);
			if (strcmp(FileName, "") == 0)
			{
				SetDlgItemTextA(hDlg, IDC_TEXT, "Не задано имя файла");
				break;
			}
			string sFileName = ("Tags\\" + string(FileName) +".csv");
			fopen_s(&fw, sFileName.c_str(), "w");
			if (!fw)
			{
				SetDlgItemTextA(hDlg, IDC_TEXT, "Ошибка открытия файла");
				break;
			}
			WriteFindValues(3, TagArr, fw, T1, T2, GetDlgItemInt(hDlg, IDC_STEP, 0, 0));
			fclose(fw);
		}
		case IDC_WRITE1:
		{
			FILE *fw;
			fopen_s(&fw, "test", "w");
			string N, D, U;
			N = "," + SelectedTags[0].Name + "\n";
			fputs(N.c_str(), fw);								//запись заголовков	
			D = "," + SelectedTags[0].Description + "\n";
			fputs(D.c_str(), fw);							//запись описания Тега
			U = "," + SelectedTags[0].Units + "\n";
			fputs(U.c_str(), fw);								//запись ед. измерения Тега
			SelectedTags[0].WriteTag(fw);		//считывание и запись в новый файл	данных							
			fclose(fw);
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	
	return (INT_PTR)FALSE;
}
//
INT_PTR CALLBACK TagWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_READ:
		{
			//**************************************
			//получение списка csv файлов в текущей директории
			//**************************************
			WIN32_FIND_DATAA findData;
			HANDLE hf;
			string path = "*.csv";
			hf = FindFirstFileA(path.c_str(), &findData);
			if (hf == INVALID_HANDLE_VALUE)
			{
				cout << "Cannot find file" << endl;
				return -1;
			}
			string Files[1000];
			int k = 0;
			do
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				{
					Files[k] = findData.cFileName;
					k++;
				}
			} while (FindNextFileA(hf, &findData));
			FindClose(hf);

			//********************************************************

			//short TagIndex = 0;
			FILE *fp;
			for (int f = 0; f < k; f++)
			{
				const char* TagGroupFileName = Files[f].c_str();
				fopen_s(&fp, TagGroupFileName, "r");				
				if (fp)
				{
					for (int i = 0; i < 8; i++)
					{									//считывание заголовков            
						TagArr[f * 8 + i].ReadHeader(fp, i);
						string Mes = "Идет обработка файла: " + Files[f];
						SetDlgItemTextA(hDlg, IDC_LOGER, Mes.c_str());
						TagArr[f * 8 + i].ReadValues(fp, i);
						//TagName[f * 8 + i] = TagArr[f * 8 + i].Name;
						string TagFileNameStr= TagArr[f * 8 + i].Name;
						for (std::string::size_type n = 0; (n = TagFileNameStr.find(".", n)) != std::string::npos; ++n)
						{
							TagFileNameStr.replace(n, 1, 1, '_'); // замена точки в имени файла на подчеркивание
						}
						TagArr[f * 8 + i].Path = "Tags\\" + string(TagGroupFileName) + "\\" + TagFileNameStr + "\\" + TagFileNameStr + ".csv";
					}
				}
				fclose(fp);
			}
			string Mes = "Считывание завершено";
			SetDlgItemTextA(hDlg, IDC_LOGER, Mes.c_str());
		}
		break;
		case IDC_EXPL: 
		{
			//**************************************
			//получение списка csv файлов в текущей директории
			//**************************************
			WIN32_FIND_DATAA findData;
			HANDLE hf;
			string path = "*.csv";
			hf = FindFirstFileA(path.c_str(), &findData);
			if (hf == INVALID_HANDLE_VALUE)
			{
				cout << "Cannot find file" << endl;
				return -1;
			}
			string Files[1000];
			int k = 0;
			do
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				{
					Files[k] = findData.cFileName;
					k++;
				}
			} while (FindNextFileA(hf, &findData));
			FindClose(hf);

			//********************************************************
			
			//short TagIndex = 0;
			FILE *fp, *fw;			
			for (int f = 0; f < k; f++) 
			{
				const char* TagGroupFileName = Files[f].c_str();				
				fopen_s(&fp, TagGroupFileName, "r");
				string Mes = "Идет обработка файла: " + Files[f];
				if (!fp) Mes = "Ошибка открытия файла: " + Files[f];
				SetDlgItemTextA(hDlg, IDC_LOGER, Mes.c_str());
				if (!fp) perror("Ошибка открытия файла");
				if (fp)
				{
					for (int i = 0; i < 8; i++)
					{									//считывание заголовков            
						TagArr[f * 8 + i].ReadHeader(fp, i);
						TagName[f * 8 + i] = TagArr[f * 8 + i].Name;
						if (TagArr[f * 8 + i].Name != "")
						{
							string TagFileNameStr = TagArr[f * 8 + i].Name;
							//*************************************************
							for (std::string::size_type n = 0; (n = TagFileNameStr.find(".", n)) != std::string::npos; ++n)
							{
								TagFileNameStr.replace(n, 1, 1, '_'); // замена точки в имени файла на подчеркивание
							}
							//**************************************************
							
							//*****************************
							//формирование шапки таблицы
							//*****************************
							string TagFileNameStr_ = ',' + TagFileNameStr + '\n';
							string TagDescrStr = ',' + TagArr[f * 8 + i].Description + '\n';
							string TagMeshStr = ',' + TagArr[f * 8 + i].Units + '\n';
							//******************
							//перевод string-> const char*
							//******************
							const char* TagName = TagFileNameStr_.c_str();
							const char* TagMesh = TagMeshStr.c_str();
							const char* TagDescr = TagDescrStr.c_str();
							//*****************************************							
							TagArr[f * 8 + i].Path = "Tags\\" + string(TagGroupFileName) + "\\" + TagFileNameStr + "\\" + TagFileNameStr + ".csv";
							const char* TagFileName = TagArr[f * 8 + i].Path.c_str();
							string Dir1 = "Tags\\" + string(TagGroupFileName);
							string Dir = "Tags\\" + string(TagGroupFileName) + "\\" + TagFileNameStr;
							//string commandLine = "MD Tags\\" + string(TagGroupFileName) + "\\" + TagFileNameStr + "\\";
							//const char* chComLine = commandLine.c_str();
							//TagArr[TagIndex++] = TagArr[f * 8 + i];
							//**********************************************
							hf = FindFirstFileA(TagFileName, &findData);	//проверка на существование пути
							if (hf == INVALID_HANDLE_VALUE) 
							{
								CreateDirectoryA("Tags", NULL);
								CreateDirectoryA(Dir1.c_str(), NULL);
								CreateDirectoryA(Dir.c_str(), NULL);
							}
							FindClose(hf);
							//**********************************************
							fopen_s(&fw, TagFileName, "w");
							fputs(TagName, fw);								//запись заголовков		
							fputs(TagDescr, fw);							//запись описания Тега
							fputs(TagMesh, fw);								//запись ед. измерения Тега
							while (!feof(fp)) { TagArr[8*f+i].WriteValues(fp, fw, i); }		//считывание и запись в новый файл	данных							
							fclose(fw);										//закрытие редактируемого файла
						}
					}
					fclose(fp);
					SetDlgItemTextA(hDlg, IDC_LOGER, "");
				}
			}
			break;
		}
		case IDC_TIMERANGE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hDlg, TimeWindow);
			break;
		case IDCANCEL:
		case IDC_WRITE: 
		{
			FILE *fw;
			fopen_s(&fw, "test", "w");
			string N, D, U;
			N = ","+ TagArr[0].Name +"\n";
			fputs(N.c_str(), fw);								//запись заголовков	
			D = "," + TagArr[0].Description + "\n";
			fputs(D.c_str(), fw);							//запись описания Тега
			U = "," + TagArr[0].Units + "\n";
			fputs(U.c_str(), fw);								//запись ед. измерения Тега
			TagArr[0].WriteTag(fw);		//считывание и запись в новый файл	данных							
			fclose(fw);
		}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//
int GetTagByName(Tag TagAll[],Tag &TagResponse, char* Name, int N)
{	
	for (int i = 0; i<N; i++)
	{
		Tag CurTag = TagAll[i];
		if (strcmp(CurTag.Name.c_str(), Name)==0) TagResponse = CurTag;//j++;		
	}
	if (strcmp(TagResponse.Name.c_str(), "") == 0) { return 1; }
	return 0;	
}
int WriteFindValues(int _numTags, Tag TagAll[],  FILE * fw, time_t _tbegin, time_t _tend, int _step)
{
	//FILE *fp;
	struct tm	*time = localtime(&_tbegin);
	time_t		curTime;
	char strTime[20];
	string line;
	for (int i = 0; i<_numTags; i++)
	{
		Tag CurTag = TagAll[i];
		line += "," + CurTag.Name;

	}	
	line += "\n";
	fputs(line.c_str(), fw);				//Запись Имен
	line = "";
	for (int i = 0; i<_numTags; i++)
	{
		Tag CurTag = TagAll[i];
		line += "," + CurTag.Description;

	}
	line += "\n";
	fputs(line.c_str(), fw);				//Запись Описаний
	line = "";
	for (int i = 0; i<_numTags; i++)
	{
		Tag CurTag = TagAll[i];
		line += "," + CurTag.Units;

	}
	line += "\n";
	fputs(line.c_str(), fw);				//Запись единиц измерений
	line = "";
	int d = (int)difftime(_tend, _tbegin);
	for (int i = 0; d >= _step; i++)
	{
		time->tm_sec += _step;
		curTime = mktime(time);
		strftime(strTime, 20, "%d/%m/%Y %X", time);
		line = string(strTime);
		for (int i = 0; i<_numTags; i++)
		{
			Tag CurTag = TagAll[i];
			//fopen_s(&fp, CurTag.Path.c_str(), "r");		
			CurTag.Value.Date = string(strTime);
			d = (int)difftime(_tend, curTime);
			CurTag.Value.Val = CurTag.ReadValueByDateTime(CurTag.Value.Date);
			line += "," + CurTag.Value.Val;
			//fclose(fp);
		}		
		line += "\n";
		fputs(line.c_str(), fw);
	}	
	return 0;
}