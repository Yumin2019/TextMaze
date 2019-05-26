#include<iostream>
#include<conio.h>
#include<Windows.h>
#include"Text.h"
#include"Maze.h"
#include "fmod.hpp"

extern FMOD::System	*Fmod;
extern FMOD::Channel *channel1; // 배경음악 sound 조절. 함수(지역)에서 안 쓰고 외부 지역에서 쓰는 것. 내부에서 쓰지 않아서 적용 됨.
extern FMOD::Sound *music; // SetMaze에서 사용하는데 이후에 Fmod-> close에서 미리 해제를 해줘야 한다.
extern float speaker; // speaker 소환.
extern int death;
extern int stage;
extern int create;
extern int quest;
extern int hansei;
extern int titleOn;
extern int arrowSpeedSet;
using namespace std;

int block9; // 블럭9 open STAR (endPos)

#define ESC 27
#define SPACEBAR 32
#define BACKSPACE 8


struct _tagPoint
{
	int x;
	int y;
};


int main()
{

	system("mode con cols=103 lines=34  "); //CLI 크기
	system("title TEXT MAZE");//상단 이름 

	/* 커서 없애기 */
	
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = false;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);


	stage = -1;
	

	SetMaze(); // 그니까 구체적으로 표현하면 여기서는 인자를 따로 받지 않지만, 다른 코드에서 온갖 데이터 처리는 모두 한다.
	//미로 설정 title이 내부에서 시작되어 키값을 받고, 이후에 stage값을 변동.
	
	SetMaze(); // 바뀐 stage값을 이용하여 다시 셋팅.

	char cInput = NULL;

	Fmod->update();

	while (_kbhit()) _getch();

	while (true)
	{
			OutPut();
			//미로 출력 


			//개발 함수
			if(create)
			playerPos();

			// tEndPos == tPlayerPos 에 대한 처리는 move에서 해주고 있다.

			playerInform(); // 플레이어 inform

		if (_kbhit())
		{
				cInput = _getch();

				while (_kbhit()) _getch();

				// 방향키에 대힌 처리에 오류가 계속 되어 없애버림. 문자인식 X ,  m으로 인식. 방향키의 경우를 대비하여 _getch()

			switch (cInput)
			{
			case '1':
			case '2':
			case '3':
				arrowSpeedSet = cInput - '0';
				speedShow(); // 바뀐 값 보여주기.
				speedSetting(); // 값이 변하면 다시 셋팅.
				
				break; // 이후 셋팅 값에 따라 화살 속도를 변경해준다.
			
			case ESC:
				exit(0);
				break;
			case SPACEBAR:
				if(stage != 0 || quest >= 2) // 튜토리얼이 아니거나, 튜토리얼 이지만, quest가 2이상인 경우 폭탄 설치 가능 (따라서 사망 퀘스트에서 폭탄으로 죽을 수 없다.)
				CreateBomb();
				break;
			case 'q':
			case 'Q':
				Fire();
				break;
			case BACKSPACE: // 이 경우는 게임중에 back을 누른 것이다. 따라서 title과는 무관.
				SetMaze();	//튜토리얼이면 return
				OutPut();
				if(stage != 0) // 튜토리얼이 아닐 때만.
				death++; // backspace 죽은 것으로 처리
				break;
			case '=':
				//+
				speakerPlus();
				break;
			case '-':
				speakerMinus();
				break;
			case 't':
			case 'T':
				mute();
				break;
			case '9': // status
				status();
				break;
			case '0':
				cheat();
				break;
			case 'm':
			case 'M':
				if (stage == 0) break; // tutorial에서 m을 막음
				//하위 요소 해제
				music->release();
				// 상위 요소 해제.
				Fmod->release();
				
				stage = -1;

				SetMaze();  // title에서 stage 설정.

				SetMaze();  // 다음 셋팅에서 적용.

				OutPut();

				while (_kbhit()) _getch();

				break;
		
			}
				MovePlayer(cInput);
				arrowMove();
				openDoor();
				// 스위치 개수에 따른 처리.
				
		}
		else
			arrowMove();
		// 입력이 있으면 값을 받아 처리하고 움직이는 동시에 화살도 움직이도록 한다.
		// 입력이 없으면 화살만 움직이도록 한다.


		if (stage == 0) // tutorial
			tutorial(cInput);
		//일단 튜토리얼에서 맵이 뜨면 움직일 거니까. 그때마다 계속 실시간 처리.

		
		channel1->setVolume(speaker); // 배경음만 따로 처리.
		Fmod->update();

	}
	
	
	system("pause");
	return 0;
	
}

