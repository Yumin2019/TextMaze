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
	int death; // �÷��̶��� ��� ������� �ҷ��� �� ���� �� / ����

	Lank(const string& name, int death) : name(name), death(death) {}



	friend ofstream& operator<<(ofstream& a, Lank& b);

	friend bool cmp(const Lank& a, const Lank& b);
	
	Lank() { } // �⺻ ������ 100 �׷�
};

ofstream& operator<<(ofstream& a, Lank& b) // ���Ͽ� �Է�
{
	a << b.name.c_str() << "." << b.death << "\n"; // �̸��� ���Ͽ� ���  �̸�.death        string�� �����ϸ� char*
	return a;
}
bool cmp(const Lank& a, const Lank& b)
{
	if (a.death != b.death) return a.death < b.death;

	//������ ���� ������.
	return a.name < b.name;
}

Lank arr[100];
/* ����� ����.

�ϴ� ������ �ִٸ�, �� �κп� ���� ������ �޾ƿ´�.  ( 100��ġ�� ��´�.)

�޾ƿ� ������ ���� (���� ����)
+ �Է� �� ������ ����(���� + 1)

�� �������� sort

sorted �� �迭�� ������� ���.


�츮�� ������ �ؾ��ϴ� ����. (������ �߰� X ) �׳� ��ŷ ų �� ( ������ �״�θ� ���� ���� �״�� ���.) �о�� �� = ���� ��(���� �� + �߰� 1)
������ �߰� O �翬�� ��!  �� �����͸� ��� �κп� ��� �־�� ���� �����Ͱ� ������� ������ ? ���� ������ �˰� �־�� �Ѵ�.

�׷� �ϴ�. �̸� �о�� ���� ������.  *������ ������ �ϴ� �о�;� ���� �ȴ�.


��� Ÿ�ֿ̹� ������ �ؾ� �ϴ°� ?

ranking ų �� (�ֳĸ� �̸� �س��Ƶ� ���� �� ��.)
ranking ų ������ sort�ϸ� ��ȿ���� �� �ƴϳ� ?
=> ��¥�� 100�� �����͸� �� �� �� �մϴ�. 
�׳� ���� ���� .
*/
int read; // title���� ó��.

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
FMOD::Sound		*music(nullptr); // ������������ ����� ������  createSound�� ������ ���� ��� ����(ȿ���� ó��) createStream�� �ϳ��� ��� ���� ( ������� )
FMOD::Sound		*jump(nullptr);	// ȿ�����̶�� �Ͽ��� ���������� �����ϸ� �Ҹ��� ����.
FMOD::Sound		*button(nullptr);




// �뷮 ���� createSound < createStream // ��������� createSound�� �ϸ� �ſ� ��ȿ����. ���������� ������ �� release�� ���� ����.
													// ���� �����͸� release�ϴ� �����̶�� ������.

//�Ҹ��� �Լ������� �����ؼ� ����Ѵ�. ���� �Ҹ��� ���ÿ� ���� ��츦 ����ؼ� 4���� ����.


FMOD::Channel	*channel1(nullptr); // ä�� 1�� �뷡�� Ʋ� �Ҹ��� �������ش�.
FMOD::Channel	*channel2(nullptr);// ä�� 2�� ȿ�������� �Ҹ��� �������� �ʴ´�.
FMOD::Channel	*channel3(nullptr);
FMOD::Channel	*channel4(nullptr); // ȿ���� ä��2 �л�.


// ä���� ������ ���� �Ҹ��� �����ص� ���/ init �κ��� real ä�μ�. title (���ۺκп��� ó����)


FMOD_RESULT		result;

/* ���� ����ϴ� ����� �̸� ����ָ� �ǰ� �ƴϸ� ���������� ���.*/

void			*extradriverdata(nullptr);

double speaker = 0.5;
int soundEffect = 1;// ȿ���� on off
int dataCheck; // data�� �޾Ҵ°� �޾����� 1
int create; // ������ ����.
int hansei; // hansei�� ���ϸ� 1 ( ���� ������ �ٽ� ���� ���� )

int quest; // tutorial ����
int death; // death count
int num;
int stage;
int number; // Ƚ��. Ʃ�丮�󿡼� ���.
long arrowSpeed; // arrow speed�� stage�� �����ϵ��� �Ѵ�. ����� sleep�� �μ��� ������ ũ�Ⱑ ũ�� ������
int titleOn; // Ÿ��Ʋ�� ���۵� ���� ������.
extern int block9;

//end ����Ʈ�� ��Ƶΰ� �װͰ� ���� arrow �� ������ ó��.

POINT arrowPos[15];

int arrowCount;

char arrowVector[15]; // �� arrow���� �츮�� ��� �������� ���ư��� ������ �ν��� �հ��� �ʿ���.


POINT arrowStart[15]; // count�� ��Ī�ؼ� �޾ƾ� ��.

int switchOnCount;


int switchCount; // swī��Ʈ 

// �������� ó���ϴ� start end 
POINT tStartPos;
POINT tEndPos;
POINT	tPlayerPos; // ��� �̰͵� ������ ó���� �̰����� �ϸ� �ξ� �� ���ϴ�. �츮�� main�ʿ��� �����͸� �޾ƿ� ���� ������ ������ �̿�������, ���⼭ �°� ó
//���� ���ϴ� ��Ȳ���� ���� ���� �׳� ����ü ������.x ooool.y


char	Maze[30][31];

POINT tBombPos[5];

int iBombCount;

// Maze , boom ����ü , count;


enum COLOR {
	BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY
	, DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW,
	DEEP_WHITE
};


