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
	int death; // �÷��̶��� ��� ������� �ҷ��� �� ���� �� / ����

	Rank(const string& name, int death) : name(name), death(death) {}



	friend ofstream& operator<<(ofstream& a, Rank& b);

	friend bool cmp(const Rank& a, const Rank& b);
	
	Rank() { } // �⺻ ������ 100 �׷�
};

ofstream& operator<<(ofstream& a, Rank& b) // ���Ͽ� �Է�
{
	a << b.name.c_str() << "." << b.death << "\n"; // �̸��� ���Ͽ� ���  �̸�.death       
	return a;
}
bool cmp(const Rank& a, const Rank& b)
{
	if (a.death != b.death) return a.death < b.death;

	//������ ���� ������.
	return a.name < b.name;
}

Rank arr[100];
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
#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define SPACEBAR 32

FMOD::System	*Fmod(nullptr);
// FMOD
FMOD::Sound		*music(nullptr); // ������������ ����� ������  createSound�� ������ ���� ��� ����(ȿ���� ó��) createStream�� �ϳ��� ��� ���� ( ������� )
FMOD::Sound		*jump(nullptr);	// ȿ�����̶�� �Ͽ��� ���������� �����ϸ� �Ҹ��� ����.
FMOD::Sound		*button(nullptr);



FMOD::Channel	*channel1(nullptr); // ä�� 1�� �뷡�� Ʋ� �Ҹ��� �������ش�.
FMOD::Channel	*channel2(nullptr);// ä�� 2�� ȿ�������� �Ҹ��� �������� �ʴ´�.
FMOD::Channel	*channel3(nullptr);
FMOD::Channel	*channel4(nullptr); // ȿ���� ä��2 �л�.

FMOD_RESULT		result;


void			*extradriverdata(nullptr);

int arrowSpeedSet = 2; // ��ǻ�͸��� �ӵ��� ���̰� ���� ��찡 �ִ�. �� ��쿡 ������ �ؼ��ϱ� ���� arrowSpeed�� �����Ѵ�. 2 -> �⺻ 1������ 3 ������ 

float speaker = 0.5f;
int soundEffect = 1;// ȿ���� on off
int dataCheck; // data�� �޾Ҵ°� �޾����� 1
int create; // ������ ����.
int hansei; // hansei�� ���ϸ� 1 ( ���� ������ �ٽ� ���� ���� )

int locate; // select���� ���.
int quest; // tutorial ����
int death; // death count
int num;
int stage;
int number; // Ƚ��. Ʃ�丮�󿡼� ���.
long arrowSpeed; // arrow speed�� stage�� �����ϵ��� �Ѵ�. ����� sleep�� �μ��� ������ ũ�Ⱑ ũ�� ������
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
POINT tPlayerPos;


char	Maze[30][31];

POINT tBombPos[5];

int iBombCount;

// Maze , boom ����ü , count;


enum COLOR {
	BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY
	, DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW,
	DEEP_WHITE
};


