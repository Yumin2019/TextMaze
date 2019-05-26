#include<iostream>
#include<Windows.h>
#include<conio.h>
#include"Text.h"
#include"Maze.h"
#include "fmod.hpp"
#include<string>
#include<fstream>
#include<algorithm>

using namespace std;


class Lank  // Lank class
{
	
	

public:
	string name;
	int death; // 플레이랑은 사실 상관없고 불러올 때 버릴 값 / 제어

	Lank(const string& name, int death) : name(name), death(death) {}



	friend ofstream& operator<<(ofstream& a, Lank& b);

	friend bool cmp(const Lank& a, const Lank& b);
	
	Lank() { } // 기본 생성자 100 그룹
};

ofstream& operator<<(ofstream& a, Lank& b) // 파일에 입력
{
	a << b.name.c_str() << "." << b.death << "\n"; // 이름을 파일에 출력  이름.death        string이 지랄하면 char*
	return a;
}
bool cmp(const Lank& a, const Lank& b)
{
	if (a.death != b.death) return a.death < b.death;

	//데스가 같다 사전순.
	return a.name < b.name;
}

Lank arr[100];
/* 입출력 생각.

일단 파일이 있다면, 그 부분에 대한 내용을 받아온다.  ( 100명치만 잡는다.)

받아온 파일을 저장 (개수 세기)
+ 입력 된 파일을 저장(개수 + 1)

위 기준으로 sort

sorted 된 배열을 순서대로 출력.


우리가 정렬을 해야하는 시점. (데이터 추가 X ) 그냥 랭킹 킬 때 ( 개수가 그대로면 변함 없이 그대로 출력.) 읽어온 수 = 지금 수(읽은 수 + 추가 1)
데이터 추가 O 당연히 쫙!  이 데이터를 어느 부분에 잡아 넣어야 이전 데이터가 사라지지 않을까 ? 역시 개수를 알고 있어야 한다.

그럼 일단. 미리 읽어온 수를 구하자.  *개수를 떠나서 일단 읽어와야 뭐가 된다.


어느 타이밍에 정렬을 해야 하는가 ?

ranking 킬 때 (왜냐면 미리 해놓아도 도움 안 됨.)
ranking 킬 때마다 sort하면 비효율인 것 아니냐 ?
=> 어짜피 100개 데이터면 몇 번 안 합니다. 
그냥 가는 거죠 .
*/
int read; // title에서 처리.

#define LEFTSPACE 3
#define LIGHTSPACE 3
#define UPSPACE 2
#define DOWNSPACE 5
#define MAP 30
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define SPACEBAR 32

FMOD::System	*Fmod(nullptr);
// FMOD
FMOD::Sound		*music(nullptr); // 스테이지마다 사용할 포인터  createSound는 여러개 동시 재생 가능(효과음 처리) createStream은 하나만 재생 가능 ( 배경음악 )
FMOD::Sound		*jump(nullptr);	// 효과음이라고 하여도 지역변수로 선언하면 소리가 묻힘.
FMOD::Sound		*button(nullptr);




// 용량 차이 createSound < createStream // 배경음악을 createSound로 하면 매우 비효율적. 지역변수로 선언할 시 release가 되지 않음.
													// 없는 데이터를 release하는 개념이라고 생각함.

//소리는 함수내에서 지정해서 사용한다. 여러 소리가 동시에 있을 경우를 대비해서 4개를 선언.


FMOD::Channel	*channel1(nullptr); // 채널 1은 노래를 틀어서 소리를 조정해준다.
FMOD::Channel	*channel2(nullptr);// 채널 2는 효과음으로 소리를 조정하지 않는다.
FMOD::Channel	*channel3(nullptr);
FMOD::Channel	*channel4(nullptr); // 효과음 채널2 분산.


// 채널의 개념은 같이 소리를 지정해둘 목록/ init 부분은 real 채널수. title (시작부분에서 처리중)


FMOD_RESULT		result;

/* 많이 사용하는 사운드는 미리 잡아주면 되고 아니면 유동적으로 사용.*/

void			*extradriverdata(nullptr);

double speaker = 0.5;
int soundEffect = 1;// 효과음 on off
int dataCheck; // data를 받았는가 받았으면 1
int create; // 개발자 변수.
int hansei; // hansei를 말하면 1 ( 게임 내에서 다시 묻지 않음 )

int quest; // tutorial 변수
int death; // death count
int num;
int stage;
int number; // 횟수. 튜토리얼에서 사용.
long arrowSpeed; // arrow speed는 stage별 구상하도록 한다. 참고로 sleep의 인수기 때문에 크기가 크면 느려짐
int titleOn; // 타이틀이 시작될 때를 인지함.
extern int block9;

//end 포인트를 잡아두고 그것과 같은 arrow 가 있으면 처리.

POINT arrowPos[15];

int arrowCount;

char arrowVector[15]; // 각 arrow간에 우리는 어느 방향으로 나아가는 놈인지 인식할 먼가가 필요함.


POINT arrowStart[15]; // count와 매칭해서 받아야 함.

int switchOnCount;


int switchCount; // sw카운트 

// 전역변수 처리하는 start end 
POINT tStartPos;
POINT tEndPos;
POINT	tPlayerPos; // 사실 이것도 데이터 처리는 이곳에서 하면 훨씬 더 편하다. 우리가 main쪽에서 데이터를 받아올 때는 포인터 형식을 이용했지만, 여기서 온갖 처
//리를 다하는 상황에서 굳이 ㅇㅇ 그냥 구조체 ㅇㅇㅇ.x ooool.y


char	Maze[30][31];

POINT tBombPos[5];

int iBombCount;

// Maze , boom 구조체 , count;


enum COLOR {
	BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY
	, DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW,
	DEEP_WHITE
};


