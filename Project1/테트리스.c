#define _CRT_NONSTDC_NO_WARNINGS	// 비표준 함수를 사용했을 때 에러 방지
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <time.h>

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SPACE 32
#define ENTER 13
#define P 112
#define ESC 27
#define SPACE 32

#define ACTIVE -1
#define EMPTY 0
#define WALL 1
#define INACTIVE 2

#define false 0
#define true 1

#define BOARD_X 12	// 가로 10칸
#define BOARD_Y 22	// 세로 20칸

enum Color { BLUE = 9, YELLOWGREEN, SKY, RED, PINK, YELLOW, WHITE };
enum CURSOR_TYPE { NOCURSOR, SOLIDCURSOR, NORMALCURSOR };

typedef struct Block
{
	int block;
	int color;
} Block;

void title();
void gotoxy(int x, int y);
void setCursor(enum CURSOR_TYPE c);
void setColor(int text);
void reset();
void newBlock();
void drawBoard();
void drawInterface();
void moveBlock(int dir);
void dropBlock();
void checkKey();
int checkCrush(int x, int y, int rotation);
void checkLine();
void hardDrop();
void checkGameOver();
void checkLevelUp();

int score;	// 점수
int level;	// 레벨
int next_level_goal;	// 다음 레벨 목표 점수
int block_index;	// 현재 블록 모양
int next_block_index = 7;	// 다음 블록 모양
int rotation_index = 0;	// 회전 모양
int new_block_flag = 0;	// 새 블록의 필요 여부
int speed;
int key;	// 입력 키
int color;
const int startX = 5, startY = 4; // 시작 위치
int board_startX, board_startY;	// 게임판 시작 위치
int bx, by;	// 현재 블록의 위치 좌표
Block board[BOARD_Y][BOARD_X];	// 게임판 정보
Block board_temp[BOARD_Y][BOARD_X];	// 임시 게임판 정보
int blocks[7][4][4][4] = {	// 블록 정보
	{ { {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } },	// 네모 난 거
	{ { {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0} },
		{ {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0} } },	// 길쭉이
	{ { {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} } },	// Z
	{ { {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
		{ {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} } },	// Z 좌우반전
	{ { {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0 ,0} },
		{ {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} } },	// ㄴ 모양
	{ { {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} } },	// ㄴ 좌우반전
	{ { {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} },
		{ {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
		{ {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} } },	// ㅗ 모양
};	// 7가지의 모양, 4가지의 회전 모양, 4행 4열로 찍어낼 모양

int main(void)
{
	int i;
	srand((unsigned)time(NULL));
	setCursor(NOCURSOR);
	title();
	reset();
	newBlock();
	while (1)
	{
		for (i = 0; i < 5; i++)
		{
			checkKey();
			drawBoard();
			drawInterface();
			Sleep(speed);
		}
		dropBlock();
		checkLine();
		checkGameOver();
		checkLevelUp();
		if (new_block_flag == 1)
			newBlock();
	}
	return 0;
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void title()
{
	system("cls");
	gotoxy(startX, startY);
	printf("□□□□□□□□□□□□□□□□□□");
	Sleep(100);
	int i;
	for (i = 0; i < 9; i++)
	{
		if (i == 4)
		{
			gotoxy(startX, startY + (i + 1));
			printf("□            테트리스            □");
			Sleep(100);
			continue;
		}
		gotoxy(startX, startY + (i + 1));
		printf("□                                □");
		Sleep(100);
	}
	gotoxy(startX, startY + (++i));
	printf("□□□□□□□□□□□□□□□□□□");
	Sleep(100);
	gotoxy(startX, startY + (++i));
	Sleep(100);
	gotoxy(startX, startY + (++i));
	printf("      Press any key to start");
	Sleep(100);
	gotoxy(startX, startY + (i + 4));
	int j = 0;
	while (1)
	{
		++j;
		if (kbhit())
			break;
		else if (j == 300)
		{
			gotoxy(startX, startY + i);
			printf("      Press any key to start.");
		}
		else if (j == 600)
		{
			gotoxy(startX, startY + i);
			printf("      Press any key to start..");
		}
		else if (j == 900)
		{
			gotoxy(startX, startY + i);
			printf("      Press any key to start...");
		}
		else if (j == 1200)
		{
			gotoxy(startX, startY + i);
			printf("      Press any key to start   ");
			j = 0;
		}
		Sleep(1);
	}
	while (kbhit()) key = getch();	// 버퍼에 기록된 키값 비우기
	system("cls");
}

void setCursor(enum CURSOR_TYPE c)	// 커서 숨기기
{
	CONSOLE_CURSOR_INFO curInfo = { 0, };

	switch (c)
	{
		case NOCURSOR :
			curInfo.dwSize = 1;
			curInfo.bVisible = FALSE;
			break;
		case SOLIDCURSOR :
			curInfo.dwSize = 100;
			curInfo.bVisible = TRUE;
			break;
		case NORMALCURSOR :
			curInfo.dwSize = 20;
			curInfo.bVisible = TRUE;
			break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void reset()
{
	score = 0;
	level = 1;
	next_level_goal = 5;
	speed = 100;
	next_block_index = rand() % 7;

	int i, j;
	for (i = 0; i < BOARD_Y; i++)
	{
		for (j = 0; j < BOARD_X; j++)
		{
			if (i == 0 || i == BOARD_Y - 1 || j == 0 || j == BOARD_X - 1)
				board[i][j].block = WALL;
			else
				board[i][j].block = EMPTY;
		}
	}
}

void newBlock()
{
	if (new_block_flag == 1)
		new_block_flag = 0;
	block_index = next_block_index;
	next_block_index = rand() % 7;
	rotation_index = 0;
	bx = (BOARD_X / 2) - 1;
	by = 1;

	switch (block_index)
	{
		case 0:
			color = BLUE;
			break;
		case 1:
			color = YELLOWGREEN;
			break;
		case 2:
			color = SKY;
			break;
		case 3:
			color = RED;
			break;
		case 4:
			color = PINK;
			break;
		case 5:
			color = YELLOW;
			break;
		case 6:
			color = WHITE;
			break;
	}

	int i, j, k = 0, l = 0;
	int preX = -1, preY = -1;
	int lastX = -1, lastY = -1;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (blocks[block_index][rotation_index][i][j] == 1)
			{
				if (preX < 0 && preY < 0) { preX = j; preY = i; }
				else if (preX > j) preX = j;
				else if (lastX < j) lastX = j;
				lastY = i;
			}
		}
	}

	for (i = preY; i <= lastY; i++)
	{
		for (j = preX; j <= lastX; j++)
		{
			if (blocks[block_index][rotation_index][i][j] == 1)
			{
				// if (board[by + k][bx + l].block == INACTIVE)
				//	checkGameOver();
				board[by + k][bx + l].block = ACTIVE;
				board[by + k][bx + l].color = color;
			}
			l++;
		}
		l = 0;
		k++;
	}
}

void drawBoard()
{
	int i, j;
	for (i = 0; i < BOARD_Y; i++)
	{
		for (j = 0; j < BOARD_X; j++)
		{
			if (board_temp[i][j].block != board[i][j].block ||
				board_temp[i][j].color != board[i][j].color)
			{
				gotoxy(startX + (j * 2), startY + i);
				switch (board[i][j].block)
				{
					case WALL:
						printf("▩");
						break;
					case EMPTY:
						printf("  ");
						break;
					case ACTIVE:
						setColor(board[i][j].color);
						printf("■");
						break;
					case INACTIVE:
						setColor(board[i][j].color);
						printf("■");
						break;
				}
				setColor(7);
			}
		}
	}

	for (i = 0; i < BOARD_Y; i++)
		for (j = 0; j < BOARD_X; j++)
			board_temp[i][j] = board[i][j];
}

void drawInterface()
{
	int i, j, k = 0, l = 0;
	int preX = -1, preY = -1;
	int lastX = -1, lastY = -1;
	int c;

	gotoxy(startX + 30, startY + 2);
	printf("점수 : %d", score);
	gotoxy(startX + 30, startY + 4);
	printf("레벨 : %d", level);
	gotoxy(startX + 30, startY + 6);
	printf("남은 라인 수 : %2d", next_level_goal);
	gotoxy(startX + 30, startY + 8);
	printf("NEXT");

	switch (next_block_index)
	{
	case 0:
		c = BLUE;
		break;
	case 1:
		c = YELLOWGREEN;
		break;
	case 2:
		c = SKY;
		break;
	case 3:
		c = RED;
		break;
	case 4:
		c = PINK;
		break;
	case 5:
		c = YELLOW;
		break;
	case 6:
		c = WHITE;
		break;
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			gotoxy(startX + 31 + (j * 2), startY + 10 + i);
			printf("  ");
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (blocks[next_block_index][0][i][j] == 1)
			{
				if (preX < 0 && preY < 0) { preX = j; preY = i; }
				else if (preX > j) preX = j;
				else if (lastX < j) lastX = j;
				lastY = i;
			}
		}
	}
	
	for (i = preY; i <= lastY; i++)
	{
		for (j = preX; j <= lastX; j++)
		{
			if (blocks[next_block_index][0][i][j] == 1)
			{
				gotoxy(startX + 31 + (l * 2), startY + 10 + k);
				setColor(c);
				printf("■");
			}
			l++;
		}
		l = 0;
		k++;
	}

	setColor(7);
}

void moveBlock(int dir)
{
	switch (dir)
	{
		int i, j, k, l, preX, preY, lastX, lastY;
		case LEFT:
			k = 0, l = 0, preX = -1, preY = -1, lastX = -1, lastY = -1;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						if (preX < 0 && preY < 0) { preX = j; preY = i; }
						else if (preX > j) preX = j;
						else if (lastX < j) lastX = j;
						lastY = i;
					}
				}
			}

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k][bx + l].block = EMPTY;
						board[by + k][bx + l].color = EMPTY;
					}
					l++;
				}
				l = 0;
				k++;
			}

			k = 0, l = 0;

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k][bx + l - 1].block = ACTIVE;
						board[by + k][bx + l - 1].color = color;
					}
					l++;
				}
				l = 0;
				k++;
			}
			bx--;
			break;
		case RIGHT:
			k = 0, l = 0, preX = -1, preY = -1, lastX = -1, lastY = -1;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						if (preX < 0 && preY < 0) { preX = j; preY = i; }
						else if (preX > j) preX = j;
						else if (lastX < j) lastX = j;
						lastY = i;
					}
				}
			}

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k][bx + l].block = EMPTY;
						board[by + k][bx + l].color = EMPTY;
					}
					l++;
				}
				l = 0;
				k++;
			}

			k = 0, l = 0;

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k][bx + l + 1].block = ACTIVE;
						board[by + k][bx + l + 1].color = color;
					}
					l++;
				}
				l = 0;
				k++;
			}
			bx++;
			break;
		case UP:
			k = 0, l = 0, preX = -1, preY = -1, lastX = -1, lastY = -1;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						if (preX < 0 && preY < 0) { preX = j; preY = i; }
						else if (preX > j) preX = j;
						else if (lastX < j) lastX = j;
						lastY = i;
					}
				}
			}

			k = 0; l = 0;

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k][bx + l].block = EMPTY;
						board[by + k][bx + l].color = EMPTY;
					}
					l++;
				}
				l = 0;
				k++;
			}

			k = 0, l = 0;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[block_index][(rotation_index + 1) % 4][i][j] == 1)
					{
						if (preX < 0 && preY < 0) { preX = j; preY = i; }
						else if (preX > j) preX = j;
						else if (lastX < j) lastX = j;
						lastY = i;
					}
				}
			}

			k = 0, l = 0;

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][(rotation_index + 1) % 4][i][j] == 1)
					{
						board[by + k][bx + l].block = ACTIVE;
						board[by + k][bx + l].color = color;
					}
					l++;
				}
				l = 0;
				k++;
			}
			rotation_index = (rotation_index + 1) % 4;
			break;
		case DOWN:
			k = 0, l = 0, preX = -1, preY = -1, lastX = -1, lastY = -1;

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						if (preX < 0 && preY < 0) { preX = j; preY = i; }
						else if (preX > j) preX = j;
						else if (lastX < j) lastX = j;
						lastY = i;
					}
				}
			}

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k][bx + l].block = EMPTY;
						board[by + k][bx + l].color = EMPTY;
					}
					l++;
				}
				l = 0;
				k++;
			}

			k = 0, l = 0;

			for (i = preY; i <= lastY; i++)
			{
				for (j = preX; j <= lastX; j++)
				{
					if (blocks[block_index][rotation_index][i][j] == 1)
					{
						board[by + k + 1][bx + l].block = ACTIVE;
						board[by + k + 1][bx + l].color = color;
					}
					l++;
				}
				l = 0;
				k++;
			}
			by++;
			break;
	}
}