void SetMaze()
{
	if (quest >= 1) return; // quest �������� �� ���� �ٽ� �� ����.

	Fmod->update();


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


	*/

	
	
	
	num = 0;
	arrowCount = 0;
	switchCount = 0;
	switchOnCount = 0;
	block9 = 0; // ������ ó�� ���� ������ �ʾҴٴ� ǥ��.

	// death�� �ǵ帮�� �ʴ´�. (1~6 stage���� ���� ��ŷ�� ��� ��) death 0�� Ʃ�丮�� or title���� �� ó����.
	
	if(locate == 0) // ���� ���� �ƴ� ��
	system("cls");


	while (_kbhit()) _getch(); 

	
	 // �������� ���ø��� num = 0�� ���ؼ� ����ġ, arrow ���� ������ �ٽ� �����ش�.  endPos �� startPos �� stage ���� ����.
	switch(stage)
	{
	case -1:
		title(); // Ÿ��Ʋ ������ Ÿ��Ʋ���� ó���ϰ� �ִ�. title() SetMaze() �� stage = -1; SetMaze() SetMaze()
		break;
	case 0: // tutorial ���� ���ΰ� tutorial ���ǿ� �°� open �� ó��.
		

		

		music->release();

		Fmod->createStream(".\\sounds\\tutorial.mp3", FMOD_LOOP_NORMAL, 0, &music);
		Fmod->playSound(music, 0, false, &channel1);

		tPlayerPos.x = 14;
		tPlayerPos.y = 2;
		
		arrowSpeed = 5; // Ʃ�丮���� �׷� �� ����. �׳� �ӵ����

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

		
		break; // Ʃ�丮�� 

	case 1:
		speedShow();
		
		// ���ǵ� �����ֱ�

		music->release();

		Fmod->createStream(".\\sounds\\stage1.mp3", FMOD_LOOP_NORMAL, 0, &music);
			Fmod->playSound(music, 0, false, &channel1); //���� ���� stage 1.

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

		titleSpace(); // Ÿ��Ʋ �׸����� ���� �� �� ���.

		gotoxy_C(MAP + 5, 8,  "�� Thank You for Playing ", DEEP_YELLOW);  Sleep(1000);

		gotoxy_C(MAP + 5, 10, "�� Creator : Yumin Kim   ", DEEP_JAJU);  Sleep(1000); 

		gotoxy_C(MAP + 5, 12, "�� �Ѽ����̹����Ȱ���б�", DEEP_OC);  Sleep(1000);
	
		gotoxy_C(MAP + 5, 14, "�� blog.naver.com/lmj3322", DEEP_GREEN);   Sleep(1000);
		
		gotoxy_C(MAP + 5, 16, "�� PassWord : hansei",DEEP_WHITE);  Sleep(1000);


		titlePicture();

		

		OutPut();

		while (_kbhit()) _getch(); // ����. 

		gotoxy_C(14, 18, "What's Your Name ? : ",DEEP_WHITE);  

		string name;
		getline(cin, name); // �������� cin

		arr[read] = { name, death }; // �Է¹��� �����͸� ������ Rank ������ ����.
		read++; // read�� �׻� ������ ���� +1�� ����Ű�� �־�� sort�� �� �򰥸��� ����. 
				// �ٽ� ��ŷ ���� ���� �ߺ��� ��ҿ� ������ �� �Ǵ� �̰� �´�.
		// �Է� ��.
		//���Ŀ� main�޴��� �̵��ϰ� �ؾ� �Ѵ�.
		
		
		//���� ��� ����
		music->release();
		// ���� ��� ����.
		Fmod->release(); 
		stage = -1;
		SetMaze(); // �� �� ���Ŀ� ouput loop�� ���ư�.

		// �Է¹��� ������ setting				 
		SetMaze(); 
		OutPut();
		return;

	}
	if (locate > 0) return; // �հ��� �� ��쿡  ���̻� ��� �� ��.

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "�� Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "�� button :  + - ");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);

	channel1->setVolume(speaker);

	TextColor(DEEP_WHITE);

	if(stage != 0) // main�޴������� 'm'�� ���Ƶ�.(������ ���� �߻�) �׷��� �������� �ʵ���
	gotoxy(MAP + 7, 6, "M : ���θ޴�");

	gotoxy(MAP + 7, 8, "T : ȿ���� OFF");

	gotoxy(MAP + 7, 10, "SPACE : ��ź ��ġ");

	gotoxy(MAP + 7, 12, "Q : ��ź ��Ʈ����");

	gotoxy(MAP + 7, 14, "W : ��   S : �Ʒ�");

	gotoxy(MAP + 7, 16, "A : ���� D : ������ ");

	gotoxy(MAP + 7, 18, "Backspcae �� : �����");

	gotoxy(MAP + 7, 20, "ESC : ���� ");

	speedSetting(); // Ÿ��Ʋ������ �������� �ٲ�ΰ� ���� Setmaze���� �ӵ��� �������ش�. ( Ÿ��Ʋ�� Ʃ�丮�󿡼��� �������� ���� ) 7 �������� ( end ) �� �ش���� ����.
					// ���Ŀ� �Է��� ���� ���� ��ü.

		
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

				

			}
			
			Fmod->update();
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

	Fmod->createSound(".\\sounds\\bomb.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel3);
	channel3->setVolume(speaker);
	Sleep(50); 
	// ��ź��ġ�� ��ġ�� �ʰų� �ִ���� ���� ���� ��쿡�� ���� count�� �µ��� �÷��̾� ��ġ ���� 
	tBombPos[iBombCount] = tPlayerPos;
	++iBombCount;
	Maze[tPlayerPos.y][tPlayerPos.x] = '4';
	Fmod->update();
	

}


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
			Maze[tPlayerPos.y - 2][tPlayerPos.x] = '6'; Maze[tPlayerPos.y - 1][tPlayerPos.x] = '1';
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
		

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel3);

	}
	//// push block 
	


	if (pushBlock == 1)
	{// Ǫ�ú��� �̵�.
		FMOD::Sound		*push(nullptr);

		Fmod->createSound(".\\sounds\\push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel3);

	}

	// �����̴� ���� ������� ����. jump.mp3

	 Fmod->createSound(".\\sounds\\jump.mp3", FMOD_DEFAULT, 0, &jump);// 1���ڸ�
	 if (soundEffect)
	 Fmod->playSound(jump, 0, false, &channel3);
	--tPlayerPos.y;
	channel3->setVolume(speaker); // Play ���Ŀ� �� �ؾ� �����.

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
			//�÷��̾� - Ǫ�� �� - ���� ���
			return;
		}
		break;
	}

	//����ġ ��� �κ� 

	

	if (sw == 1)
	{// ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 



	if (pushBlock == 1) // Ǫ�ú��� �̵�.
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
	////����ġ ��� �κ� 

	
	
	if (sw == 1) // ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.
	{

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	
	

	if (pushBlock == 1) // Ǫ�ú��� �̵�.
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
	channel4->setVolume(speaker); // push, ����ġ�� 1 �߿��� 0.5
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

	

	if (sw == 1) // ����ġ�� on �̷� ��쿡�� ����� �� ���� ��ٸ�����.
	{

		Fmod->createSound(".\\sounds\\switch.mp3", FMOD_DEFAULT, 0, &button);
		if (soundEffect)
		Fmod->playSound(button, 0, false, &channel4);
	}

	//// push block 
	


	if (pushBlock == 1) // Ǫ�ú��� �̵�.
	{
		FMOD::Sound		*push(nullptr);

		Fmod->createSound(".\\sounds\\push.mp3", FMOD_DEFAULT, 0, &push);
		if (soundEffect)
		Fmod->playSound(push, 0, false, &channel4);
	}

	 // push, ����ġ�� 1 �߿��� 0.5

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

	if (iBombCount == 0) return; // ��ġ�� �� �� ��� �׳� ����. (�Ҹ���� X)

	FMOD::Sound		*bomb(nullptr);

	Fmod->createSound(".\\sounds\\explosion.mp3", FMOD_DEFAULT, 0, &bomb);
	if (soundEffect)
	Fmod->playSound(bomb, 0, false, &channel4);
	channel4->setVolume(speaker);
	
	int block = 0; // �� �����°� ? 
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
			Fmod->createSound(".\\sounds\\die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);// �Ҹ�1 
			
			channel2->setVolume(speaker);

			death++;

			tPlayerPos = tStartPos;  
		}
	
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

		// �÷��̾� ��ġ�� ȭ����ġ�� ��ĥ �� ��ġ�ϸ� �� �ȴ�. @@@@ (ȭ���� �����)

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
		Fmod->createSound(".\\sounds\\cookie.mp3", FMOD_DEFAULT, 0, &cookie);
		if (soundEffect)
		Fmod->playSound(cookie, 0, 0, &channel3); // �ٻ�ٻ� �Ҹ� 1

		channel3->setVolume(speaker);
	}


	Fmod->update();

	iBombCount = 0; //��Ʈ�� ��
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

	if (Maze[tPlayerPos.y + 1][tPlayerPos.x] == '9' || // �÷��̾� ��ġ�� �¿���ϰ� ��� ���̸� sleep
		Maze[tPlayerPos.y - 1][tPlayerPos.x] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x + 1] == '9' ||
		Maze[tPlayerPos.y][tPlayerPos.x - 1] == '9')
		Sleep(150);


	if (tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y) // �׷��ϱ� �츮�� stage clear ������ ��ǻ� ������ ���� �ν��ϸ� �ȴ�. 
	{
		if (quest == 6) return; // �ȳ����� ������ �ؼ� ó�� �� �ϰ� ����. (���ʿ��� ó��)

		// �Ѽ� == 2�� �� �츮�� Ÿ��Ʋ�� ������ �Ѵ�.

		if (hansei == 2) 
		{
			music->release();
			while (_kbhit()) _getch(); 

			system("cls");
			title(); 
			SetMaze(); 
			OutPut(); //arrow move ���� ó��.

			return;
		}
		//������ ��찡 �ƴ�.

		FMOD::Sound *clear(nullptr);
		Fmod->createSound(".\\sounds\\clear.mp3", FMOD_DEFAULT, 0, &clear);
		if (soundEffect)
		Fmod->playSound(clear, 0, 0, &channel2);
		channel2->setVolume(speaker);
		stagePlus(); // stage clear
		SetMaze();
		OutPut(); // ����ġ ���� �� ��ġ�� ������ ������ ���� ����.
	}
	Fmod->update();

}

