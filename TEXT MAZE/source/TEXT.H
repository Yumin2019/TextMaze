#include<iostream>
#include<string>
using namespace std;

void TextColor(int color_num);
void Speak(const char speaking[]);
void Speak_C(const char speaking[], int C);
char copy_str(const char *dest,const char *src);
int str_num(const char *str); 
char compare(const char *str1, const char *str2);
void gotoxy(int x, int y, const char* s);
void gotoxy_C(int x, int y, const char* s, int Color);