void SetMaze(int* block9)
{
	if (quest >= 1) return; // quest �������� �� ���� �ٽ� �� ����.

	Fmod->update();

	// �������� ���� �ٸ� ��ǥ�� ? �� ����� �� �ϴ�. �׸��� ���� ���� ���� �迭�� �����ؾ� �Ѵ�.
	//�ϴ� ���������� 5�� ���� ����� .. �ʹ� �������� ������ ����
	


	/*


	0 : ��
	1 : ��
	2 : ������ �� �������� ǥ���̴�. ��ǻ� end�� start�� ����ü�� ���� �ִ� ���� �ƴϰ�, stage�� ���� end start ����Ʈ ����.
	3 : ������
	4 : ��ź

	5 : ��ź���� �ν� �� �ִ� ���� ��

	6 : pushable Block
	7 : SW ����ġ ���� ( ��� ����ġ�� on�� �� Ż�� ����)
	(��ü�����δ� ����ġ ������ �м��ִ� ������ ����� �� ���� �ִ� ���� ������ 
	���������� �� �� �ִ� ����.)

	8 : Sw ����ġ ���� ON����.

	9 : ��� ����ġ�� ON�̸� �ո��� ��.

	< : to ���� arrow
	> : to ������ arrow
	
	- : to �Ʒ��� arrow
	+ : to UP arrow

	������ arrow�� �� ��ġ�� �����Ѵ�. ���� ������ �״��. ���� arrow�� ���� �����ص� ����
	? ������. �ֳĸ� ���ΰ��ų� �Ʒ��� ���ų� �������� �����ϱ� �ش� arrow�� ���� �� �ִ�
	end������ �׻� �ϳ��ۿ� �����Ƿ� (2���� �Ѵٸ� �𸦱.)

	e : arrow end Point 

	e�� ������ ���.


	���� ȭ���� ��� ǥ������ . . .

	l : died left arrow

	r : died right arrow

	u : died up arrow

	d : died down arrow

	o : died pushable block

	�� ���⼭ ���캸��, �츮�� �� ���� arrow�� �ٽ� ���� �ʿ䰡 ������ ? 
	���� �ٽ� ���� �ʿ䰡 ����. ���� �̰��� arrow�� �ν����� ���ϰ� �ؾ��Ѵ�.
	�׷��� �׳� Maze���� ���Ը� �صΰ�, move���� �������� ����.
	

	' ' : ���� �ǵ帮�� �ʴ� �κ� ( ����ٰ� gotoxy�� �׸� �׷��ΰ� ���Ŀ� �� �ǵ帲)

	*/


	//MazeList.txt ������ �о�ͼ� �̷� ����� �����.

	/*
	FILE*	pFile = NULL;

	fopen_s(&pFile, "MazeList.txt", "rt");

	if (pFile)
	{
		char	cCount;

		fread(&cCount, 1, 1, pFile);

		int MazeCount = atoi(&cCount);

		char** MazeList = NULL;

		fread(&cCount, 1, 1, pFile); // \n���� ���ֱ�.
		// char**�� �����Ͽ� char*�� ������ ���.

		MazeList = new char*[MazeCount];

		// char* �� (�̷� �̸�)�� �޴´�.

		

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
			// ���ڿ��� ���� NULL ����. ���ڿ��� ��.
			// test �Ϸ�.
		}
		fclose(pFile);

		for (int i = 0; i < MazeCount; i++)
		{
			cout << i + 1 << " , " << MazeList[i] << endl;
		}

		cout << "�̷� ���� : ";
		int a;
		cin >> a;
		
		// ������ �̸��� �̷θ� �о��.
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
	*/  // ���� �����.



	// ���Ŀ� �� ���� �� �ϳ��� ������ �̷θ� �о�´�.
	num = 0;
	arrowCount = 0;
	switchCount = 0;
	switchOnCount = 0;
	*block9 = 0; // ������ ó�� ���� ������ �ʾҴٴ� ǥ��.

	// death�� �ǵ帮�� �ʴ´�. (1~6 stage���� ���� ��ŷ�� ��� ��) death 0�� Ʃ�丮�� or title���� �� ó����.
	
	system("cls");

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "�� Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "�� button :  + - ");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);

	while (_kbhit()) _getch(); 
	
	 // �������� ���ø��� num = 0�� ���ؼ� ����ġ, arrow ���� ������ �ٽ� �����ش�.  endPos �� startPos �� stage ���� ����.
	switch(stage)
	{
	case -1:
		title(); // Ÿ��Ʋ ������ Ÿ��Ʋ���� ó���ϰ� �ִ�.
		break;
	case 0: // tutorial ���� ���ΰ� tutorial ���ǿ� �°� open �� ó��.
		

	
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
		strcpy_s(Maze[4 ], 31, "000000000000000000000000000000"); //q2 ���� �����ְ�
		strcpy_s(Maze[5 ], 31, "000000000000>1111e000000000000"); // ȭ�쿡 �׾�ô�.
		strcpy_s(Maze[6 ], 31, "000000000000001000000000000000"); //���� ��������, ��� ��ź �ڻ��ص�
		strcpy_s(Maze[7 ], 31, "000000000000000000000000000000"); // �ǵ��� ? (�ƴϸ� ��ź�� ����)good
		strcpy_s(Maze[8 ], 31, "000000000000001000000000000000"); // �����ϸ� �����ְ� ��ź on
		strcpy_s(Maze[9 ], 31, "000000000000005000000000000000"); // �ش� �ڸ��� �ְ� ��. ǥ�ð� �ʿ��ѵ� Ư������ �ϳ� ������.
		strcpy_s(Maze[10], 31, "000000000000001000000000000000"); // 4 - 3����, 
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

		
		break; // Ʃ�丮��
	case 1:

		music->release();

		
		//�Ҹ����� ��ɸ� �ֵ��� ����.
		//�������� �̿� setvolume ���߿�
		Fmod->createStream("stage1.mp3", FMOD_LOOP_NORMAL, 0, &music);
			Fmod->playSound(music, 0, false, &channel1); //���� ���� stage 1.

		tPlayerPos.x = 1; //1 1
		tPlayerPos.y = 1;
		// stage 1�� ���� �Ѿ�� ���� �ļ���. test
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
		//�̰� arrow������ ���� ó���ӵ��� �簢���̶�, ������ �ӵ� ������ �ʿ��ϴ�. Ȥ�� case �� ggo
		tPlayerPos.x = 1; // 1 1
		tPlayerPos.y = 1;
		// ���߿� ó���ϵ���.
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
		// ��ġ�� �κ��� ������ end point 

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
		// �׸� ������ �� �̻� ���� �����ε� ���� �� �ǰ� �׳� ��¯ ��.
		// �������� ���� �� �÷��� ���� �� �� �غ����� ���Ŀ��� ���������.
		// ���� ����.
	case 7: //Ending
		
		// ending �ϴܿ� sound �κ��� ���� ������ �Ѵ�. = > ������ �� ���� ����.

		Sleep(500);

		music->release();

		Fmod->createStream("ending.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		titleSpace(); // Ÿ��Ʋ �׸����� ���� �� �� ���.

		gotoxy_C(MAP + 5, 8,  "�� Thank You for Playing ", DEEP_YELLOW);  Sleep(1000);

		gotoxy_C(MAP + 5, 10, "�� Creator : Yumin Kim   ", DEEP_JAJU);  Sleep(1000); 

		gotoxy_C(MAP + 5, 12, "�� Hansei Cyber High School", DEEP_OC);  Sleep(1000);
	
		gotoxy_C(MAP + 5, 14, "�� blog.naver.com/lmj3322", DEEP_GREEN);   Sleep(1000);
		
		gotoxy_C(MAP + 5, 16, "�� PassWord : hansei",DEEP_WHITE);  Sleep(1000);


		titlePicture();

		

		OutPut();

		while (_kbhit()) _getch(); // ����. 

		gotoxy_C(14, 18, "What's Your Name ? : ",DEEP_WHITE);  

		string name;
		cin >> name;

		arr[read] = { name, death }; // �Է¹��� �����͸� ������ Lank ������ ����.
		read++; // read�� �׻� ������ ���� +1�� ����Ű�� �־�� sort�� �� �򰥸��� ����. 
				// �ٽ� ��ŷ ���� ���� �ߺ��� ��ҿ� ������ �� �Ǵ� �̰� �´�.
		// �Է� ��.
		//���Ŀ� main�޴��� �̵��ϰ� �ؾ� �Ѵ�.
		
		/* �߰� �ٶ� */
		
		//���� ��� ����
		music->release();
		// ���� ��� ����.
		Fmod->release(); 
		stage = -1;
		SetMaze(block9); // ���� ���, �̷��� ������ �ϴ� ���Ŀ� arrow���� �÷��̾� move � ������ �� �ֵ�.

		// �Է¹��� ������ setting				 
		SetMaze(block9); 
		OutPut();
		return;

	}

	channel1->setVolume(speaker);

	TextColor(DEEP_WHITE);

	gotoxy(MAP + 7, 6, "M : ���θ޴�");

	gotoxy(MAP + 7, 8, "T : ȿ���� OFF");

	gotoxy(MAP + 7, 10, "SPACE : ��ź ��ġ");

	gotoxy(MAP + 7, 12, "Q : ��ź ��Ʈ����");

	gotoxy(MAP + 7, 14, "W : ��   S : �Ʒ�");

	gotoxy(MAP + 7, 16, "A : ���� D : ������ ");

	gotoxy(MAP + 7, 18, "Backspcae �� : �����");

	gotoxy(MAP + 7, 20, "ESC : ���� ");

	// ��� �ǵ帮�� �ʴ´�.

	//3 = ���������� ��ǥ�� �¿���ϸ� �˻��ؾ� �Ѵ�. ���� Maze���� 3�� ������ �ھƵθ� �� �ȴ�. (�߸��� ����)


			/*
			����ڷ� ǥ���ϴ� ������ ���� �������� ��Ÿ���µ�
			tPlayerPos ��ġ�� ���� ���ÿ� ����� �Ǹ� �з�
			������ ������ �÷��̾� ��ġ�� �� ��ġ�� ���� ��
			�÷��̾� ��ġ�� ����ϵ��� ����
			*/
		
	//�� stage ���� ������ ���� ��ǥ�� startPoint �� �ȴ�. (���߿� ����� ) 
	if(stage != -1)
	tStartPos = tPlayerPos;

	Fmod->update();

	while (_kbhit()) _getch();
	// �ȳ�

}


void OutPut()
{
	Fmod->update();

	for (int i = 0; i < 30; i++) //�� y
	{
		for (int j = 0; j < 30; j++) //�� x
		{

			/*
			����ڷ� ǥ���ϴ� ������ ���� �������� ��Ÿ���µ�
			tPlayerPos ��ġ�� ���� ���ÿ� ����� �Ǹ� �з�
			������ ������ �÷��̾� ��ġ�� �� ��ġ�� ���� ��
			�÷��̾� ��ġ�� ����ϵ��� ����
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
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
					// title�� ��� ���� �ڸ��� �� �����. (�ӵ��� ������ )
					if (stage == -1 || stage == 7)
					{
						TextColor(DEEP_WHITE);
						gotoxy(LEFTSPACE + j, UPSPACE + i, "��");

					}
				}
				break;
			case '3':
				TextColor(DEEP_YELLOW);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
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
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				break;
			case '6':
				TextColor(GRAY);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				break;
			case '7':
				TextColor(DEEP_GREEN);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��"); // sw off

				// ����ġ�� ������ ���־�� �ϴϱ� off�� ���� ����.

				if (num == 0)
					switchCount++;

				// Q1. arrowó�� ����ġ�� ��ǥ, ������ǥ, end Point�� �ʿ��Ѱ� ?
				// A1. Ǫ�ú��� ���� ó����(�̹� move����) ���� ��ǥ�� �ʿ����. ������ǥ( �ٽ� ���ư��� �ʴ´� ) endPoint( ����ġ�� �ǹ̰� ����)
				// �� �ʿ����. ������ ���� �ȴ�.
				break;

			case '8':
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��"); // sw on //�Ѿ �Ұ�.
				break;
			case '9':
				switch (stage)
				{
				case 2:
					TextColor(DEEP_OC);
					break;
				default:
					TextColor(DEEP_RED); // 2���������� ����,

				}
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��"); // ��� ����ġ on . OPEN  :: ���� ��.
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
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
					break;
				case '-':
				case 'd':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
					break;
				case '>':
				case 'r':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
					break;
				case '<':
				case 'l':
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
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
			default: // 1 4 2�� �Էµ� ��쿡�� if������ ó�����ֱ�. 

				if (Maze[i][j] == '1' && tPlayerPos.x == j && tPlayerPos.y == i
					|| Maze[i][j] == '2' && tPlayerPos.x == j && tPlayerPos.y == i
					|| Maze[i][j] == '4' && tPlayerPos.x == j && tPlayerPos.y == i)
				{
					TextColor(DEEP_RED);
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				}// ��ó�� �Ϸ�. ���Ŀ� �ٸ� ���� ��� ? ...
				else if (Maze[i][j] == '1' || Maze[i][j] == '2')
					gotoxy(LEFTSPACE + j, UPSPACE + i, " ");

				else if (Maze[i][j] == '4')
				{
					TextColor(DEEP_JAJU);
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				} // 1,2,4 �κ��� if�� �켱������ ���� ó���ϵ��� �Ѵ�.

				// ��� ������ �κ�. �����ϱ�. 1,2,4�� �ʹ� ���� �־ if�� ���� ���ڴ�.

			}
			//����ġ ���̽������� �ٲٴ� ���� ��������... ���� �Ҹ� 

			Fmod->update();




			 // if ���� switch case �� �ٲ㼭 ó�� : 
		/*
			if (Maze[i][j] == '0' || Maze[i][j] == 'e')
			{
				if (num == 0)
				{
						TextColor(WHITE);
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				}
			}
			//
			else if (Maze[i][j] == '1' && tPlayerPos.x == j && tPlayerPos.y == i
				|| Maze[i][j] == '2' && tPlayerPos.x == j && tPlayerPos.y == i
				|| Maze[i][j] == '4' && tPlayerPos.x == j && tPlayerPos.y == i)
			{
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
			}// ��ó�� �Ϸ�. ���Ŀ� �ٸ� ���� ��� ? ...
			else if (Maze[i][j] == '1' || Maze[i][j] == '2')
				gotoxy(LEFTSPACE + j, UPSPACE + i, " ");
			//
			else if (Maze[i][j] == '3')
			{
				TextColor(DEEP_YELLOW);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");

			}//
			else if (Maze[i][j] == '4')
			{
				TextColor(DEEP_JAJU);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");

			}//
			else if (Maze[i][j] == '5')
			{
				TextColor(DEEP_YELLOW);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");

			}//
			else if (Maze[i][j] == '6')
			{
				TextColor(GRAY);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				//pushable block
			}//
			else if (Maze[i][j] == '7')
			{
				TextColor(DEEP_GREEN);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��"); // sw off

				// ����ġ�� ������ ���־�� �ϴϱ� off�� ���� ����.

				if (num == 0)
					switchCount++;

				// Q1. arrowó�� ����ġ�� ��ǥ, ������ǥ, end Point�� �ʿ��Ѱ� ?
				// A1. Ǫ�ú��� ���� ó����(�̹� move����) ���� ��ǥ�� �ʿ����. ������ǥ( �ٽ� ���ư��� �ʴ´� ) endPoint( ����ġ�� �ǹ̰� ����)
				// �� �ʿ����. ������ ���� �ȴ�.

			}//
			else if(Maze[i][j] == '8')
			{
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��"); // sw on //�Ѿ �Ұ�.

			}//
			else if (Maze[i][j] == '9')
			{
				TextColor(DEEP_RED);
				gotoxy(LEFTSPACE + j, UPSPACE + i, "��"); // ��� ����ġ on . OPEN  :: ���� ��.
			}

			//
			else if(Maze[i][j] == '-' || Maze[i][j] == '+' ||
				Maze[i][j] == '>' || Maze[i][j] == '<')
			{
				TextColor(DEEP_WHITE);

				 if (Maze[i][j] == '+')
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				else if (Maze[i][j] == '-')
					gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				 else if (Maze[i][j] == '>')
					 gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
				 else if (Maze[i][j] == '<')
					 gotoxy(LEFTSPACE + j, UPSPACE + i, "��");

				 //arrowPos[i]�÷� �츮�� ������ ����ü�� ������ �� �ִ�. x y GOOD
				 // �� ��ü�� ������. �� �����Ͱ� ����Ű�� ����ü�� ��� x // arrowPos[i]->x
				 // ����� end�� arrow��ġ�� ��üŽ������ ���� �Ŵϱ� ������. (������ �������.)

				 if (num == 0)
				 {
					 arrowStart[arrowCount].x = j;
					 arrowStart[arrowCount].y = i;
					 arrowPos[arrowCount] = arrowStart[arrowCount];
					 arrowVector[arrowCount] = Maze[i][j];
					 arrowCount++;
				 }


				 // ȭ���̶�� �ν��� �Ǵ� ��쿡 �츮�� ��ġ�� �����ؾ� �Ѵ�.
				 //��ź�� ��쿡�� �÷��̾� ��ġ�� �ִ� ���̶� �׶� �׶� ó���ϸ� ��. ����

			}//
			*/

		}

	}
	if (num == 0) // ó�� set���Ŀ� num == 1�̴�. 
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
			return; // ���� ��ź�� ��ġ�� �÷��̾��� ��ġ�� ������ ���� ������ ��ġ�Ұ� return;
	}

	//���� ���� ���
	FMOD::Sound		*bomb(nullptr);

	Fmod->createSound("bomb.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel3);
	channel3->setVolume(speaker);
	Sleep(50); // �̼��� �г�Ƽ 
	// ��ź��ġ�� ��ġ�� �ʰų� �ִ���� ���� ���� ��쿡�� ���� count�� �µ��� �÷��̾� ��ġ ���� 
	tBombPos[iBombCount] = tPlayerPos;
	++iBombCount;
	Maze[tPlayerPos.y][tPlayerPos.x] = '4';
	Fmod->update();
	

}



/*


0 : ��
1 : ��
2 : ������
3 : ������
4 : ��ź

5 : ��ź���� �ν� �� �ִ� ���� ��

6 : pushable Block
7 : SW ����ġ ���� ( ��� ����ġ�� on�� �� Ż�� ����)
(��ü�����δ� ����ġ ������ �м��ִ� ������ ����� �� ���� �ִ� ���� ������
���������� �� �� �ִ� ����.)

8 : Sw ����ġ ���� ON����.

< : to ���� arrow
> : to ������ arrow

- : to �Ʒ��� arrow
+ : to UP arrow

������ arrow�� �� ��ġ�� �����Ѵ�. ���� ������ �״��. ���� arrow�� ���� �����ص� ����
? ������. �ֳĸ� ���ΰ��ų� �Ʒ��� ���ų� �������� �����ϱ� �ش� arrow�� ���� �� �ִ�
end������ �׻� �ϳ��ۿ� �����Ƿ� (2���� �Ѵٸ� �𸦱.)

e : arrow end Point

e�� ������ ���.


*/


/*
		�츮�� player�� ������ ��(�����ӿ� �������� �� jump.mp3������ �����Ų��. (release X)

*/
void MoveUp()
{
	Fmod->update();

	if (tPlayerPos.y == 0)
		return;
	//�츮�� �����ϴ� x �� ������ ���� ���� �ǹ� 
	//y�� ���� ���� �ǹ��Ѵ�.

	//���� Maze[y][x] �� ����Ѵ�.
	
	int sw = 0;
	int pushBlock = 0;
	// ���� ����� ���� : �÷��̾� - 6�� - arrow �϶� ����ó���� �Ǿ� ���� �������� ����. = > �����̵��� ����.
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
			// �� ������
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1'; pushBlock = 1;
		
			break;
			// ����ġ
		case '7':

			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '8'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1'; sw = 1;
			switchOnCount++;
		
			break;
			//ȭ�� ����� ó��
		case '>':
		case '<':
		case '-':
		case '+':
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '1'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';
			break;
			// �����̸� �� �Ǵ� ���
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
	{// ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.
		

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel3);

	}
	//// push block 
	


	if (pushBlock == 1)
	{// Ǫ�ú��� �̵�.
		FMOD::Sound		*push(nullptr);

		Fmod->createSound("push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel3);

	}

	// �����̴� ���� ������� ����. jump.mp3

	 Fmod->createSound("jump.mp3", FMOD_DEFAULT, 0, &jump);// 1���ڸ�
	 if (soundEffect)
	 Fmod->playSound(jump, 0, false, &channel3);
	--tPlayerPos.y;
	channel3->setVolume(speaker); // Play ���Ŀ� �� �ؾ� �����.

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
		)// u d l r o �� ���� ȭ�� / ���� push �� ������ �������� ���ϵ��� �Ѵ�.
	{
		if (Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y - 2][tPlayerPos.x] == '1'||
			Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y - 2][tPlayerPos.x] == '2') // �÷��̾� - Push block - ���� ���
		{
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';//
		}
		else if (Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6' && Maze[tPlayerPos.y - 2][tPlayerPos.x] == '7')
		{
			// �÷��̾� - push - sw �� ���
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '8'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';
			switchOnCount++;
		}//
		else if (Maze[tPlayerPos.y - 1][tPlayerPos.x] == '6') return; // �÷��̾� - Ǫ�� �� - ���� ���//
		--tPlayerPos.y;

	}
	*/
}