void SetMaze(int* block9)
{
	if (quest >= 1) return; // quest 진행중일 때 맵을 다시 안 찍음.

	Fmod->update();

	// 스테이지 마다 다른 좌표값 ? 을 줘야할 듯 하다. 그리고 죽은 이후 또한 배열로 지정해야 한다.
	//일단 스테이지는 5개 정도 고려중 .. 너무 많아지면 빡세다 ㅇㅇ
	


	/*


	0 : 벽
	1 : 길
	2 : 시작점 는 형식적인 표현이다. 사실상 end와 start는 구조체가 따로 있는 것이 아니고, stage에 따라 end start 포인트 데입.
	3 : 도착점
	4 : 폭탄

	5 : 폭탄으로 부실 수 있는 약한 벽

	6 : pushable Block
	7 : SW 스위치 변수 ( 모든 스위치가 on일 때 탈출 가능)
	(구체적으로는 스위치 변수를 밀수있는 블럭으로 덮어씌울 때 닫혀 있던 벽이 열리고 
	도착점으로 갈 수 있는 형태.)

	8 : Sw 스위치 변수 ON형태.

	9 : 모든 스위치가 ON이면 뚫리는 벽.

	< : to 왼쪽 arrow
	> : to 오른쪽 arrow
	
	- : to 아래쪽 arrow
	+ : to UP arrow

	각각의 arrow는 끝 위치가 존재한다. 따라서 패턴은 그대로. ㅇㅇ arrow의 끝은 통일해도 문제
	? 없겠죠. 왜냐면 위로가거나 아래로 가거나 기행종은 없으니까 해당 arrow가 만날 수 있는
	end지점은 항상 하나밖에 없으므로 (2개씩 한다면 모를까나.)

	e : arrow end Point 

	e는 벽으로 출력.


	죽은 화살은 어떻게 표현할지 . . .

	l : died left arrow

	r : died right arrow

	u : died up arrow

	d : died down arrow

	o : died pushable block

	자 여기서 살펴보자, 우리는 이 죽은 arrow를 다시 찍을 필요가 있을까 ? 
	ㄴㄴ 다시 찍을 필요가 없다. 단지 이것을 arrow로 인식하지 못하게 해야한다.
	그러면 그냥 Maze에서 데입만 해두고, move에서 못가도록 하자.
	

	' ' : 공백 건드리지 않는 부분 ( 여기다가 gotoxy로 그림 그려두고 이후에 안 건드림)

	*/


	//MazeList.txt 파일을 읽어와서 미로 목록을 만든다.

	/*
	FILE*	pFile = NULL;

	fopen_s(&pFile, "MazeList.txt", "rt");

	if (pFile)
	{
		char	cCount;

		fread(&cCount, 1, 1, pFile);

		int MazeCount = atoi(&cCount);

		char** MazeList = NULL;

		fread(&cCount, 1, 1, pFile); // \n문자 없애기.
		// char**로 선언하여 char*를 가지고 놀다.

		MazeList = new char*[MazeCount];

		// char* 형 (미로 이름)을 받는다.

		

		for (int i = 0; i < MazeCount; i++)
		{
			int NameCount = 0;
			

			MazeList[i] = new char[256];
			// 
			while (true)
			{
				fread(&cCount, 1, 1, pFile);
				while (cCount != '\n')
				{
					MazeList[i][NameCount] = cCount;
					NameCount++;
					fread(&cCount, 1, 1, pFile);
				}
				if (cCount == '\n') break;
				
			}
			MazeList[i][NameCount] = NULL;
			// 문자열의 끝에 NULL 데입. 문자열의 끝.
			// test 완료.
		}
		fclose(pFile);

		for (int i = 0; i < MazeCount; i++)
		{
			cout << i + 1 << " , " << MazeList[i] << endl;
		}

		cout << "미로 선택 : ";
		int a;
		cin >> a;
		
		// 선택한 이름의 미로를 읽어옴.
		fopen_s(&pFile, MazeList[a - 1], "rt");
		if (pFile)
	{
		for (int i = 0; i < 30; i++)
		{
			
				fread(&Maze[i], 1, 30, pFile);
			fread(&cCount, 1, 1, pFile); // \n
			
		}

		
			fclose(pFile);
		}
	
	}
	*/  // 파일 입출력.



	// 이후에 각 파일 중 하나를 선택해 미로를 읽어온다.
	num = 0;
	arrowCount = 0;
	switchCount = 0;
	switchOnCount = 0;
	*block9 = 0; // 깜빡임 처리 셋팅 열리지 않았다는 표시.

	// death는 건드리지 않는다. (1~6 stage에서 세고서 랭킹에 써야 함) death 0은 튜토리얼 or title에서 잘 처리함.
	
	system("cls");

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "▣ Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "▣ button :  + - ");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);

	while (_kbhit()) _getch(); 
	
	 // 스테이지 셋팅마다 num = 0을 통해서 스위치, arrow 등의 개수를 다시 세어준다.  endPos 와 startPos 는 stage 마다 구상.
	switch(stage)
	{
	case -1:
		title(); // 타이틀 음악은 타이틀에서 처리하고 있다.
		break;
	case 0: // tutorial 맵은 찍어두고 tutorial 조건에 맞게 open 등 처리.
		

	
		music->release();

		Fmod->createStream("tutorial.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 14;
		tPlayerPos.y = 2;
		arrowSpeed = 5;

		tEndPos.x = 14;
		tEndPos.y = 27;

		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "011111111111111111111111111110");
		strcpy_s(Maze[2 ], 31, "011111111111112111111111111110");
		strcpy_s(Maze[3 ], 31, "011111111111111111111111111110");
		strcpy_s(Maze[4 ], 31, "000000000000000000000000000000"); //q2 에서 열어주고
		strcpy_s(Maze[5 ], 31, "000000000000>1111e000000000000"); // 화살에 죽어봅시다.
		strcpy_s(Maze[6 ], 31, "000000000000001000000000000000"); //라고는 말하지만, 사실 폭탄 자살해도
		strcpy_s(Maze[7 ], 31, "000000000000000000000000000000"); // 되도록 ? (아니면 폭탄을 무시)good
		strcpy_s(Maze[8 ], 31, "000000000000001000000000000000"); // 만족하면 열어주고 폭탄 on
		strcpy_s(Maze[9 ], 31, "000000000000005000000000000000"); // 해당 자리에 넣고 펑. 표시가 필요한데 특수문자 하나 넣을까.
		strcpy_s(Maze[10], 31, "000000000000001000000000000000"); // 4 - 3같이, 
		strcpy_s(Maze[11], 31, "000000000000006000000000000000");
		strcpy_s(Maze[12], 31, "000000000000>1111e000000000000");
		strcpy_s(Maze[13], 31, "000000000000000000000000000000");
		strcpy_s(Maze[14], 31, "000000000000001000000000000000");
		strcpy_s(Maze[15], 31, "000000000076111116700000000000"); // 7
		strcpy_s(Maze[16], 31, "000000000000000000000000000000");
		strcpy_s(Maze[17], 31, "000000000000091900000000000000");
		strcpy_s(Maze[18], 31, "000000000000091900000000000000");
		strcpy_s(Maze[19], 31, "000000000000091900000000000000");
		strcpy_s(Maze[20], 31, "000000000000091900000000000000");
		strcpy_s(Maze[21], 31, "000000000000091900000000000000"); // 9
		strcpy_s(Maze[22], 31, "000000000000000000000000000000");
		strcpy_s(Maze[23], 31, "000000000000001000000000000000");
		strcpy_s(Maze[24], 31, "000000000000001000000000000000");
		strcpy_s(Maze[25], 31, "000000000000001000000000000000");
		strcpy_s(Maze[26], 31, "000000000000001000000000000000");
		strcpy_s(Maze[27], 31, "000000000000093900000000000000");
		strcpy_s(Maze[28], 31, "000000000000009000000000000000");
		strcpy_s(Maze[29], 31, "000000000000000000000000000000");

		
		break; // 튜토리얼
	case 1:

		music->release();

		
		//소리조절 기능만 넣도록 하자.
		//전역변수 이용 setvolume 나중에
		Fmod->createStream("stage1.mp3", FMOD_LOOP_NORMAL, 0, &music);
			Fmod->playSound(music, 0, false, &channel1); //음악 관련 stage 1.

		tPlayerPos.x = 1; //1 1
		tPlayerPos.y = 1;
		// stage 1은 빨리 넘어가기 위한 꼼수다. test
		arrowSpeed = 12; // ok
		tEndPos.x = 1;
		tEndPos.y = 28;
		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "02116111170        3        00");
		strcpy_s(Maze[2 ], 31, "00000000100   3   3 3   3   00");
		strcpy_s(Maze[3 ], 31, "00000000100  3 3   3   3 3  00");
		strcpy_s(Maze[4 ], 31, "00000000100   3    7    3   00");
		strcpy_s(Maze[5 ], 31, "00000000500   7    7    7   00");
		strcpy_s(Maze[6 ], 31, "00000000100   7    7    7   00");
		strcpy_s(Maze[7 ], 31, "000000001000000000000000000000");
		strcpy_s(Maze[8 ], 31, "000000001511111111611700000000");
		strcpy_s(Maze[9 ], 31, "000000000000000000100000000000");
		strcpy_s(Maze[10], 31, "000             00100000000000");
		strcpy_s(Maze[11], 31, "000   9     9   00500000000000");
		strcpy_s(Maze[12], 31, "000  999   999  00100000000000");
		strcpy_s(Maze[13], 31, "000   9     9   00100000000000");
		strcpy_s(Maze[14], 31, "000   7     7   00100000000000");
		strcpy_s(Maze[15], 31, "000   7     7   00500000000000");
		strcpy_s(Maze[16], 31, "000   7     7   00100000000000");
		strcpy_s(Maze[17], 31, "000000000000000000100000000000");
		strcpy_s(Maze[18], 31, "000-0-0-0-00000000100000000000");
		strcpy_s(Maze[19], 31, "000101010107611111100000000000");
		strcpy_s(Maze[20], 31, "000101010100010000000000000000");
		strcpy_s(Maze[21], 31, "000101010100010000000000000000");
		strcpy_s(Maze[22], 31, "000101010100010000000000000000");
		strcpy_s(Maze[23], 31, "000101010100011111116111117000");
		strcpy_s(Maze[24], 31, "000101010100000000000000010000");
		strcpy_s(Maze[25], 31, "000101010100000000000000050000");
		strcpy_s(Maze[26], 31, "000101010100000000000000010000");
		strcpy_s(Maze[27], 31, "000101010100000000000000010000");
		strcpy_s(Maze[28], 31, "031151515111111161111111110000");
		strcpy_s(Maze[29], 31, "000e0e0e0e0e0e0e00000000000000");

		
		break; // o . k
	case 2:
		music->release();

		Fmod->createStream("stage2.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1); 


		arrowSpeed = 0.8;// ok
		//이게 arrow개수에 따라 처리속도가 재각각이라, 적당한 속도 조정이 필요하다. 혹은 case 문 ggo
		tPlayerPos.x = 1; // 1 1
		tPlayerPos.y = 1;
		// 나중에 처리하도록.
		tEndPos.x = 28;
		tEndPos.y = 28;
		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "021111110000000000000000000000");
		strcpy_s(Maze[2 ], 31, "000000110000070000000000000000");
		strcpy_s(Maze[3 ], 31, "000000110000010000000000000000");
		strcpy_s(Maze[4 ], 31, "000001111000060000000000000000");
		strcpy_s(Maze[5 ], 31, "751111161111611111111111111170");
		strcpy_s(Maze[6 ], 31, "000001111000060000000000000100");
		strcpy_s(Maze[7 ], 31, "000000000000010000000000000100");
		strcpy_s(Maze[8 ], 31, "000000000000070000000000000500");
		strcpy_s(Maze[9 ], 31, "000000000000000000000000000100");
		strcpy_s(Maze[10], 31, "000011111115111151111111111100");
		strcpy_s(Maze[11], 31, "000010000000000000000000000000");
		strcpy_s(Maze[12], 31, "000010000000000000000000000000");
		strcpy_s(Maze[13], 31, "000010000000000000000000000000");
		strcpy_s(Maze[14], 31, "000050000000000000000000000000");
		strcpy_s(Maze[15], 31, "000010000000000000000000000000");
		strcpy_s(Maze[16], 31, "000011111111111111111000000000");
		strcpy_s(Maze[17], 31, "000000000006666666666000000000");
		strcpy_s(Maze[18], 31, "0000>11111111111111111111e0000");
		strcpy_s(Maze[19], 31, "0000>11111111111111111111e0000");
		strcpy_s(Maze[20], 31, "0000e11111111111111111111<0000");
		strcpy_s(Maze[21], 31, "0000e11111111111111111111<0000");
		strcpy_s(Maze[22], 31, "0000>11111111111111111111e0000");
		strcpy_s(Maze[23], 31, "0000>11111111111111111111e0000");
		strcpy_s(Maze[24], 31, "0000e11111111111111111111<0000");
		strcpy_s(Maze[25], 31, "0000e11111111111111111111<0000");
		strcpy_s(Maze[26], 31, "0000>11111111111111111111e0000");
		strcpy_s(Maze[27], 31, "0000>11111111111111111111e0000");
		strcpy_s(Maze[28], 31, "000011111111111111111111115930");
		strcpy_s(Maze[29], 31, "000000000000000000000000000000");
		break;
		// 겹치는 부분은 없도록 end point 

	case 3:
		music->release();


		Fmod->createStream("stage3.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 14;
		tPlayerPos.y = 14; // 14 14
		tEndPos.x = 25;
		tEndPos.y = 28;
		arrowSpeed = 4;

		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "0000 0000000000000000000 00000");
		strcpy_s(Maze[2 ], 31, "000 9 00000000000000000 9 0000");
		strcpy_s(Maze[3 ], 31, "0000 9 000000000000000 9 00000");
		strcpy_s(Maze[4 ], 31, "000 999 0000000000000 999 0000");
		strcpy_s(Maze[5 ], 31, "000 999 0000000000000 999 0000");
		strcpy_s(Maze[6 ], 31, "0000   000000000000000   00000");
		strcpy_s(Maze[7 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[8 ], 31, "00000000e>1111111111e000000000");
		strcpy_s(Maze[9 ], 31, "00000000111111111111-000000000");
		strcpy_s(Maze[10], 31, "000000001161111111611000000000");
		strcpy_s(Maze[11], 31, "000000001111111111111000000000");
		strcpy_s(Maze[12], 31, "000000001111111111111000000000");
		strcpy_s(Maze[13], 31, "000000001111171711111000000000");
		strcpy_s(Maze[14], 31, "000000001111112111111000000000");
		strcpy_s(Maze[15], 31, "000000001111171711111000000000");
		strcpy_s(Maze[16], 31, "000000001111111111111000000000");
		strcpy_s(Maze[17], 31, "000000001111111111111000000000");
		strcpy_s(Maze[18], 31, "000000001161111111611000000000");
		strcpy_s(Maze[19], 31, "00000000+111111111111000000000");
		strcpy_s(Maze[20], 31, "00000000e1111111111<e000000000");
		strcpy_s(Maze[21], 31, "000000000000001000000000000000");
		strcpy_s(Maze[22], 31, "00000000e11111111111<000000000");
		strcpy_s(Maze[23], 31, "000000000000001000000000000000");
		strcpy_s(Maze[24], 31, "00000000>11111111111e000000000");
		strcpy_s(Maze[25], 31, "000000000000001000000000000000");
		strcpy_s(Maze[26], 31, "000000000000001111115111500000");
		strcpy_s(Maze[27], 31, "000000000000000000000000100000");
		strcpy_s(Maze[28], 31, "000000000000000000000000930000");
		strcpy_s(Maze[29], 31, "000000000000000000000000000000");
		break;

	case 4:
		music->release();

		Fmod->createStream("stage4.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);


		tPlayerPos.x =  2;
		tPlayerPos.y = 28; // 2 28  ok
		tEndPos.x = 20;
		tEndPos.y = 28;
		arrowSpeed = 2;
		strcpy_s(Maze[0 ], 31, "0000000000000000000e000e000000");
		strcpy_s(Maze[1 ], 31, "000011115111161111111111111100");
		strcpy_s(Maze[2 ], 31, "000011115111161111111111111100");
		strcpy_s(Maze[3 ], 31, "000050000111111111111111111100");
		strcpy_s(Maze[4 ], 31, "e1111111<0000000000+000+001100");
		strcpy_s(Maze[5 ], 31, "0515115150000000000e0000001100");
		strcpy_s(Maze[6 ], 31, "0>1111111e00000050001515111100");
		strcpy_s(Maze[7 ], 31, "011555511000000015000151511100");
		strcpy_s(Maze[8 ], 31, "e1111111<000000011500015151100");
		strcpy_s(Maze[9 ], 31, "055111155000000011150001515100");
		strcpy_s(Maze[10], 31, "0>1111111e00000011115111151500");
		strcpy_s(Maze[11], 31, "055111155000000019111511115100");
		strcpy_s(Maze[12], 31, "e1111111<000000011911150001500");
		strcpy_s(Maze[13], 31, "011555511000000011191115000100");
		strcpy_s(Maze[14], 31, "0>1111111e00000011119111500000");
		strcpy_s(Maze[15], 31, "011555511000000091111911150000");
		strcpy_s(Maze[16], 31, "e1111111<000000059111191115000");
		strcpy_s(Maze[17], 31, "000055000000000055911119111500");

		strcpy_s(Maze[18], 31, "0>1111111e00000000000161100000");
		strcpy_s(Maze[19], 31, "055555555500000000000111100000");
		strcpy_s(Maze[20], 31, "011111111500000000000e55100000");
		strcpy_s(Maze[21], 31, "01111111150000   000011100   0");
		strcpy_s(Maze[22], 31, "01111111150000 9 000111000 9 0");
		strcpy_s(Maze[23], 31, "055555511500003930001110003930");
		strcpy_s(Maze[24], 31, "011111511500003330001100003330");
		strcpy_s(Maze[25], 31, "01111151150000 7 000110000 7 0");
		strcpy_s(Maze[26], 31, "05551151150000 7 000110000 7 0");
		strcpy_s(Maze[27], 31, "01151151150000 7 000110000 7 0");
		strcpy_s(Maze[28], 31, "02151151150000 7 0003+0000 7 0");
		strcpy_s(Maze[29], 31, "000000000000000000000000000000");
		break;
	case 5:
		music->release();


		Fmod->createStream("stage5.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 1;
		tPlayerPos.y = 1; // 1 1
		tEndPos.x = 24;
		tEndPos.y = 28;
		arrowSpeed = 2;
		
		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "021111111111100000000000000000");
		strcpy_s(Maze[2 ], 31, "000155555555100000000000000000");
		strcpy_s(Maze[3 ], 31, "000155555555100000000000000000");
		strcpy_s(Maze[4 ], 31, "000155666655100000000000000000");
		strcpy_s(Maze[5 ], 31, "000155111155100000000000000000");
		strcpy_s(Maze[6 ], 31, "000155171755100000000000000000");
		strcpy_s(Maze[7 ], 31, "000155717155100000000000000000");
		strcpy_s(Maze[8 ], 31, "000155555555100000000000000000");
		strcpy_s(Maze[9 ], 31, "000155555555100000000000000000");
		strcpy_s(Maze[10], 31, "000111111111111111100000000000");
		strcpy_s(Maze[11], 31, "000000000100000000100000000000");
		strcpy_s(Maze[12], 31, "00e1111111111<>1111111111e0000");
		strcpy_s(Maze[13], 31, "000000000100000000100000000000");
		strcpy_s(Maze[14], 31, "00e1111111111<>1111111111e0000");
		strcpy_s(Maze[15], 31, "000000000100000000100000000000");
		strcpy_s(Maze[16], 31, "00e1111111111<>1111111111e0000");
		strcpy_s(Maze[17], 31, "000000000100000000100000000000");
		strcpy_s(Maze[18], 31, "000000000500000000500000000000");
		strcpy_s(Maze[19], 31, "000005111111111511111111100000");
		strcpy_s(Maze[20], 31, "000001000100000155555555100000");
		strcpy_s(Maze[21], 31, "000001000100000155555555100000");
		strcpy_s(Maze[22], 31, "000001000100000155666655100000");
		strcpy_s(Maze[23], 31, "005111111500000155111155100000");
		strcpy_s(Maze[24], 31, "001001000000000155111155100000");
		strcpy_s(Maze[25], 31, "001001000000000155777755100000");
		strcpy_s(Maze[26], 31, "006115000000000155555555100000");
		strcpy_s(Maze[27], 31, "001000000000000155555555900000");
		strcpy_s(Maze[28], 31, "007000000000000111111119300000");
		strcpy_s(Maze[29], 31, "000000000000000000000000000000");
		break;
	case 6:
		music->release();

		Fmod->createStream("stage6.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 15; // 15 21
		tPlayerPos.y =21;
		tEndPos.x = 15;
		tEndPos.y = 6;
		arrowSpeed = 5;
		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "000000000076555555567000000000");
		strcpy_s(Maze[2 ], 31, "000000000000000500000000000000");
		strcpy_s(Maze[3 ], 31, "000000000000000500000000000000");
		strcpy_s(Maze[4 ], 31, "000000011111111111111111000000");
		strcpy_s(Maze[5 ], 31, "000000119999999999999991100000");
		strcpy_s(Maze[6 ], 31, "000001199999999399999999110000");
		strcpy_s(Maze[7 ], 31, "000019199999999999999999191000");
		strcpy_s(Maze[8 ], 31, "000111111111111111111111111100");
		strcpy_s(Maze[9 ], 31, "000100000000000000000000000100");
		strcpy_s(Maze[10], 31, "000100111100000000000111100100");
		strcpy_s(Maze[11], 31, "000100111100000000000111100100");
		strcpy_s(Maze[12], 31, "000100111100000000000111100100");
		strcpy_s(Maze[13], 31, "000100111100000000000111100100");
		strcpy_s(Maze[14], 31, "000100000000100000100000000100");
		strcpy_s(Maze[15], 31, "000100000000000000000000000100");
		strcpy_s(Maze[16], 31, "000110011110011111001111001100");
		strcpy_s(Maze[17], 31, "000010010010010001001001001000");
		strcpy_s(Maze[18], 31, "000011110011110001111001111000");
		strcpy_s(Maze[19], 31, "000000100000000000000000100000");
		strcpy_s(Maze[20], 31, "000000100000000000000000100000");
		strcpy_s(Maze[21], 31, "000000111111111211111111100000");
		strcpy_s(Maze[22], 31, "000000000006000000060000000000");
		strcpy_s(Maze[23], 31, "00000000e111111111111<00000000");
		strcpy_s(Maze[24], 31, "000000000001000000010000000000");
		strcpy_s(Maze[25], 31, "000000000>111111111111e0000000");
		strcpy_s(Maze[26], 31, "000000000001000000010000000000");
		strcpy_s(Maze[27], 31, "00000000e111111111111<00000000");
		strcpy_s(Maze[28], 31, "000000000007000000070000000000");
		strcpy_s(Maze[29], 31, "000000000000000000000000000000");
		break;
		// 그만 만들자 더 이상 만들어도 포폴로도 도움 안 되고 그냥 말짱 꽝.
		// 스테이지 구상 및 플레이 점검 몇 번 해봤으니 이후에는 파입입출력.
		// 여기 까지.
	case 7: //Ending
		
		// ending 하단에 sound 부분좀 지워 버려야 한다. = > 귀찮은 데 하지 마오.

		Sleep(500);

		music->release();

		Fmod->createStream("ending.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		titleSpace(); // 타이틀 그림에서 글자 뺀 것 출력.

		gotoxy_C(MAP + 5, 8,  "ω Thank You for Playing ", DEEP_YELLOW);  Sleep(1000);

		gotoxy_C(MAP + 5, 10, "ω Creator : Yumin Kim   ", DEEP_JAJU);  Sleep(1000); 

		gotoxy_C(MAP + 5, 12, "ω Hansei Cyber High School", DEEP_OC);  Sleep(1000);
	
		gotoxy_C(MAP + 5, 14, "ω blog.naver.com/lmj3322", DEEP_GREEN);   Sleep(1000);
		
		gotoxy_C(MAP + 5, 16, "ω PassWord : hansei",DEEP_WHITE);  Sleep(1000);


		titlePicture();

		

		OutPut();

		while (_kbhit()) _getch(); // 버퍼. 

		gotoxy_C(14, 18, "What's Your Name ? : ",DEEP_WHITE);  

		string name;
		cin >> name;

		arr[read] = { name, death }; // 입력받은 데이터를 가지고 Lank 정렬을 해줌.
		read++; // read는 항상 마지막 인자 +1을 가리키고 있어야 sort할 때 헷갈림이 없다. 
				// 다시 랭킹 받을 때도 중복된 장소에 받으면 안 되니 이게 맞다.
		// 입력 후.
		//이후에 main메뉴로 이동하게 해야 한다.
		
		/* 추가 바람 */
		
		//하위 요소 해제
		music->release();
		// 상위 요소 해제.
		Fmod->release(); 
		stage = -1;
		SetMaze(block9); // 값을 골라서, 이렇게 셋팅을 하니 이후에 arrow무브 플레이어 move 등도 무시할 수 있따.

		// 입력받은 값으로 setting				 
		SetMaze(block9); 
		OutPut();
		return;

	}

	channel1->setVolume(speaker);

	TextColor(DEEP_WHITE);

	gotoxy(MAP + 7, 6, "M : 메인메뉴");

	gotoxy(MAP + 7, 8, "T : 효과음 OFF");

	gotoxy(MAP + 7, 10, "SPACE : 폭탄 설치");

	gotoxy(MAP + 7, 12, "Q : 폭탄 터트리기");

	gotoxy(MAP + 7, 14, "W : 위   S : 아래");

	gotoxy(MAP + 7, 16, "A : 왼쪽 D : 오른쪽 ");

	gotoxy(MAP + 7, 18, "Backspcae ← : 재시작");

	gotoxy(MAP + 7, 20, "ESC : 종료 ");

	// 찍고서 건드리지 않는다.

	//3 = 도착지점의 좌표의 좌우상하를 검사해야 한다. 따라서 Maze에서 3을 구석에 박아두면 안 된다. (잘못된 접근)


			/*
			요로코롬 표현하는 이유는 길을 공백으로 나타내는데
			tPlayerPos 위치와 길이 동시에 출력이 되면 밀려
			나가기 때문에 플레이어 위치가 길 위치랑 같을 때
			플레이어 위치만 출력하도록 설정
			*/
		
	//각 stage 마다 설정된 현재 좌표가 startPoint 가 된다. (나중에 사용함 ) 
	if(stage != -1)
	tStartPos = tPlayerPos;

	Fmod->update();

	while (_kbhit()) _getch();
	// 냠냠

}


void OutPut()
{
	Fmod->update();

	for (int i = 0; i < 30; i++) //행 y
	{
		for (int j = 0; j < 30; j++) //열 x
		{

			/*
			요로코롬 표현하는 이유는 길을 공백으로 나타내는데
			tPlayerPos 위치와 길이 동시에 출력이 되면 밀려
			나가기 때문에 플레이어 위치가 길 위치랑 같을 때
			플레이어 위치만 출력하도록 설정
			*/

			switch (Maze[i][j])
			{
			case ' ':
				 if (num == 0)
					gotoxy(LEFTSPACE + j, UPSPACE + i, " ");
				break;
			case '0':
			case 'e':
				if (num == 0)
				{
					switch (stage)
					{
					case 3:
						TextColor(GRAY);
						break;
					default:
						TextColor(WHITE);
					}
					gotoxy(LEFTSPACE + j, UPSPACE + i, "■");
					// title의 경우 찍은 자리에 또 찍어줌. (속도는 괜찮음 )
					if (stage == -1 || stage == 7)
					{
						TextColor(DEEP_WHITE);
						gotoxy(LEFTSPACE + j, UPSPACE + i, "◆");

					}
				}
				break;
			case '3':
				TextColor(DEEP_YELLOW);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "★");
				break;
			case '5':
				switch (stage)
				{
				case 1:
					TextColor(DEEP_OC);
					break;
				case 2:
					TextColor(DEEP_RED);
					break;
				case 3:
					TextColor(DEEP_JAJU);
					break;
				case 5:
					TextColor(GRAY);
					break;
				default:
					TextColor(DEEP_YELLOW);
				}
				gotoxy(LEFTSPACE + j, UPSPACE + i, "▦");
				break;
			case '6':
				TextColor(GRAY);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "Ο");
				break;
			case '7':
				TextColor(DEEP_GREEN);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "Ω"); // sw off

				// 스위치의 개수를 세주어야 하니까 off의 수를 센다.

				if (num == 0)
					switchCount++;

				// Q1. arrow처럼 스위치의 좌표, 시작좌표, end Point가 필요한가 ?
				// A1. 푸시블럭을 통한 처리로(이미 move에서) 굳이 좌표가 필요없다. 시작좌표( 다시 돌아가지 않는다 ) endPoint( 스위치라서 의미가 없다)
				// ㄴ 필요없다. 개수만 세면 된다.
				break;

			case '8':
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "Φ"); // sw on //넘어감 불가.
				break;
			case '9':
				switch (stage)
				{
				case 2:
					TextColor(DEEP_OC);
					break;
				default:
					TextColor(DEEP_RED); // 2스테이지만 옥색,

				}
				gotoxy(LEFTSPACE + j, UPSPACE + i, "※"); // 모든 스위치 on . OPEN  :: 막힌 블럭.
				break;

			case '<':
			case '>':
			case '-':
			case '+':
				TextColor(DEEP_WHITE);
				switch (Maze[i][j])
				{
				case '+':
				case 'u':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "↑");
					break;
				case '-':
				case 'd':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "↓");
					break;
				case '>':
				case 'r':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "→");
					break;
				case '<':
				case 'l':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "←");
					break;

				}
				if (num == 0)
				{
					arrowStart[arrowCount].x = j;
					arrowStart[arrowCount].y = i;
					arrowPos[arrowCount] = arrowStart[arrowCount];
					arrowVector[arrowCount] = Maze[i][j];
					arrowCount++;
				}
				break;
			default: // 1 4 2가 입력된 경우에는 if문으로 처리해주기. 

				if (Maze[i][j] == '1' && tPlayerPos.x == j && tPlayerPos.y == i
					|| Maze[i][j] == '2' && tPlayerPos.x == j && tPlayerPos.y == i
					|| Maze[i][j] == '4' && tPlayerPos.x == j && tPlayerPos.y == i)
				{
					TextColor(DEEP_RED);
					gotoxy(LEFTSPACE + j, UPSPACE + i, "♥");
				}// 선처리 완료. 이후에 다른 것은 어떻게 ? ...
				else if (Maze[i][j] == '1' || Maze[i][j] == '2')
					gotoxy(LEFTSPACE + j, UPSPACE + i, " ");

				else if (Maze[i][j] == '4')
				{
					TextColor(DEEP_JAJU);
					gotoxy(LEFTSPACE + j, UPSPACE + i, "δ");
				} // 1,2,4 부분은 if문 우선순위에 따라 처리하도록 한다.

				// 상단 문제의 부분. 수정하기. 1,2,4는 너무 꼬여 있어서 if가 가장 쉽겠다.

			}
			//스위치 케이스문으로 바꾸니 정말 빨라졌다... ㅇㅇ 소름 

			Fmod->update();




			 // if 문을 switch case 로 바꿔서 처리 : 
		/*
			if (Maze[i][j] == '0' || Maze[i][j] == 'e')
			{
				if (num == 0)
				{
						TextColor(WHITE);
					gotoxy(LEFTSPACE + j, UPSPACE + i, "■");
				}
			}
			//
			else if (Maze[i][j] == '1' && tPlayerPos.x == j && tPlayerPos.y == i
				|| Maze[i][j] == '2' && tPlayerPos.x == j && tPlayerPos.y == i
				|| Maze[i][j] == '4' && tPlayerPos.x == j && tPlayerPos.y == i)
			{
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "♥");
			}// 선처리 완료. 이후에 다른 것은 어떻게 ? ...
			else if (Maze[i][j] == '1' || Maze[i][j] == '2')
				gotoxy(LEFTSPACE + j, UPSPACE + i, " ");
			//
			else if (Maze[i][j] == '3')
			{
				TextColor(DEEP_YELLOW);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "★");

			}//
			else if (Maze[i][j] == '4')
			{
				TextColor(DEEP_JAJU);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "δ");

			}//
			else if (Maze[i][j] == '5')
			{
				TextColor(DEEP_YELLOW);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "▦");

			}//
			else if (Maze[i][j] == '6')
			{
				TextColor(GRAY);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "Ο");
				//pushable block
			}//
			else if (Maze[i][j] == '7')
			{
				TextColor(DEEP_GREEN);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "Ω"); // sw off

				// 스위치의 개수를 세주어야 하니까 off의 수를 센다.

				if (num == 0)
					switchCount++;

				// Q1. arrow처럼 스위치의 좌표, 시작좌표, end Point가 필요한가 ?
				// A1. 푸시블럭을 통한 처리로(이미 move에서) 굳이 좌표가 필요없다. 시작좌표( 다시 돌아가지 않는다 ) endPoint( 스위치라서 의미가 없다)
				// ㄴ 필요없다. 개수만 세면 된다.

			}//
			else if(Maze[i][j] == '8')
			{
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "Φ"); // sw on //넘어감 불가.

			}//
			else if (Maze[i][j] == '9')
			{
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "※"); // 모든 스위치 on . OPEN  :: 막힌 블럭.
			}

			//
			else if(Maze[i][j] == '-' || Maze[i][j] == '+' ||
				Maze[i][j] == '>' || Maze[i][j] == '<')
			{
				TextColor(DEEP_WHITE);

				 if (Maze[i][j] == '+')
					gotoxy(LEFTSPACE + j, UPSPACE + i, "↑");
				else if (Maze[i][j] == '-')
					gotoxy(LEFTSPACE + j, UPSPACE + i, "↓");
				 else if (Maze[i][j] == '>')
					 gotoxy(LEFTSPACE + j, UPSPACE + i, "→");
				 else if (Maze[i][j] == '<')
					 gotoxy(LEFTSPACE + j, UPSPACE + i, "←");

				 //arrowPos[i]꼴로 우리는 각각의 구조체에 접근할 수 있다. x y GOOD
				 // 이 자체가 포인터. 이 포인터가 가리키는 구조체의 멤버 x // arrowPos[i]->x
				 // 참고로 end나 arrow위치는 전체탐색으로 뒤질 거니까 괜찮다. (순서에 상관없다.)

				 if (num == 0)
				 {
					 arrowStart[arrowCount].x = j;
					 arrowStart[arrowCount].y = i;
					 arrowPos[arrowCount] = arrowStart[arrowCount];
					 arrowVector[arrowCount] = Maze[i][j];
					 arrowCount++;
				 }


				 // 화살이라고 인식이 되는 경우에 우리는 위치를 저장해야 한다.
				 //폭탄의 경우에는 플레이어 위치에 넣는 것이라서 그때 그때 처리하면 됨. ㅇㅇ

			}//
			*/

		}

	}
	if (num == 0) // 처음 set이후에 num == 1이다. 
		num++;

}

