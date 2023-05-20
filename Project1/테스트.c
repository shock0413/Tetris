#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
void threadbody();

/*
int main(void)
{
	HANDLE thread;
	thread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadbody, NULL, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	return 0;
}
*/

void threadbody()
{
	int i = 0;
	while (1)
	{
		gotoxy(0, 0);
		printf("%d√ ", i);
		if (i == 15)
			break;
		Sleep(1000);
		i++;
	}
}