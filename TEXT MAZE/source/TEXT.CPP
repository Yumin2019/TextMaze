#include<windows.h>
#include<iostream>
#include<time.h>
#include<windows.h>
#include<conio.h>
#include"Maze.h"
#include"Text.h"


 enum COLOR {
	BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY
	, DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW,
	DEEP_WHITE
};

char compare(const char *str1,const char *str2) 
{
	while (*str1)
	{
		if (*str1 != *str2)
		{
			return 0;
		}
		str1++;
		str2++;
	}
	if (*str2 == '\0')
		return 1;
	return 0;
}
//복사 함수
char copy_str(char *dest, char *src)
{
	while (*src)
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';
	return 1;
}
void gotoxy(int x, int y, const char *s)
{
	COORD pos;

	pos.X = 2 * x;//2배의 x 좌표
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	printf("%s", s);


}
void gotoxy_C(int x, int y, const char *s, int Color) 
{
	COORD pos;

	pos.X = 2 * x;//2배의 x 좌표
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	Speak_C(s, Color); //받은 문자열을 받은 색깔로 하나씩 출력 자연스럽게 


}



void Speak(const char speaking[])
{ // 흰색으로 한 글자씩
	int i = 0;


	TextColor(DEEP_WHITE);

	for (; i < str_num(speaking); i++)
	{
		printf("%c", speaking[i]); Sleep(25);
	}



}
//흰색을 지정하고 싶지 않을 때 (나는 TextColor의 색을 지정하고 싶어)
void Speak_C(const char speaking[], int C)
{
	int i = 0;


	TextColor(C);

	for (; i < str_num(speaking); i++)
	{
		printf("%c", speaking[i]); Sleep(25);
	}



}
void TextColor(int color_num)  /*텍스트 컬러 함수   */
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);


}

/* 배열의 문자수 확인 */
int str_num(const char *str)
{
	int i = 0;
	while (1)
	{

		if (*(str + i) == NULL)
			break;


		if (*(str + i) != NULL)
			i++;

	}

	return i; // 문자열의 수 출력 

}