void arrowMove() 
{
	Fmod->update();


	for (int i = 0; i < arrowCount; i++)
	{
		// �ϴ� ������ �ȴٴ� ���� �ϳ��� �ִ�. �� �ڸ��� ��� ����� �ű��� �ڸ��� �°� ����


		if (Maze[arrowPos[i].y][arrowPos[i].x] == '6') // movePlayer���� �ϴ� �÷��̾� Ǫ�� ȭ���� �� ȭ���� ��ǥ�� 6�� �����Ѵ�. ���� arrowmove �̰����� ����.
		{														// ȭ��� ���� ��ģ �� �ڸ��� 1�� �����Ͽ� ��� ����� ȭ���� ���� ��ǥ�� �д�.	== �츮�� ������ �� �ִ�.
			Maze[arrowPos[i].y][arrowPos[i].x] = '1';
			arrowPos[i].x = 0; arrowPos[i].y = 0;
			continue; 
		}

		if (Maze[arrowPos[i].y][arrowPos[i].x] == '0') continue; // ���� ȭ�쿡 ���� ó��. 0.0�� ���� ��ǥ. (���� X)

		
		// ���� ȭ�쿡 0,0�� �����ϵ���.

		// ȭ���� ��ź�� ��ġ�� ��ź�� ������, 1�� ������ �ʰ� ȭ���� �� �ڸ��� ��� ����. ( ��¶�� ���� ������ ���������� ó���ȴ�)

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
		{						

			//�״� �Ҹ�
			FMOD::Sound *die(nullptr);
			Fmod->createSound(".\\sounds\\die.mp3", FMOD_DEFAULT, 0, &die);
			if (soundEffect)
			Fmod->playSound(die, 0, 0, &channel2);
			channel2->setVolume(speaker);
			Fmod->update();
			death++;

			tPlayerPos = tStartPos;
		}

		Sleep(arrowSpeed); //  stage�� �ӵ�.
	}
}
void stagePlus()
{
	stage++;
}

