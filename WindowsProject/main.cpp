#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include "framework.h"
#include "AI-arith.h"

/* ��¼��ǰ״̬��Ϣ�ı���ָ�� */
state* t;

/* �����ʾ������ɫ�ĺ� */
#define UNKNOW 0
#define BLACK -1
#define WHITE 1

/* ���崰�ڿ�Ⱥ͸߶� */
#define MY_WIDTH 700
#define MY_HEIGHT 670

/* ����һ������ */
LONG pieceWidth = 100;	//���ӿ��
typedef struct PIECE {
	int i;
	int j;
	int color;
}Piece[255];
Piece pieces;
int ip = 0;	//��ǰ������

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	int i, j;

	switch (msg)
	{
		/* ���ڴ�С�ı� */
	case WM_SIZE:
		static int cxClient = LOWORD(lParam);
		static int cyClient = HIWORD(lParam);
		//��ȡ���ӿ��
		pieceWidth = cxClient / 18;
		return 0;

		/* ������� */
	case WM_LBUTTONDOWN:
		POINT ptClick;

		//��ȡ����λ��
		ptClick.x = GET_X_LPARAM(lParam);
		ptClick.y = GET_Y_LPARAM(lParam);

		//�������Ӱ���λ��
		i = (ptClick.x + pieceWidth / 2) / pieceWidth - 2;
		j = (ptClick.y + pieceWidth / 2) / pieceWidth - 1;

		//�ж����Ӱ���λ���Ƿ����
		if (i < 0 || i > 14 || j < 0 || j > 14 || t->CB[i][j] != UNKNOW)
			return 0;

		//�����������������Ӷ���
		pieces[ip].i = i;
		pieces[ip].j = j;
		pieces[ip].color = BLACK;
		t->CB[i][j] = BLACK;
		ip++;

		//�ػ�
		SetRect(&rect, pieceWidth * (i + 2) - pieceWidth / 2, pieceWidth * (j + 1) - pieceWidth / 2,
			pieceWidth * (i + 2) + pieceWidth / 2, pieceWidth * (j + 1) + pieceWidth / 2);
		InvalidateRect(hWnd, &rect, FALSE);

		//Ŀ�����
		if (t->GoalTest() == BLACK)
			if (MessageBox(hWnd, TEXT("�������"), TEXT("������С��Ϸ"), MB_OK) == IDOK)
				DestroyWindow(hWnd);

		//����Ӧ�Բ���
		t = t->minimax(2);	//����Ӧ�Բ���
		t->clear();	//���³�ʼ������һ����������ʱ����

		//��AI�����������Ӷ���
		pieces[ip].i = t->Last_i;
		pieces[ip].j = t->Last_j;
		pieces[ip].color = WHITE;
		t->CB[t->Last_i][t->Last_j] = WHITE;
		ip++;

		//�ػ�
		SetRect(&rect, pieceWidth * (t->Last_i + 2) - pieceWidth / 2, pieceWidth * (t->Last_j + 1) - pieceWidth / 2,
			pieceWidth * (t->Last_i + 2) + pieceWidth / 2, pieceWidth * (t->Last_j + 1) + pieceWidth / 2);
		InvalidateRect(hWnd, &rect, FALSE);

		//Ŀ�����
		if (t->GoalTest() == WHITE)
			if (MessageBox(hWnd, TEXT("�����ˣ������٣�"), TEXT("������С��Ϸ"), MB_OK) == IDOK)
				DestroyWindow(hWnd);

		return 0;

		/* �����Ҽ� */
	case WM_RBUTTONDOWN:
		if (ip == 0)
			return 0;

		for (int k = 0; k < 2; ++k) {
			//���ö�������
			ip--;
			i = pieces[ip].i;
			j = pieces[ip].j;
			t->CB[i][j] = UNKNOW;
			pieces[ip].i = 0;
			pieces[ip].j = 0;
			pieces[ip].color = UNKNOW;

			//�ػ�
			SetRect(&rect, pieceWidth * (i + 2) - pieceWidth / 2, pieceWidth * (j + 1) - pieceWidth / 2,
				pieceWidth * (i + 2) + pieceWidth / 2, pieceWidth * (j + 1) + pieceWidth / 2);
			InvalidateRect(hWnd, &rect, TRUE);
		}

		return 0;

		/* ���ڵȴ�ˢ�� */
	case WM_PAINT:
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		//��������
		for (int i = 0; i < 15; ++i) {
			MoveToEx(hdc, pieceWidth * (i + 2), pieceWidth * 1, NULL);
			LineTo(hdc, pieceWidth * (i + 2), pieceWidth * 15);
		}
		for (int j = 0; j < 15; ++j) {
			MoveToEx(hdc, pieceWidth * 2, pieceWidth * (j + 1), NULL);
			LineTo(hdc, pieceWidth * 16, pieceWidth * (j + 1));
		}

		//��������
		HBRUSH hBlackBrush, hWhiteBrush;
		hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
		hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
		for (int i = 0, j = 0, k = 0; k < ip; ++k) {
			if (pieces[k].color == UNKNOW)
				continue;
			if (pieces[k].color == BLACK)
				SelectObject(hdc, hBlackBrush);
			if (pieces[k].color == WHITE)
				SelectObject(hdc, hWhiteBrush);
			int m = pieces[k].i, n = pieces[k].j;
			Ellipse(hdc, pieceWidth * (m + 2) - pieceWidth / 2, pieceWidth * (n + 1) - pieceWidth / 2,
				pieceWidth * (m + 2) + pieceWidth / 2, pieceWidth * (n + 1) + pieceWidth / 2);
		}
		EndPaint(hWnd, &ps);

		return 0;

		/* ���ڹر� */
	case WM_CLOSE:
		if (MessageBox(hWnd, TEXT("��Ϸ��������л֧�֣�"), TEXT("������С��Ϸ"), MB_OK) == IDOK)
			DestroyWindow(hWnd);

		return 0;

		/* ���ڱ��ݻ� */
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;

		/* ������ť/�˵���/������ */
	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			MessageBox(hWnd, TEXT("��Ϸ������\n\t������� - ����\n\t�����Ҽ� - ����\n\n��Ϸ������\n\t���Ʒ�"),
				TEXT("������С��Ϸ"), MB_OK);
			break;
		case IDM_EXIT:
			if (MessageBox(hWnd, TEXT("��Ϸ��������л֧�֣�"), TEXT("������С��Ϸ"), MB_OK) == IDOK)
				DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR szCmdLine, int iCmdShow)
{
	/* �����㷨�ڲ����� */
	t = new state;

	/* ���崰���� */
	WNDCLASS wndClass;	//���崰����
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//�������񣺵������ڴ�Сʱ�Ƿ��ػ�
	wndClass.lpfnWndProc = WndProc;	//���ڹ��̺���
	wndClass.cbClsExtra = 0;	//ָ��������ṹ��Ҫ����Ķ����ֽ���
	wndClass.cbWndExtra = 0;	//ָ������ʵ���ṹ��Ҫ����Ķ����ֽ���
	wndClass.hInstance = hInstance;	//ʵ�����
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//ͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//���
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//����ˢ
	wndClass.lpszMenuName = MAKEINTRESOURCEW(IDC_FIVEINAROW);	//�˵�
	wndClass.lpszClassName = TEXT("�˹�����ԭ���뼼��");	//����������

	/* ע�ᴰ���� */
	if (!RegisterClass(&wndClass)) {
		MessageBox(NULL, TEXT("����ע��ʧ�ܣ�"), TEXT("�˹�����ԭ���뼼��"), MB_ICONERROR);
		return 0;
	}

	/* �������� */
	HWND hWnd = CreateWindow(TEXT("�˹�����ԭ���뼼��"), TEXT("������С��Ϸ"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		0, 0, MY_WIDTH, MY_HEIGHT,
		NULL, NULL, hInstance, NULL);

	/* ��ʾ���� */
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	/* ��Ϣѭ�� */
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);	//���ô��ڹ��̺���
	}

	return msg.wParam;
}