void CreateBomb()
{
	Fmod->update();

	if (iBombCount == 5) // max = 5
		return;

	for (int i = 0; i < iBombCount; i++)
	{
		if (tPlayerPos.x == tBombPos[i].x &&
			tPlayerPos.y == tBombPos[i].y)
			return; // 지금 폭탄의 위치랑 플레이어의 위치가 같은지 보고 같으면 설치불가 return;
	}

	//문제 없는 경우
	FMOD::Sound		*bomb(nullptr);

	Fmod->createSound("bomb.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel3);
	channel3->setVolume(speaker);
	Sleep(50); // 미세한 패널티 
	// 폭탄위치와 겹치지 않거나 최대수를 넘지 않은 경우에는 현재 count에 맞도록 플레이어 위치 데입 
	tBombPos[iBombCount] = tPlayerPos;
	++iBombCount;
	Maze[tPlayerPos.y][tPlayerPos.x] = '4';
	Fmod->update();
	

}



/*


0 : 벽
1 : 길
2 : 시작점
3 : 도착점
4 : 폭탄

5 : 폭탄으로 부실 수 있는 약한 벽

6 : pushable Block
7 : SW 스위치 변수 ( 모든 스위치가 on일 때 탈출 가능)
(구체적으로는 스위치 변수를 밀수있는 블럭으로 덮어씌울 때 닫혀 있던 벽이 열리고
도착점으로 갈 수 있는 형태.)

8 : Sw 스위치 변수 ON형태.

< : to 왼쪽 arrow
> : to 오른쪽 arrow

- : to 아래쪽 arrow
+ : to UP arrow

각각의 arrow는 끝 위치가 존재한다. 따라서 패턴은 그대로. ㅇㅇ arrow의 끝은 통일해도 문제
? 없겠죠. 왜냐면 위로가거나 아래로 가거나 기행종은 없으니까 해당 arrow가 만날 수 있는
end지점은 항상 하나밖에 없으므로 (2개씩 한다면 모를까나.)

e : arrow end Point

e는 벽으로 출력.


*/


/*
		우리는 player가 움직일 때(움직임에 성공했을 때 jump.mp3파일을 실행시킨다. (release X)

*/
void MoveUp()
{
	Fmod->update();

	if (tPlayerPos.y == 0)
		return;
	//우리가 생각하는 x 의 개념은 열의 수를 의미 
	//y는 행의 수를 의미한다.

	//따라서 Maze[y][x] 로 사용한다.
	
	int sw = 0;
	int pushBlock = 0;
	// 현재 생기는 문제 : 플레이어 - 6블럭 - arrow 일때 예외처리가 되어 공이 움직이지 않음. = > 움직이도록 변경.
	switch (Maze[tPlayerPos.y - 1][tPlayerPos.x])
	{
	case '0':
	case '4':
	case '5':
	case '7':
	case '8':
	case 'e':
	case '9':
	case 'u':
	case 'd':
	case 'l':
	case 'r':
	case 'o':
		return;
	case '6':
		switch (Maze[tPlayerPos.y - 2][tPlayerPos.x])
		{
		case '1':
		case '2':
			// 공 굴리기
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1'; pushBlock = 1;
		
			break;
			// 스위치
		case '7':

			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '8'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1'; sw = 1;
			switchOnCount++;
		
			break;
			//화살 사라짐 처리
		case '>':
		case '<':
		case '-':
		case '+':
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '1'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';
			break;
			// 움직이면 안 되는 경우
		case '6': // push push return
		case '0':
		case '4':
		case '5':
		case '8':
		case '9':
		case 'e':

			return;
		}
		break;
	}
	

	if (sw == 1)
	{// 스위치가 on 이런 경우에는 실행될 때 까지 기다리도록.
		

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel3);

	}
	//// push block 
	


	if (pushBlock == 1)
	{// 푸시블럭이 이동.
		FMOD::Sound		*push(nullptr);

		Fmod->createSound("push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel3);

	}

	// 움직이는 놈은 여기까지 도착. jump.mp3

	 Fmod->createSound("jump.mp3", FMOD_DEFAULT, 0, &jump);// 1번자리
	 if (soundEffect)
	 Fmod->playSound(jump, 0, false, &channel3);
	--tPlayerPos.y;
	channel3->setVolume(speaker); // Play 이후에 셋 해야 적용됨.

	Fmod->update();
	/*else if (Maze[tPlayerPos.y - 1][tPlayerPos.x] != '0' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != '4' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != '5' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != '7' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != '8' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != 'e' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != '9' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != 'u' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != 'd' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != 'l' &&
		Maze[tPlayerPos.y - 1][tPlayerPos.x] != 'r' &&

		Maze[tPlayerPos.y - 1][tPlayerPos.x] != 'o' ok
		)// u d l r o 는 죽은 화살 / 죽은 push 기 때문에 움직이지 못하도록 한다.
	{
		if (Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y - 2][tPlayerPos.x] == '1'||
			Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y - 2][tPlayerPos.x] == '2') // 플레이어 - Push block - 길인 경우
		{
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';//
		}
		else if (Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y - 2][tPlayerPos.x] == '7')
		{
			// 플레이어 - push - sw 인 경우
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '8'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';
			switchOnCount++;
		}//
		else if (Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6') return; // 플레이어 - 푸시 블럭 - 벽인 경우//
		--tPlayerPos.y;

	}
	*/
}

void MoveDown()
{
	Fmod->update();

	if (tPlayerPos.y == 30)
		return;
	//우리가 생각하는 x 의 개념은 열의 수를 의미 
	//y는 행의 수를 의미한다.

	//따라서 Maze[y][x] 로 사용한다.

	int sw = 0;
	int pushBlock = 0;

	switch (Maze[tPlayerPos.y + 1][tPlayerPos.x])
	{
	case '0':
	case '4':
	case '5':
	case '7':
	case '8':
	case 'e':
	case '9':
	case 'u':
	case 'd':
	case 'l':
	case 'r':
	case 'o':
		return;
	case '6':
		switch (Maze[tPlayerPos.y + 2][tPlayerPos.x])
		{
		case '1':
		case '2':
			Maze[tPlayerPos.y + 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y + 1][tPlayerPos.x] = '1'; pushBlock = 1;
			break;
		case '7':
			Maze[tPlayerPos.y + 2][tPlayerPos.x] = '8'; Maze[tPlayerPos.y + 1][tPlayerPos.x] = '1'; sw = 1;
			switchOnCount++;
			break;
		case '>':
		case '<':
		case '-':
		case '+':
			Maze[tPlayerPos.y + 2][tPlayerPos.x] = '1'; Maze[tPlayerPos.y + 1][tPlayerPos.x] = '1';
			break;
		case '6': // push push return
		case '0':
		case '4':
		case '5':
		case '8':
		case '9':
		case 'e':

			return;
			//플레이어 - 푸시 블럭 - 벽인 경우
			return;
		}
		break;
	}

	//스위치 출력 부분 

	

	if (sw == 1)
	{// 스위치가 on 이런 경우에는 실행될 때 까지 기다리도록.

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 



	if (pushBlock == 1) // 푸시블럭이 이동.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound("push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	

	Fmod->createSound("jump.mp3", FMOD_DEFAULT, 0, &jump);
	if (soundEffect)
	Fmod->playSound(jump, 0, false, &channel4);
	++tPlayerPos.y;

	channel4->setVolume(speaker);
	Fmod->update();


	/*
	else if (Maze[tPlayerPos.y + 1][tPlayerPos.x] != '0'&&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != '4' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != '5' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != '7' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != '8' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != 'e' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != '9' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != 'u' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != 'd' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != 'l' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != 'r' &&
		Maze[tPlayerPos.y + 1][tPlayerPos.x] != 'o')
	{
		if (Maze[tPlayerPos.y + 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y + 2][tPlayerPos.x] == '1' ||
			 Maze[tPlayerPos.y + 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y + 2][tPlayerPos.x] == '2' )

		{
			Maze[tPlayerPos.y + 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y + 1][tPlayerPos.x] = '1';
		}
		else if (Maze[tPlayerPos.y +1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y + 2][tPlayerPos.x] == '7')
		{
			// 플레이어 - push - sw 인 경우
			Maze[tPlayerPos.y + 2][tPlayerPos.x] = '8'; Maze[tPlayerPos.y + 1][tPlayerPos.x] = '1';
			switchOnCount++;

		}
		else if (Maze[tPlayerPos.y + 1][tPlayerPos.x] == '6') return;
		++tPlayerPos.y;

	}

	*/
}


void MoveRight()
{
	Fmod->update();

	int sw = 0;
	int pushBlock = 0;
	
	if (tPlayerPos.x >= 30)
		return;
	//우리가 생각하는 x 의 개념은 열의 수를 의미 
	//y는 행의 수를 의미한다.
	
	//따라서 Maze[y][x] 로 사용한다.
	switch (Maze[tPlayerPos.y][tPlayerPos.x + 1])
	{
	case '0':
	case '4':
	case '5':
	case '7':
	case '8':
	case 'e':
	case '9':
	case 'u':
	case 'd':
	case 'l':
	case 'r':
	case 'o':
		return;
	case '6':
		switch (Maze[tPlayerPos.y][tPlayerPos.x + 2])
		{
		case '1':
		case '2':
			Maze[tPlayerPos.y][tPlayerPos.x + 2] = '6'; Maze[tPlayerPos.y][tPlayerPos.x + 1] = '1'; pushBlock = 1;
			break;
		case '7':
			Maze[tPlayerPos.y][tPlayerPos.x + 2] = '8'; Maze[tPlayerPos.y][tPlayerPos.x + 1] = '1'; sw = 1;
			switchOnCount++;
			break;
		case '>':
		case '<':
		case '-':
		case '+':
			Maze[tPlayerPos.y][tPlayerPos.x + 2] = '1'; Maze[tPlayerPos.y][tPlayerPos.x + 1] = '1';
			break;
		case '6': // push push return
		case '0':
		case '4':
		case '5':
		case '8':
		case '9':
		case 'e':

			return;
			//플레이어 - 푸시 블럭 - 벽인 경우
			return;
		}
		break;
	}
	////스위치 출력 부분 

	
	
	if (sw == 1) // 스위치가 on 이런 경우에는 실행될 때 까지 기다리도록.
	{

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	
	

	if (pushBlock == 1) // 푸시블럭이 이동.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound("push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	


	Fmod->createSound("jump.mp3", FMOD_DEFAULT, 0, &jump);
	if (soundEffect)
	Fmod->playSound(jump, 0, false, &channel4);
	++tPlayerPos.x;
	channel4->setVolume(speaker); // push, 스위치는 1 삐용은 0.5
	Fmod->update();

	

	/*

	else if (Maze[tPlayerPos.y][tPlayerPos.x + 1] != '0'&&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != '4' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != '5' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != '7' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != '8' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != 'e' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != '9' &&

		Maze[tPlayerPos.y][tPlayerPos.x + 1] != 'l' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != 'r' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != 'u' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != 'd' &&
		Maze[tPlayerPos.y][tPlayerPos.x + 1] != 'o'
		)

	{
		if (Maze[tPlayerPos.y][tPlayerPos.x + 1] == '6' && Maze[tPlayerPos.y][tPlayerPos.x + 2] == '1' ||
			Maze[tPlayerPos.y][tPlayerPos.x + 1] == '6' && Maze[tPlayerPos.y][tPlayerPos.x + 2] == '2')
		{
			Maze[tPlayerPos.y][tPlayerPos.x + 2] = '6'; Maze[tPlayerPos.y][tPlayerPos.x + 1] = '1';
		}
		else if (Maze[tPlayerPos.y][tPlayerPos.x + 1] == '6' && Maze[tPlayerPos.y][tPlayerPos.x + 2] == '7')
		{
			// 플레이어 - push - sw 인 경우
			Maze[tPlayerPos.y][tPlayerPos.x+2] = '8'; Maze[tPlayerPos.y][tPlayerPos.x+1] = '1';
			switchOnCount++;

		}
		else if (Maze[tPlayerPos.y][tPlayerPos.x + 1] == '6') return;
		++tPlayerPos.x;


	}

	*/
}


void MoveLeft()
{
	Fmod->update();

	if (tPlayerPos.x == 0)
		return;

	//우리가 생각하는 x 의 개념은 열의 수를 의미 
	//y는 행의 수를 의미한다.

	int sw = 0;
	int pushBlock = 0;
	//따라서 Maze[y][x] 로 사용한다.
	switch (Maze[tPlayerPos.y][tPlayerPos.x - 1])
	{
	case '0':
	case '4':
	case '5':
	case '7':
	case '8':
	case 'e':
	case '9':
	case 'u':
	case 'd':
	case 'l':
	case 'r':
	case 'o':
		return;
	case '6':
		switch (Maze[tPlayerPos.y][tPlayerPos.x - 2])
		{
		case '1':
		case '2':
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '6'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1'; pushBlock = 1;
			break;
		case '7':
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '8'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1'; sw = 1;
			switchOnCount++;
			break;
		case '>':
		case '<':
		case '-':
		case '+':
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '1'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1';
			break;
		case '6': // push push return
		case '0':
		case '4':
		case '5':
		case '8':
		case '9':
		case 'e':

			return;
			//플레이어 - 푸시 블럭 - 벽인 경우
			return;
		}
		break;
	}

	

	if (sw == 1) // 스위치가 on 이런 경우에는 실행될 때 까지 기다리도록.
	{

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	


	if (pushBlock == 1) // 푸시블럭이 이동.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound("push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	 // push, 스위치는 1 삐용은 0.5

	Fmod->createSound("jump.mp3", FMOD_DEFAULT, 0, &jump);
	if (soundEffect)
	Fmod->playSound(jump, 0, false, &channel4);
	channel4->setVolume(speaker);
	--tPlayerPos.x;
	Fmod->update();
	


	/*

	else if (Maze[tPlayerPos.y][tPlayerPos.x - 1] != '0' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != '4' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != '5' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != '7' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != '8' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != 'e' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != '9' &&

		Maze[tPlayerPos.y][tPlayerPos.x - 1] != 'u' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != 'd' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != 'l' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != 'r' &&
		Maze[tPlayerPos.y][tPlayerPos.x - 1] != 'o')
	{
		if (Maze[tPlayerPos.y][tPlayerPos.x - 1] == '6' && Maze[tPlayerPos.y][tPlayerPos.x - 2] == '1' ||
			Maze[tPlayerPos.y][tPlayerPos.x - 1] == '6' && Maze[tPlayerPos.y][tPlayerPos.x - 2] == '2')
		{
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '6'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1';
		}
		else if (Maze[tPlayerPos.y][tPlayerPos.x - 1] == '6' && Maze[tPlayerPos.y][tPlayerPos.x - 2] == '7')
		{
			// 플레이어 - push - sw 인 경우
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '8'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1';
			switchOnCount++;

		}
		else if (Maze[tPlayerPos.y][tPlayerPos.x - 1] == '6') return;
		--tPlayerPos.x;


	}

	*/
}



void Fire() // 폭탄이 있어서 fire함수가 실행이 된거 아니야 그치 ? 어 ? 아닌데 ? 폭탄을 설치를 안 하고서 스페이스바를 누른 경우 (소리를 출력하면 안 된다.) return
{
	Fmod->update();

	if (iBombCount == 0) return; // 설치를 안 한 경우 그냥 나옴. (소리출력 X)

	FMOD::Sound		*bomb(nullptr);

	Fmod->createSound("explosion.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel4);
	channel4->setVolume(speaker);
	
	int block = 0; // 블럭 터졌는가 ? 
	for (int i = 0; i < iBombCount; i++) // >폭탄 처리 (사라짐..) 아 ! 여기다 ! 그니까 . . . arrow 처리를 한다고 
	{

		if (Maze[tBombPos[i].y + 1][tBombPos[i].x] == '5')
		{
			Maze[tBombPos[i].y + 1][tBombPos[i].x] = '1';
			block = 1;
		}

		if (Maze[tBombPos[i].y - 1][tBombPos[i].x] == '5')
		{
			block = 1;
			Maze[tBombPos[i].y - 1][tBombPos[i].x] = '1';
		}
		if (Maze[tBombPos[i].y][tBombPos[i].x + 1] == '5')
		{
			Maze[tBombPos[i].y][tBombPos[i].x + 1] = '1';
			block = 1;
		}

		if (Maze[tBombPos[i].y][tBombPos[i].x - 1] == '5')
		{
			Maze[tBombPos[i].y][tBombPos[i].x - 1] = '1';
			block = 1;
		}

		//상단 . 폭탄위치의 좌우 상하가 부서지는 벽인지 확인하여 맞으면
		// 길로 바꿈.
		// 여기서 상하좌우는 중간에 블럭이 있는 경우를 따짐. 맨 위에서 터트리는
		// 경우는 생각하지 않음.(그렇게 안 함)

		if (tPlayerPos.x == tBombPos[i].x && tPlayerPos.y == tBombPos[i].y ||
			tPlayerPos.x == tBombPos[i].x + 1 && tPlayerPos.y == tBombPos[i].y ||
			tPlayerPos.x == tBombPos[i].x - 1 && tPlayerPos.y == tBombPos[i].y ||
			tPlayerPos.x == tBombPos[i].x && tPlayerPos.y == tBombPos[i].y + 1 ||
			tPlayerPos.x == tBombPos[i].x && tPlayerPos.y == tBombPos[i].y - 1)
		{

			//죽는 소리
			FMOD::Sound *die(nullptr);
			Fmod->createSound("die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);// 소리1 
			
			channel2->setVolume(speaker);

			death++;

			tPlayerPos = tStartPos;  //스테이지 배열에 따라 처리해야 할 부분.
								// 폭탄위치 좌우 상하 혹은 그 위에 있었다면 죽은(?) 것으로 처리 시작위치로.
		}
		// 폭탄 위치 != arrow위치다. 그러면, 1로 데입해도 됨. arrow위치랑 같으면, 1데입 X 그걸 떠나서 데입 이전 값이 1이면 1을 데입하면 되지 않을까?

		int sw = 0;
		for (int j = 0; j < arrowCount; j++) // 터트리고 처리하고 , 나서 화살이랑 좌표가 같은 경우가 있으면 break break 걸어서 길로 만들지 않는다.
			// 대신에 다른 폭탄의 경우 겹치지 않았으면 터지고 해당 폭탄은 그냥 화살에 밟힘. 그니까 그냥 화살의 좌표가 덮어씌운 건데 
			// 터지는 모션은 하고 있어서, 터지는데 화살을 유지시킨다고 보면 됨. 하나가 화살과 위치가 같아서 fire 4개 다터지고 
			//남은 해당 폭탄은 화살에 밟히고(터지고)
		{
			if (arrowPos[j].x == tBombPos[i].x && arrowPos[j].y == tBombPos[i].y)
			{
				sw = 1;
				break;
			}

		}
		if (sw == 1) break;
		// 현재 폭탄이 arrow의 좌표 중에 하나라도 겹치면 현재 폭탄에 대한 처리를 하지 않는다. 꼬아둔 경우는 없어서 하나만 겹침.
		// 이후에 처리를 안 하고 나중에 화살의 위치 가 이동하면 그 자리를 씹어먹겠지. (없던 일 마냥)
		// 나는 근데 이것을, 플레이어와 위치를 따져보고 옆에 있으면 죽도록 만든 이후에 1로 안 하는 쪽으로 설정하고 싶다. 터지고 화살의 처리.

		// 플레이어 위치가 화살위치랑 겹칠 때 설치하면 안 된다. @@@@

		Maze[tBombPos[i].y][tBombPos[i].x] = '1';
		//x 좌표가 다르거나 y좌표가 다르면 다른 좌표. 다른 좌표일 때만 1로.


		// 모든 폭탄의 위치가.  arrow와 겹치지 않은 경우에만 1을 데입.
		//겹치는 경우에 대한 처리는 안 한다.

	// 우리가 폭탄의 위치를 가지 못하는 이유는 4로 인식이 되어있기 때문.
	// 배열의 위치는 상관없고, (다시 잘 이용하면 되고) 1로만 데입해두면 
	// 지나갈 수 있다.

	}
	if (block == 1) // 하나라도 터짐 아삭 먹는 소리 
	{
		FMOD::Sound *cookie;
		Fmod->createSound("cookie.mp3", FMOD_DEFAULT, 0, &cookie);
		if (soundEffect)
		Fmod->playSound(cookie, 0, 0, &channel3); // 바삭바삭 소리 1

		channel3->setVolume(speaker);
	}


	Fmod->update();

	iBombCount = 0; //터트린 후
}


void MovePlayer(char cInput, int* block9)
{
	Fmod->update();

	switch (cInput)
	{
	case UP:
	case 'w':
	case 'W':
		MoveUp();
		break;
	case 's':
	case 'S':
	case DOWN:
		MoveDown();
		break;
	case 'a':
	case 'A':
	case LEFT:
		MoveLeft();
		break;
	case 'd':
	case 'D':
	case RIGHT:
		MoveRight();
		break;
	}

	if (Maze[tPlayerPos.y + 1][tPlayerPos.x] == '9' || // 플레이어 위치의 좌우상하가 까시 블럭이면 sleep(10)
		Maze[tPlayerPos.y - 1][tPlayerPos.x] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x + 1] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x - 1] == '9')
		Sleep(150);


	if (tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y) // 그러니까 우리는 stage clear 시점을 사실상 움직일 때만 인식하면 된다. 
	{
		if (quest == 6) return; // 안내문구 던져야 해서 처리 안 하고 나옴. (이쪽에서 처리)

		// 한세 == 2일 때 우리는 타이틀로 보내야 한다. or 선택 ? 

		if (hansei == 2)
		{
			music->release();
			hansei = 3;
			while (_kbhit()) _getch(); // 잘 먹는다면, 다시 실행되지도 않겠지..
			// block9 off
			
			return;
		}

		FMOD::Sound *clear(nullptr);
		Fmod->createSound("clear.mp3", FMOD_DEFAULT, 0, &clear);
		if (soundEffect)
		Fmod->playSound(clear, 0, 0, &channel2);
		channel2->setVolume(speaker);
		stagePlus(); // stage clear
		SetMaze(block9);
		OutPut(); // 스위치 개수 등 수치를 세주지 않으면 문제 생김.
	}
	Fmod->update();

}

void arrowMove() // 컴퓨터 사양에 따라서 속도차이가 발생함. 일단 gotoxy를 없앨 것이 아니라면, if else if 를 switch case 문으로 교체하는 걸로 하자.
{
	Fmod->update();

	// ENd 지점은 maze로 구할 수 있다. maze [ i ] [j ] == 'e' ? 이전 arrow위치는 1로 옮기고 위치는>
	for (int i = 0; i < arrowCount; i++)
	{
		// 일단 실행이 된다는 것은 하나라도 있다. 그 자리를 길로 만들고 옮긴후 자리에 맞게 데입


		//if (Maze[arrowPos[i].y][arrowPos[i].x] == '1' ||
		//	Maze[arrowPos[i].y][arrowPos[i].x] == '6') continue;

		if (Maze[arrowPos[i].y][arrowPos[i].x] == '6') // 머가 어쩃뜬. 살아있는 화살이 공과 만났으니, 그 부분을 길로 만들고 / 화살의 좌표를 0,0 하면 문제 없다.
		{
			Maze[arrowPos[i].y][arrowPos[i].x] = '1';
			arrowPos[i].x = 0; arrowPos[i].y = 0;
			continue; // 화살과 폭탄이 겹친 경우도 아니고 / 그렇다고 플레이어와 화살이 겹친 경우도 아니므로 더 이상 처리해줄 필요가 없다. 해당화살 out

			// 근데 이렇게 해줘도 오류는 없어지지 않음. 
			// 마치 반응속도 차이가 심하게 나는 듯한 기분;; 영상 참조
		}

		if (Maze[arrowPos[i].y][arrowPos[i].x] == '0') continue;

		// 화살의 위치와 폭탄의 위치가 겹친 경우에 다시 화살이 살아난다. 근데 화살과 폭탄에 대한 처리를 막으면 아래의 경우가 해결이 되지 않는다.
		// 따라서 우리는 죽은 화살을 좌표값을 바꿔주는 것으로 하자. ex : 0,0 같은 경우는 항상 벽이므로 그 자리가 0이면 continue 처리를 하면 깔끔하다.
		// 해당 자리로 공을 굴릴 수도 없기 때문에 6번 오류 또한 생길 수 없다. 폭탄과 위치가 겹쳐서 생기는 오류 또한 생길 수 없다. (가지 못해서) 1인 경우는 없다.
		// 죽은 화살 좌표에(0.0) 폭탄을 설치하는 오류 또한 생길 수 없다. ( 못가니까) 즉, 화살과 폭탄이 겹치는 경우는 화살이 살아 있는 경우로 한정된다.

		// 화살이 죽지 않은 경우에는 0,0 이 아니기 떄문에 아래의 로직으로 처리하면 된다. O K!

		// 죽은 화살에 0,0을 데입하도록.

		// 화살이 폭탄과 겹치고 폭탄은 터지고, 1로 만들지 않고 화살이 그 자리를 밟고 간다. ( 어쨋든 터진 것으로 정상적으로 처리된다)

		// 공? 이랑 화살과의 만남은 둘을 모두 없애버린다. 하지만 화살의 좌표는 그대로이다. 우리는 이것을 돌아가지 않도록 해야한다.
		// 일단 길로 만들어진 화살은 무시해야 하고, 또 다른 경우 플레이어 위치와 겹치는 경우도 없애야 한다.
		// 왜 플레이어와 겹치는 경우를 따져야 하는가 ?
		// 일단 플레이어move에서는 특수한 케이스를 적용하여 플레이어 push 길 이런 경우를 처리한다. 
		// 그때 플레이어 push 아래 화살. 


		// 처리해주기 . @@ 별표 화살이 다시 생성이되요. 플레이어 위치와 같으면 why 나중에. 문제 ? 없는데 ? 다시 검토 ㅇㅇ 5스테이지에서
		// 공이랑 겹친 경우 다시 생기는 오류 ㅇㅇ 공일때도 out !


		switch (arrowVector[i])
		{
		case '>':
			if (Maze[arrowPos[i].y][arrowPos[i].x + 1] == '6')
			{
				Maze[arrowPos[i].y][arrowPos[i].x] = '1';
				Maze[arrowPos[i].y][arrowPos[i].x + 1] = '1';
				arrowPos[i].x = 0; arrowPos[i].y = 0; // 죽은 화살.
				break;
			}
			else Maze[arrowPos[i].y][arrowPos[i].x] = '1';
			// 화살은 기본적으로 절대무적 그냥 블럭다 없애고 이동함 ; oo 그래서 플레이어는 다시시작해야하는 상황이 있을 것 ㅋㅋ
			arrowPos[i].x++;

			if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')//이동해보니 end지점이다 -> start로
				arrowPos[i] = arrowStart[i];
			Maze[arrowPos[i].y][arrowPos[i].x] = '>';
			break;


		case '<':
			if (Maze[arrowPos[i].y][arrowPos[i].x - 1] == '6') {
				Maze[arrowPos[i].y][arrowPos[i].x] = '1';
				Maze[arrowPos[i].y][arrowPos[i].x - 1] = '1';
				arrowPos[i].x = 0; arrowPos[i].y = 0;
				break;
			}
			else Maze[arrowPos[i].y][arrowPos[i].x] = '1';

			--arrowPos[i].x;
			if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
				arrowPos[i] = arrowStart[i];
			Maze[arrowPos[i].y][arrowPos[i].x] = '<';
			break;


		case '-':
			if (Maze[arrowPos[i].y + 1][arrowPos[i].x] == '6') {
				Maze[arrowPos[i].y][arrowPos[i].x] = '1';
				Maze[arrowPos[i].y + 1][arrowPos[i].x] = '1';
				arrowPos[i].x = 0; arrowPos[i].y = 0;
				break;
			}
			else Maze[arrowPos[i].y][arrowPos[i].x] = '1';

			++arrowPos[i].y;
			if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
				arrowPos[i] = arrowStart[i];
			Maze[arrowPos[i].y][arrowPos[i].x] = '-';
			break;


		case '+':
			if (Maze[arrowPos[i].y - 1][arrowPos[i].x] == '6') {
				Maze[arrowPos[i].y][arrowPos[i].x] = '1';
				Maze[arrowPos[i].y - 1][arrowPos[i].x] = '1';
				arrowPos[i].x = 0; arrowPos[i].y = 0;
				break;
			}
			else Maze[arrowPos[i].y][arrowPos[i].x] = '1';

			--arrowPos[i].y;
			if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
				arrowPos[i] = arrowStart[i];
			Maze[arrowPos[i].y][arrowPos[i].x] = '+';

			break;
		}
		// 폭탄과 좌표가 겹쳤는지 확인한다. e를 제외하고 폭탄을 설치가능 ( 부딪힘 ㅇㅇ) 어쨌든 폭탄 좌표 == 화살의 좌표.
		for (int j = 0; j < iBombCount; j++)
		{
			if (arrowPos[i].x == tBombPos[j].x && arrowPos[i].y == tBombPos[j].y) // 폭탄과 화살이 겹치고 동시에 플레이어가 겹친다 X 이런 경우는 없다. 왜냐면 폭탄에 접근불가.
			{
				Fire(); break;
			}
		} // 모든 화살의 위치와 모든 폭탄의 위치를 비교하여 하나라도 겹치면 PANG ! 이후에 Pang 의 목적 달성 break;


		if (arrowPos[i].x == tPlayerPos.x && arrowPos[i].y == tPlayerPos.y) // arrow위치와 player위치가 같으면 시작 위치로. 
		{						//end에서는 갈 수가 없고, start or 길에서 부딪히게 된다. 위치를 바꾸고 처리를 해줘도 아무 문제가 없다.

			//죽는 소리
			FMOD::Sound *die(nullptr);
			Fmod->createSound("die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);
			channel2->setVolume(speaker);
			Fmod->update();
			death++;

			tPlayerPos = tStartPos;
		}

		Sleep(arrowSpeed); //  ??
	}
	// 증가 처리 이후에 end Pos와 같은지 비교 후 같으면 start Pos 데입
}
void stagePlus()
{
	stage++;
}

void openDoor(int* block9) //아 ! arrow랑 혼동해서 몰랐는데 arrow개수는 세주나, 수위치는 안 세고 있음 ㅋ 
{
	Fmod->update();

	if (*block9 == 1) return; // 한번만 출력하고 이후에 return; stage가 바뀌면 block9 = 0을 셋팅을 해야한다.

	if (switchOnCount == switchCount) // on개수랑 전체 개수가 같으면 (모든 스위치 on)
	{
		FMOD::Sound *door;
		Fmod->createSound("9block.mp3", FMOD_DEFAULT, 0, &door);
		if(soundEffect)
		Fmod->playSound(door, 0, 0, &channel2);

		channel2->setVolume(speaker);
		Fmod->update();
		*block9 = 1;



		if (Maze[tEndPos.y][tEndPos.x + 1] == '9')
		{
			Maze[tEndPos.y][tEndPos.x + 1] = '1'; // 여기는 길. 우리가 출력하면 공백. 
			OutPut();
			Sleep(250);
			//* 문이 열린 것이 눈에 안 띄므로 출력 / 대기를 섞어서 눈에 띄도록 한다.  공백출력 term / 다시찍고 텀/ 공백텀

			Maze[tEndPos.y][tEndPos.x + 1] = '9';
			Sleep(250);


			Maze[tEndPos.y][tEndPos.x + 1] = '1';
			OutPut();
			Sleep(250);

			// 한 군데가 아니면 다 따로따로.
		}
		if (Maze[tEndPos.y][tEndPos.x - 1] == '9')
		{
			Fmod->update();

			Maze[tEndPos.y][tEndPos.x - 1] = '1';
			OutPut();
			Sleep(250);

			Fmod->update();

			Maze[tEndPos.y][tEndPos.x - 1] = '9';
			OutPut();
			Sleep(250);
			Fmod->update();


			Maze[tEndPos.y][tEndPos.x - 1] = '1';
			OutPut();
			Sleep(250);

		}
		if (Maze[tEndPos.y + 1][tEndPos.x] == '9')
		{
			Fmod->update();

			Maze[tEndPos.y + 1][tEndPos.x] = '1';
			OutPut();
			Sleep(250);

			Fmod->update();

			Maze[tEndPos.y + 1][tEndPos.x] = '9';
			OutPut();
			Sleep(250);
			Fmod->update();

			Maze[tEndPos.y + 1][tEndPos.x] = '1';
			OutPut();
			Sleep(250);
		}
		if (Maze[tEndPos.y - 1][tEndPos.x] == '9')
		{
			Fmod->update();

			Maze[tEndPos.y - 1][tEndPos.x] = '1';
			OutPut();
			Sleep(250);
			Fmod->update();

			Maze[tEndPos.y - 1][tEndPos.x] = '9';
			OutPut();
			Sleep(250);

			Fmod->update();

			Maze[tEndPos.y - 1][tEndPos.x] = '1';
			OutPut();
			Sleep(250);

			//endPos의 좌우상하가 9에 해당할 때(특별 블럭) 그 부분을 길로 고친다.

		}
	}

	//3 = 도착지점의 좌표의 좌우상하를 검사해야 한다. 따라서 Maze에서 3을 구석에 박아두면 안 된다. (잘못된 접근)
}


void title() // 타이틀은 stage = -1으로 말 그대로 그림 그리기 위한 설정이다.
{

	/* 사운드 파일 없는 곳에서 mp3파일 만들고, 들리지도 않는 소리로 result 구분하여 지랄하는 것은 너무나도 많은 비효율을 만든다. 
	
	따라서 여기서 사운드 관련은 만지지 말고, 파일 입출력에 관해 공부하자.

	
	*/

	while (_kbhit()) _getch(); // 하나로는 부족해...

	/* 이것을 playSound // createSound 부분만 체크해주자. */
	result = FMOD::System_Create(&Fmod); // 다른 곳에서도 사용 가능 ?


	FMOD::Sound		*sing(nullptr);

	FMOD::Sound		*select(nullptr);

	 result = Fmod->init(25, FMOD_INIT_NORMAL, extradriverdata);
	 if (result != FMOD_OK) return;

	// 채널은 25개 설정 / 일반적 / 안 쓰는 포인터

	 result = Fmod->createStream("title.mp3", FMOD_LOOP_NORMAL, 0, &sing);
	 if (result != FMOD_OK) return;

	//배경화면에서는 계속 재생 sing 
	// Stream or Sound
	 result = Fmod->createSound("select.mp3", FMOD_DEFAULT, 0, &select);
	 if (result != FMOD_OK) return;

	//select effect


	//설마 그냥 꺼지는 오류가 .... 소스파일과 프로젝트 위치가 달라서 일까?
	// .... 그럴 수도 있다. 아까 dll파일에서 소스말고 프로젝트에 넣었을 때 실행.

	//시발 진짜였네..

	 result = Fmod->playSound(sing, 0, false, &channel1);
	 if (result != FMOD_OK) return;

	// sing, puase없음 채널1
	
	channel1->setVolume(speaker); // 기본 0.5
	
	
	titlePicture();
	block9 = 1;
	num = 0;
	stage = -1;
	// 더 이상 안 추가 안 해요 여기서는. 상단

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 9, "＃─────────────────────────＃");
	

	gotoxy(MAP + 5, 11, "＃─────────────────────────＃");

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "▣ Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "▣ button :  + - ");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
	// 초기 0.5 Setmaze에서 처리하지만, title에서는 setmaze를 사용하지 않으므로 중복은 아니다.

	// 나의 구상 일단 text 옆에 작은 아이콘 ? 을 해두고 이거 위에 세모로 표시해서 덮음. oo
	TextColor(DEEP_RED);
	gotoxy(MAP + 5, 10, "♥        Text Maze        ♥");
	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "         새로운 게임       ");
	// 처음 셋팅시 새로운 게임을 잡고 있음.

	gotoxy(MAP + 5, 16, "           튜토리얼        ");
	gotoxy(MAP + 5, 18, "        스테이지 선택      ");
	gotoxy(MAP + 5, 20, "           Ranking         ");
	gotoxy(MAP + 5, 22, "           종료하기        ");
	gotoxy(MAP + 5, 25, "    Copyright ⓒ2018-Yumin ");
	// ICON 의 위치는 바뀌지 않는다.
	titleIcon();



	// 꾸미기 
	TextColor(DEEP_OC);
	gotoxy(MAP + 5, 25, "▦");
	gotoxy(MAP + 19, 25, "▦");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 6, 25, "δ");
	gotoxy(MAP + 18, 25, "δ");
	// copyright

	/*
	TextColor(DEEP_GREEN);
	gotoxy(MAP + 18, 25, "Ω");
	TextColor(DEEP_RED);
	gotoxy(MAP + 19, 25, "Φ"); 
	*/
	// 레퍼런스
	TextColor(DEEP_YELLOW);
	gotoxy(1, 32, "★");
	TextColor(DEEP_WHITE);
	gotoxy(3 , 32, "References : http://assortrock.com (Maze Game) ");
	

	int locate = 0; // 맨위 무엇을 눌렀는지, 어디 위치 였는지.
	char str[40];// 비밀번호와 같은지에 사용할 배열. 너무 쪼잔하게 주면 문자를 많이 입력할 때 재미없어짐.

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ▶"); 

	// 사실상 처음에는 입력이 없다면 세모가 뜨지 않으므로 미리 잡아둬야 한다.

	//우리는 title에서 while문으로 키보드 입력을 받아 ▶ 처리로 인터페이스를 처리.
	// 이후에 누른 것 따로 처리 ..
	OutPut();

	//파일 입출력으로 실행시킬 수도 있겠네... ㅎㄷ; // 하지마라. 오지게 걸림.





	// read ( 이전 데이터가 있는지 확인합니다. ) 

	ifstream in("TMRank.txt");
	string name;
	int death;

	if (dataCheck == 0)
	{
		if (in.is_open()) // read는 처음에만 받고 이후에 clear에 따라 +1 처리가 되어 sort 저장 ...
		{
			string buffer;


			while (!in.eof())
			{// EOF가 아니면 계속. 	

				in.getline(&name[0], 100, '.'); // name[0] 부터 100개의 데이터를 넣는데 .이 나오면 그만 넣음.  name 받음.

				in.getline(&buffer[0], 10); // 이제 . 뒤에 "숫자"\n 에서 "숫자" 부분을 숫자로 바꿔서 death에 데입.
				death = atoi(buffer.c_str());

				// 받은 값을 데입. 

				arr[read] = { name, death };
				// 0인 경우가 아니라 - 1.
				if(death == 0 && name[0] || death != 0) // 시작과 동시에 들어온 경우에는 0을 가리킴( 잘못된 데이터 ) 그래서 read = 0의 상태로 유지시켜준다. -1의 경우 read를 그대로 해야한다.
				read++; //  read는 다음 원소를 (빈 자리를) 가리킨다. 따라서 새로운 값을 넣어줄 때 arr[read]하면 된다.
				//size == 0 이면 잘못도니 경우이다. 기록된 데이터가 이름이 없는 것은 말이 안 된다.
				

				/*
				조건을 size로 구분하면 안 되겠당. death가 0이고 name[0]이 있거나, death != 0 이라면 정상적인 접근. 아닌 경우에 냅두기. name[0] 씐의 한수.
				*/







				 // c++ 오버로딩 string == "" ? 로 물어보면 해결이 안 된다.. -1이면 잘못된 거 // size 0이여도 잘못된 것.  numsize하면 항상 0이 나오는 마법.
				// 메모장이 빈 경우에는 \0 을 만족하여 증가 X

				// 예외 처리. 메모장이 빈 경우(ios:: end == ios::beg ) read ++ 을 해서는 안 된다. ( read ++ 로 1이 되면 받는 값도 1부터 시작되고 이상한 값이 들어감)


			} // 이후에 이 받은 값을 가지고 sort를 한다. 다시 파일입출력을 하기 때문에 close 처음 입력시 : 시작되고 i == j라서 check =1 이후에 돌리지 않음. 
						// sort의 경우에도 read ++이 되지 않은 상태라서 돌리지 않는다. 오직 clear 이후에만 sort 후 save가 실행됨.

		
		}
		 // read = 0; 이라면 정렬도 안 되고 뭐도 안 되고... .0은 없을 것이다.
												// EOF 면 clear 부터 입력받고 촥촥 처리. 그 이전에는 EOF 그대로. OK
		dataCheck = 1;
		in.close();
		// 아무것도 없으면 처리를 하지 않고 close read = 0 sort 저장도 없다. 이후에는 일단 체크를 했으니 다시 검사하지 않는다.
	}
	
	
	// 지금 현재 랭킹을 눌렀을 때만 sort후에 저장을 하고 있다.

	// 하지만, 이 과정은 랭킹을 누르지 않더라도 처리를 해야 하는 부분이다. 그러니 title에서 처리를 하면 어떨까 ? 

	// title에서 처리를 하는 경우 1. 처음 시작 : 기존데이터 setting sort 저장 lanking 잘 처리.  2. clear 이후에 main메뉴 + 1 되었으니 sort 저장 잘 처리함.
	// 만약에 플레이 중에 메인으로 나오는 경우 정렬한 데이터를 다시 정렬을 하고 저장하니 의미없는 짓 아닌가요 ? ( 물론 맞다. 근데 컴퓨터는 빠르다.. 그리고 데이터 100개...)


	// 파일이 없었고 read == 0이면  그냥.. 자기 자신... ㅇㅇ 입력도 없죠. 

	sort(arr, arr + read, cmp);  // 정렬을 한 이후에는 적어줘야 한다. cmp good

	ofstream txt("TMRank.txt"); // 기본 형식이라 다 지우고 처리함.  없으면 만들고, 있으면 지우고

	 // 처음에 시작이 된 경우에 1이라서 out  만약에 한 가지 값이라도 있었다면, 0번째를 잡아서 넣고 1을 가리키고 EOF탈출. 
					// 어짜피 하나의 값이기에 데입을 하지 않지만, 여러 값이 있다면


	for (int i = 0; i < read; i++) // read는 마지막 인자보다 1큰수.아 ! 위에서 0 이고 오픈이라 ++해주고 여기서 한번 데입을 하는데 name이 없지.. 
	{											// 결론적으로 오류... ㅇㅇ 처음 열릴 때 read = 1 로 들어오는데 이 경우를 막아야 한다.


												// 그러면 처음에 받은 daeth 0 name없는 부분은 문제가 없는가 ? 
		txt << arr[i]; // 여기서 데입이 안 되네 . . .  만든 문자열을 (이거 오버로딩 왜 만든 거 ;;)  문자열 생성. 이것을 파일에 데입. 애초에 name을 이상하게 처리함.

					   // 정렬된 데이터를 이름.데스\n이름.데스\n ...으로 입력. 
	}
	txt.close(); // 소멸자가 있긴 할텐데 설마 이거 때문에 입력이 제대로 안 될까... 이름을 왜 저장을 못할까 ?
				 //아무래도 모호한 이야기를 하는 것은 case문에서 ofstream을 선언했기 때문인 것 같다. 함수처리 하면 될 줄 알았는데  .. . 쓰읍.  어! 잠만 그러면... 스위치문만 나오면 되네 ?
				 // 아니 std:: 함수랑 이름이 겹치는 거였나보네; ....
	

	//이렇게 title마다 sort후 저장. 랭킹을 누른 경우에 출력. 반복하는 처리. 


	while (_kbhit()) _getch(); // 버퍼.

	while (true)
	{
		 Fmod->update();
		if (_kbhit())
		{
			char input = _getch();
			if (input == 224)
				input = _getch();// 키보드 입력시 2번째 값으로.

			// 메인메뉴에서 배경음 소리 지정할 수 있도록 한다.
			// 여기서 설정한 값이 이후 게임 내 모든 sound설정.



			switch (input)  // 예를 들어 맨 위에서 위를 누르면 더 이상 못 가도록. 맨 아래에서 아래를 누른 경우 더 이상 못 가도록. 위치를 파악하는 변수가 필요.
			{	
				// locate 변수 이용, locate 변수에 따른 아이콘 찍기. 그냥 = 함수로 한 번 찍고 (어짜피 5번... 처리빠름) 해당 부분만 다시찍기.
			case '=':
				//+를 누르면 증가하고 찍어야함.
				speaker += 0.1;
				if (speaker > 1)
					speaker = 1;
				TextColor(DEEP_YELLOW);
				gotoxy(MAP + 17, 31, "            "); // 해당 자리 청소.
				gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
				channel1->setVolume(speaker);
				break;
			case '-':
				speaker -= 0.1;
				if (speaker < 0) // 0까지는 가능.
					speaker = 0;
				TextColor(DEEP_YELLOW);
				gotoxy(MAP + 17, 31, "            ");
				gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
				channel1->setVolume(speaker);

				break;
			case UP:
			case 'w':
			case 'W':
				titleIcon();
				switch (locate) // 현재의 상태 4에서 up -> 3으로 이동할 예정임으로 3번위치에 찍어야 함. (이후에 --처리)
				{
				case 0: 
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 14, "  ▶"); //더 이상 올라갈 곳이 없으므로 자신의 자리.
					break;
				case 1:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 16, "  ▶");
					gotoxy(MAP + 5, 14, "  ▶");
					break;
				case 2:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 18, "  ▶");
					gotoxy(MAP + 5, 16, "  ▶");
					break;
				case 3:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 20, "  ▶");
					gotoxy(MAP + 5, 18, "  ▶");

					break;
				case 4:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 22, "  ▶");
					gotoxy(MAP + 5, 20, "  ▶");
					break;
				}
				locate--;
				if (locate == -1)
					locate = 1; // 맨 위(0)에서 위를 누른 경우. 

				//위로 이동
				break;
			case DOWN:
			case 's':
			case 'S':
				titleIcon();
				switch (locate) // 현재의 상태 0에서 아래로 이동 1의 위치를 찍고 이후에 ++ 처리
				{
				case 0:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 16, "  ▶");
					break;
				case 1:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 18, "  ▶");
					break;
				case 2:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 20, "  ▶");
					break;
				case 3:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 22, "  ▶");

					break;
				case 4:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 22, "  ▶"); // 더 이상 내려갈 곳이 없어서 자신의 위치.
					break;
				}
				locate++;
				if (locate == 5)
					locate = 4; // 맨 아래에서 아래를 누름. 
				
				//아래로 이동// 맨 아래에서 아래를 누른 경우 자리 유지. 처리 OK 근데 너무 빠름.
				break;

			case SPACEBAR:
			case ENTER:
				 result = Fmod->playSound(select, 0, false, &channel2);
				 if (result != FMOD_OK) return;

				// select음은 채널2로 1.0  0초미만이라 release할 필요 X
				switch (locate)
				{


				case 0: // 시작 하기 (기본 셋팅 대로)
					locate = 5;
					stage = 1; // 1부터 시작.
					break;
					// 처리. 



				case 1: //튜토리얼 (새로운 맵 떤져 주기) 튜토리얼이면 stage = 0
					locate = 5; 
					stage = 0; // 이후에 튜토리얼 스테이지에서 클리어시 1stage로 이동. 타이틀은 case -1:
					break;
					// 맵 만들어야 한다.



				case 2: // 스테이지 선택 (비밀번호)
					
					if (hansei == 0)
					{
					gotoxy_C(MAP + 5, 3, "Password : ",DEEP_WHITE);  // 포인터는 배열을 따라하는 거지, 배열의 역할을 대신하지 않는다. 
																		// 배열은 값을 잡지만, 포인터는 scnaf를 통해 바로 받을 수는 없다. 이후에 받은 값을 인자로
																		// 받아오는 경우에는 사용가능 . 배열이 있어서 포인터가 있다.
					
																	// 처음부터 포인터를 배열처럼 scanf로 받아올 수 없다.

					while (_kbhit()) _getch(); // 버퍼. 

					
						scanf_s("%s", str, sizeof(str));
						if (compare(str, "hansei")) // 파라메터가 상태가 달라서 찾느라 애먹음 .. . 한 20분 쓴 듯 다행이다...
						{
							hansei = 1;
							locate = 8; // 맞으면 8 틀리면 데입 없음. 다시 선택하는 부분. 반복.  제대로 입력시에 내려가서 함수처리.
						} // OK 

						gotoxy(MAP + 5, 3, "                               "); // 입력한 경우에만 청소하면 되죠. free 패스는 할 필요가.. ( 할 필요는 없지만 보이지 않게 하고있엇쥐)

					}
					else
						locate = 8; // 입력한 이후에는 프리패스


					//틀리면 무시 맞을 때만 이동. -> 함수처리 seletor 그냥 간단하게 숫자받아서 처리. compare.
					// 여유삼아 3줄 clear
					// 혹시나 배열칸을 넘어서 감히 타이틀 부분을 침범했을 경우를 대비하여 다시 찍어주기.  
					num = 0; // num = 0을 해야 벽 ◆ 부분을 처리함. ' '도 마찬가지.
					 // 잘 실행된다. 이제 실질적인 처리가 필요.
					break;




				case 3: //랭킹 ( 파일 입출력을 통한 offline 랭킹 )
					system("cls");
					num = 0;
					// 타이틀이 출력된 이후라서 벽을 출력하지 않는다. 공백은 출력해도 보이지 않으니 결국 아무 것도 출력이 안 되는 것 처럼 보인 것이다. 
					titleSpace(); // 문제 왜 안 뜨냐 ? => 아무래도 출력부분에서는 셋메이즈에서 설정해주지 않는 값이 output에서 돌리는 것 아닐까 ? 맞네.. 
										// OutPut에서는 PlayerPos.x y를 가지고 노는데 그 값을 전혀 설정을 안 해주면 .. ..  제대로 돌아갈 리가 없지. ? 아니 그래도 0 셋팅이...

					ranking(); //랭킹 보다가 다시 나와서 loop돌도록. 이후에 ..
				
					system("cls"); // 나는 title에 cls가 있는 줄 알았는데 없다. 잘 실행되나 노래는 안 끊김.... release sing이 부모 함수에 있어서 못씀.. 그냥 통째로 없애버림.
					sing->release(); // 이전 노래 꺼지고. 아래 내려가서 바로 생긴 노래 꺼지지 만 곧 다시 켜진다. 그나마 제일 낫다.
				
					title(); //다시 Fmod 재생성 / 노래 재생성. 아 짜증나 .. 노래 끊기는게 더 병맛이야.. (ㅋㅋㅋ)  근데 그냥 냅두자. 좆같다. ㅋㅋㅋ

					locate = 7;

					// 그냥 이전 title의 노래만 release 해주면 이후에 title루프에서는 아라서 처리가 되고 결국 노래를 ..
					//부모의 노래만 계속 없애.  . . .  그러다가 다른 걸 고르면 끊나고 끊나고 끊나고 ... 해당 부분으로 덮에 씌어짐. GOOD 

					// 좋은 생각이고 잘 적용된다. ( 근데 ERROR로 계속 누르면 이상해짐 ... ) ㅎㄸ;;

					break;



				case 4:// 종료
					exit(0);
					break;
				}

				while (_kbhit()) _getch(); // 버퍼. enter이후에 누른 값을 일단 무시. ( 무시하지 않으면 이상하게 됨)

				//확인
				break;
			case ESC: // 종료  확인과 종료는 다시 찍을 필요가 없는 부분. 혹시 조금 이펙트를 주고 싶다면 필요하긴 함.
				exit(0); // 혹시나 ESC 누른 경우 그냥 끝내기 처리. (별 신경 ㄴㄴ)
				break;
			
			}
			
		} // kbhit if
		switch (locate)
		{
		case 8: // hansei 함수처리후 리턴. 해서 부모함수를 끝냄.
			sing->release();

			stageSelect();
			return;
			// 오기전에 return.
			case 5:
			
			case 7:
			sing->release();
			while (_kbhit()) _getch(); // 버퍼.  여기닷.  다시 타이틀로 이동할 때의 시간동안 만든 버퍼를 먹는다. 냠냠. 

			return;
		}// 이후에 main -> set 1스테이지부터 . . .  // 튜토리얼 시작

		
	}//while true

	
}
void stageSelect()
{
	// stage 셀렉트를 통해 한번 이용을 한 경우에는 랭킹 등록에 불가 ( 왜냐면 처음부터 안 하잖아. )  1번 이용 -> main으로 이동.

	// 그림보고 출력하도록 해줄까 ? ㅋㅋㅋ
	system("cls");
	int locate = 1;
	char input = NULL;
	
	// 좌우 이동으로 처리하고 싶다만... 어또카쥐 ? 
	selectOpption();

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ▶");

	while (_kbhit()) _getch();

	if (hansei == 2) hansei = 1; // 다시 시작하는 경우.
	while (true)
	{
		

		
		
		// 계속 cls 쓸 것이라서 출력의 반복.
		Fmod->update();
		if (_kbhit())
		{
			char input = _getch();
			if (input == 224)
				input = _getch();// 키보드 입력시 2번째 값으로.

								 // 메인메뉴에서 배경음 소리 지정할 수 있도록 한다.
								 // 여기서 설정한 값이 이후 게임 내 모든 sound설정.
			switch (input)
			{
			case ESC:
				exit(0);
				break; // 화살표 말고 다른 값에 대한 처리. m M backspace는 함수를 끝내고.
			}
			

			system("cls");
			selectOpption();
			selectPrint(input, locate); //처음에 기본적으로 찍고 이후에 입력 cls 기본적인 옆구리 찍고, 화살표. 그리고 맵. 이후에 다시 입력을 하면. cls 옆구리. 화살표. 맵. 반복.
		
			if (hansei == 2) { // enter input or spacebar input 
				// 그곳으로 떠나야 하는 부분. 
				SetMaze(&block9);
				while (_kbhit()) _getch();
				return; // SetMaze하고 return. 일단 title이니까. return return 되어서 끝. ( 처음 입력의 타이틀 이라면) OK main 참조. 
				// 처음 입력의 타이틀이 아니라, 나중에 main while 내부 title의 경우에는.( 사실 setmaze내부지만)
			}
				// GOD C ++ 포인터가 입력을 씹어먹는 걸 레퍼런스로 성공시키다.
				
			num = 0; // 해주지 않으면 맵이 제각각 따로 논다.
			stage = locate; // 스테이지 별로 다르게 설정한 부분이 있어서.. 이렇게 해야한다.

			switch (locate) // 위치에 따른 (map에 따른) 노래와 그림. break 없으면 충돌사고 납니다.
			{
			case 1:
				
				music->release();

				Fmod->createStream("stage1.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1); //음악 관련 stage 1.

				strcpy_s(Maze[0], 31, "000000000000000000000000000000");
				strcpy_s(Maze[1], 31, "02116111170        3        00");
				strcpy_s(Maze[2], 31, "00000000100   3   3 3   3   00");
				strcpy_s(Maze[3], 31, "00000000100  3 3   3   3 3  00");
				strcpy_s(Maze[4], 31, "00000000100   3    7    3   00");
				strcpy_s(Maze[5], 31, "00000000500   7    7    7   00");
				strcpy_s(Maze[6], 31, "00000000100   7    7    7   00");
				strcpy_s(Maze[7], 31, "000000001000000000000000000000");
				strcpy_s(Maze[8], 31, "000000001511111111611700000000");
				strcpy_s(Maze[9], 31, "000000000000000000100000000000");
				strcpy_s(Maze[10], 31, "000             00100000000000");
				strcpy_s(Maze[11], 31, "000   9     9   00500000000000");
				strcpy_s(Maze[12], 31, "000  999   999  00100000000000");
				strcpy_s(Maze[13], 31, "000   9     9   00100000000000");
				strcpy_s(Maze[14], 31, "000   7     7   00100000000000");
				strcpy_s(Maze[15], 31, "000   7     7   00500000000000");
				strcpy_s(Maze[16], 31, "000   7     7   00100000000000");
				strcpy_s(Maze[17], 31, "000000000000000000100000000000");
				strcpy_s(Maze[18], 31, "000-0-0-0-00000000100000000000");
				strcpy_s(Maze[19], 31, "000101010107611111100000000000");
				strcpy_s(Maze[20], 31, "000101010100010000000000000000");
				strcpy_s(Maze[21], 31, "000101010100010000000000000000");
				strcpy_s(Maze[22], 31, "000101010100010000000000000000");
				strcpy_s(Maze[23], 31, "000101010100011111116111117000");
				strcpy_s(Maze[24], 31, "000101010100000000000000010000");
				strcpy_s(Maze[25], 31, "000101010100000000000000050000");
				strcpy_s(Maze[26], 31, "000101010100000000000000010000");
				strcpy_s(Maze[27], 31, "000101010100000000000000010000");
				strcpy_s(Maze[28], 31, "031151515111111161111111110000");
				strcpy_s(Maze[29], 31, "000e0e0e0e0e0e0e00000000000000");

				break;
			case 2:

				music->release();

				Fmod->createStream("stage2.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1);

				strcpy_s(Maze[0], 31, "000000000000000000000000000000");
				strcpy_s(Maze[1], 31, "021111110000000000000000000000");
				strcpy_s(Maze[2], 31, "000000110000070000000000000000");
				strcpy_s(Maze[3], 31, "000000110000010000000000000000");
				strcpy_s(Maze[4], 31, "000001111000060000000000000000");
				strcpy_s(Maze[5], 31, "751111161111611111111111111170");
				strcpy_s(Maze[6], 31, "000001111000060000000000000100");
				strcpy_s(Maze[7], 31, "000000000000010000000000000100");
				strcpy_s(Maze[8], 31, "000000000000070000000000000500");
				strcpy_s(Maze[9], 31, "000000000000000000000000000100");
				strcpy_s(Maze[10], 31, "000011111115111151111111111100");
				strcpy_s(Maze[11], 31, "000010000000000000000000000000");
				strcpy_s(Maze[12], 31, "000010000000000000000000000000");
				strcpy_s(Maze[13], 31, "000010000000000000000000000000");
				strcpy_s(Maze[14], 31, "000050000000000000000000000000");
				strcpy_s(Maze[15], 31, "000010000000000000000000000000");
				strcpy_s(Maze[16], 31, "000011111111111111111000000000");
				strcpy_s(Maze[17], 31, "000000000006666666666000000000");
				strcpy_s(Maze[18], 31, "0000>11111111111111111111e0000");
				strcpy_s(Maze[19], 31, "0000>11111111111111111111e0000");
				strcpy_s(Maze[20], 31, "0000e11111111111111111111<0000");
				strcpy_s(Maze[21], 31, "0000e11111111111111111111<0000");
				strcpy_s(Maze[22], 31, "0000>11111111111111111111e0000");
				strcpy_s(Maze[23], 31, "0000>11111111111111111111e0000");
				strcpy_s(Maze[24], 31, "0000e11111111111111111111<0000");
				strcpy_s(Maze[25], 31, "0000e11111111111111111111<0000");
				strcpy_s(Maze[26], 31, "0000>11111111111111111111e0000");
				strcpy_s(Maze[27], 31, "0000>11111111111111111111e0000");
				strcpy_s(Maze[28], 31, "000011111111111111111111115930");
				strcpy_s(Maze[29], 31, "000000000000000000000000000000");

				break;
			case 3:
				music->release();


				Fmod->createStream("stage3.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1);

				strcpy_s(Maze[0], 31, "000000000000000000000000000000");
				strcpy_s(Maze[1], 31, "0000 0000000000000000000 00000");
				strcpy_s(Maze[2], 31, "000 9 00000000000000000 9 0000");
				strcpy_s(Maze[3], 31, "0000 9 000000000000000 9 00000");
				strcpy_s(Maze[4], 31, "000 999 0000000000000 999 0000");
				strcpy_s(Maze[5], 31, "000 999 0000000000000 999 0000");
				strcpy_s(Maze[6], 31, "0000   000000000000000   00000");
				strcpy_s(Maze[7], 31, "000000000000000000000000000000");
				strcpy_s(Maze[8], 31, "00000000e>1111111111e000000000");
				strcpy_s(Maze[9], 31, "00000000111111111111-000000000");
				strcpy_s(Maze[10], 31, "000000001161111111611000000000");
				strcpy_s(Maze[11], 31, "000000001111111111111000000000");
				strcpy_s(Maze[12], 31, "000000001111111111111000000000");
				strcpy_s(Maze[13], 31, "000000001111171711111000000000");
				strcpy_s(Maze[14], 31, "000000001111112111111000000000");
				strcpy_s(Maze[15], 31, "000000001111171711111000000000");
				strcpy_s(Maze[16], 31, "000000001111111111111000000000");
				strcpy_s(Maze[17], 31, "000000001111111111111000000000");
				strcpy_s(Maze[18], 31, "000000001161111111611000000000");
				strcpy_s(Maze[19], 31, "00000000+111111111111000000000");
				strcpy_s(Maze[20], 31, "00000000e1111111111<e000000000");
				strcpy_s(Maze[21], 31, "000000000000001000000000000000");
				strcpy_s(Maze[22], 31, "00000000e11111111111<000000000");
				strcpy_s(Maze[23], 31, "000000000000001000000000000000");
				strcpy_s(Maze[24], 31, "00000000>11111111111e000000000");
				strcpy_s(Maze[25], 31, "000000000000001000000000000000");
				strcpy_s(Maze[26], 31, "000000000000001111115111500000");
				strcpy_s(Maze[27], 31, "000000000000000000000000100000");
				strcpy_s(Maze[28], 31, "000000000000000000000000930000");
				strcpy_s(Maze[29], 31, "000000000000000000000000000000");

				break;


			case 4:

				music->release();

				Fmod->createStream("stage4.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1);


				strcpy_s(Maze[0], 31, "0000000000000000000e000e000000");
				strcpy_s(Maze[1], 31, "000011115111161111111111111100");
				strcpy_s(Maze[2], 31, "000011115111161111111111111100");
				strcpy_s(Maze[3], 31, "000050000111111111111111111100");
				strcpy_s(Maze[4], 31, "e1111111<0000000000+000+001100");
				strcpy_s(Maze[5], 31, "0515115150000000000e0000001100");
				strcpy_s(Maze[6], 31, "0>1111111e00000050001515111100");
				strcpy_s(Maze[7], 31, "011555511000000015000151511100");
				strcpy_s(Maze[8], 31, "e1111111<000000011500015151100");
				strcpy_s(Maze[9], 31, "055111155000000011150001515100");
				strcpy_s(Maze[10], 31, "0>1111111e00000011115111151500");
				strcpy_s(Maze[11], 31, "055111155000000019111511115100");
				strcpy_s(Maze[12], 31, "e1111111<000000011911150001500");
				strcpy_s(Maze[13], 31, "011555511000000011191115000100");
				strcpy_s(Maze[14], 31, "0>1111111e00000011119111500000");
				strcpy_s(Maze[15], 31, "011555511000000091111911150000");
				strcpy_s(Maze[16], 31, "e1111111<000000059111191115000");
				strcpy_s(Maze[17], 31, "000055000000000055911119111500");

				strcpy_s(Maze[18], 31, "0>1111111e00000000000161100000");
				strcpy_s(Maze[19], 31, "055555555500000000000111100000");
				strcpy_s(Maze[20], 31, "011111111500000000000e55100000");
				strcpy_s(Maze[21], 31, "01111111150000   000011100   0");
				strcpy_s(Maze[22], 31, "01111111150000 9 000111000 9 0");
				strcpy_s(Maze[23], 31, "055555511500003930001110003930");
				strcpy_s(Maze[24], 31, "011111511500003330001100003330");
				strcpy_s(Maze[25], 31, "01111151150000 7 000110000 7 0");
				strcpy_s(Maze[26], 31, "05551151150000 7 000110000 7 0");
				strcpy_s(Maze[27], 31, "01151151150000 7 000110000 7 0");
				strcpy_s(Maze[28], 31, "02151151150000 7 0003+0000 7 0");
				strcpy_s(Maze[29], 31, "000000000000000000000000000000");

				break;
			case 5:

				music->release();


				Fmod->createStream("stage5.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1);


				strcpy_s(Maze[0], 31, "000000000000000000000000000000");
				strcpy_s(Maze[1], 31, "021111111111100000000000000000");
				strcpy_s(Maze[2], 31, "000155555555100000000000000000");
				strcpy_s(Maze[3], 31, "000155555555100000000000000000");
				strcpy_s(Maze[4], 31, "000155666655100000000000000000");
				strcpy_s(Maze[5], 31, "000155111155100000000000000000");
				strcpy_s(Maze[6], 31, "000155171755100000000000000000");
				strcpy_s(Maze[7], 31, "000155717155100000000000000000");
				strcpy_s(Maze[8], 31, "000155555555100000000000000000");
				strcpy_s(Maze[9], 31, "000155555555100000000000000000");
				strcpy_s(Maze[10], 31, "000111111111111111100000000000");
				strcpy_s(Maze[11], 31, "000000000100000000100000000000");
				strcpy_s(Maze[12], 31, "00e1111111111<>1111111111e0000");
				strcpy_s(Maze[13], 31, "000000000100000000100000000000");
				strcpy_s(Maze[14], 31, "00e1111111111<>1111111111e0000");
				strcpy_s(Maze[15], 31, "000000000100000000100000000000");
				strcpy_s(Maze[16], 31, "00e1111111111<>1111111111e0000");
				strcpy_s(Maze[17], 31, "000000000100000000100000000000");
				strcpy_s(Maze[18], 31, "000000000500000000500000000000");
				strcpy_s(Maze[19], 31, "000005111111111511111111100000");
				strcpy_s(Maze[20], 31, "000001000100000155555555100000");
				strcpy_s(Maze[21], 31, "000001000100000155555555100000");
				strcpy_s(Maze[22], 31, "000001000100000155666655100000");
				strcpy_s(Maze[23], 31, "005111111500000155111155100000");
				strcpy_s(Maze[24], 31, "001001000000000155111155100000");
				strcpy_s(Maze[25], 31, "001001000000000155777755100000");
				strcpy_s(Maze[26], 31, "006115000000000155555555100000");
				strcpy_s(Maze[27], 31, "001000000000000155555555900000");
				strcpy_s(Maze[28], 31, "007000000000000111111119300000");
				strcpy_s(Maze[29], 31, "000000000000000000000000000000");

				break;
			case 6:

				music->release();

				Fmod->createStream("stage6.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1);


				strcpy_s(Maze[0], 31, "000000000000000000000000000000");
				strcpy_s(Maze[1], 31, "000000000076555555567000000000");
				strcpy_s(Maze[2], 31, "000000000000000500000000000000");
				strcpy_s(Maze[3], 31, "000000000000000500000000000000");
				strcpy_s(Maze[4], 31, "000000011111111111111111000000");
				strcpy_s(Maze[5], 31, "000000119999999999999991100000");
				strcpy_s(Maze[6], 31, "000001199999999399999999110000");
				strcpy_s(Maze[7], 31, "000019199999999999999999191000");
				strcpy_s(Maze[8], 31, "000111111111111111111111111100");
				strcpy_s(Maze[9], 31, "000100000000000000000000000100");
				strcpy_s(Maze[10], 31, "000100111100000000000111100100");
				strcpy_s(Maze[11], 31, "000100111100000000000111100100");
				strcpy_s(Maze[12], 31, "000100111100000000000111100100");
				strcpy_s(Maze[13], 31, "000100111100000000000111100100");
				strcpy_s(Maze[14], 31, "000100000000100000100000000100");
				strcpy_s(Maze[15], 31, "000100000000000000000000000100");
				strcpy_s(Maze[16], 31, "000110011110011111001111001100");
				strcpy_s(Maze[17], 31, "000010010010010001001001001000");
				strcpy_s(Maze[18], 31, "000011110011110001111001111000");
				strcpy_s(Maze[19], 31, "000000100000000000000000100000");
				strcpy_s(Maze[20], 31, "000000100000000000000000100000");
				strcpy_s(Maze[21], 31, "000000111111111211111111100000");
				strcpy_s(Maze[22], 31, "000000000006000000060000000000");
				strcpy_s(Maze[23], 31, "00000000e111111111111<00000000");
				strcpy_s(Maze[24], 31, "000000000001000000010000000000");
				strcpy_s(Maze[25], 31, "000000000>111111111111e0000000");
				strcpy_s(Maze[26], 31, "000000000001000000010000000000");
				strcpy_s(Maze[27], 31, "00000000e111111111111<00000000");
				strcpy_s(Maze[28], 31, "000000000007000000070000000000");
				strcpy_s(Maze[29], 31, "000000000000000000000000000000");
				break;
			}
			OutPut(); // 맵 보여주기. 



		}//while
	}
}
void selectPrint(char input, int& locate)
{
	switch (input)  // 입력에 대한 처리 ▶ 이동
	{


	case UP:
	case 'w':
	case 'W':
		
		switch (locate) // 현재의 상태 4에서 up -> 3으로 이동할 예정임으로 3번위치에 찍어야 함. (이후에 --처리)
		{
		case 1:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 14, "  ▶"); //더 이상 올라갈 곳이 없으므로 자신의 자리.
			break;
		case 2:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 16, "  ▶");
			gotoxy(MAP + 5, 14, "  ▶");
			break;
		case 3:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 18, "  ▶");
			gotoxy(MAP + 5, 16, "  ▶");
			break;
		case 4:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 20, "  ▶");
			gotoxy(MAP + 5, 18, "  ▶");

			break;
		case 5:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 22, "  ▶");
			gotoxy(MAP + 5, 20, "  ▶");
			break;
		case 6:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 22, "  ▶");
			break;
		}
		locate--;
		if (locate == 0)
			locate = 1; // 맨 위 1에서 위를 누른 경우. 

						//위로 이동
		break;
	case DOWN:
	case 's':
	case 'S':
		
		switch (locate)
		{
		case 1:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 16, "  ▶");
			break;
		case 2:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 18, "  ▶");
			break;
		case 3:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 20, "  ▶");
			break;
		case 4:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 22, "  ▶");

			break;
		case 5:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 24, "  ▶");
			break;
		case 6:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 24, "  ▶"); // 내려갈 곳이 없다.
			break;
		}
		locate++;
		if (locate == 7)
			locate = 6; // 맨 아래에서 아래를 누름. 

						//아래로 이동// 맨 아래에서 아래를 누른 경우 자리 유지. 처리 OK 근데 너무 빠름.
		break;

	case SPACEBAR:
	case ENTER:
		stage = locate;
		hansei = 2; // 어짜피 0은 아니니까 else 로 처리 되어 패스 되니 상관 없이 이용해도 된다.
		
		// 자세한 것은 set Maze에서 처리해 주리라.

		break;
	}

