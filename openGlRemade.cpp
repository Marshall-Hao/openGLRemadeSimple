// openGlRemade.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "openGlRemade.h"
#include"Canvas.h"
#include"Image.h"
#include <math.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND    hWnd;
INT     wWidth = 800;
INT     wHeight = 600;
HDC     hDC;
HDC     hMem;
BITMAPINFO  bmpInfo;
HBITMAP hBmp;
void* buffer = 0;
GT::Canvas* _canvas = NULL;

GT::Image * _image = NULL;
GT::Image *_bkImage = NULL;
GT::Image* _zoomImage = NULL;
GT::Image* _zoomImageSimple = NULL;

float _angle = 0.0f;
float _xCam = 0.0f;
float _zRun = 0.0f;
float speed = 0.01;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Render();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENGLREMADE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLREMADE));

    /*===========创建绘图用的位图========*/
   

    hDC = GetDC(hWnd);
    // optimization
    hMem = CreateCompatibleDC(hDC);

    
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = wWidth;
    bmpInfo.bmiHeader.biHeight = wHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB; //实际上存储方式为bgr
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;

    // buffer is the thing for hMem INSIDE hDC
    hBmp = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, 0, 0);//在这里创建buffer的内存
    SelectObject(hMem, hBmp);
  
    memset(buffer, 0, wWidth * wHeight * 4); //清空buffer为0

    /*===========创建绘图用的位图========*/

    _canvas = new GT::Canvas(wWidth, wHeight, buffer);
    _image = GT::Image::readFromFile("res/sun.jpg");
    _bkImage = GT::Image::readFromFile("res/bk.jpg");
    _zoomImage = GT::Image::zoomImage(_image, 2, 2);
    _zoomImageSimple = GT::Image::zoomImageSimple(_image, 2, 2);
    GT::Image* _zoomImage = NULL;
    MSG msg;

    // 主消息循环:
    // 没有消息就不停的渲染
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Render();   
    }

    return (int) msg.wParam;
}