void openDoor() 
{
	Fmod->update();

	if (block9 == 1) return; // �ѹ��� ����ϰ� ���Ŀ� return; stage�� �ٲ�� block9 = 0�� ������ �ؾ��Ѵ�.

	if (switchOnCount == switchCount) // on������ ��ü ������ ������ (��� ����ġ on)
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


void title() // Ÿ��Ʋ�� stage = -1���� �׸� �׸��� ���� �����̴�.
{
	for (int i = 0; i < arrowCount; i++)
		arrowPos[i] = { 0,0 };
	// Ÿ��Ʋ�� ���� ��� ������ �����ִ� arrow�� ��ǥ�� ���� ��ǥ ó���ؾ� �Ѵ�.


	while (_kbhit()) _getch(); 

	
	result = FMOD::System_Create(&Fmod);


	FMOD::Sound		*sing(nullptr);

	FMOD::Sound		*select(nullptr);

	 result = Fmod->init(25, FMOD_INIT_NORMAL, extradriverdata);
	 if (result != FMOD_OK) return;

	// ä���� 25�� ���� / �Ϲ��� / �� ���� ������

	 result = Fmod->createStream(".\\sounds\\title.mp3", FMOD_LOOP_NORMAL, 0, &sing);
	 if (result != FMOD_OK) return;

	
	//���ȭ�鿡���� ��� ��� sing 
	// Stream or Sound
	 result = Fmod->createSound(".\\sounds\\select.mp3", FMOD_DEFAULT, 0, &select);
	 if (result != FMOD_OK) return;

	//select effect


	 result = Fmod->playSound(sing, 0, false, &channel1);
	 if (result != FMOD_OK) return;

	
	channel1->setVolume(speaker); // �⺻ 0.5
	
	
	titlePicture();
	block9 = 1;
	num = 0;
	stage = -1;

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 9, "������������������������������������������������������");
	

	gotoxy(MAP + 5, 11, "������������������������������������������������������");

	TextColor(DEEP_OC);
	gotoxy(MAP + 11, 31, "�� Sound  :");

	TextColor(DEEP_JAJU);
	gotoxy(MAP + 11, 32, "�� button :  + - ");

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 10, 2, "    button : 1 ~ 3");
	gotoxy(MAP + 10, 1, "    Arrow Speed : "); TextColor(GRAY); printf("%d", arrowSpeedSet);


	TextColor(GRAY);
	gotoxy(MAP + 10, 1, "��");
	gotoxy(MAP + 10, 2, "��");

	TextColor(DEEP_YELLOW);
	gotoxy(MAP + 17, 31, ""); printf(" %.1lf", speaker);
	// �ʱ� 0.5

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
	
	TextColor(DEEP_YELLOW);
	gotoxy(1, 32, "��");
	TextColor(DEEP_WHITE);
	gotoxy(3 , 32, "References : http://assortrock.com (Maze Game) ");
	

	int locate = 0; // ���� ������ ��������, ��� ��ġ ������. // ���⼭ ����ϴ� locate�� select���� ����ϴ� locate�� �ٸ���.
	char str[40];// ��й�ȣ�� �������� ����� �迭. �ʹ� �����ϰ� �ָ� ���ڸ� ���� �Է��� �� ��̾�����.

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ��"); 

	// ��ǻ� ó������ �Է��� ���ٸ� ���� ���� �����Ƿ� �̸� ��Ƶ־� �Ѵ�.

	//�츮�� title���� while������ Ű���� �Է��� �޾� �� ó���� �������̽��� ó��.
	// ���Ŀ� ���� �� ���� ó�� ..
	OutPut();


	// read ( ���� �����Ͱ� �ִ��� Ȯ���մϴ�. ) 

	ifstream in("TMRank.txt");
	string name;
	int death;
	int con = 0;

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


				if(death == 0 && name[0] || death != 0) 
				read++; //  read�� ���� ���Ҹ� (�� �ڸ���) ����Ų��. ���� ���ο� ���� �־��� �� arr[read]�ϸ� �ȴ�.
				// ��ϵ� �����Ͱ� �̸��� ���� ���� ���� �� �ȴ�. name[0] �ʱⰪ���� �ƴ��� �˻�. 
				

				/*
				death�� 0�̰� name[0]�� �ְų�, death != 0 �̶�� �������� ����. �ƴ� ��쿡 ���α�.
				*/

				 // c++ �����ε� string == "" ? �� ����� �ذ��� �� �ȴ�..  // size 0�̿��� �߸��� �� => ó��X.  numsize�ϸ� �׻� 0�� ������ ����.
				

				// ���� ó��. �޸����� �� ���(ios:: end == ios::beg ) read ++ �� �ؼ��� �� �ȴ�. ( read ++ �� 1�� �Ǹ� �޴� ���� 1���� ���۵ǰ� �̻��� ���� ��)
			} 
		
		}
		
		dataCheck = 1;
		in.close();// ���Ŀ� �� ���� ���� ������ sort�� �Ѵ�. �ٽ� ����������� �ϱ� ������ close
		// �ƹ��͵� ������ �߸��� �������� �ν��ϰ� �Է¹����� read++�� �� �Ѵ�. ���� �� �κ��� ���Ŀ��� ���ܴ�. end : (arr+ read)
	}
	
	
	// ���� ���� ��ŷ�� ������ ���� sort�Ŀ� ������ �ϰ� �ִ�.


	sort(arr, arr + read, cmp); // ���� ���� 1. death  2. ������

	ofstream txt("TMRank.txt"); // �⺻ �����̶� �� ����� ó����.  ������ �����, ������ �����


	for (int i = 0; i < read; i++) 
	{										
		txt << arr[i]; // ��� �����ε� ����.

					   // ���ĵ� �����͸� �̸�.����\n�̸�.����\n ...���� �Է�. 
	}
	txt.close(); // ���� ���Ŀ� �ݱ�.

	//�̷��� title���� sort�� ����. ��ŷ�� ���� ��쿡 ���. �ݺ��ϴ� ó��. 

	while (_kbhit()) _getch(); // ����.

	while (true)
	{
		 Fmod->update();
		if (_kbhit())
		{
			char input = _getch();
										
			while(_kbhit()) _getch();


			// ���θ޴����� ����� �Ҹ� ������ �� �ֵ��� �Ѵ�.
			// ���⼭ ������ ���� ���� ���� �� ��� soundVolume�� ����. 


			switch (input)  
			{	
				// �ӵ� ���� OR �ʹ� ���� ? ���� 

			case '1':
			case '2':
			case '3':
				arrowSpeedSet = input - '0';
				speedShow(); // ���⼭�� Ÿ��Ʋ�̶� �ӵ� ������ ���������� ���� �ʿ䰡 ����. �׳� set���� �޾Ƶα�.

				break; //  Ÿ��Ʋ������ ������ �� �ְ� ���ӿ�����.

				// locate ���� �̿�, locate ������ ���� ������ ���. �̸����,  �ش� �κи� �ٽ����.
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
					gotoxy(MAP + 5, 14, "  ��");
					break;
				case 2:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 16, "  ��");
					break;
				case 3:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 18, "  ��");

					break;
				case 4:
					TextColor(DEEP_WHITE);
					gotoxy(MAP + 5, 20, "  ��");
					break;
				}
				locate--;
				if (locate == -1)
					locate = 0; // �� ��(0)���� ���� ���� ���. 

				//���� �̵�
				break;
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
				
				break;

			case SPACEBAR:
			case ENTER:
				 result = Fmod->playSound(select, 0, false, &channel2);
				 if (result != FMOD_OK) return;

				// select���� ä��2�� 1.0  0�ʹ̸��̶� release�� �ʿ� X
				switch (locate)
				{

					// title������ setting�� �ϰ� ���Ŀ� Setmazeó��.

				case 0: // ���� �ϱ� (�⺻ ���� ���)
					locate = 5;
					stage = 1; 
					break;



				case 1: //Ʃ�丮�� 
					locate = 5; 
					stage = 0; // ���Ŀ� Ʃ�丮�� ������������ Ŭ����� 1stage�� �̵�. Ÿ��Ʋ�� case -1:
					break;
					// �� ������ �Ѵ�.



				case 2: // �������� ���� (��й�ȣ)
					
					if (hansei == 0)
					{
					gotoxy_C(MAP + 5, 3, "Password : ",DEEP_WHITE);  


					while (_kbhit()) _getch(); // ����. 

					
						scanf_s("%s", str, sizeof(str));
						if (compare(str, "hansei")) 
						{
							hansei = 1;
							locate = 8; // ������ 8 Ʋ���� ���� ����. �ٽ� ����(title)�ϴ� �κ�. �ݺ�.  ����� �Է½ÿ� �������� �Լ�ó��.
						} 

						gotoxy(MAP + 5, 3, "                               "); // �Է��� ��쿡�� û���ϸ� ����.

					}
					else
						locate = 8; // �Է��� ���Ŀ��� �����н�

					num = 0; // num = 0�� �ؾ� �� �� �κ��� ó����. ' '�� ��������. ( �� ó������ ��� �Ŵ� �̷� ���� ) stage select �κ�.
					
					break;




				case 3: //��ŷ ( ���� ������� ���� offline ��ŷ )
					system("cls");
					num = 0;

					titleSpace(); 

					locate = 7;

					break;


					
				case 4:// ����
					exit(0);
					break;
				}

				while (_kbhit()) _getch(); // ����. enter���Ŀ� ���� ���� �ϴ� ����. ( �������� ������ �̻��ϰ� ��)
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
			

			stageSelect(); // stage ������ �̰�����.

		 // �������� ���� ���Ŀ� SetMaze 
			return;
















		case 7: // ��ŷ.
			sing->release();

			ranking(); //��ŷ ���ٰ� �ٽ� ���ͼ� loop������. ���Ŀ� ..

			system("cls");
			con = 1;
			stage = -1;
			SetMaze(); // Setmaze(��ŷ���� title) -> stage ���� ���Ŀ� -> setMaze loop ( �ٵ� �� Ʃ�丮��� ? )

			return;







			case 5:
				sing->release(); // �뷡���� ������ ���·� Setmaze -> loop
			
			while (_kbhit()) _getch(); // ����. 

			return;
		}// ���Ŀ� main -> set 1������������ . . .  // Ʃ�丮�� ����

		if (con == 1) return; // ���� return���� ó���� ���� ����. 
	}//while true

	
}
void ranking()
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

		if (arr[i].death != 0 || arr[i].name[0]) // ������ 0�� �ƴϸ� ����ε� ����.  OR 0������, �̸��� �ִ� ���.
												 // ������ 1 ����.   ������ 0 "����������" ����. 
			printf("%2d . %s Death : %d", i + 1, arr[i].name.c_str(), arr[i].death);
		else if (arr[i].death == 0)  // ������ �߿� death = 0 �� ���� �Է��� ���.
			printf("%2d . ������", i + 1);

		j += 2;
	}

	while (true)
	{

		if (_kbhit()) // if���� �ִٸ� �Է��ִ� ���� Ȯ���ϰ� �׳� ��������.
		{
			music->release();
			return;
		} //�Է��� ������ out
	}


	while (_kbhit()) _getch(); // ����. 
}