void dropBlock()
{
	if (checkCrush(bx, by + 1, rotation_index) == false) moveBlock(DOWN);
	else if (checkCrush(bx, by + 1, rotation_index) == true)
	{
		int i, j;
		for (i = 0; i < BOARD_Y; i++)
			for (j = 0; j < BOARD_X; j++)
				if (board[i][j].block == ACTIVE)
					board[i][j].block = INACTIVE;
		new_block_flag = 1;
	}
}

void checkKey()
{
	if (kbhit())
	{
		key = getch();
		if (key == 224)
		{
			key = getch();
			switch (key)
			{
				case LEFT:
					if (checkCrush(bx - 1, by, rotation_index) == false) moveBlock(LEFT);
					break;
				case RIGHT:
					if (checkCrush(bx + 1, by, rotation_index) == false) moveBlock(RIGHT);
					break;
				case UP:
					if (checkCrush(bx, by, (rotation_index + 1) % 4) == false) moveBlock(UP);
					break;
				case DOWN:
					if (checkCrush(bx, by + 1, rotation_index) == false) moveBlock(DOWN);
					break;
			}
		}
		else
		{
			switch (key)
			{
				case ESC:
					exit(0);
				case SPACE:
					hardDrop();
					break;
			}
		}
	}
}

int checkCrush(int x, int y, int rotation)
{
	int i, j, k = 0, l = 0, preX = -1, preY = -1, lastX = -1, lastY = -1;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (blocks[block_index][rotation][i][j] == 1)
			{
				if (preX < 0 && preY < 0) { preX = j; preY = i; }
				else if (preX > j) preX = j;
				else if (lastX < j) lastX = j;
				lastY = i;
			}
		}
	}

	for (i = preY; i <= lastY; i++)
	{
		for (j = preX; j <= lastX; j++)
		{
			if (blocks[block_index][rotation][i][j] == 1)
			{
				if (board[y + k][x + l].block != EMPTY &&
					board[y + k][x + l].block != ACTIVE)
					return true;
			}
			l++;
		}
		l = 0;
		k++;
	}
	return false;
}