void Render()
{
    _canvas->clear();

    /*_canvas->setAlphaLimit(10);*/
    //_canvas->setBlend(true);
   // wwill always between 0 and wWidth -1
  /* _canvas->drawPoint(rand() % wWidth, rand() % wHeight, _color);
   */
   /*  for (int x = 0; x < wWidth; x++)
     {
         for (int y = 0; y < wHeight; y++)
         {
             GT::RGBA _color(rand() % 255, rand() % 255, rand() % 255);
             _canvas->drawPoint(x, y, _color);
         }


     }*/
     //_canvas->drawLine(GT::intV2(100, 100), GT::intV2(150, 180), GT::RGBA(255, 0, 0));


       /* GT::RGBA _color(rand() % 255, rand() % 255, rand() % 255);
        GT::intV2  pt1(100,100);*/

        /*    GT::Point pt1(200, 200, GT::RGBA(255,0,0));


            float dis = 100;
            for (float i = 0; i < 360;i += 20)
            {
                float radian = DEG2RAD(i);
                int x = dis * sin(radian) + pt1.m_x;
                int y = dis * cos(radian) + pt1.m_y;
                GT::Point pt2(x, y, GT::RGBA(0,255,0));
                _canvas->drawLine(pt1, pt2);
            }*/

            /*  GT::Point pt1(0, 0, GT::RGBA(255, 0, 0));
              GT::Point pt2(100, 50, GT::RGBA(255, 0, 0));
              GT::Point pt3(150, 200, GT::RGBA(255, 0, 0));

              _canvas->drawTriangle(pt1, pt2, pt3);*/

              //   // 极端情况  
              //GT::Point pt1(-5000, -5000, GT::RGBA(255, 0, 0));
              //GT::Point pt2(0, 5000, GT::RGBA(0, 255, 0));
              //GT::Point pt3(5000, -5000, GT::RGBA(0, 0, 255));

              //GT::Point pt1(120, 10, GT::RGBA(255, 0, 0));
              //GT::Point pt2(100, 100, GT::RGBA(0, 255, 0));
              //GT::Point pt3(150, 50, GT::RGBA(0, 0, 255));
           /*   _canvas->drawTriangle(pt1, pt2, pt3);*/

              // 画图

             //_canvas->drawImage(100, 100, _bkImage);

             /*_canvas->drawImage(50, 50, _image);
             _canvas->drawImage(10, 10, _zoomImage);
             _canvas->drawImage(200, 200, _zoomImageSimple);*/

 //   GT::Point ptArray1[] =
 //   {
 //       {0,0,GT::RGBA(255,0,0), GT::floatV2(0,0)},
 //       {500,0,GT::RGBA(0,255,0), GT::floatV2(1.0,0)},
 //       {500,300,GT::RGBA(0,0,255),GT::floatV2(1.0,1.0)}
 //   };

 //   GT::Point ptArray2[] =
 //   {
 //       {0,0,GT::RGBA(255,0,0), GT::floatV2(0,0)},
 //       {0,300,GT::RGBA(0,255,0), GT::floatV2(0.0,1.0)},
 //       {500,300,GT::RGBA(0,0,255),GT::floatV2(1.0,1.0)}
 //   };

 //   for (int i = 0; i < 3; i++)
 //   {
 //       ptArray1[i].m_uv.x += speed;
 //       ptArray2[i].m_uv.x += speed;
	//}

 //   speed += 0.08;
 //   _canvas->enableTexture(true);
 //   _canvas->bindTexture(_bkImage);
 //   _canvas->setTextureType(GT::Image::TX_REPEAT);
 //   _canvas->drawTriangle(ptArray1[0], ptArray1[1], ptArray1[2]);
 //   _canvas->drawTriangle(ptArray2[0], ptArray2[1], ptArray2[2]);
       

    //GT::Point ptArray[] =
    //     {
    //         {0.0f,200.0f,GT::RGBA(255,0,0), GT::floatV2(0,0)},
    //         {800.0f,200.0f,GT::RGBA(0,255,0), GT::floatV2(1.0,0)},
    //         {100.0f,600.0f,GT::RGBA(0,0,255),GT::floatV2(1.0,1.0)},
    //         {800.0f,300.0f,GT::RGBA(0,0,255),GT::floatV2(1.0,1.0)},
    //         {600.0f,150.0f,GT::RGBA(0,0,255),GT::floatV2(1.0,1.0)}
    //      }

    GT::Point ptArray[] =
    {
        {0.0f,0.0f,0.0f,       GT::RGBA(255,0,0) , GT::floatV2(0,0)},
        {300.0f,0.0f,0.0f,    GT::RGBA(0,255,0) , GT::floatV2(1.0,0)},
        {300.0f,300.0f,0.0f,   GT::RGBA(0,0,255) , GT::floatV2(1.0,1.0)},

        {300.0f,0.0f,0.0f,       GT::RGBA(255,0,0) , GT::floatV2(0,0)},
        {300.0f,300.0f,0.0f,    GT::RGBA(0,255,0) , GT::floatV2(0.0,0)},
        {300.0f,0.0f,-500.0f,   GT::RGBA(0,0,255) , GT::floatV2(0.0,0.0)},
        
    };
    //VP变换
    glm::mat4 vMat(1.0f);
    vMat = glm::lookAt(glm::vec3(0, 0, 1000), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glm::mat4 pMat(1.0f);
    pMat = glm::perspective(glm::radians(60.0f), (float)wWidth / (float)wHeight, 1.0f, 1000.0f);

    //       ptv4 = pMat * vMat * rMat * mMat * ptv4;


    //       ptArray[i].m_x = (ptv4.x / ptv4.w + 1.0) * (float)wWidth / 2.0;
    //       ptArray[i].m_y = (ptv4.y / ptv4.w + 1.0) * (float)wHeight / 2.;
    //       ptArray[i].m_z = ptv4.z / ptv4.w;
       //}
    _canvas->gtMatrixMode(GT::MatrixMode::GT_VIEW);

    _canvas->gtLoadMatrix(vMat);

    _canvas->gtMatrixMode(GT::MatrixMode::GT_PROJECTION);

    _canvas->gtLoadMatrix(pMat);

   /* for (int i = 0;i < 6;i++)
    {*/
        /*glm::vec4 ptv4(ptArray[i].m_x, ptArray[i].m_y, 0.0f, 1.0f);
        glm::mat4 rMat(1.0f);
        rMat = glm::rotate(rMat, glm::radians(_angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 tMat(1.0f);
        tMat = glm::translate(tMat, glm::vec3(200.0f, 200.0f, 0.0f));
        ptv4 = tMat* rMat * ptv4;
        ptArray[i].m_x = ptv4.x;
        ptArray[i].m_y = ptv4.y;*/

        // include the z temp
 //       glm::vec4 ptv4(ptArray[i].m_x, ptArray[i].m_y, ptArray[i].m_z, 1);


        glm::mat4 mMat(1.0f);
        mMat = glm::translate(mMat, glm::vec3(-300, 0, 0));

        glm::mat4 rMat(1.0f);
        // xyz
        rMat = glm::rotate(rMat, glm::radians(_angle), glm::vec3(0, 1, 0));
        rMat = glm::rotate(rMat, glm::radians(50.0f), glm::vec3(1, 0, 0));
   
 


    _canvas->gtMatrixMode(GT::MatrixMode::GT_MODEL);
    _canvas->gtLoadIdentity();
    _canvas->gtMulplyMatrix(mMat);
    _canvas->gtMulplyMatrix(rMat);

  
    

    _angle += 0.2f;
 


    // draw
    _canvas->gtVertexPointer(2, GT::GT_FLOAT, sizeof(GT::Point),(unsigned char*)ptArray);
    _canvas->gtColorPoionter(4, GT::GT_FLOAT, sizeof(GT::Point), (unsigned char*)&ptArray[0].m_color);
    _canvas->gtTexCoordPointer(2, GT::GT_FLOAT, sizeof(GT::Point), (unsigned char*)&ptArray[0].m_uv);

    /*_canvas->gtDrawArray(GT::GT_LINE, 0, 5);*/
    _canvas->enableTexture(true);
    _canvas->bindTexture(_bkImage);
    _canvas->gtDrawArray(GT::GT_TRIANGLE, 0, 6);

    BitBlt(hDC, 0, 0, wWidth, wHeight, hMem, 0, 0, SRCCOPY);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGLREMADE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
      100, 100, wWidth, wHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
        
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOVE:
        // Update wWidth, wHeight, or position if necessary
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
        break;
    //case WM_SIZE:
    //{
    //    // Get new dimensions
    //    wWidth = LOWORD(lParam);
    //    wHeight = HIWORD(lParam);

    //    if (hBmp) {
    //        DeleteObject(hBmp); // Delete old bitmap
    //    }

    //    // Create new bitmap
    //    bmpInfo.bmiHeader.biWidth = wWidth;
    //    bmpInfo.bmiHeader.biHeight = -wHeight; // Negative for top-down DIB
    //    hBmp = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
    //    SelectObject(hMem, hBmp);

    //    // Redraw window
    //    InvalidateRect(hWnd, NULL, TRUE);
    //    UpdateWindow(hWnd);
    //}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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