void MoveDown()
{
	Fmod->update();

	if (tPlayerPos.y == 30)
		return;
	//�츮�� �����ϴ� x �� ������ ���� ���� �ǹ� 
	//y�� ���� ���� �ǹ��Ѵ�.

	//���� Maze[y][x] �� ����Ѵ�.

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
			//�÷��̾� - Ǫ�� �� - ���� ���
			return;
		}
		break;
	}

	//����ġ ��� �κ� 

	

	if (sw == 1)
	{// ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 



	if (pushBlock == 1) // Ǫ�ú��� �̵�.
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
			// �÷��̾� - push - sw �� ���
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
	//�츮�� �����ϴ� x �� ������ ���� ���� �ǹ� 
	//y�� ���� ���� �ǹ��Ѵ�.
	
	//���� Maze[y][x] �� ����Ѵ�.
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
			//�÷��̾� - Ǫ�� �� - ���� ���
			return;
		}
		break;
	}
	////����ġ ��� �κ� 

	
	
	if (sw == 1) // ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.
	{

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	
	

	if (pushBlock == 1) // Ǫ�ú��� �̵�.
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
	channel4->setVolume(speaker); // push, ����ġ�� 1 �߿��� 0.5
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
			// �÷��̾� - push - sw �� ���
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

	//�츮�� �����ϴ� x �� ������ ���� ���� �ǹ� 
	//y�� ���� ���� �ǹ��Ѵ�.

	int sw = 0;
	int pushBlock = 0;
	//���� Maze[y][x] �� ����Ѵ�.
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
			//�÷��̾� - Ǫ�� �� - ���� ���
			return;
		}
		break;
	}

	

	if (sw == 1) // ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.
	{

		Fmod->createSound("switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	


	if (pushBlock == 1) // Ǫ�ú��� �̵�.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound("push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	 // push, ����ġ�� 1 �߿��� 0.5

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
			// �÷��̾� - push - sw �� ���
			Maze[tPlayerPos.y][tPlayerPos.x - 2] = '8'; Maze[tPlayerPos.y][tPlayerPos.x - 1] = '1';
			switchOnCount++;

		}
		else if (Maze[tPlayerPos.y][tPlayerPos.x - 1] == '6') return;
		--tPlayerPos.x;


	}

	*/
}