void checkLine()
{
	int i, j, k, l;
	int cnt, combo = 0;

	for (i = BOARD_Y - 2; i > 0;)
	{
		cnt = 0;
		for (j = BOARD_X - 2; j > 0; j--)
			if (board[i][j].block == INACTIVE) cnt++;
		if (cnt == BOARD_X - 2)
		{
			for (k = i; k > 1; k--)
				for (l = BOARD_X - 2; l > 0; l--)
					board[k][l] = board[k - 1][l];
			combo++;
		}
		else i--;
	}

	if (combo > 1)
	{
		score += (100 * combo) * 2;
		gotoxy(startX + 9, startY + 8);
		printf("%d COMBO", combo);
		Sleep(500);
		for (i = 0; i < 6; i++)
		{
				gotoxy(startX + 9 + i, startY + 8);
				setColor(board[startX - 2 + i][startY + 1].color);
				printf("■");
				setColor(7);
		}
	}
	else if (combo == 1) score += 100;

	for (i = 0; i < combo; i++)
	{
		if (next_level_goal > 0)
			next_level_goal--;
		else
			break;
	}
}

void hardDrop()
{
	int len = 0;
	while (!checkCrush(bx, by + 1, rotation_index))
	{
		moveBlock(DOWN);
		len++;
	}
	
	int i, j;
	for (i = 0; i < BOARD_Y; i++)
		for (j = 0; j < BOARD_X; j++)
			if (board[i][j].block == ACTIVE)
				board[i][j].block = INACTIVE;
	new_block_flag = 1;
	score += len;
}