// cls 이후에 다시 찍고 셋팅. 그림 셋. 와리가리 하고 다시 지우고 셋. 맵셋. ㅇㅇ cls 과정에서 화살표 안 나옴..; 지금 포인터형식에서 *p++; 이 안 먹힌다. 레빠란스 ? 

}
void selectOpption()
{
	TextColor(DEEP_WHITE);
	
	gotoxy(MAP + 5, 9, "＃─────────────────────────＃");


	gotoxy(MAP + 5, 11, "＃─────────────────────────＃");
	gotoxy(MAP + 5, 14, "           Sewer a       ");
	gotoxy(MAP + 5, 16, "           Sewer b       ");
	gotoxy(MAP + 5, 18, "            Fury         ");
	gotoxy(MAP + 5, 20, "           Crispy        ");
	gotoxy(MAP + 5, 22, "            Gray         ");
	gotoxy(MAP + 5, 24, "            Skull        ");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 5, 10, "         Stage Choose");
	selectIcon();
}

void titleIcon()
{
	TextColor(DEEP_OC);
	gotoxy(MAP + 5, 14, "  ※");

	TextColor(DEEP_GREEN);
	gotoxy(MAP + 5, 16, "  Ω");

	TextColor(GRAY);
	gotoxy(MAP + 5, 18, "  Ο");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 5, 20, "  ★");

	TextColor(DEEP_RED);
	gotoxy(MAP + 5, 22, "  Φ");
}