void Fire() // ��ź�� �־ fire�Լ��� ������ �Ȱ� �ƴϾ� ��ġ ? �� ? �ƴѵ� ? ��ź�� ��ġ�� �� �ϰ� �����̽��ٸ� ���� ��� (�Ҹ��� ����ϸ� �� �ȴ�.) return
{
	Fmod->update();

	if (iBombCount == 0) return; // ��ġ�� �� �� ��� �׳� ����. (�Ҹ���� X)

	FMOD::Sound		*bomb(nullptr);

	Fmod->createSound("explosion.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel4);
	channel4->setVolume(speaker);
	
	int block = 0; // �� �����°� ? 
	for (int i = 0; i < iBombCount; i++) // >��ź ó�� (�����..) �� ! ����� ! �״ϱ� . . . arrow ó���� �Ѵٰ� 
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

		//��� . ��ź��ġ�� �¿� ���ϰ� �μ����� ������ Ȯ���Ͽ� ������
		// ��� �ٲ�.
		// ���⼭ �����¿�� �߰��� ���� �ִ� ��츦 ����. �� ������ ��Ʈ����
		// ���� �������� ����.(�׷��� �� ��)

		if (tPlayerPos.x == tBombPos[i].x && tPlayerPos.y == tBombPos[i].y ||
			tPlayerPos.x == tBombPos[i].x + 1 && tPlayerPos.y == tBombPos[i].y ||
			tPlayerPos.x == tBombPos[i].x - 1 && tPlayerPos.y == tBombPos[i].y ||
			tPlayerPos.x == tBombPos[i].x && tPlayerPos.y == tBombPos[i].y + 1 ||
			tPlayerPos.x == tBombPos[i].x && tPlayerPos.y == tBombPos[i].y - 1)
		{

			//�״� �Ҹ�
			FMOD::Sound *die(nullptr);
			Fmod->createSound("die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);// �Ҹ�1 
			
			channel2->setVolume(speaker);

			death++;

			tPlayerPos = tStartPos;  //�������� �迭�� ���� ó���ؾ� �� �κ�.
								// ��ź��ġ �¿� ���� Ȥ�� �� ���� �־��ٸ� ����(?) ������ ó�� ������ġ��.
		}
		// ��ź ��ġ != arrow��ġ��. �׷���, 1�� �����ص� ��. arrow��ġ�� ������, 1���� X �װ� ������ ���� ���� ���� 1�̸� 1�� �����ϸ� ���� ������?

		int sw = 0;
		for (int j = 0; j < arrowCount; j++) // ��Ʈ���� ó���ϰ� , ���� ȭ���̶� ��ǥ�� ���� ��찡 ������ break break �ɾ ��� ������ �ʴ´�.
			// ��ſ� �ٸ� ��ź�� ��� ��ġ�� �ʾ����� ������ �ش� ��ź�� �׳� ȭ�쿡 ����. �״ϱ� �׳� ȭ���� ��ǥ�� ����� �ǵ� 
			// ������ ����� �ϰ� �־, �����µ� ȭ���� ������Ų�ٰ� ���� ��. �ϳ��� ȭ��� ��ġ�� ���Ƽ� fire 4�� �������� 
			//���� �ش� ��ź�� ȭ�쿡 ������(������)
		{
			if (arrowPos[j].x == tBombPos[i].x && arrowPos[j].y == tBombPos[i].y)
			{
				sw = 1;
				break;
			}

		}
		if (sw == 1) break;
		// ���� ��ź�� arrow�� ��ǥ �߿� �ϳ��� ��ġ�� ���� ��ź�� ���� ó���� ���� �ʴ´�. ���Ƶ� ���� ��� �ϳ��� ��ħ.
		// ���Ŀ� ó���� �� �ϰ� ���߿� ȭ���� ��ġ �� �̵��ϸ� �� �ڸ��� �þ�԰���. (���� �� ����)
		// ���� �ٵ� �̰���, �÷��̾�� ��ġ�� �������� ���� ������ �׵��� ���� ���Ŀ� 1�� �� �ϴ� ������ �����ϰ� �ʹ�. ������ ȭ���� ó��.

		// �÷��̾� ��ġ�� ȭ����ġ�� ��ĥ �� ��ġ�ϸ� �� �ȴ�. @@@@

		Maze[tBombPos[i].y][tBombPos[i].x] = '1';
		//x ��ǥ�� �ٸ��ų� y��ǥ�� �ٸ��� �ٸ� ��ǥ. �ٸ� ��ǥ�� ���� 1��.


		// ��� ��ź�� ��ġ��.  arrow�� ��ġ�� ���� ��쿡�� 1�� ����.
		//��ġ�� ��쿡 ���� ó���� �� �Ѵ�.

	// �츮�� ��ź�� ��ġ�� ���� ���ϴ� ������ 4�� �ν��� �Ǿ��ֱ� ����.
	// �迭�� ��ġ�� �������, (�ٽ� �� �̿��ϸ� �ǰ�) 1�θ� �����صθ� 
	// ������ �� �ִ�.

	}
	if (block == 1) // �ϳ��� ���� �ƻ� �Դ� �Ҹ� 
	{
		FMOD::Sound *cookie;
		Fmod->createSound("cookie.mp3", FMOD_DEFAULT, 0, &cookie);
		if (soundEffect)
		Fmod->playSound(cookie, 0, 0, &channel3); // �ٻ�ٻ� �Ҹ� 1

		channel3->setVolume(speaker);
	}


	Fmod->update();

	iBombCount = 0; //��Ʈ�� ��
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

	if (Maze[tPlayerPos.y + 1][tPlayerPos.x] == '9' || // �÷��̾� ��ġ�� �¿���ϰ� ��� ���̸� sleep(10)
		Maze[tPlayerPos.y - 1][tPlayerPos.x] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x + 1] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x - 1] == '9')
		Sleep(150);


	if (tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y) // �׷��ϱ� �츮�� stage clear ������ ��ǻ� ������ ���� �ν��ϸ� �ȴ�. 
	{
		if (quest == 6) return; // �ȳ����� ������ �ؼ� ó�� �� �ϰ� ����. (���ʿ��� ó��)

		// �Ѽ� == 2�� �� �츮�� Ÿ��Ʋ�� ������ �Ѵ�. or ���� ? 

		if (hansei == 2)
		{
			music->release();
			hansei = 3;
			while (_kbhit()) _getch(); // �� �Դ´ٸ�, �ٽ� ��������� �ʰ���..
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
		OutPut(); // ����ġ ���� �� ��ġ�� ������ ������ ���� ����.
	}
	Fmod->update();

}

