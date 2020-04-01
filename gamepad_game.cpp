#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma _CRT_SECURE_NO_WARNINGS

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
ID2D1Factory* pD2DFactory = NULL;
ID2D1SolidColorBrush* brush;

#define ID_TIMER1 101

int bar1Y, ballX,ballY,ball_speedX,ball_speedY;

void init_game(void);
void render_scene(void);
void ball_update(void);

//***********STANDARD WIN32API************

ID2D1HwndRenderTarget* pRT = NULL;
#define HIBA_00 TEXT("Error:Program initialisation process.")
HINSTANCE hInstGlob;
int SajatiCmdShow;
HWND Form1; //Windows handler
LRESULT CALLBACK WndProc0(HWND, UINT, WPARAM, LPARAM);
//******************************************************

//*********************************
//main program entry point
//*********************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
 static TCHAR szAppName[] = TEXT("StdWinClassName");
 HWND hwnd;
 MSG msg;
 WNDCLASS wndclass0;
 SajatiCmdShow = iCmdShow;
 hInstGlob = hInstance;

 //*********************************
 //Preparing of the window
 //*********************************
 wndclass0.style = CS_HREDRAW | CS_VREDRAW;
 wndclass0.lpfnWndProc = WndProc0;
 wndclass0.cbClsExtra = 0;
 wndclass0.cbWndExtra = 0;
 wndclass0.hInstance = hInstance;
 wndclass0.hIcon = LoadIcon(NULL, IDI_APPLICATION);
 wndclass0.hCursor = LoadCursor(NULL, IDC_ARROW);
 wndclass0.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
 wndclass0.lpszMenuName = NULL;
 wndclass0.lpszClassName = TEXT("WIN0");

 //*********************************
 //Registrating the window
 //*********************************
 if (!RegisterClass(&wndclass0))
 {
  MessageBox(NULL, HIBA_00, TEXT("Program Start"), MB_ICONERROR);
  return 0;
 }

 //*********************************
 //Creating the window
 //*********************************
 Form1 = CreateWindow(TEXT("WIN0"),
  TEXT("Gampead programming – Sample program"),
  (WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
  0,
  0,
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  NULL,
  NULL,
  hInstance,
  NULL);

 //*********************************
 //Showing the window
 //*********************************
 ShowWindow(Form1, SajatiCmdShow);
 UpdateWindow(Form1);

 //*********************************
 //Make the window working
 //*********************************
 while (GetMessage(&msg, NULL, 0, 0))
 {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 return msg.wParam;
}

//*********************************
//Message handling
//*********************************
LRESULT CALLBACK WndProc0(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 HDC hdc;
 PAINTSTRUCT ps;
 unsigned int yPos;

 switch (message)
 {
 //*********************************
 //Window creation
 //*********************************
 case WM_CREATE:
  D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
  pD2DFactory->CreateHwndRenderTarget(
   D2D1::RenderTargetProperties(),
   D2D1::HwndRenderTargetProperties(
    hwnd, D2D1::SizeU(SCREEN_WIDTH, SCREEN_HEIGHT)),
   &pRT);
  pRT->CreateSolidColorBrush(
   D2D1::ColorF(D2D1::ColorF::Black, 1.0f),
   &brush);
  init_game();
  if ((joyGetNumDevs()) > 0) joySetCapture(hwnd, JOYSTICKID1, NULL, FALSE);
  SetTimer(hwnd,ID_TIMER1,10,(TIMERPROC)NULL);
  return 0;
 //*********************************
 //Against flickering…
 //*********************************
 case WM_ERASEBKGND:
  return (LRESULT)1;
 case WM_TIMER:
  switch (wParam)
  {
  case ID_TIMER1:
   ball_update();
   render_scene();
   break;
  }
  return 0;
 case MM_JOY1MOVE:
  yPos = HIWORD(lParam);
  if (yPos == 65535) {
   if (bar1Y < SCREEN_HEIGHT - 80)
   {
    bar1Y += 10;
   }   
  }
  else if (yPos == 0) {
   if (bar1Y > 0)
   {
    bar1Y -= 10;
   }
  }
  break;
 //*********************************
 //Redrawing the window
 //*********************************
 case WM_PAINT:
  hdc = BeginPaint(hwnd, &ps);
  EndPaint(hwnd, &ps);
  render_scene();
  return 0;
 //*********************************
 //Closing the window
 //*********************************
 case WM_CLOSE:
  KillTimer(hwnd, ID_TIMER1);
  pRT->Release();
  brush->Release();
  pD2DFactory->Release();
  DestroyWindow(hwnd);
  return 0;
 //*********************************
 //Destroying the window
 //*********************************
 case WM_DESTROY:
  PostQuitMessage(0);
  return 0;
 }
 return DefWindowProc(hwnd, message, wParam, lParam);
}

//********************************
//Initial settings
//********************************
void init_game(void)
{
 bar1Y=SCREEN_HEIGHT/2;
 ballX = SCREEN_WIDTH / 2;
 ballY = SCREEN_HEIGHT / 2;
 ball_speedX = 4;
 ball_speedY = 2;
}

void render_scene(void)
{
 pRT->BeginDraw();
 pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
 D2D1_RECT_F rectangle = D2D1::RectF(
  10.0f,
  bar1Y,
  30.0f,
  bar1Y+70);
 pRT->FillRectangle(&rectangle, brush);
 D2D1_RECT_F rectangle3 = D2D1::RectF(
  ballX,
  ballY,
  ballX+10,
  ballY+10);
 pRT->FillRectangle(&rectangle3, brush);
 pRT->EndDraw();
}

void ball_update(void)
{
 //geneal movements
 ballX += ball_speedX;
 ballY += ball_speedY;
 if (ballX < 0) init_game();
 else if (ballX > SCREEN_WIDTH - 10) {
  ball_speedX += 2;
  if (ball_speedX == 15) ball_speedX = 14;
  ball_speedX *= -1;
  }
 
 if (ballY > SCREEN_HEIGHT - 10) ball_speedY *= -1;
 else if (ballY < 10) ball_speedY *= -1;
 
 //collision detection
 if (ballX < 30 && ballX > 10 && ballY >= bar1Y && ballY <= bar1Y + 70) ball_speedX *= -1;
}