void selectIcon()
{
	TextColor(DEEP_OC);
	gotoxy(MAP + 5, 14, "  ▦"); 

	TextColor(DEEP_RED);
	gotoxy(MAP + 5, 16, "  ▦");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 5, 18, "  ▦");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 5, 20, "  ▦");

	TextColor(GRAY);
	gotoxy(MAP + 5, 22, "  ▦");


	TextColor(DEEP_YELLOW);

	gotoxy(MAP + 5, 24, "  ★");
}

						
void titlePicture()
{
	strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
	strcpy_s(Maze[1 ], 31, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
	strcpy_s(Maze[2 ], 31, "000000000000000000000000000000");
	strcpy_s(Maze[3 ], 31, "0 0 0 0 0 0 0 0 0 0 0 0 0 0000");
	strcpy_s(Maze[4 ], 31, " 9                         000");
	strcpy_s(Maze[5 ], 31, "9      9                    00");
	strcpy_s(Maze[6 ], 31, "99999999      9999     99    0");
	strcpy_s(Maze[7 ], 31, "9   9  9       9  9    9   9  ");
	strcpy_s(Maze[8 ], 31, " 9  9 9  9999   9  9  9    9  ");
	strcpy_s(Maze[9 ], 31, "    9   9    9   9   9    999 ");
	strcpy_s(Maze[10], 31, "    9   9         9  9     9  ");
	strcpy_s(Maze[11], 31, "    9   9999      9   9    9  ");
	strcpy_s(Maze[12], 31, "    9   9        9  9  9   9  ");
	strcpy_s(Maze[13], 31, "    9   9    9  9    9  9  9  ");
	strcpy_s(Maze[14], 31, "  9 9    9999  99     9999 9 9");
	strcpy_s(Maze[15], 31, "   999                     99 ");
	strcpy_s(Maze[16], 31, "                              ");
	strcpy_s(Maze[17], 31, "                              ");
	strcpy_s(Maze[18], 31, " 9      9  9999  999999 999999");
	strcpy_s(Maze[19], 31, " 99    99 9    9      9 9     ");
	strcpy_s(Maze[20], 31, " 9 9  9 9 9    9   9 9  9     ");
	strcpy_s(Maze[21], 31, " 9  99  9 999999    9   999999");
	strcpy_s(Maze[22], 31, " 9      9 9    9   9 9  9     ");
	strcpy_s(Maze[23], 31, "09      9 9    9  9     9     ");
	strcpy_s(Maze[24], 31, "00      9 9    9 999999 999999");
	strcpy_s(Maze[25], 31, "000                           ");
	strcpy_s(Maze[26], 31, "00000 0 0 0 0 0 0 0 0 0 0 0 0 ");
	strcpy_s(Maze[27], 31, "000000000000000000000000000000");
	strcpy_s(Maze[28], 31, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
	strcpy_s(Maze[29], 31, "000000000000000000000000000000");
}

void cheat(int* block9)
{
	Fmod->update();
	stage++;
	SetMaze(block9);
	OutPut();
}
void speakerPlus()
{
	speaker += 0.1;
	if (speaker > 1)
		speaker = 1;
	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, "            "); // 해당 자리 청소.
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
	//우리는 speaker값에 맞게 찍어주기만 하면 된다. (이미 함수내에서 speaker 값으로 처리해서 소리 조절함)
}
void speakerMinus()
{
	speaker -= 0.1;
	if (speaker < 0) // 0까지는 가능.
		speaker = 0;
	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, "            ");
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);

}
void mute()
{
	switch (soundEffect) // 0이면 키고 1이면 닫고    켜지면 소리출력 // 꺼지면 소리 출력 X
	{
	case 1:
		soundEffect = 0;
		break;
	case 0:
		soundEffect = 1;
		break;
	}
}
void status()
{
	switch (create)
	{
	case 0:
		create = 1;
		break;
	case 1:
		statusOff();
		create = 0;
		break;
	}
}
void playerPos() //개발 전용 !
{
	TextColor(GRAY);
	gotoxy(MAP + 5, 1, ""); cout << "x : " << tPlayerPos.x << " y : " << tPlayerPos.y;
	gotoxy(MAP + 5, 2, ""); cout << "Now Stage : " << stage;
	gotoxy(MAP + 5, 3, "switch Count : "); cout << switchCount;
	gotoxy(MAP + 5, 4, ""); cout << " switch On Count : " << switchOnCount;
	gotoxy(MAP + 5, 5, ""); cout << "EndPos : ( " << tEndPos.x << ", " << tEndPos.y << ")";
}
void statusOff()
{
	gotoxy(MAP + 5, 1, "                           ");
	gotoxy(MAP + 5, 2, "                           ");
	gotoxy(MAP + 5, 3, "                           ");
	gotoxy(MAP + 5, 4, "                           ");
	gotoxy(MAP + 5, 5, "                           ");
	gotoxy(MAP + 5, 6, "                           ");
}
void playerInform()
{
	TextColor(DEEP_JAJU);
	gotoxy(MAP + 7, 22, "ω Death  : "); printf("%d", death);

	TextColor(DEEP_OC);

	if (switchOnCount < switchCount) // 아직 스위치가 남았으면,
	{
		gotoxy(MAP + 7, 26, "Φ Switch : "); 
		printf("%d", switchOnCount);
	}
	else // door 오픈 문구를 여기서 처리한다.
	{
		gotoxy(MAP + 7, 26, "                  "); // clean
		gotoxy(MAP + 7, 26, "§ Door Open");
	}

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 7, 24, "★ STAGE  : "); printf("%d", stage);
}
void tutorial(char input,int* block9) // 어 맞아. 퀘스트 부분도 무시좀 해야지.
{

	switch (quest)
	{

	case 0: //0. Backspace를 눌러보아요 0일때 한번 출력하게 하구. 이후에 미션 성공시에 넘어가주는 식.
		if (number == 0)
		{
			
			gotoxy_C(5, 3, "BackSpace를 눌러보아요.", DEEP_WHITE); number = 1; //number =1 number를 통해서 한번만 출력해주도록 설정.
			Sleep(600);
			gotoxy(5, 3, "                                                ");
			while (_kbhit()) _getch(); // 버퍼. 
			number = 1;
		}
		if (input == BACKSPACE)
		{
			quest++;
			Maze[4][14] = '1';
			number = 0;
		}
	break;
	case 1://2. 화살에 맞으면 죽어요.
		if (number == 0)
		{
			
			gotoxy_C(5, 3, "화살에 맞으면 죽어요. 화살로 다가가봅시다. ", DEEP_WHITE);
			number = 1;
			Sleep(600);
			gotoxy(5, 3, "                                             ");
			while (_kbhit()) _getch(); // 버퍼. 

		}
		// quest변수를 통해 q를 못누르도록 설정.  죽는 경우가 2가지가 있는데, 하나는 backspace 하나는 arrow 하나는 폭탄(X) 백스페이스 or 화살로 죽는다.
		// 아마 ? ... 착한 어린이면 잘 해줄꺼야.. 아니면... 그냥 넘어가는 거지.. (사실 큰 문제는 없다. ) 그럼 넘어가자.
		if (death >= 1) {  // 제대로 죽었다면 startPos 라서 푸시블럭을 이용할 수 있다.
			quest++;
			Maze[7][14] = '1'; number = 0;
			Maze[4][14] = '6';
		}
		break;
	case 2:

		//3. 스페이스바를 통해 폭탄을 설치
		if (number == 0)
		{
		
			number = 1;
			gotoxy_C(5, 3, "▦ 을 터트려 봅시다. 폭탄설치 : space 터트리기 : q", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                                   ");
			while (_kbhit()) _getch(); // 버퍼. 

		}
		if (Maze[9][14] == '1') {
			quest++; number = 0;
			}
		break;
	case 3:
		if (number == 0)
		{
			// 사실상 화살 없애기 + 공 밀기.
			number = 1;
			gotoxy_C(5, 3, "화살을 공으로 없애요. ", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                           ");
			while (_kbhit()) _getch(); // 버퍼. 

		}
		if (tPlayerPos.y == 12 && tPlayerPos.x == 14) { quest++; 
		Maze[13][14] = '1'; number = 0;
		}
		break;
	case 4:
		if (number == 0)
		{
			 //공밀기 + 스위치 on
			number = 1;
			gotoxy_C(5, 3, "공을 스위치로 가져가요. ", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                         ");
			while (_kbhit()) _getch(); // 버퍼. 

		}
		if (switchCount == switchOnCount) {
			quest++; Maze[16][14] = '1'; number = 0;
		}
		break;
	case 5:
		if (number == 0)
		{
			
			number = 1;
			gotoxy_C(5, 3, "※ 블럭을 지나가요.", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                           ");
			while (_kbhit()) _getch(); // 버퍼. 

		}
		if (tPlayerPos.x == 14 && tPlayerPos.y == 21) {
			Maze[22][14] = '1'; quest++; number = 0;
		}
		break;
	case 6:
		if (number == 0)
		{
			
			number = 1;
			gotoxy_C(5, 3, "★ 을 만나요.", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                     ");
			while (_kbhit()) _getch(); // 버퍼. 

		}
		if (tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y) // 지금 이 부분이, move에서 먼저 처리를 해주고 있다. 따라서 move쪽에서 quest = 6인 경우 처리 X
		{ 
			number = 0;
			gotoxy_C(5, 3, "Tutorial이 끝났습니다. 1 Stage로 이동합니다.", DEEP_WHITE);
			_getch();
			quest = 0; // 혹시나 다시 할 수도 있다. 
			death = 0;
			stage++;
			SetMaze(block9);
			OutPut();
			//이후에 stage = 1이므로 tutorial을 실행안 함.
		}
	
		break;


	}

}
void titleSpace()
{
	strcpy_s(Maze[0], 31, "000000000000000000000000000000");
	strcpy_s(Maze[1], 31, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
	strcpy_s(Maze[2], 31, "000000000000000000000000000000");
	strcpy_s(Maze[3], 31, "0 0 0 0 0 0 0 0 0 0 0 0 0 0000");
	strcpy_s(Maze[4], 31, "                           000");
	strcpy_s(Maze[5], 31, "                            00");
	strcpy_s(Maze[6], 31, "                             0");
	strcpy_s(Maze[7], 31, "                              ");
	strcpy_s(Maze[8], 31, "                              ");
	strcpy_s(Maze[9], 31, "                              ");
	strcpy_s(Maze[10], 31, "                              ");
	strcpy_s(Maze[11], 31, "                              ");
	strcpy_s(Maze[12], 31, "                              ");
	strcpy_s(Maze[13], 31, "                              ");
	strcpy_s(Maze[14], 31, "                              ");
	strcpy_s(Maze[15], 31, "                              ");
	strcpy_s(Maze[16], 31, "                              ");
	strcpy_s(Maze[17], 31, "                              ");
	strcpy_s(Maze[18], 31, "                              ");
	strcpy_s(Maze[19], 31, "                              ");
	strcpy_s(Maze[20], 31, "                              ");
	strcpy_s(Maze[21], 31, "                              ");
	strcpy_s(Maze[22], 31, "                              ");
	strcpy_s(Maze[23], 31, "0                             ");
	strcpy_s(Maze[24], 31, "00                            ");
	strcpy_s(Maze[25], 31, "000                           ");
	strcpy_s(Maze[26], 31, "00000 0 0 0 0 0 0 0 0 0 0 0 0 ");
	strcpy_s(Maze[27], 31, "000000000000000000000000000000");
	strcpy_s(Maze[28], 31, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
	strcpy_s(Maze[29], 31, "000000000000000000000000000000");

	OutPut();

}
void ranking()
{

	
	while (true)
	{

		TextColor(DEEP_YELLOW);
		gotoxy(14, 7, "《 명예의 전당 》"); //스테이지는 -1이라 동일하게 처리됨 다이아.

		int j = 9;
		for (int i = 0; i < 10; i++) // TOP 10
		{
			if (i < 3) TextColor(DEEP_JAJU);
			else if (i < 6) TextColor(DEEP_OC);
			else TextColor(DEEP_YELLOW);
			gotoxy(14, j, "");
			//string name = arr[i].name; // 왜 이 병신 샛기가 안 나오는지는 모르겠다.  c_str로 해보자.

			if (arr[i].death != 0 || arr[i].name[0]) // 데스가 0이 아니면 제대로된 접근.  OR 0이지만, 이름이 있는 경우.
				// 데스가 1 정상.   데스가 0 "ㅇㅁㄹㅇㄹ" 정상. 
				printf("%2d . %s Death : %d", i + 1, arr[i].name.c_str(), arr[i].death);
			else if (arr[i].death == 0) //디폴트     -1 ( 기본배열값 ) or name 이null 이면 --- 그 외에는 제대로 . 입력 X 들.
				// 그 이외 데스 = 0 out 
				printf("%2d . ───", i + 1);
			
			
			//cout << name << " Death : " << arr[i].death;
			
			// cout를 사용하지 않고 printf를 이용하면 string 문법을 받아내지 못한다.
				// 내 생각에는 없는 문자를 뱉으려고 해서 문제가 생기는 것 같다. 생성자를 수정해주자. (아니, 이게 문제가 아니네 ? 그냥 gotoxy...)
			//ㅋㅋㅋㅋㅋ
			j += 2;
		}

		

		if (_kbhit())
		{ 
			music->release();
			break;
		} //입력이 있으면 out
	}


	while (_kbhit()) _getch(); // 버퍼. 
}