void arrowMove() // ��ǻ�� ��翡 ���� �ӵ����̰� �߻���. �ϴ� gotoxy�� ���� ���� �ƴ϶��, if else if �� switch case ������ ��ü�ϴ� �ɷ� ����.
{
	Fmod->update();

	// ENd ������ maze�� ���� �� �ִ�. maze [ i ] [j ] == 'e' ? ���� arrow��ġ�� 1�� �ű�� ��ġ��>
	for (int i = 0; i < arrowCount; i++)
	{
		// �ϴ� ������ �ȴٴ� ���� �ϳ��� �ִ�. �� �ڸ��� ��� ����� �ű��� �ڸ��� �°� ����


		//if (Maze[arrowPos[i].y][arrowPos[i].x] == '1' ||
		//	Maze[arrowPos[i].y][arrowPos[i].x] == '6') continue;

		if (Maze[arrowPos[i].y][arrowPos[i].x] == '6') // �Ӱ� ���. ����ִ� ȭ���� ���� ��������, �� �κ��� ��� ����� / ȭ���� ��ǥ�� 0,0 �ϸ� ���� ����.
		{
			Maze[arrowPos[i].y][arrowPos[i].x] = '1';
			arrowPos[i].x = 0; arrowPos[i].y = 0;
			continue; // ȭ��� ��ź�� ��ģ ��쵵 �ƴϰ� / �׷��ٰ� �÷��̾�� ȭ���� ��ģ ��쵵 �ƴϹǷ� �� �̻� ó������ �ʿ䰡 ����. �ش�ȭ�� out

			// �ٵ� �̷��� ���൵ ������ �������� ����. 
			// ��ġ �����ӵ� ���̰� ���ϰ� ���� ���� ���;; ���� ����
		}

		if (Maze[arrowPos[i].y][arrowPos[i].x] == '0') continue;

		// ȭ���� ��ġ�� ��ź�� ��ġ�� ��ģ ��쿡 �ٽ� ȭ���� ��Ƴ���. �ٵ� ȭ��� ��ź�� ���� ó���� ������ �Ʒ��� ��찡 �ذ��� ���� �ʴ´�.
		// ���� �츮�� ���� ȭ���� ��ǥ���� �ٲ��ִ� ������ ����. ex : 0,0 ���� ���� �׻� ���̹Ƿ� �� �ڸ��� 0�̸� continue ó���� �ϸ� ����ϴ�.
		// �ش� �ڸ��� ���� ���� ���� ���� ������ 6�� ���� ���� ���� �� ����. ��ź�� ��ġ�� ���ļ� ����� ���� ���� ���� �� ����. (���� ���ؼ�) 1�� ���� ����.
		// ���� ȭ�� ��ǥ��(0.0) ��ź�� ��ġ�ϴ� ���� ���� ���� �� ����. ( �����ϱ�) ��, ȭ��� ��ź�� ��ġ�� ���� ȭ���� ��� �ִ� ���� �����ȴ�.

		// ȭ���� ���� ���� ��쿡�� 0,0 �� �ƴϱ� ������ �Ʒ��� �������� ó���ϸ� �ȴ�. O K!

		// ���� ȭ�쿡 0,0�� �����ϵ���.

		// ȭ���� ��ź�� ��ġ�� ��ź�� ������, 1�� ������ �ʰ� ȭ���� �� �ڸ��� ��� ����. ( ��¶�� ���� ������ ���������� ó���ȴ�)

		// ��? �̶� ȭ����� ������ ���� ��� ���ֹ�����. ������ ȭ���� ��ǥ�� �״���̴�. �츮�� �̰��� ���ư��� �ʵ��� �ؾ��Ѵ�.
		// �ϴ� ��� ������� ȭ���� �����ؾ� �ϰ�, �� �ٸ� ��� �÷��̾� ��ġ�� ��ġ�� ��쵵 ���־� �Ѵ�.
		// �� �÷��̾�� ��ġ�� ��츦 ������ �ϴ°� ?
		// �ϴ� �÷��̾�move������ Ư���� ���̽��� �����Ͽ� �÷��̾� push �� �̷� ��츦 ó���Ѵ�. 
		// �׶� �÷��̾� push �Ʒ� ȭ��. 


		// ó�����ֱ� . @@ ��ǥ ȭ���� �ٽ� �����̵ǿ�. �÷��̾� ��ġ�� ������ why ���߿�. ���� ? ���µ� ? �ٽ� ���� ���� 5������������
		// ���̶� ��ģ ��� �ٽ� ����� ���� ���� ���϶��� out !


		switch (arrowVector[i])
		{
		case '>':
			if (Maze[arrowPos[i].y][arrowPos[i].x + 1] == '6')
			{
				Maze[arrowPos[i].y][arrowPos[i].x] = '1';
				Maze[arrowPos[i].y][arrowPos[i].x + 1] = '1';
				arrowPos[i].x = 0; arrowPos[i].y = 0; // ���� ȭ��.
				break;
			}
			else Maze[arrowPos[i].y][arrowPos[i].x] = '1';
			// ȭ���� �⺻������ ���빫�� �׳� ���� ���ְ� �̵��� ; oo �׷��� �÷��̾�� �ٽý����ؾ��ϴ� ��Ȳ�� ���� �� ����
			arrowPos[i].x++;

			if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')//�̵��غ��� end�����̴� -> start��
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
		// ��ź�� ��ǥ�� ���ƴ��� Ȯ���Ѵ�. e�� �����ϰ� ��ź�� ��ġ���� ( �ε��� ����) ��·�� ��ź ��ǥ == ȭ���� ��ǥ.
		for (int j = 0; j < iBombCount; j++)
		{
			if (arrowPos[i].x == tBombPos[j].x && arrowPos[i].y == tBombPos[j].y) // ��ź�� ȭ���� ��ġ�� ���ÿ� �÷��̾ ��ģ�� X �̷� ���� ����. �ֳĸ� ��ź�� ���ٺҰ�.
			{
				Fire(); break;
			}
		} // ��� ȭ���� ��ġ�� ��� ��ź�� ��ġ�� ���Ͽ� �ϳ��� ��ġ�� PANG ! ���Ŀ� Pang �� ���� �޼� break;


		if (arrowPos[i].x == tPlayerPos.x && arrowPos[i].y == tPlayerPos.y) // arrow��ġ�� player��ġ�� ������ ���� ��ġ��. 
		{						//end������ �� ���� ����, start or �濡�� �ε����� �ȴ�. ��ġ�� �ٲٰ� ó���� ���൵ �ƹ� ������ ����.

			//�״� �Ҹ�
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
	// ���� ó�� ���Ŀ� end Pos�� ������ �� �� ������ start Pos ����
}
void stagePlus()
{
	stage++;
}

void openDoor(int* block9) //�� ! arrow�� ȥ���ؼ� �����µ� arrow������ ���ֳ�, ����ġ�� �� ���� ���� �� 
{
	Fmod->update();

	if (*block9 == 1) return; // �ѹ��� ����ϰ� ���Ŀ� return; stage�� �ٲ�� block9 = 0�� ������ �ؾ��Ѵ�.

	if (switchOnCount == switchCount) // on������ ��ü ������ ������ (��� ����ġ on)
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
			Maze[tEndPos.y][tEndPos.x + 1] = '1'; // ����� ��. �츮�� ����ϸ� ����. 
			OutPut();
			Sleep(250);
			//* ���� ���� ���� ���� �� ��Ƿ� ��� / ��⸦ ��� ���� �絵�� �Ѵ�.  ������� term / �ٽ���� ��/ ������

			Maze[tEndPos.y][tEndPos.x + 1] = '9';
			Sleep(250);


			Maze[tEndPos.y][tEndPos.x + 1] = '1';
			OutPut();
			Sleep(250);

			// �� ������ �ƴϸ� �� ���ε���.
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

			//endPos�� �¿���ϰ� 9�� �ش��� ��(Ư�� ��) �� �κ��� ��� ��ģ��.

		}
	}

	//3 = ���������� ��ǥ�� �¿���ϸ� �˻��ؾ� �Ѵ�. ���� Maze���� 3�� ������ �ھƵθ� �� �ȴ�. (�߸��� ����)
}


void title() // Ÿ��Ʋ�� stage = -1���� �� �״�� �׸� �׸��� ���� �����̴�.
{

	/* ���� ���� ���� ������ mp3���� �����, �鸮���� �ʴ� �Ҹ��� result �����Ͽ� �����ϴ� ���� �ʹ����� ���� ��ȿ���� �����. 
	
	���� ���⼭ ���� ������ ������ ����, ���� ����¿� ���� ��������.

	
	*/

	while (_kbhit()) _getch(); // �ϳ��δ� ������...

	/* �̰��� playSound // createSound �κи� üũ������. */
	result = FMOD::System_Create(&Fmod); // �ٸ� �������� ��� ���� ?


	FMOD::Sound		*sing(nullptr);

	FMOD::Sound		*select(nullptr);

	 result = Fmod->init(25, FMOD_INIT_NORMAL, extradriverdata);
	 if (result != FMOD_OK) return;

	// ä���� 25�� ���� / �Ϲ��� / �� ���� ������

	 result = Fmod->createStream("title.mp3", FMOD_LOOP_NORMAL, 0, &sing);
	 if (result != FMOD_OK) return;

	//���ȭ�鿡���� ��� ��� sing 
	// Stream or Sound
	 result = Fmod->createSound("select.mp3", FMOD_DEFAULT, 0, &select);
	 if (result != FMOD_OK) return;

	//select effect


	//���� �׳� ������ ������ .... �ҽ����ϰ� ������Ʈ ��ġ�� �޶� �ϱ�?
	// .... �׷� ���� �ִ�. �Ʊ� dll���Ͽ��� �ҽ����� ������Ʈ�� �־��� �� ����.

	//�ù� ��¥����..

	 result = Fmod->playSound(sing, 0, false, &channel1);
	 if (result != FMOD_OK) return;

	// sing, puase���� ä��1
	
	channel1->setVolume(speaker); // �⺻ 0.5
	
	
	titlePicture();
	block9 = 1;
	num = 0;
	stage = -1;
	// �� �̻� �� �߰� �� �ؿ� ���⼭��. ���

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 9, "������������������������������������������������������");
	

	gotoxy(MAP + 5, 11, "������������������������������������������������������");

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "�� Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "�� button :  + - ");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
	// �ʱ� 0.5 Setmaze���� ó��������, title������ setmaze�� ������� �����Ƿ� �ߺ��� �ƴϴ�.

	// ���� ���� �ϴ� text ���� ���� ������ ? �� �صΰ� �̰� ���� ����� ǥ���ؼ� ����. oo
	TextColor(DEEP_RED);
	gotoxy(MAP + 5, 10, "��        Text Maze        ��");
	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "         ���ο� ����       ");
	// ó�� ���ý� ���ο� ������ ��� ����.

	gotoxy(MAP + 5, 16, "           Ʃ�丮��        ");
	gotoxy(MAP + 5, 18, "        �������� ����      ");
	gotoxy(MAP + 5, 20, "           Ranking         ");
	gotoxy(MAP + 5, 22, "           �����ϱ�        ");
	gotoxy(MAP + 5, 25, "    Copyright ��2018-Yumin ");
	// ICON �� ��ġ�� �ٲ��� �ʴ´�.
	titleIcon();



	// �ٹ̱� 
	TextColor(DEEP_OC);
	gotoxy(MAP + 5, 25, "��");
	gotoxy(MAP + 19, 25, "��");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 6, 25, "��");
	gotoxy(MAP + 18, 25, "��");
	// copyright

	/*
	TextColor(DEEP_GREEN);
	gotoxy(MAP + 18, 25, "��");
	TextColor(DEEP_RED);
	gotoxy(MAP + 19, 25, "��"); 
	*/
	// ���۷���
	TextColor(DEEP_YELLOW);
	gotoxy(1, 32, "��");
	TextColor(DEEP_WHITE);
	gotoxy(3 , 32, "References : http://assortrock.com (Maze Game) ");
	

	int locate = 0; // ���� ������ ��������, ��� ��ġ ������.
	char str[40];// ��й�ȣ�� �������� ����� �迭. �ʹ� �����ϰ� �ָ� ���ڸ� ���� �Է��� �� ��̾�����.

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ��"); 

	// ��ǻ� ó������ �Է��� ���ٸ� ���� ���� �����Ƿ� �̸� ��Ƶ־� �Ѵ�.

	//�츮�� title���� while������ Ű���� �Է��� �޾� �� ó���� �������̽��� ó��.
	// ���Ŀ� ���� �� ���� ó�� ..
	OutPut();

	//���� ��������� �����ų ���� �ְڳ�... ����; // ��������. ������ �ɸ�.





	// read ( ���� �����Ͱ� �ִ��� Ȯ���մϴ�. ) 

	ifstream in("TMRank.txt");
	string name;
	int death;

	if (dataCheck == 0)
	{
		if (in.is_open()) // read�� ó������ �ް� ���Ŀ� clear�� ���� +1 ó���� �Ǿ� sort ���� ...
		{
			string buffer;


			while (!in.eof())
			{// EOF�� �ƴϸ� ���. 	

				in.getline(&name[0], 100, '.'); // name[0] ���� 100���� �����͸� �ִµ� .�� ������ �׸� ����.  name ����.

				in.getline(&buffer[0], 10); // ���� . �ڿ� "����"\n ���� "����" �κ��� ���ڷ� �ٲ㼭 death�� ����.
				death = atoi(buffer.c_str());

				// ���� ���� ����. 

				arr[read] = { name, death };
				// 0�� ��찡 �ƴ϶� - 1.
				if(death == 0 && name[0] || death != 0) // ���۰� ���ÿ� ���� ��쿡�� 0�� ����Ŵ( �߸��� ������ ) �׷��� read = 0�� ���·� ���������ش�. -1�� ��� read�� �״�� �ؾ��Ѵ�.
				read++; //  read�� ���� ���Ҹ� (�� �ڸ���) ����Ų��. ���� ���ο� ���� �־��� �� arr[read]�ϸ� �ȴ�.
				//size == 0 �̸� �߸����� ����̴�. ��ϵ� �����Ͱ� �̸��� ���� ���� ���� �� �ȴ�.
				

				/*
				������ size�� �����ϸ� �� �ǰڴ�. death�� 0�̰� name[0]�� �ְų�, death != 0 �̶�� �������� ����. �ƴ� ��쿡 ���α�. name[0] ���� �Ѽ�.
				*/







				 // c++ �����ε� string == "" ? �� ����� �ذ��� �� �ȴ�.. -1�̸� �߸��� �� // size 0�̿��� �߸��� ��.  numsize�ϸ� �׻� 0�� ������ ����.
				// �޸����� �� ��쿡�� \0 �� �����Ͽ� ���� X

				// ���� ó��. �޸����� �� ���(ios:: end == ios::beg ) read ++ �� �ؼ��� �� �ȴ�. ( read ++ �� 1�� �Ǹ� �޴� ���� 1���� ���۵ǰ� �̻��� ���� ��)


			} // ���Ŀ� �� ���� ���� ������ sort�� �Ѵ�. �ٽ� ����������� �ϱ� ������ close ó�� �Է½� : ���۵ǰ� i == j�� check =1 ���Ŀ� ������ ����. 
						// sort�� ��쿡�� read ++�� ���� ���� ���¶� ������ �ʴ´�. ���� clear ���Ŀ��� sort �� save�� �����.

		
		}
		 // read = 0; �̶�� ���ĵ� �� �ǰ� ���� �� �ǰ�... .0�� ���� ���̴�.
												// EOF �� clear ���� �Է¹ް� �d�d ó��. �� �������� EOF �״��. OK
		dataCheck = 1;
		in.close();
		// �ƹ��͵� ������ ó���� ���� �ʰ� close read = 0 sort ���嵵 ����. ���Ŀ��� �ϴ� üũ�� ������ �ٽ� �˻����� �ʴ´�.
	}
	
	
	// ���� ���� ��ŷ�� ������ ���� sort�Ŀ� ������ �ϰ� �ִ�.

	// ������, �� ������ ��ŷ�� ������ �ʴ��� ó���� �ؾ� �ϴ� �κ��̴�. �׷��� title���� ó���� �ϸ� ��� ? 

	// title���� ó���� �ϴ� ��� 1. ó�� ���� : ���������� setting sort ���� lanking �� ó��.  2. clear ���Ŀ� main�޴� + 1 �Ǿ����� sort ���� �� ó����.
	// ���࿡ �÷��� �߿� �������� ������ ��� ������ �����͸� �ٽ� ������ �ϰ� �����ϴ� �ǹ̾��� �� �ƴѰ��� ? ( ���� �´�. �ٵ� ��ǻ�ʹ� ������.. �׸��� ������ 100��...)


	// ������ ������ read == 0�̸�  �׳�.. �ڱ� �ڽ�... ���� �Էµ� ����. 

	sort(arr, arr + read, cmp);  // ������ �� ���Ŀ��� ������� �Ѵ�. cmp good

	ofstream txt("TMRank.txt"); // �⺻ �����̶� �� ����� ó����.  ������ �����, ������ �����

	 // ó���� ������ �� ��쿡 1�̶� out  ���࿡ �� ���� ���̶� �־��ٸ�, 0��°�� ��Ƽ� �ְ� 1�� ����Ű�� EOFŻ��. 
					// ��¥�� �ϳ��� ���̱⿡ ������ ���� ������, ���� ���� �ִٸ�


	for (int i = 0; i < read; i++) // read�� ������ ���ں��� 1ū��.�� ! ������ 0 �̰� �����̶� ++���ְ� ���⼭ �ѹ� ������ �ϴµ� name�� ����.. 
	{											// ��������� ����... ���� ó�� ���� �� read = 1 �� �����µ� �� ��츦 ���ƾ� �Ѵ�.


												// �׷��� ó���� ���� daeth 0 name���� �κ��� ������ ���°� ? 
		txt << arr[i]; // ���⼭ ������ �� �ǳ� . . .  ���� ���ڿ��� (�̰� �����ε� �� ���� �� ;;)  ���ڿ� ����. �̰��� ���Ͽ� ����. ���ʿ� name�� �̻��ϰ� ó����.

					   // ���ĵ� �����͸� �̸�.����\n�̸�.����\n ...���� �Է�. 
	}
	txt.close(); // �Ҹ��ڰ� �ֱ� ���ٵ� ���� �̰� ������ �Է��� ����� �� �ɱ�... �̸��� �� ������ ���ұ� ?
				 //�ƹ����� ��ȣ�� �̾߱⸦ �ϴ� ���� case������ ofstream�� �����߱� ������ �� ����. �Լ�ó�� �ϸ� �� �� �˾Ҵµ�  .. . ����.  ��! �Ḹ �׷���... ����ġ���� ������ �ǳ� ?
				 // �ƴ� std:: �Լ��� �̸��� ��ġ�� �ſ�������; ....
	

	//�̷��� title���� sort�� ����. ��ŷ�� ���� ��쿡 ���. �ݺ��ϴ� ó��. 


	while (_kbhit()) _getch(); // ����.

	while (true)
	{
		 Fmod->update();
		if (_kbhit())
		{
			char input = _getch();
			if (input == 224)
				input = _getch();// Ű���� �Է½� 2��° ������.

			// ���θ޴����� ����� �Ҹ� ������ �� �ֵ��� �Ѵ�.
			// ���⼭ ������ ���� ���� ���� �� ��� sound����.



			switch (input)  // ���� ��� �� ������ ���� ������ �� �̻� �� ������. �� �Ʒ����� �Ʒ��� ���� ��� �� �̻� �� ������. ��ġ�� �ľ��ϴ� ������ �ʿ�.
			{	
				// locate ���� �̿�, locate ������ ���� ������ ���. �׳� = �Լ��� �� �� ��� (��¥�� 5��... ó������) �ش� �κи� �ٽ����.
			case '=':
				//+�� ������ �����ϰ� ������.
				speaker += 0.1;
				if (speaker > 1)
					speaker = 1;
				TextColor(DEEP_YELLOW);
				gotoxy(MAP + 17, 31, "            "); // �ش� �ڸ� û��.
				gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
				channel1->setVolume(speaker);
				break;
			case '-':
				speaker -= 0.1;
				if (speaker < 0) // 0������ ����.
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
				switch (locate) // ������ ���� 4���� up -> 3���� �̵��� ���������� 3����ġ�� ���� ��. (���Ŀ� --ó��)
				{
				case 0: 
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 14, "  ��"); //�� �̻� �ö� ���� �����Ƿ� �ڽ��� �ڸ�.
					break;
				case 1:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 16, "  ��");
					gotoxy(MAP + 5, 14, "  ��");
					break;
				case 2:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 18, "  ��");
					gotoxy(MAP + 5, 16, "  ��");
					break;
				case 3:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 20, "  ��");
					gotoxy(MAP + 5, 18, "  ��");

					break;
				case 4:
					TextColor(DEEP_WHITE);
					//gotoxy(MAP + 5, 22, "  ��");
					gotoxy(MAP + 5, 20, "  ��");
					break;
				}
				locate--;
				if (locate == -1)
					locate = 1; // �� ��(0)���� ���� ���� ���. 

				//���� �̵�
				break;
			case DOWN:
			case 's':
			case 'S':
				titleIcon();
				switch (locate) // ������ ���� 0���� �Ʒ��� �̵� 1�� ��ġ�� ��� ���Ŀ� ++ ó��
				{
				case 0:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 16, "  ��");
					break;
				case 1:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 18, "  ��");
					break;
				case 2:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 20, "  ��");
					break;
				case 3:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 22, "  ��");

					break;
				case 4:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 22, "  ��"); // �� �̻� ������ ���� ��� �ڽ��� ��ġ.
					break;
				}
				locate++;
				if (locate == 5)
					locate = 4; // �� �Ʒ����� �Ʒ��� ����. 
				
				//�Ʒ��� �̵�// �� �Ʒ����� �Ʒ��� ���� ��� �ڸ� ����. ó�� OK �ٵ� �ʹ� ����.
				break;

			case SPACEBAR:
			case ENTER:
				 result = Fmod->playSound(select, 0, false, &channel2);
				 if (result != FMOD_OK) return;

				// select���� ä��2�� 1.0  0�ʹ̸��̶� release�� �ʿ� X
				switch (locate)
				{


				case 0: // ���� �ϱ� (�⺻ ���� ���)
					locate = 5;
					stage = 1; // 1���� ����.
					break;
					// ó��. 



				case 1: //Ʃ�丮�� (���ο� �� ���� �ֱ�) Ʃ�丮���̸� stage = 0
					locate = 5; 
					stage = 0; // ���Ŀ� Ʃ�丮�� ������������ Ŭ����� 1stage�� �̵�. Ÿ��Ʋ�� case -1:
					break;
					// �� ������ �Ѵ�.



				case 2: // �������� ���� (��й�ȣ)
					
					if (hansei == 0)
					{
					gotoxy_C(MAP + 5, 3, "Password : ",DEEP_WHITE);  // �����ʹ� �迭�� �����ϴ� ����, �迭�� ������ ������� �ʴ´�. 
																		// �迭�� ���� ������, �����ʹ� scnaf�� ���� �ٷ� ���� ���� ����. ���Ŀ� ���� ���� ���ڷ�
																		// �޾ƿ��� ��쿡�� ��밡�� . �迭�� �־ �����Ͱ� �ִ�.
					
																	// ó������ �����͸� �迭ó�� scanf�� �޾ƿ� �� ����.

					while (_kbhit()) _getch(); // ����. 

					
						scanf_s("%s", str, sizeof(str));
						if (compare(str, "hansei")) // �Ķ���Ͱ� ���°� �޶� ã���� �ָ��� .. . �� 20�� �� �� �����̴�...
						{
							hansei = 1;
							locate = 8; // ������ 8 Ʋ���� ���� ����. �ٽ� �����ϴ� �κ�. �ݺ�.  ����� �Է½ÿ� �������� �Լ�ó��.
						} // OK 

						gotoxy(MAP + 5, 3, "                               "); // �Է��� ��쿡�� û���ϸ� ����. free �н��� �� �ʿ䰡.. ( �� �ʿ�� ������ ������ �ʰ� �ϰ��־���)

					}
					else
						locate = 8; // �Է��� ���Ŀ��� �����н�


					//Ʋ���� ���� ���� ���� �̵�. -> �Լ�ó�� seletor �׳� �����ϰ� ���ڹ޾Ƽ� ó��. compare.
					// ������� 3�� clear
					// Ȥ�ó� �迭ĭ�� �Ѿ ���� Ÿ��Ʋ �κ��� ħ������ ��츦 ����Ͽ� �ٽ� ����ֱ�.  
					num = 0; // num = 0�� �ؾ� �� �� �κ��� ó����. ' '�� ��������.
					 // �� ����ȴ�. ���� �������� ó���� �ʿ�.
					break;




				case 3: //��ŷ ( ���� ������� ���� offline ��ŷ )
					system("cls");
					num = 0;
					// Ÿ��Ʋ�� ��µ� ���Ķ� ���� ������� �ʴ´�. ������ ����ص� ������ ������ �ᱹ �ƹ� �͵� ����� �� �Ǵ� �� ó�� ���� ���̴�. 
					titleSpace(); // ���� �� �� �߳� ? => �ƹ����� ��ºκп����� �¸������ ���������� �ʴ� ���� output���� ������ �� �ƴұ� ? �³�.. 
										// OutPut������ PlayerPos.x y�� ������ ��µ� �� ���� ���� ������ �� ���ָ� .. ..  ����� ���ư� ���� ����. ? �ƴ� �׷��� 0 ������...

					ranking(); //��ŷ ���ٰ� �ٽ� ���ͼ� loop������. ���Ŀ� ..
				
					system("cls"); // ���� title�� cls�� �ִ� �� �˾Ҵµ� ����. �� ����ǳ� �뷡�� �� ����.... release sing�� �θ� �Լ��� �־ ����.. �׳� ��°�� ���ֹ���.
					sing->release(); // ���� �뷡 ������. �Ʒ� �������� �ٷ� ���� �뷡 ������ �� �� �ٽ� ������. �׳��� ���� ����.
				
					title(); //�ٽ� Fmod ����� / �뷡 �����. �� ¥���� .. �뷡 ����°� �� �����̾�.. (������)  �ٵ� �׳� ������. ������. ������

					locate = 7;

					// �׳� ���� title�� �뷡�� release ���ָ� ���Ŀ� title���������� �ƶ� ó���� �ǰ� �ᱹ �뷡�� ..
					//�θ��� �뷡�� ��� ����.  . . .  �׷��ٰ� �ٸ� �� ���� ������ ������ ������ ... �ش� �κ����� ���� ������. GOOD 

					// ���� �����̰� �� ����ȴ�. ( �ٵ� ERROR�� ��� ������ �̻����� ... ) ����;;

					break;



				case 4:// ����
					exit(0);
					break;
				}

				while (_kbhit()) _getch(); // ����. enter���Ŀ� ���� ���� �ϴ� ����. ( �������� ������ �̻��ϰ� ��)

				//Ȯ��
				break;
			case ESC: // ����  Ȯ�ΰ� ����� �ٽ� ���� �ʿ䰡 ���� �κ�. Ȥ�� ���� ����Ʈ�� �ְ� �ʹٸ� �ʿ��ϱ� ��.
				exit(0); // Ȥ�ó� ESC ���� ��� �׳� ������ ó��. (�� �Ű� ����)
				break;
			
			}
			
		} // kbhit if
		switch (locate)
		{
		case 8: // hansei �Լ�ó���� ����. �ؼ� �θ��Լ��� ����.
			sing->release();

			stageSelect();
			return;
			// �������� return.
			case 5:
			
			case 7:
			sing->release();
			while (_kbhit()) _getch(); // ����.  �����.  �ٽ� Ÿ��Ʋ�� �̵��� ���� �ð����� ���� ���۸� �Դ´�. �ȳ�. 

			return;
		}// ���Ŀ� main -> set 1������������ . . .  // Ʃ�丮�� ����

		
	}//while true

	
}
void stageSelect()
{
	// stage ����Ʈ�� ���� �ѹ� �̿��� �� ��쿡�� ��ŷ ��Ͽ� �Ұ� ( �ֳĸ� ó������ �� ���ݾ�. )  1�� �̿� -> main���� �̵�.

	// �׸����� ����ϵ��� ���ٱ� ? ������
	system("cls");
	int locate = 1;
	char input = NULL;
	
	// �¿� �̵����� ó���ϰ� �ʹٸ�... ���ī�� ? 
	selectOpption();

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ��");

	while (_kbhit()) _getch();

	if (hansei == 2) hansei = 1; // �ٽ� �����ϴ� ���.
	while (true)
	{
		

		
		
		// ��� cls �� ���̶� ����� �ݺ�.
		Fmod->update();
		if (_kbhit())
		{
			char input = _getch();
			if (input == 224)
				input = _getch();// Ű���� �Է½� 2��° ������.

								 // ���θ޴����� ����� �Ҹ� ������ �� �ֵ��� �Ѵ�.
								 // ���⼭ ������ ���� ���� ���� �� ��� sound����.
			switch (input)
			{
			case ESC:
				exit(0);
				break; // ȭ��ǥ ���� �ٸ� ���� ���� ó��. m M backspace�� �Լ��� ������.
			}
			

			system("cls");
			selectOpption();
			selectPrint(input, locate); //ó���� �⺻������ ��� ���Ŀ� �Է� cls �⺻���� ������ ���, ȭ��ǥ. �׸��� ��. ���Ŀ� �ٽ� �Է��� �ϸ�. cls ������. ȭ��ǥ. ��. �ݺ�.
		
			if (hansei == 2) { // enter input or spacebar input 
				// �װ����� ������ �ϴ� �κ�. 
				SetMaze(&block9);
				while (_kbhit()) _getch();
				return; // SetMaze�ϰ� return. �ϴ� title�̴ϱ�. return return �Ǿ ��. ( ó�� �Է��� Ÿ��Ʋ �̶��) OK main ����. 
				// ó�� �Է��� Ÿ��Ʋ�� �ƴ϶�, ���߿� main while ���� title�� ��쿡��.( ��� setmaze��������)
			}
				// GOD C ++ �����Ͱ� �Է��� �þ�Դ� �� ���۷����� ������Ű��.
				
			num = 0; // ������ ������ ���� ������ ���� ���.
			stage = locate; // �������� ���� �ٸ��� ������ �κ��� �־.. �̷��� �ؾ��Ѵ�.

			switch (locate) // ��ġ�� ���� (map�� ����) �뷡�� �׸�. break ������ �浹��� ���ϴ�.
			{
			case 1:
				
				music->release();

				Fmod->createStream("stage1.mp3", FMOD_LOOP_NORMAL, 0, &music);
				Fmod->playSound(music, 0, false, &channel1); //���� ���� stage 1.

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
			OutPut(); // �� �����ֱ�. 



		}//while
	}
}
void selectPrint(char input, int& locate)
{
	switch (input)  // �Է¿� ���� ó�� �� �̵�
	{


	case UP:
	case 'w':
	case 'W':
		
		switch (locate) // ������ ���� 4���� up -> 3���� �̵��� ���������� 3����ġ�� ���� ��. (���Ŀ� --ó��)
		{
		case 1:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 14, "  ��"); //�� �̻� �ö� ���� �����Ƿ� �ڽ��� �ڸ�.
			break;
		case 2:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 16, "  ��");
			gotoxy(MAP + 5, 14, "  ��");
			break;
		case 3:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 18, "  ��");
			gotoxy(MAP + 5, 16, "  ��");
			break;
		case 4:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 20, "  ��");
			gotoxy(MAP + 5, 18, "  ��");

			break;
		case 5:
			TextColor(DEEP_WHITE);
			//gotoxy(MAP + 5, 22, "  ��");
			gotoxy(MAP + 5, 20, "  ��");
			break;
		case 6:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 22, "  ��");
			break;
		}
		locate--;
		if (locate == 0)
			locate = 1; // �� �� 1���� ���� ���� ���. 

						//���� �̵�
		break;
	case DOWN:
	case 's':
	case 'S':
		
		switch (locate)
		{
		case 1:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 16, "  ��");
			break;
		case 2:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 18, "  ��");
			break;
		case 3:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 20, "  ��");
			break;
		case 4:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 22, "  ��");

			break;
		case 5:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 24, "  ��");
			break;
		case 6:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 24, "  ��"); // ������ ���� ����.
			break;
		}
		locate++;
		if (locate == 7)
			locate = 6; // �� �Ʒ����� �Ʒ��� ����. 

						//�Ʒ��� �̵�// �� �Ʒ����� �Ʒ��� ���� ��� �ڸ� ����. ó�� OK �ٵ� �ʹ� ����.
		break;

	case SPACEBAR:
	case ENTER:
		stage = locate;
		hansei = 2; // ��¥�� 0�� �ƴϴϱ� else �� ó�� �Ǿ� �н� �Ǵ� ��� ���� �̿��ص� �ȴ�.
		
		// �ڼ��� ���� set Maze���� ó���� �ָ���.

		break;
	}

