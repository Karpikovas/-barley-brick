#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WFunc (HWND,UINT,WPARAM,LPARAM);
 
void CreateMas(HWND[],int[]);
bool GetResult(int m[]);

const size_t N = 10;
char str[N] = "";
 
HINSTANCE hInst;
HWND hwnd;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, 
    LPSTR lpCmdLine, int nCmdShow)
{
    
    hInst = hInstance;
    WNDCLASS win;
    win.style=CS_HREDRAW|CS_VREDRAW;
    win.lpfnWndProc=WFunc;
    win.cbClsExtra=win.cbWndExtra=0;
    win.hInstance=hInstance;
    win.hIcon=LoadIcon(NULL,IDI_ASTERISK);
    win.hCursor=LoadCursor(NULL,IDC_ARROW);
    win.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    win.lpszMenuName=NULL;
    win.lpszClassName="Qwerty";

    if (!RegisterClass(&win))
    {
        MessageBoxW(NULL,L"Окно не зарегистрировано",L"Ошибка",MB_OK);
        return -1;
    }

    hwnd=CreateWindowW(L"Qwerty",L"Пятнашки",WS_OVERLAPPEDWINDOW,50,50,
                      500,600,HWND_DESKTOP,NULL,hInstance,NULL);
 
    ShowWindow(hwnd,nCmdShow); 
    UpdateWindow (hwnd);
 
    MSG msg;
    while (GetMessage(&msg,0,0,0))
    {
        TranslateMessage(&msg);  
        DispatchMessage(&msg); 
    }
    return msg.wParam; 
}
 
LRESULT CALLBACK WFunc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hBut[16];
    static HWND but;
    static int mas[16];
    PAINTSTRUCT ps;
    HDC hdc; 
    static int sx,sy;
    int i,j;
 
    switch (msg)
    {
    
        case WM_DESTROY:
            PostQuitMessage(0); 
            break;
 
        case WM_CREATE:
        {
        
            but=CreateWindowW(L"BUTTON",L"Перемешать",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,100,500,300,50,hwnd,0,hInst,0);
       
        
            for(i = 0; i < 16; i++)
                hBut[i]=CreateWindow("BUTTON","",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,0,0,0,hwnd,0,hInst,0);
            //CreateMas(hBut,mas);
            for (i = 0; i < 13; i++)
            {
                mas[i] = i+1;
            }
            mas[13] = 0;
            mas[14] = 14;
            mas[15] = 15;
            char s[N]="";
            for(int i = 0; i < 16;i++)
            if(mas[i]!=0)
                {
                    sprintf(s,"%d",mas[i]);
                    SetWindowText(hBut[i],s);
                }
            else
                SetWindowText(hBut[i],"");
            break;
        }
 
        case WM_SIZE:
            sx = LOWORD(lParam)/4;
            sy = sx;
            break;
 
        case WM_COMMAND:
        
            for(i = 0; i < 16; i++){
                if(HWND(lParam)==hBut[i])
                {
                if(mas[i-4]==0 && i>3)         
                    {
                        GetWindowText(hBut[i],str, N);
                        SetWindowText(hBut[i-4],str);
                        SetWindowText(hBut[i],"");
                        mas[i-4]= mas[i];
                       	mas[i] = 0; 
                     }
                
                if(mas[i-1]==0 && i%4!=0)         
                    {
                        GetWindowText(hBut[i],str,N);
                        SetWindowText(hBut[i-1],str);
                        SetWindowText(hBut[i],"");
                       	mas[i-1] = mas[i];
                       	mas[i] = 0;
                    }   
                if(mas[i+1]==0 && (i+1)%4!=0)         
                    {
                        GetWindowText(hBut[i],str, N);
                        SetWindowText(hBut[i+1],str);
                        SetWindowText(hBut[i],"");
                        mas[i+1] = mas[i];
                        mas[i] = 0;
                    }   
                if(mas[i+4]==0 && i < 12)         
                    {
                        GetWindowText(hBut[i],str, N);
                        SetWindowText(hBut[i+4],str);
                        SetWindowText(hBut[i],"");
                        mas[i+4] = mas[i];
                        mas[i] = 0;
                    }  
                }

            }
            if (HWND(lParam)==but){
            	if(MessageBoxW(0, L"Начать заново?", TEXT(L"Игра закончена!"), MB_YESNO) == IDYES){
                     for(int i = 0; i < 16; i++)
                        {
                            if (IsWindow(hBut[i]))
                                    SendMessage(hBut[i],WM_CLOSE,0,0);
                            hBut[i]=CreateWindow("BUTTON","",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,0,0,0,hwnd,0,hInst,0);
                            }
                    CreateMas(hBut,mas);
                }	
            }

            if (GetResult(mas)){
            	MessageBoxW(NULL, L"Вы выиграли!", TEXT(L"Победа!"), MB_OK);
                for(int i = 0; i < 16; i++)
                {
                    if (IsWindow(hBut[i]))
                        SendMessage(hBut[i],WM_CLOSE,0,0);
                    hBut[i]=CreateWindow("BUTTON","",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,0,0,0,hwnd,0,hInst,0);
                }
                CreateMas(hBut,mas);
            }
            break;
 
    case WM_PAINT:  
        hdc=BeginPaint(hwnd,&ps);
        for(int i = 0;i < 16; i++)
        	MoveWindow(hBut[i],(i%4)*sx,(i/4)*sy,sx,sy,1);
        EndPaint(hwnd,&ps);
        break;
 
    default:
        return
            DefWindowProc(hwnd,msg,wParam,lParam);//возврат неотработанного сообщения
    }
    return 0L;
}
 
void CreateMas(HWND h[],int m[])
{
    int buf;
    int pos, pos2;
    char s[N]="";

    for(int i = 0; i < 16; i++)
        m[i] = i;

    for (int i = 0; i < rand() % 50 + 10; i++)
    {
        pos = rand() % 15;
        pos2 = rand() % 15;
        int buf = m[pos];
        m[pos] = m[pos2];
        m[pos2] = buf;
    }  
    for(int i = 0; i < 16; i++)
        if(m[i]!=0)
            {
                sprintf(s,"%d",m[i]);
                SetWindowText(h[i],s);
            }
        else
            SetWindowText(h[i],"");

}

bool GetResult(int m[])
{
	bool res = true;
	for(int i = 0;i < 15;i++){
		if (m[i] != i+1)
			res = false;
	}
	return res;
}