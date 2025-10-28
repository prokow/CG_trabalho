#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>  

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
//Configura o contexto de renderização OpenGL para a janela especificada.
// Pré-condição: A janela (HWND) deve ter sido criada e ser válida. hDC e hRC devem ser ponteiros para HDC* e HGLRC* (podem ser nullptr).
// Pós-condição: Cria um Contexto de Dispositivo (HDC) e um Contexto de Renderização (HGLRC) para a janela e os torna atuais. Os ponteiros hDC e hRC apontam para os novos handles.
void EnableOpenGL(HWND hwnd, HDC*& hDC, HGLRC*& hRC);

//Desativa e libera os recursos do OpenGL (HGLRC e HDC).
// Pré-condição: hDC e hRC devem ser handles válidos obtidos por EnableOpenGL.
// Pós-condição: O contexto de renderização (HGLRC) é desativado e deletado, e o Contexto de Dispositivo (HDC) é liberado.
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC);


//A função principal do programa, usada para criar uma janela e renderizar uma pirâmide 3D em wireframe (contorno) usando OpenGL. Esta função parece ser uma alternativa ou uma versão mais antiga da 'WinMain' no mesmo arquivo.
// Pré-condição: A biblioteca OpenGL deve estar disponível. O sistema operacional deve ser Windows.
// Pós-condição: Cria uma janela, executa o loop de renderização da pirâmide em wireframe (rotacionando) até que o usuário feche a janela ou pressione ESC, libera os recursos do OpenGL e fecha o programa.
int WINAPI Poligono(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wcex{};
    HWND hwnd;
    HDC* hDC = nullptr;
    HGLRC* hRC = nullptr;
    MSG msg{};
    bool bQuit = false;
    float theta = 0.0f;

    // Registrar classe da janela
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = TEXT("GLSample");
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) return 0;

    // Criar janela principal
    hwnd = CreateWindowEx(0, TEXT("GLSample"), TEXT("Atividade 4 - Desenho de Poligono 3D"),
                          WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          800, 800, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, nCmdShow);

    // Ativar OpenGL
    EnableOpenGL(hwnd, hDC, hRC);

    // Configuração de projeção 3D
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.0, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop principal
    while (!bQuit)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                bQuit = true;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Limpar tela e z-buffer
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRotatef(theta, 1.0f, 1.0f, 0.0f);

            // Desenhar pirâmide com contorno
            glBegin(GL_LINE_LOOP);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glEnd();

            // Base quadrada
            glBegin(GL_LINE_LOOP);
            glVertex3f(-1.0f, -1.0f,  1.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glEnd();

            glPopMatrix();

            SwapBuffers(*hDC);
            theta += 1.0f;
            Sleep(1);
        }
    }

    DisableOpenGL(hwnd, *hDC, *hRC);
    DestroyWindow(hwnd);
    return static_cast<int>(msg.wParam);
}


//Função de procedimento de janela (Window Procedure). Processa mensagens enviadas ou postadas para a janela.
// Pré-condição: Deve ser registrada na estrutura WNDCLASSEX antes da criação da janela.
// Pós-condição: Retorna 0 após processar a mensagem, ou o resultado de DefWindowProc se não processar a mensagem.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void EnableOpenGL(HWND hwnd, HDC*& hDC, HGLRC*& hRC)
{
    PIXELFORMATDESCRIPTOR pfd{};
    int iFormat;

    hDC = new HDC;
    *hDC = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    hRC = new HGLRC;
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


//A função principal do programa, usada para criar uma janela e renderizar uma pirâmide 3D em wireframe (preenchida) usando OpenGL
// Pré-condição: A biblioteca OpenGL deve estar disponível. O sistema operacional deve ser Windows.
// Pós-condição: Cria uma janela, executa o loop de renderização da pirâmide em wireframe (rotacionando) até que o usuário feche a janela ou pressione ESC, libera os recursos do OpenGL e fecha o programa.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wcex{};
    HWND hwnd;
    HDC* hDC = nullptr;
    HGLRC* hRC = nullptr;
    MSG msg{};
    bool bQuit = false;
    float theta = 0.0f;

    // Registrar classe da janela
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszClassName = TEXT("GLSample");
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) return 0;

    // Criar janela principal
    hwnd = CreateWindowEx(0, TEXT("GLSample"), TEXT("Atividade 5 - Preenchimento de Poligono 3D"),
                          WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          800, 800, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, nCmdShow);

    // Ativar OpenGL
    EnableOpenGL(hwnd, hDC, hRC);

    // Configuração de projeção 3D
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.0, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop principal
    while (!bQuit)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                bQuit = true;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Limpar tela e z-buffer
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -5.0f);
            glRotatef(theta, 1.0f, 1.0f, 0.0f);

            // Desenhar pirâmide com contorno (wireframe)
            glBegin(GL_TRIANGLES);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glEnd();

            // Base quadrada
            glBegin(GL_QUADS);
            glVertex3f(-1.0f, -1.0f,  1.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glEnd();

            glPopMatrix();

            SwapBuffers(*hDC);
            theta += 1.0f;
            Sleep(1);
        }
    }

    DisableOpenGL(hwnd, *hDC, *hRC);
    DestroyWindow(hwnd);
    return static_cast<int>(msg.wParam);
}
