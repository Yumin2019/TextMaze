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


class Rank  // Rank class
{
	
	

public:
	string name;
	int death; // 플레이랑은 사실 상관없고 불러올 때 버릴 값 / 제어

	Rank(const string& name, int death) : name(name), death(death) {}



	friend ofstream& operator<<(ofstream& a, Rank& b);

	friend bool cmp(const Rank& a, const Rank& b);
	
	Rank() { } // 기본 생성자 100 그룹
};

ofstream& operator<<(ofstream& a, Rank& b) // 파일에 입력
{
	a << b.name.c_str() << "." << b.death << "\n"; // 이름을 파일에 출력  이름.death       
	return a;
}
bool cmp(const Rank& a, const Rank& b)
{
	if (a.death != b.death) return a.death < b.death;

	//데스가 같다 사전순.
	return a.name < b.name;
}

Rank arr[100];
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
#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define SPACEBAR 32

FMOD::System	*Fmod(nullptr);
// FMOD
FMOD::Sound		*music(nullptr); // 스테이지마다 사용할 포인터  createSound는 여러개 동시 재생 가능(효과음 처리) createStream은 하나만 재생 가능 ( 배경음악 )
FMOD::Sound		*jump(nullptr);	// 효과음이라고 하여도 지역변수로 선언하면 소리가 묻힘.
FMOD::Sound		*button(nullptr);



FMOD::Channel	*channel1(nullptr); // 채널 1은 노래를 틀어서 소리를 조정해준다.
FMOD::Channel	*channel2(nullptr);// 채널 2는 효과음으로 소리를 조정하지 않는다.
FMOD::Channel	*channel3(nullptr);
FMOD::Channel	*channel4(nullptr); // 효과음 채널2 분산.

FMOD_RESULT		result;


void			*extradriverdata(nullptr);

int arrowSpeedSet = 2; // 컴퓨터마다 속도가 차이가 나는 경우가 있다. 이 경우에 격차를 해소하기 위해 arrowSpeed을 조정한다. 2 -> 기본 1느리게 3 빠르게 

float speaker = 0.5f;
int soundEffect = 1;// 효과음 on off
int dataCheck; // data를 받았는가 받았으면 1
int create; // 개발자 변수.
int hansei; // hansei를 말하면 1 ( 게임 내에서 다시 묻지 않음 )

int locate; // select에서 사용.
int quest; // tutorial 변수
int death; // death count
int num;
int stage;
int number; // 횟수. 튜토리얼에서 사용.
long arrowSpeed; // arrow speed는 stage별 구상하도록 한다. 참고로 sleep의 인수기 때문에 크기가 크면 느려짐
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
POINT tPlayerPos;


char	Maze[30][31];

POINT tBombPos[5];

int iBombCount;

// Maze , boom 구조체 , count;


enum COLOR {
	BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY
	, DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW,
	DEEP_WHITE
};


