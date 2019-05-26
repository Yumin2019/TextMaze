#include<iostream>
#include<conio.h>
#include<Windows.h>
#include"Text.h"
#include"Maze.h"
#include "fmod.hpp"

extern FMOD::System	*Fmod;
extern FMOD::Channel *channel1; // ������� sound ����. �Լ�(����)���� �� ���� �ܺ� �������� ���� ��. ���ο��� ���� �ʾƼ� ���� ��.
extern FMOD::Sound *music; // SetMaze���� ����ϴµ� ���Ŀ� Fmod-> close���� �̸� ������ ����� �Ѵ�.
extern float speaker; // speaker ��ȯ.
extern int death;
extern int stage;
extern int create;
extern int quest;
extern int hansei;
extern int titleOn;
extern int arrowSpeedSet;
using namespace std;

int block9; // ��9 open STAR (endPos)

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

	system("mode con cols=103 lines=34  "); //CLI ũ��
	system("title TEXT MAZE");//��� �̸� 

	/* Ŀ�� ���ֱ� */
	
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = false;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);


	stage = -1;
	

	SetMaze(); // �״ϱ� ��ü������ ǥ���ϸ� ���⼭�� ���ڸ� ���� ���� ������, �ٸ� �ڵ忡�� �°� ������ ó���� ��� �Ѵ�.
	//�̷� ���� title�� ���ο��� ���۵Ǿ� Ű���� �ް�, ���Ŀ� stage���� ����.
	
	SetMaze(); // �ٲ� stage���� �̿��Ͽ� �ٽ� ����.

	char cInput = NULL;

	Fmod->update();

	while (_kbhit()) _getch();

	while (true)
	{
			OutPut();
			//�̷� ��� 


			//���� �Լ�
			if(create)
			playerPos();

			// tEndPos == tPlayerPos �� ���� ó���� move���� ���ְ� �ִ�.

			playerInform(); // �÷��̾� inform

		if (_kbhit())
		{
				cInput = _getch();

				while (_kbhit()) _getch();

				// ����Ű�� ���� ó���� ������ ��� �Ǿ� ���ֹ���. �����ν� X ,  m���� �ν�. ����Ű�� ��츦 ����Ͽ� _getch()

			switch (cInput)
			{
			case '1':
			case '2':
			case '3':
				arrowSpeedSet = cInput - '0';
				speedShow(); // �ٲ� �� �����ֱ�.
				speedSetting(); // ���� ���ϸ� �ٽ� ����.
				
				break; // ���� ���� ���� ���� ȭ�� �ӵ��� �������ش�.
			
			case ESC:
				exit(0);
				break;
			case SPACEBAR:
				if(stage != 0 || quest >= 2) // Ʃ�丮���� �ƴϰų�, Ʃ�丮�� ������, quest�� 2�̻��� ��� ��ź ��ġ ���� (���� ��� ����Ʈ���� ��ź���� ���� �� ����.)
				CreateBomb();
				break;
			case 'q':
			case 'Q':
				Fire();
				break;
			case BACKSPACE: // �� ���� �����߿� back�� ���� ���̴�. ���� title���� ����.
				SetMaze();	//Ʃ�丮���̸� return
				OutPut();
				if(stage != 0) // Ʃ�丮���� �ƴ� ����.
				death++; // backspace ���� ������ ó��
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
				if (stage == 0) break; // tutorial���� m�� ����
				//���� ��� ����
				music->release();
				// ���� ��� ����.
				Fmod->release();
				
				stage = -1;

				SetMaze();  // title���� stage ����.

				SetMaze();  // ���� ���ÿ��� ����.

				OutPut();

				while (_kbhit()) _getch();

				break;
		
			}
				MovePlayer(cInput);
				arrowMove();
				openDoor();
				// ����ġ ������ ���� ó��.
				
		}
		else
			arrowMove();
		// �Է��� ������ ���� �޾� ó���ϰ� �����̴� ���ÿ� ȭ�쵵 �����̵��� �Ѵ�.
		// �Է��� ������ ȭ�츸 �����̵��� �Ѵ�.


		if (stage == 0) // tutorial
			tutorial(cInput);
		//�ϴ� Ʃ�丮�󿡼� ���� �߸� ������ �Ŵϱ�. �׶����� ��� �ǽð� ó��.

		
		channel1->setVolume(speaker); // ������� ���� ó��.
		Fmod->update();

	}
	
	
	system("pause");
	return 0;
	
}