// cls ���Ŀ� �ٽ� ��� ����. �׸� ��. �͸����� �ϰ� �ٽ� ����� ��. �ʼ�. ���� cls �������� ȭ��ǥ �� ����..; ���� ���������Ŀ��� *p++; �� �� ������. �������� ? 

}
void selectOpption()
{
	TextColor(DEEP_WHITE);
	
	gotoxy(MAP + 5, 9, "������������������������������������������������������");


	gotoxy(MAP + 5, 11, "������������������������������������������������������");
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
	gotoxy(MAP + 5, 14, "  ��");

	TextColor(DEEP_GREEN);
	gotoxy(MAP + 5, 16, "  ��");

	TextColor(GRAY);
	gotoxy(MAP + 5, 18, "  ��");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 5, 20, "  ��");

	TextColor(DEEP_RED);
	gotoxy(MAP + 5, 22, "  ��");
}

void selectIcon()
{
	TextColor(DEEP_OC);
	gotoxy(MAP + 5, 14, "  ��"); 

	TextColor(DEEP_RED);
	gotoxy(MAP + 5, 16, "  ��");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 5, 18, "  ��");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 5, 20, "  ��");

	TextColor(GRAY);
	gotoxy(MAP + 5, 22, "  ��");


	TextColor(DEEP_YELLOW);

	gotoxy(MAP + 5, 24, "  ��");
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
	gotoxy(MAP + 17, 31, "            "); // �ش� �ڸ� û��.
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
	//�츮�� speaker���� �°� ����ֱ⸸ �ϸ� �ȴ�. (�̹� �Լ������� speaker ������ ó���ؼ� �Ҹ� ������)
}
void speakerMinus()
{
	speaker -= 0.1;
	if (speaker < 0) // 0������ ����.
		speaker = 0;
	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, "            ");
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);

}
void mute()
{
	switch (soundEffect) // 0�̸� Ű�� 1�̸� �ݰ�    ������ �Ҹ���� // ������ �Ҹ� ��� X
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
void playerPos() //���� ���� !
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
	gotoxy(MAP + 7, 22, "�� Death  : "); printf("%d", death);

	TextColor(DEEP_OC);

	if (switchOnCount < switchCount) // ���� ����ġ�� ��������,
	{
		gotoxy(MAP + 7, 26, "�� Switch : "); 
		printf("%d", switchOnCount);
	}
	else // door ���� ������ ���⼭ ó���Ѵ�.
	{
		gotoxy(MAP + 7, 26, "                  "); // clean
		gotoxy(MAP + 7, 26, "�� Door Open");
	}

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 7, 24, "�� STAGE  : "); printf("%d", stage);
}
void tutorial(char input,int* block9) // �� �¾�. ����Ʈ �κе� ������ �ؾ���.
{

	switch (quest)
	{

	case 0: //0. Backspace�� �������ƿ� 0�϶� �ѹ� ����ϰ� �ϱ�. ���Ŀ� �̼� �����ÿ� �Ѿ�ִ� ��.
		if (number == 0)
		{
			
			gotoxy_C(5, 3, "BackSpace�� �������ƿ�.", DEEP_WHITE); number = 1; //number =1 number�� ���ؼ� �ѹ��� ������ֵ��� ����.
			Sleep(600);
			gotoxy(5, 3, "                                                ");
			while (_kbhit()) _getch(); // ����. 
			number = 1;
		}
		if (input == BACKSPACE)
		{
			quest++;
			Maze[4][14] = '1';
			number = 0;
		}
	break;
	case 1://2. ȭ�쿡 ������ �׾��.
		if (number == 0)
		{
			
			gotoxy_C(5, 3, "ȭ�쿡 ������ �׾��. ȭ��� �ٰ������ô�. ", DEEP_WHITE);
			number = 1;
			Sleep(600);
			gotoxy(5, 3, "                                             ");
			while (_kbhit()) _getch(); // ����. 

		}
		// quest������ ���� q�� ���������� ����.  �״� ��찡 2������ �ִµ�, �ϳ��� backspace �ϳ��� arrow �ϳ��� ��ź(X) �齺���̽� or ȭ��� �״´�.
		// �Ƹ� ? ... ���� ��̸� �� ���ٲ���.. �ƴϸ�... �׳� �Ѿ�� ����.. (��� ū ������ ����. ) �׷� �Ѿ��.
		if (death >= 1) {  // ����� �׾��ٸ� startPos �� Ǫ�ú��� �̿��� �� �ִ�.
			quest++;
			Maze[7][14] = '1'; number = 0;
			Maze[4][14] = '6';
		}
		break;
	case 2:

		//3. �����̽��ٸ� ���� ��ź�� ��ġ
		if (number == 0)
		{
		
			number = 1;
			gotoxy_C(5, 3, "�� �� ��Ʈ�� ���ô�. ��ź��ġ : space ��Ʈ���� : q", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                                   ");
			while (_kbhit()) _getch(); // ����. 

		}
		if (Maze[9][14] == '1') {
			quest++; number = 0;
			}
		break;
	case 3:
		if (number == 0)
		{
			// ��ǻ� ȭ�� ���ֱ� + �� �б�.
			number = 1;
			gotoxy_C(5, 3, "ȭ���� ������ ���ֿ�. ", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                           ");
			while (_kbhit()) _getch(); // ����. 

		}
		if (tPlayerPos.y == 12 && tPlayerPos.x == 14) { quest++; 
		Maze[13][14] = '1'; number = 0;
		}
		break;
	case 4:
		if (number == 0)
		{
			 //���б� + ����ġ on
			number = 1;
			gotoxy_C(5, 3, "���� ����ġ�� ��������. ", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                         ");
			while (_kbhit()) _getch(); // ����. 

		}
		if (switchCount == switchOnCount) {
			quest++; Maze[16][14] = '1'; number = 0;
		}
		break;
	case 5:
		if (number == 0)
		{
			
			number = 1;
			gotoxy_C(5, 3, "�� ���� ��������.", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                           ");
			while (_kbhit()) _getch(); // ����. 

		}
		if (tPlayerPos.x == 14 && tPlayerPos.y == 21) {
			Maze[22][14] = '1'; quest++; number = 0;
		}
		break;
	case 6:
		if (number == 0)
		{
			
			number = 1;
			gotoxy_C(5, 3, "�� �� ������.", DEEP_WHITE);
			Sleep(600);
			gotoxy(5, 3, "                                     ");
			while (_kbhit()) _getch(); // ����. 

		}
		if (tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y) // ���� �� �κ���, move���� ���� ó���� ���ְ� �ִ�. ���� move�ʿ��� quest = 6�� ��� ó�� X
		{ 
			number = 0;
			gotoxy_C(5, 3, "Tutorial�� �������ϴ�. 1 Stage�� �̵��մϴ�.", DEEP_WHITE);
			_getch();
			quest = 0; // Ȥ�ó� �ٽ� �� ���� �ִ�. 
			death = 0;
			stage++;
			SetMaze(block9);
			OutPut();
			//���Ŀ� stage = 1�̹Ƿ� tutorial�� ����� ��.
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
		gotoxy(14, 7, "�� ���� ���� ��"); //���������� -1�̶� �����ϰ� ó���� ���̾�.

		int j = 9;
		for (int i = 0; i < 10; i++) // TOP 10
		{
			if (i < 3) TextColor(DEEP_JAJU);
			else if (i < 6) TextColor(DEEP_OC);
			else TextColor(DEEP_YELLOW);
			gotoxy(14, j, "");
			//string name = arr[i].name; // �� �� ���� ���Ⱑ �� ���������� �𸣰ڴ�.  c_str�� �غ���.

			if (arr[i].death != 0 || arr[i].name[0]) // ������ 0�� �ƴϸ� ����ε� ����.  OR 0������, �̸��� �ִ� ���.
				// ������ 1 ����.   ������ 0 "����������" ����. 
				printf("%2d . %s Death : %d", i + 1, arr[i].name.c_str(), arr[i].death);
			else if (arr[i].death == 0) //����Ʈ     -1 ( �⺻�迭�� ) or name ��null �̸� --- �� �ܿ��� ����� . �Է� X ��.
				// �� �̿� ���� = 0 out 
				printf("%2d . ������", i + 1);
			
			
			//cout << name << " Death : " << arr[i].death;
			
			// cout�� ������� �ʰ� printf�� �̿��ϸ� string ������ �޾Ƴ��� ���Ѵ�.
				// �� �������� ���� ���ڸ� �������� �ؼ� ������ ����� �� ����. �����ڸ� ����������. (�ƴ�, �̰� ������ �ƴϳ� ? �׳� gotoxy...)
			//����������
			j += 2;
		}

		

		if (_kbhit())
		{ 
			music->release();
			break;
		} //�Է��� ������ out
	}


	while (_kbhit()) _getch(); // ����. 
}