void SetMaze()
{
	if (quest >= 1) return; // quest 진행중일 때 맵을 다시 안 찍음.

	Fmod->update();


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


	*/

	
	
	
	num = 0;
	arrowCount = 0;
	switchCount = 0;
	switchOnCount = 0;
	block9 = 0; // 깜빡임 처리 셋팅 열리지 않았다는 표시.

	// death는 건드리지 않는다. (1~6 stage에서 세고서 랭킹에 써야 함) death 0은 튜토리얼 or title에서 잘 처리함.
	
	if(locate == 0) // 선택 중이 아닐 때
	system("cls");


	while (_kbhit()) _getch(); 

	
	 // 스테이지 셋팅마다 num = 0을 통해서 스위치, arrow 등의 개수를 다시 세어준다.  endPos 와 startPos 는 stage 마다 구상.
	switch(stage)
	{
	case -1:
		title(); // 타이틀 음악은 타이틀에서 처리하고 있다. title() SetMaze() ≒ stage = -1; SetMaze() SetMaze()
		break;
	case 0: // tutorial 맵은 찍어두고 tutorial 조건에 맞게 open 등 처리.
		

		

		music->release();

		Fmod->createStream(".\\sounds\\tutorial.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 14;
		tPlayerPos.y = 2;
		
		arrowSpeed = 5; // 튜토리얼은 그런 거 없음. 그냥 속도대로

		tEndPos.x = 14;
		tEndPos.y = 27;

		strcpy_s(Maze[0 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[1 ], 31, "011111111111111111111111111110");
		strcpy_s(Maze[2 ], 31, "011111111111112111111111111110");
		strcpy_s(Maze[3 ], 31, "011111111111111111111111111110");
		strcpy_s(Maze[4 ], 31, "000000000000000000000000000000");
		strcpy_s(Maze[5 ], 31, "000000000000>1111e000000000000"); 
		strcpy_s(Maze[6 ], 31, "000000000000001000000000000000");
		strcpy_s(Maze[7 ], 31, "000000000000000000000000000000"); 
		strcpy_s(Maze[8 ], 31, "000000000000001000000000000000"); 
		strcpy_s(Maze[9 ], 31, "000000000000005000000000000000");
		strcpy_s(Maze[10], 31, "000000000000001000000000000000"); 
		strcpy_s(Maze[11], 31, "000000000000006000000000000000");
		strcpy_s(Maze[12], 31, "000000000000>1111e000000000000");
		strcpy_s(Maze[13], 31, "000000000000000000000000000000");
		strcpy_s(Maze[14], 31, "000000000000001000000000000000");
		strcpy_s(Maze[15], 31, "000000000076111116700000000000");
		strcpy_s(Maze[16], 31, "000000000000000000000000000000");
		strcpy_s(Maze[17], 31, "000000000000091900000000000000");
		strcpy_s(Maze[18], 31, "000000000000091900000000000000");
		strcpy_s(Maze[19], 31, "000000000000091900000000000000");
		strcpy_s(Maze[20], 31, "000000000000091900000000000000");
		strcpy_s(Maze[21], 31, "000000000000091900000000000000"); 
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
		speedShow();
		
		// 스피드 보여주기

		music->release();

		Fmod->createStream(".\\sounds\\stage1.mp3", FMOD_LOOP_NORMAL, 0, &music);
			Fmod->playSound(music, 0, false, &channel1); //음악 관련 stage 1.

		tPlayerPos.x = 1;
		tPlayerPos.y = 1;
	
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

		
		break;
	case 2:
		music->release();

		Fmod->createStream(".\\sounds\\stage2.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1); 

		speedShow();

		tPlayerPos.x = 1; 
		tPlayerPos.y = 1;
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


	case 3:
		music->release();


		Fmod->createStream(".\\sounds\\stage3.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 14;
		tPlayerPos.y = 14; 
		tEndPos.x = 25;
		tEndPos.y = 28;
		
		speedShow();

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

		Fmod->createStream(".\\sounds\\stage4.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		speedShow();

		tPlayerPos.x =  2;
		tPlayerPos.y = 28;
		tEndPos.x = 20;
		tEndPos.y = 28;
		
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
		speedShow();


		Fmod->createStream(".\\sounds\\stage5.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 1;
		tPlayerPos.y = 1; // 1 1
		tEndPos.x = 24;
		tEndPos.y = 28;
		
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
		speedShow();

		Fmod->createStream(".\\sounds\\stage6.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 15; // 15 21
		tPlayerPos.y =21;
		tEndPos.x = 15;
		tEndPos.y = 6;

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
		
	case 7: //Ending
		

		Sleep(500);

		music->release();

		Fmod->createStream(".\\sounds\\ending.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		titleSpace(); // 타이틀 그림에서 글자 뺀 것 출력.

		gotoxy_C(MAP + 5, 8,  "ω Thank You for Playing ", DEEP_YELLOW);  Sleep(1000);

		gotoxy_C(MAP + 5, 10, "ω Creator : Yumin Kim   ", DEEP_JAJU);  Sleep(1000); 

		gotoxy_C(MAP + 5, 12, "ω 한세사이버보안고등학교", DEEP_OC);  Sleep(1000);
	
		gotoxy_C(MAP + 5, 14, "ω blog.naver.com/lmj3322", DEEP_GREEN);   Sleep(1000);
		
		gotoxy_C(MAP + 5, 16, "ω PassWord : hansei",DEEP_WHITE);  Sleep(1000);


		titlePicture();

		

		OutPut();

		while (_kbhit()) _getch(); // 버퍼. 

		gotoxy_C(14, 18, "What's Your Name ? : ",DEEP_WHITE);  

		string name;
		getline(cin, name); // 공백포함 cin

		arr[read] = { name, death }; // 입력받은 데이터를 가지고 Rank 정렬을 해줌.
		read++; // read는 항상 마지막 인자 +1을 가리키고 있어야 sort할 때 헷갈림이 없다. 
				// 다시 랭킹 받을 때도 중복된 장소에 받으면 안 되니 이게 맞다.
		// 입력 후.
		//이후에 main메뉴로 이동하게 해야 한다.
		
		
		//하위 요소 해제
		music->release();
		// 상위 요소 해제.
		Fmod->release(); 
		stage = -1;
		SetMaze(); // 다 고른 이후에 ouput loop가 돌아감.

		// 입력받은 값으로 setting				 
		SetMaze(); 
		OutPut();
		return;

	}
	if (locate > 0) return; // 먼가를 고른 경우에  더이상 출력 안 함.

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "▣ Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "▣ button :  + - ");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);

	channel1->setVolume(speaker);

	TextColor(DEEP_WHITE);

	if(stage != 0) // main메뉴에서는 'm'을 막아둠.(잠재적 오류 발생) 그래서 보여주지 않도록
	gotoxy(MAP + 7, 6, "M : 메인메뉴");

	gotoxy(MAP + 7, 8, "T : 효과음 OFF");

	gotoxy(MAP + 7, 10, "SPACE : 폭탄 설치");

	gotoxy(MAP + 7, 12, "Q : 폭탄 터트리기");

	gotoxy(MAP + 7, 14, "W : 위   S : 아래");

	gotoxy(MAP + 7, 16, "A : 왼쪽 D : 오른쪽 ");

	gotoxy(MAP + 7, 18, "Backspcae ← : 재시작");

	gotoxy(MAP + 7, 20, "ESC : 종료 ");

	speedSetting(); // 타이틀에서는 변수값만 바꿔두고 이후 Setmaze에서 속도를 지정해준다. ( 타이틀과 튜토리얼에서는 설정하지 않음 ) 7 스테이지 ( end ) 도 해당사항 없다.
					// 이후에 입력이 있을 때만 교체.

		
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

				

			}
			
			Fmod->update();
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

	Fmod->createSound(".\\sounds\\bomb.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel3);
	channel3->setVolume(speaker);
	Sleep(50); 
	// 폭탄위치와 겹치지 않거나 최대수를 넘지 않은 경우에는 현재 count에 맞도록 플레이어 위치 데입 
	tBombPos[iBombCount] = tPlayerPos;
	++iBombCount;
	Maze[tPlayerPos.y][tPlayerPos.x] = '4';
	Fmod->update();
	

}


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
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';
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
		

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel3);

	}
	//// push block 
	


	if (pushBlock == 1)
	{// 푸시블럭이 이동.
		FMOD::Sound		*push(nullptr);

		Fmod->createSound(".\\sounds\\push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel3);

	}

	// 움직이는 놈은 여기까지 도착. jump.mp3

	 Fmod->createSound(".\\sounds\\jump.mp3", FMOD_DEFAULT, 0, &jump);// 1번자리
	 if (soundEffect)
	 Fmod->playSound(jump, 0, false, &channel3);
	--tPlayerPos.y;
	channel3->setVolume(speaker); // Play 이후에 셋 해야 적용됨.

	Fmod->update();
	
}

void MoveDown()
{
	Fmod->update();

	if (tPlayerPos.y == 30)
		return;
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
			Maze[tPlayerPos.y + 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y + 1][tPlayerPos.x] = '1';
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

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 



	if (pushBlock == 1) // 푸시블럭이 이동.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound(".\\sounds\\push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	

	Fmod->createSound(".\\sounds\\jump.mp3", FMOD_DEFAULT, 0, &jump);
	if (soundEffect)
	Fmod->playSound(jump, 0, false, &channel4);
	++tPlayerPos.y;

	channel4->setVolume(speaker);
	Fmod->update();

}


void MoveRight()
{
	Fmod->update();

	int sw = 0;
	int pushBlock = 0;
	
	if (tPlayerPos.x >= 30)
		return;
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
			Maze[tPlayerPos.y][tPlayerPos.x + 2] = '6'; Maze[tPlayerPos.y][tPlayerPos.x + 1] = '1';
			break;
		case '6': 
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
	////스위치 출력 부분 

	
	
	if (sw == 1) // 스위치가 on 이런 경우에는 실행될 때 까지 기다리도록.
	{

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	
	

	if (pushBlock == 1) // 푸시블럭이 이동.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound(".\\sounds\\push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	


	Fmod->createSound(".\\sounds\\jump.mp3", FMOD_DEFAULT, 0, &jump);
	if (soundEffect)
	Fmod->playSound(jump, 0, false, &channel4);
	++tPlayerPos.x;
	channel4->setVolume(speaker); // push, 스위치는 1 삐용은 0.5
	Fmod->update();

}


void MoveLeft()
{
	Fmod->update();

	if (tPlayerPos.x == 0)
		return;
	int sw = 0;
	int pushBlock = 0;
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
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '6'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1';
			break;
		case '6': 
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

	

	if (sw == 1) // 스위치가 on 이런 경우에는 실행될 때 까지 기다리도록.
	{

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	


	if (pushBlock == 1) // 푸시블럭이 이동.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound(".\\sounds\\push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	 // push, 스위치는 1 삐용은 0.5

	Fmod->createSound(".\\sounds\\jump.mp3", FMOD_DEFAULT, 0, &jump);
	if (soundEffect)
	Fmod->playSound(jump, 0, false, &channel4);
	channel4->setVolume(speaker);
	--tPlayerPos.x;
	Fmod->update();
	
}



void Fire()
{
	Fmod->update();

	if (iBombCount == 0) return; // 설치를 안 한 경우 그냥 나옴. (소리출력 X)

	FMOD::Sound		*bomb(nullptr);

	Fmod->createSound(".\\sounds\\explosion.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel4);
	channel4->setVolume(speaker);
	
	int block = 0; // 블럭 터졌는가 ? 
	for (int i = 0; i < iBombCount; i++) 
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
			Fmod->createSound(".\\sounds\\die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);// 소리1 
			
			channel2->setVolume(speaker);

			death++;

			tPlayerPos = tStartPos;  
		}
	
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

		// 플레이어 위치가 화살위치랑 겹칠 때 설치하면 안 된다. @@@@ (화살의 사라짐)

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
		Fmod->createSound(".\\sounds\\cookie.mp3", FMOD_DEFAULT, 0, &cookie);
		if (soundEffect)
		Fmod->playSound(cookie, 0, 0, &channel3); // 바삭바삭 소리 1

		channel3->setVolume(speaker);
	}


	Fmod->update();

	iBombCount = 0; //터트린 후
}


void MovePlayer(char cInput)
{
	Fmod->update();

	switch (cInput)
	{
	case 'w':
	case 'W':
		MoveUp();
		break;
	case 's':
	case 'S':
		MoveDown();
		break;
	case 'a':
	case 'A':
		MoveLeft();
		break;
	case 'd':
	case 'D':
		MoveRight();
		break;
	}

	if (Maze[tPlayerPos.y + 1][tPlayerPos.x] == '9' || // 플레이어 위치의 좌우상하가 까시 블럭이면 sleep
		Maze[tPlayerPos.y - 1][tPlayerPos.x] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x + 1] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x - 1] == '9')
		Sleep(150);


	if (tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y) // 그러니까 우리는 stage clear 시점을 사실상 움직일 때만 인식하면 된다. 
	{
		if (quest == 6) return; // 안내문구 던져야 해서 처리 안 하고 나옴. (이쪽에서 처리)

		// 한세 == 2일 때 우리는 타이틀로 보내야 한다.

		if (hansei == 2) 
		{
			music->release();
			while (_kbhit()) _getch(); 

			system("cls");
			title(); 
			SetMaze(); 
			OutPut(); //arrow move 등을 처리.

			return;
		}
		//선택한 경우가 아님.

		FMOD::Sound *clear(nullptr);
		Fmod->createSound(".\\sounds\\clear.mp3", FMOD_DEFAULT, 0, &clear);
		if (soundEffect)
		Fmod->playSound(clear, 0, 0, &channel2);
		channel2->setVolume(speaker);
		stagePlus(); // stage clear
		SetMaze();
		OutPut(); // 스위치 개수 등 수치를 세주지 않으면 문제 생김.
	}
	Fmod->update();

}

void arrowMove() 
{
	Fmod->update();


	for (int i = 0; i < arrowCount; i++)
	{
		// 일단 실행이 된다는 것은 하나라도 있다. 그 자리를 길로 만들고 옮긴후 자리에 맞게 데입


		if (Maze[arrowPos[i].y][arrowPos[i].x] == '6') // movePlayer에서 일단 플레이어 푸시 화살일 때 화살의 좌표에 6을 데입한다. 따라서 arrowmove 이곳으로 들어와.
		{														// 화살과 공이 겹친 이 자리에 1을 데입하여 길로 만들고 화살은 죽은 좌표로 둔다.	== 우리는 지나갈 수 있다.
			Maze[arrowPos[i].y][arrowPos[i].x] = '1';
			arrowPos[i].x = 0; arrowPos[i].y = 0;
			continue; 
		}

		if (Maze[arrowPos[i].y][arrowPos[i].x] == '0') continue; // 죽은 화살에 대한 처리. 0.0은 벽의 좌표. (접근 X)

		
		// 죽은 화살에 0,0을 데입하도록.

		// 화살이 폭탄과 겹치고 폭탄은 터지고, 1로 만들지 않고 화살이 그 자리를 밟고 간다. ( 어쨋든 터진 것으로 정상적으로 처리된다)

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
		{						

			//죽는 소리
			FMOD::Sound *die(nullptr);
			Fmod->createSound(".\\sounds\\die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);
			channel2->setVolume(speaker);
			Fmod->update();
			death++;

			tPlayerPos = tStartPos;
		}

		Sleep(arrowSpeed); //  stage별 속도.
	}
}
void stagePlus()
{
	stage++;
}

void openDoor() 
{
	Fmod->update();

	if (block9 == 1) return; // 한번만 출력하고 이후에 return; stage가 바뀌면 block9 = 0을 셋팅을 해야한다.

	if (switchOnCount == switchCount) // on개수랑 전체 개수가 같으면 (모든 스위치 on)
	{
		FMOD::Sound *door;
		Fmod->createSound(".\\sounds\\9block.mp3", FMOD_DEFAULT, 0, &door);
		if(soundEffect)
		Fmod->playSound(door, 0, 0, &channel2);

		channel2->setVolume(speaker);
		Fmod->update();
		block9 = 1;



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


void title() // 타이틀은 stage = -1으로 그림 그리기 위한 설정이다.
{
	for (int i = 0; i < arrowCount; i++)
		arrowPos[i] = { 0,0 };
	// 타이틀을 띄우는 경우 기존에 잡혀있던 arrow의 좌표를 죽은 좌표 처리해야 한다.


	while (_kbhit()) _getch(); 

	
	result = FMOD::System_Create(&Fmod);


	FMOD::Sound		*sing(nullptr);

	FMOD::Sound		*select(nullptr);

	 result = Fmod->init(25, FMOD_INIT_NORMAL, extradriverdata);
	 if (result != FMOD_OK) return;

	// 채널은 25개 설정 / 일반적 / 안 쓰는 포인터

	 result = Fmod->createStream(".\\sounds\\title.mp3", FMOD_LOOP_NORMAL, 0, &sing);
	 if (result != FMOD_OK) return;

	
	//배경화면에서는 계속 재생 sing 
	// Stream or Sound
	 result = Fmod->createSound(".\\sounds\\select.mp3", FMOD_DEFAULT, 0, &select);
	 if (result != FMOD_OK) return;

	//select effect


	 result = Fmod->playSound(sing, 0, false, &channel1);
	 if (result != FMOD_OK) return;

	
	channel1->setVolume(speaker); // 기본 0.5
	
	
	titlePicture();
	block9 = 1;
	num = 0;
	stage = -1;

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 9, "＃─────────────────────────＃");
	

	gotoxy(MAP + 5, 11, "＃─────────────────────────＃");

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "▣ Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "▣ button :  + - ");

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 10, 2, "    button : 1 ~ 3");
	gotoxy(MAP + 10, 1, "    Arrow Speed : "); TextColor(GRAY); printf("%d", arrowSpeedSet);


	TextColor(GRAY);
	gotoxy(MAP + 10, 1, "→");
	gotoxy(MAP + 10, 2, "←");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
	// 초기 0.5

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
	
	TextColor(DEEP_YELLOW);
	gotoxy(1, 32, "★");
	TextColor(DEEP_WHITE);
	gotoxy(3 , 32, "References : http://assortrock.com (Maze Game) ");
	

	int locate = 0; // 맨위 무엇을 눌렀는지, 어디 위치 였는지. // 여기서 사용하는 locate와 select에서 사용하는 locate는 다르다.
	char str[40];// 비밀번호와 같은지에 사용할 배열. 너무 쪼잔하게 주면 문자를 많이 입력할 때 재미없어짐.

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ▶"); 

	// 사실상 처음에는 입력이 없다면 세모가 뜨지 않으므로 미리 잡아둬야 한다.

	//우리는 title에서 while문으로 키보드 입력을 받아 ▶ 처리로 인터페이스를 처리.
	// 이후에 누른 것 따로 처리 ..
	OutPut();


	// read ( 이전 데이터가 있는지 확인합니다. ) 

	ifstream in("TMRank.txt");
	string name;
	int death;
	int con = 0;

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


				if(death == 0 && name[0] || death != 0) 
				read++; //  read는 다음 원소를 (빈 자리를) 가리킨다. 따라서 새로운 값을 넣어줄 때 arr[read]하면 된다.
				// 기록된 데이터가 이름이 없는 것은 말이 안 된다. name[0] 초기값인지 아닌지 검사. 
				

				/*
				death가 0이고 name[0]이 있거나, death != 0 이라면 정상적인 접근. 아닌 경우에 냅두기.
				*/

				 // c++ 오버로딩 string == "" ? 로 물어보면 해결이 안 된다..  // size 0이여도 잘못된 것 => 처리X.  numsize하면 항상 0이 나오는 마법.
				

				// 예외 처리. 메모장이 빈 경우(ios:: end == ios::beg ) read ++ 을 해서는 안 된다. ( read ++ 로 1이 되면 받는 값도 1부터 시작되고 이상한 값이 들어감)
			} 
		
		}
		
		dataCheck = 1;
		in.close();// 이후에 이 받은 값을 가지고 sort를 한다. 다시 파일입출력을 하기 때문에 close
		// 아무것도 없으면 잘못된 접근으로 인식하고 입력받으나 read++을 안 한다. 따라서 그 부분은 정렬에서 제외댐. end : (arr+ read)
	}
	
	
	// 지금 현재 랭킹을 눌렀을 때만 sort후에 저장을 하고 있다.


	sort(arr, arr + read, cmp); // 정렬 기준 1. death  2. 사전순

	ofstream txt("TMRank.txt"); // 기본 형식이라 다 지우고 처리함.  없으면 만들고, 있으면 지우고


	for (int i = 0; i < read; i++) 
	{										
		txt << arr[i]; // 상단 오버로딩 참고.

					   // 정렬된 데이터를 이름.데스\n이름.데스\n ...으로 입력. 
	}
	txt.close(); // 데입 이후에 닫기.

	//이렇게 title마다 sort후 저장. 랭킹을 누른 경우에 출력. 반복하는 처리. 

	while (_kbhit()) _getch(); // 버퍼.

	while (true)
	{
		 Fmod->update();
		if (_kbhit())
		{
			char input = _getch();
										
			while(_kbhit()) _getch();


			// 메인메뉴에서 배경음 소리 지정할 수 있도록 한다.
			// 여기서 설정한 값이 이후 게임 내 모든 soundVolume에 적용. 


			switch (input)  
			{	
				// 속도 저하 OR 너무 빠른 ? 문제 

			case '1':
			case '2':
			case '3':
				arrowSpeedSet = input - '0';
				speedShow(); // 여기서는 타이틀이라서 속도 조절을 직접적으로 해줄 필요가 없다. 그냥 set값만 받아두기.

				break; //  타이틀에서도 설정할 수 있고 게임에서도.

				// locate 변수 이용, locate 변수에 따른 아이콘 찍기. 미리찍고,  해당 부분만 다시찍기.
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
					gotoxy(MAP + 5, 14, "  ▶");
					break;
				case 2:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 16, "  ▶");
					break;
				case 3:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 18, "  ▶");

					break;
				case 4:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 20, "  ▶");
					break;
				}
				locate--;
				if (locate == -1)
					locate = 0; // 맨 위(0)에서 위를 누른 경우. 

				//위로 이동
				break;
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
				
				break;

			case SPACEBAR:
			case ENTER:
				 result = Fmod->playSound(select, 0, false, &channel2);
				 if (result != FMOD_OK) return;

				// select음은 채널2로 1.0  0초미만이라 release할 필요 X
				switch (locate)
				{

					// title에서는 setting만 하고 이후에 Setmaze처리.

				case 0: // 시작 하기 (기본 셋팅 대로)
					locate = 5;
					stage = 1; 
					break;



				case 1: //튜토리얼 
					locate = 5; 
					stage = 0; // 이후에 튜토리얼 스테이지에서 클리어시 1stage로 이동. 타이틀은 case -1:
					break;
					// 맵 만들어야 한다.



				case 2: // 스테이지 선택 (비밀번호)
					
					if (hansei == 0)
					{
					gotoxy_C(MAP + 5, 3, "Password : ",DEEP_WHITE);  


					while (_kbhit()) _getch(); // 버퍼. 

					
						scanf_s("%s", str, sizeof(str));
						if (compare(str, "hansei")) 
						{
							hansei = 1;
							locate = 8; // 맞으면 8 틀리면 데입 없음. 다시 선택(title)하는 부분. 반복.  제대로 입력시에 내려가서 함수처리.
						} 

						gotoxy(MAP + 5, 3, "                               "); // 입력한 경우에만 청소하면 되죠.

					}
					else
						locate = 8; // 입력한 이후에는 프리패스

					num = 0; // num = 0을 해야 벽 ◆ 부분을 처리함. ' '도 마찬가지. ( 너 처음으로 찍는 거니 이런 역할 ) stage select 부분.
					
					break;




				case 3: //랭킹 ( 파일 입출력을 통한 offline 랭킹 )
					system("cls");
					num = 0;

					titleSpace(); 

					locate = 7;

					break;


					
				case 4:// 종료
					exit(0);
					break;
				}

				while (_kbhit()) _getch(); // 버퍼. enter이후에 누른 값을 일단 무시. ( 무시하지 않으면 이상하게 됨)
				break;


			case ESC: 
				exit(0);
				break;
			
			}
			
		} // kbhit if
		switch (locate)
		{
		case 8:
			sing->release();
			

			stageSelect(); // stage 선택은 이곳으로.

		 // 스테이지 선택 이후에 SetMaze 
			return;
















		case 7: // 랭킹.
			sing->release();

			ranking(); //랭킹 보다가 다시 나와서 loop돌도록. 이후에 ..

			system("cls");
			con = 1;
			stage = -1;
			SetMaze(); // Setmaze(랭킹보고 title) -> stage 설정 이후에 -> setMaze loop ( 근데 왜 튜토리얼로 ? )

			return;







			case 5:
				sing->release(); // 노래끄고 설정한 상태로 Setmaze -> loop
			
			while (_kbhit()) _getch(); // 버퍼. 

			return;
		}// 이후에 main -> set 1스테이지부터 . . .  // 튜토리얼 시작

		if (con == 1) return; // 내부 return으로 처리가 되지 않음. 
	}//while true

	
}
void ranking()
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

		if (arr[i].death != 0 || arr[i].name[0]) // 데스가 0이 아니면 제대로된 접근.  OR 0이지만, 이름이 있는 경우.
												 // 데스가 1 정상.   데스가 0 "ㅇㅁㄹㅇㄹ" 정상. 
			printf("%2d . %s Death : %d", i + 1, arr[i].name.c_str(), arr[i].death);
		else if (arr[i].death == 0)  // 나저지 중에 death = 0 인 경우는 입력이 없어서.
			printf("%2d . ───", i + 1);

		j += 2;
	}

	while (true)
	{

		if (_kbhit()) // if문만 있다면 입력있니 없니 확인하고 그냥 끝내버림.
		{
			music->release();
			return;
		} //입력이 있으면 out
	}


	while (_kbhit()) _getch(); // 버퍼. 
}