void checkLevelUp()
{
	if (next_level_goal == 0)
	{
		level++;
		next_level_goal = 10;
		speed -= 10;
	}
}

void checkGameOver()
{
	int i, j = 0;
	for (i = 1; i < BOARD_X - 1; i++)
	{
		if (board[1][i].block == ACTIVE || board[1][i].block == INACTIVE)
		{
			gotoxy(startX + 7, startY + 8);
			printf("GAME OVER!");
			gotoxy(startX + 6, startY + 9);
			printf("Press any key");
			gotoxy(startX + 6, startY + 10);
			printf("to restart");
			while (1)
			{
				++j;
				if (kbhit())
				{
					key = getch();
					break;
				}
				
				if (j == 300)
				{
					gotoxy(startX + 6, startY + 10);
					printf("to restart.");
				}
				else if (j == 600)
				{
					gotoxy(startX + 6, startY + 10);
					printf("to restart..");
				}
				else if (j == 900)
				{
					gotoxy(startX + 6, startY + 10);
					printf("to restart...");
				}
				else if (j == 1200)
				{
					gotoxy(startX + 6, startY + 10);
					printf("to restart   ");
					j = 0;
				}
				Sleep(1);
			}

			while (kbhit()) key = getch();

			gotoxy(startX + 7, startY + 8);
			printf("          ");
			gotoxy(startX + 6, startY + 9);
			printf("             ");
			gotoxy(startX + 6, startY + 10);
			printf("             ");

			reset();
			newBlock();
		}
	}
}