void stageSelect()
{
	// stage ����Ʈ�� ���� �ѹ� �̿��� �� ��쿡�� ��ŷ ��Ͽ� �Ұ� ( �ֳĸ� ó������ �� ���ݾ�. )  1�� �̿� -> main���� �̵�.

	system("cls");
	char input = NULL;
	Fmod->update();
	locate = 1; // ó���� enter�� ������ stage = locate �κп��� 0�� ���ԵǾ� ������ Ʃ�丮��� �̵��Ѵ�. 

	selectOpption(); // �ɼǿ��� ���� ������ ������ ���.
	

	TextColor(DEEP_WHITE);
	gotoxy(MAP + 5, 14, "  ��");

	if (hansei == 2) hansei = 1; // �Ѽ� = 2�� ���� select�� �������� ���. -> ���Ŀ� ���� Ŭ������ �� �Ѽ� = 2�� �������� ����ä��, ���� main���� ó��.
	while (true)
	{
		Fmod->update();
		
		if (_kbhit())
		{
			
			char input = _getch();
		     // ����Ű ���� ���� . 
		   
			while (_kbhit()) _getch(); // ����

			switch (input)
			{
			case ESC:
				exit(0);
				break; // ȭ��ǥ ���� �ٸ� ���� ���� ó��. select ���������� m M BackSpace ���� �� �� ������ ��.
			}
			
			selectOpption();
			selectPrint(input); //ȭ��ǥ Enter Space � ���� ó��. stage ����. 


			if(locate != 0) 
			stage = locate; // ���� �� ���Ŀ��� locate(0) �� �����ϸ� �� �ȴ�.


			if (hansei == 2) { // enter input or spacebar input 

							   // ������ ������ title ��������.  => ������ ���� �ٽ� SET MAZE 

				while (_kbhit()) _getch();

				return;
			}

			SetMaze(); // ��ü ���� cls�� ���ƾ� �ɼ��� ���. locate == 0

		
			// NOT enter NOT SPACEBAR stage ( �Է°��� �������� ) �ʰ� �׸�.

			// �߰��� �ʱ갪 �Է¸� �ϰ� �������� ����. locate > 0  select �ϰ� ����� ���� �ٽ� �ߴܵ��� �ʵ��� locate = 0;

			OutPut(); // �� �����ֱ�.  ���⼭ ȭ���� ��ǥ�� �����Ѵ�.

		

			


		} // kbhit
	}//while
}
void selectPrint(char input)
{
	switch (input)  // �Է¿� ���� ó�� �� �̵� 
	{


	case 'w':
	case 'W':
		
		switch (locate)
		{
		case 1:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 14, "  ��"); //�� �̻� �ö� ���� �����Ƿ� �ڽ��� �ڸ�.
			break;
		case 2:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 14, "  ��");
			break;
		case 3:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 16, "  ��");
			break;
		case 4:
			TextColor(DEEP_WHITE);
			gotoxy(MAP + 5, 18, "  ��");

			break;
		case 5:
			TextColor(DEEP_WHITE);
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

		break;

	case SPACEBAR:
	case ENTER:
		stage = locate;
		hansei = 2; // ����Ʈ ������.
		locate = 0; // ���� ���Ŀ��� 0����.  ���Ŀ� ��µ� SetMaze���� ������ ����.
		
		break;
	}

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
	gotoxy(MAP + 5, 10, "         Stage Choice");
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
void tutorial(char input) 
{

	switch (quest)
	{

	case 0: //0. Backspace�� �������ƿ� 0�϶� �ѹ� ����ϰ� �ϱ�. ���Ŀ� �̼� �����ÿ� �Ѿ�ִ� ��.
		if (number == 0)
		{
			death = 0; // ù�̼��� ù ���ɿ��� ������ 0���� ����
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
		if (death >= 1) {  // ȭ�쿡 ���� ������� �ʰ� 5�������� �̴� ��츦 ����Ͽ� �� ��ġ.
			quest++;
			Maze[7][14] = '1'; number = 0;
			Maze[4][14] = '6';
			Maze[6][14] = '5';
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
			SetMaze();
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
void speedShow()
{
	TextColor(DEEP_WHITE);
	gotoxy(MAP + 10, 1, "    Arrow Speed : "); TextColor(GRAY); printf("%d", arrowSpeedSet); // �����ֱ�
	gotoxy(MAP + 10, 1, "��");
}
void speedSetting()
{
	switch (stage)  // arrowSpeedSet �� ������ ���� �ӵ� ���� 
	{
		// -1�� Ÿ��Ʋ �ӵ� ���� X 0 = Ʃ�丮�� X
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