void stageSelect()
{
	// stage 셀렉트를 통해 한번 이용을 한 경우에는 랭킹 등록에 불가 ( 왜냐면 처음부터 안 하잖아. )  1번 이용 -> main으로 이동.

	system("cls");
	char input = NULL;
	Fmod->update();
	locate = 1; // 처음에 enter를 누르면 stage = locate 부분에서 0이 데입되어 버려서 튜토리얼로 이동한다. 

	selectOpption(); // 옵션에는 좌측 문구와 아이콘 출력.
	

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ▶");

	if (hansei == 2) hansei = 1; // 한세 = 2인 경우는 select로 시작했을 경우. -> 이후에 게임 클리어일 때 한세 = 2를 조건으로 낚아채고, 이후 main에서 처리.
	while (true)
	{
		Fmod->update();
		
		if (_kbhit())
		{
			
			char input = _getch();
		     // 방향키 오류 지점 . 
		   
			while (_kbhit()) _getch(); // 버퍼

			switch (input)
			{
			case ESC:
				exit(0);
				break; // 화살표 말고 다른 값에 대한 처리. select 과정에서는 m M BackSpace 등을 할 수 없도록 함.
			}
			
			selectOpption();
			selectPrint(input); //화살표 Enter Space 등에 관한 처리. stage 설정. 


			if(locate != 0) 
			stage = locate; // 고르고 난 이후에는 locate(0) 을 데입하면 안 된다.


			if (hansei == 2) { // enter input or spacebar input 

							   // 설정한 값으로 title 빠져나옴.  => 설정을 토대로 다시 SET MAZE 

				while (_kbhit()) _getch();

				return;
			}

			SetMaze(); // 자체 내부 cls를 막아야 옵션이 뜬다. locate == 0

		
			// NOT enter NOT SPACEBAR stage ( 입력값을 기준으로 ) 맵과 그림.

			// 중간에 초깃값 입력만 하고서 뺴오도록 설정. locate > 0  select 하고 실행될 때는 다시 중단되지 않도록 locate = 0;

			OutPut(); // 맵 보여주기.  여기서 화살의 좌표를 데입한다.

		

			


		} // kbhit
	}//while
}
void selectPrint(char input)
{
	switch (input)  // 입력에 대한 처리 ▶ 이동 
	{


	case 'w':
	case 'W':
		
		switch (locate)
		{
		case 1:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 14, "  ▶"); //더 이상 올라갈 곳이 없으므로 자신의 자리.
			break;
		case 2:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 14, "  ▶");
			break;
		case 3:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 16, "  ▶");
			break;
		case 4:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 18, "  ▶");

			break;
		case 5:
			TextColor(DEEP_WHITE);
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

		break;

	case SPACEBAR:
	case ENTER:
		stage = locate;
		hansei = 2; // 셀렉트 진행중.
		locate = 0; // 선택 이후에는 0으로.  이후에 출력될 SetMaze에서 오류를 방지.
		
		break;
	}

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
	gotoxy(MAP + 5, 10, "         Stage Choice");
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

void cheat()
{
	Fmod->update();
	stage++;
	SetMaze();
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
void tutorial(char input) 
{

	switch (quest)
	{

	case 0: //0. Backspace를 눌러보아요 0일때 한번 출력하게 하구. 이후에 미션 성공시에 넘어가주는 식.
		if (number == 0)
		{
			death = 0; // 첫미션의 첫 지령에서 죽음을 0으로 설정
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
		if (death >= 1) {  // 화살에 의해 사라지지 않고 5번블럭까지 미는 경우를 대비하여 블럭 설치.
			quest++;
			Maze[7][14] = '1'; number = 0;
			Maze[4][14] = '6';
			Maze[6][14] = '5';
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
			SetMaze();
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
void speedShow()
{
	TextColor(DEEP_WHITE);
	gotoxy(MAP + 10, 1, "    Arrow Speed : "); TextColor(GRAY); printf("%d", arrowSpeedSet); // 보여주기
	gotoxy(MAP + 10, 1, "→");
}
void speedSetting()
{
	switch (stage)  // arrowSpeedSet 의 변수에 따른 속도 조절 
	{
		// -1은 타이틀 속도 조절 X 0 = 튜토리얼 X
	case 1:
		switch (arrowSpeedSet)
		{
		case 1:
			arrowSpeed = 15;
			break;
		case 2:
			arrowSpeed = 12;
			break;
		case 3:
			arrowSpeed = 9;
		}
		break;
	case 2:
		switch (arrowSpeedSet)
		{
		case 1:
			arrowSpeed = 3;
			break;
		case 2:
			arrowSpeed = 2;
			break;
		case 3:
			arrowSpeed = 1;
		}
		break;
	case 3:
		switch (arrowSpeedSet)
		{
		case 1:
			arrowSpeed = 6;
			break;
		case 2:
			arrowSpeed = 4;
			break;
		case 3:
			arrowSpeed = 2;
		}
		break;
	case 4:
		switch (arrowSpeedSet)
		{
		case 1:
			arrowSpeed = 3;
			break;
		case 2:
			arrowSpeed = 2;
			break;
		case 3:
			arrowSpeed = 1;
		}
		break;
	case 5:
		switch (arrowSpeedSet)
		{
		case 1:
			arrowSpeed = 3;
			break;
		case 2:
			arrowSpeed = 2;
			break;
		case 3:
			arrowSpeed = 1;
		}
		break;
	case 6:
		switch (arrowSpeedSet)
		{
		case 1:
			arrowSpeed = 7;
			break;
		case 2:
			arrowSpeed = 5;
			break;
		case 3:
			arrowSpeed = 3;
		}
		break;
	}
}