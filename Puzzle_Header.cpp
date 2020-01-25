#include <conio.h> //getch()와 kbhit() 함수 사용
#include <fstream>
#include <windows.h> //GetStdHandle(), Sleep(), SetConsoleCursorPosition() 함수 사용
#include "Puzzle_Header.h" //분리컴파일

ostream & operator <<(ostream & outputStream, const puzzleGame& gmBot)
{ // gameDrawd 멤버함수 대신에 << 연산자 오버로딩를 통해 사용하시오.
	namedWindow("Puzzle_Game", CV_WINDOW_AUTOSIZE);//창 이름 지정및 띄우기

	for (int i = 0; i< MDIM; i++)
		for (int j = 0; j< MDIM; j++)
			gmBot.puzzleDraw(gmBot.Pmap, gmBot.Num16, MAP_STARTX + 95 * j, MAP_STARTY + 95 * i, gmBot.map[i][j]);
	//puzzleDraw 함수 호출해 퍼즐 출력.

	float timeing = (gmBot.cTime - gmBot.sTime) / 1000.00, temp; //소요시간 계산

	string time = gmBot.Point2(to_string(timeing)); //두자리 수 까지 출력되게 뒷부분을 자름
	string move = to_string(gmBot.moveNum);
	//putText함수 사용위해 시간을 string형으로 바꾼다.

	Size timetext = getTextSize(time, 0, 1, 2, 0);
	Size movetext = getTextSize(move, 0, 1, 2, 0);
	Point timepoint, movepoint;

	movepoint.x = 185 - movetext.width/2; movepoint.y = 690;
	timepoint.x = 415 - timetext.width/2; timepoint.y = 690;
	//시간과 이동횟수를 왼쪽 아래부터가아닌 중간부터출력되게 하기위해 텍스트크기를 재고 중간으로 맞춘다.

	rectangle(gmBot.Pmap, Point(97, 655), Point(270, 700), Scalar(160, 173, 187),-1);
	rectangle(gmBot.Pmap, Point(327,653), Point(500, 700), Scalar(160, 173, 187),-1);
	//이동횟수와 시간 출력시 이미지에 중복되어 출력되는 문제가 발생하는데 이를 방지하기 위해
	//중첩되는 부분에 계속해서 배경과 같은 색상의 채워진 사각형을 그린다. 

	putText(gmBot.Pmap, time, timepoint, 0, 1, Scalar(255, 255, 255), 2, LINE_AA);
	putText(gmBot.Pmap, move, movepoint, 0, 1, Scalar(255, 255, 255), 2, LINE_AA);
	//정해진 위치에 시간과 이동횟수를 출력한다. 

	imshow("Puzzle_Game", gmBot.Pmap);
	waitKey(100);
	//이미지를 창에 표시하며 보이게 하기위해 0.1초의 대기시간을 부여한다.

	return outputStream;
}

puzzleGame::puzzleGame()
{
	Pmap = imread("C:/users/young/Pictures/Map.png", CV_LOAD_IMAGE_COLOR);
	//배경에 대한 사진을 Mat형의 Pmap에 저장한다.

	Puzzle_num = imread("C:/users/young/Pictures/Puzzle_Num.png", CV_LOAD_IMAGE_COLOR);
	Rect temp;
	int num = 0;
	// Draw circles on the detected faces
	for (int i = 0; i < PNUM / 4; i++) {
		for (int j = 0; j < PNUM / 4; j++) {
			temp.x = FSTARTX + (FSIZE + 20)*j;
			temp.y = FSTARTY + (FSIZE + 20)*i;
			temp.width = FSIZE;
			temp.height = FSIZE;
			resize(Puzzle_num(temp).clone(), Num16[num++], Size(Psize, Psize));
		}
	}
	//숫자에 관한 사진을 Mat형의 Puzzle_num에 저장한다.
	//불러온 이미지에서 사각형의 영역을 지정하고 각각의 숫자들을 Num16배열에 저장한다.

	map = new int*[MDIM]; // 이차원 포인터를 동적할당 한다.

	for (int i = 0; i < MDIM; i++)
		map[i] = new int[MDIM]; // 이차원 포인터를 동적할당한다.

	for (int i = 0; i < MDIM; i++)
		for (int j = 0; j < MDIM; j++)
			map[i][j] = i * 4 + j;
	//퍼즐 맵 동적할당 및 값 할당
	x = MDIM - 1; y = MDIM - 1;
	// 맵의 값을 1,2,3,~ ,MDIM*MDIM-1,0의 순서로 초기화 시키고 빈칸의 위치를 마지막으로 초기화시킨다.
	sTime = 0.00; //시작시간을 초기화 시킨다.
	moveNum = 0; //이동 회수를 초기화시킨다.

				 //게임 초기화 코드 작성
}

puzzleGame::~puzzleGame()
{
	for (int i = 0; i < MDIM; i++)
		delete[] map[i]; //배열 포인터를 초기화한다.

	delete[] map; // 배열 포인터를 초기화한다.
}

void puzzleGame::puzzleDraw(Mat Mapping, const Mat Puzzle[], int x, int y, int k) const
{	//숫자가 그려질 이미지와 숫자 이미지, 입력할 위치, 입력할 숫자를 입력받는다.
	Mat imageROI = Mapping(Rect(x, y, Psize, Psize));
	// 영상 ROI 정의
	// Rect는 사각형 영역 지정
	// x, y은 각각 숫자 크기의 x좌표, y좌표 시작지점
	// Psize는 숫자 하나 이미지의 가로 세로 크기

	Puzzle[k].copyTo(imageROI);
	//지정한 관심영역에 숫자 하나 이미지를 복사하여 붙인다.
}

void puzzleGame::puzzleMove(int moveKey)
{
	int temp = 0;
	switch (moveKey) //getcommand를 통해 입력받은 키보드 값으로 퍼즐을 움직인다.
	{
	case Left:
		if (x >= 1) { //예외 처리를 통해 빈칸의 위치가 판을 벗어나지 않도록 한다.
			temp = map[y][x];
			map[y][x] = map[y][x - 1];
			map[y][x - 1] = temp;
			x--; moveNum++; //퍼즐을 이동했기 때문에 이동 횟수를 1더한다.
		}
		break;

	case Right:
		if (x <= 2) {
			temp = map[y][x];
			map[y][x] = map[y][x + 1];
			map[y][x + 1] = temp;
			x++; moveNum++; //퍼즐을 이동했기 때문에 이동 횟수를 1더한다.
		}
		break;

	case Up:
		if (y >= 1) {
			temp = map[y][x];
			map[y][x] = map[y - 1][x];
			map[y - 1][x] = temp;
			y--; moveNum++; //퍼즐을 이동했기 때문에 이동 횟수를 1더한다.
		}
		break;

	case Down:
		if (y <= 2) {
			temp = map[y][x];
			map[y][x] = map[y + 1][x];
			map[y + 1][x] = temp;
			y++; moveNum++; //퍼즐을 이동했기 때문에 이동 횟수를 1더한다.
		}
		break;

	default:
		break; //아무것도 아니면 그냥 switch를 탈출한다.
	}
}

void puzzleGame::getCommand(void)
{
	bool Flag = FALSE; //퍼즐이 맞추어졌는지 확인한다.

	for (int i = 0; i<MDIM; i++)
		for (int j = 0; j < MDIM; j++) {
			if ((map[i][j] != i * 4 + j) && (i*j < (MDIM - 1)*(MDIM - 1))) { Flag = TRUE; }
		} //중간에 틀린 부분이 발생하면 Flag가 TRUE로 설정된다.
	if (Flag == FALSE)
		action = Stop; //퍼즐이 다 맞으면 action 을 Stop으로 한다.

	if (action != Stop) { //퍼즐이 맞추어지지 않은 경우 키보드로 키를 입력받는다.
		switch (cvWaitKey(0)) { //getDirectKey 함수를 통해 키를 입력받고 명령어를 디코딩해 action에 저장한다.
		case ESC:
			action = Stop;
			moveNum = 0;
			sTime = cTime = 0; //시간과 이동횟수를 초기화한다.
			break;
		case LEFT:
			action = Left;
			break;
		case RIGHT:
			action = Right;
			break;
		case DOWN:
			action = Down;
			break;
		case UP:
			action = Up;
			break;
		default:
			action = Non;
			break; // 방향키나 ESC키말고 다른키가 입력되면 NON을 리턴한다.
		}
	}
}

void puzzleGame::shuffle(int num)
{
	int numcheck[MDIM*MDIM]; //배열의 값이 중복되었는지 체크하기위한 맵과 동일한 크기의 배열을 생성한다.
	int random = 0; //생성된 난수를 저장해 놓는 변수이다.

	srand(time(NULL)); //난수의 시드 값을 바꾸어 줌으로써 난수 생성을 한다.

	putText(Pmap, "Shuffling...", Point(250, 130), 0, 1, Scalar(101, 110, 119), 4, LINE_AA);
	//섞고 있다는 안내문을 출력한다.

	for (int k = 0; k < num; k++)
	{
		while (1) {
			for (int i = 0; i < MDIM*MDIM; i++)
				numcheck[i] = 1; //난수 설정하기전에 중복체크 배열을 사용되기 전 상태인 '1'로 초기화 시킨다.

			for (int i = 0; i < MDIM; i++)
				for (int j = 0; j < MDIM; j++)
					while (1) {
						if ((numcheck[random = (rand() % (MDIM*MDIM))]) != 0) {
							if (random == 15) { //만약 난수의 값이 빈칸의 좌표인 0이라면 그의 배열 좌표를 저장한다.
								x = j; //빈칸의 위치를 저장하기 위해 멤버 변수 x,y에 2차원 배열의 좌표를 저장한다.
								y = i;
							}
							map[i][j] = random;
							numcheck[random] = 0;
							break;
						} //생성된 난수가 사용되지 않은 난수이면 사용 표시를 하고 배열에 저장하며 루프를 탈출한다.
					}	  // 난수가 사용 전이면 numcheck에 1의 형태로, 후면 0의 형태로 저장된다.

			if (solvable() == 1)//퍼즐을 풀지 못하는 경우로 섞일 수 있다. 풀 수 있는지 판별하여
				break;				   //풀 수 있으면 1을 리턴하며 풀지못하면 0을 리턴하고 다시 섞는다.
		}
		cTime = sTime = 0.00; //섞을 때는 시작 시간을 계속 초기화 시켜줌으로써 시간이 0으로 보이도록 한다.
		cout << *this; //현재 클래스를 출력하기 위해 this포인터를 사용한다.
		Sleep(100); //맵 변경이 보이도록 대기시간을 준다.
	}
	rectangle(Pmap, Point(240, 140), Point(600, 0), Scalar(239,248,250), -1);
	putText(Pmap, "Start!", Point(250, 130), 0, 1, Scalar(101, 110, 119), 4, LINE_AA);
	//시작하라는 안내문구를 출력하며 문자의 중복출력을 막기위해 배경과 동일한 색의 사각형을 그린다.
}

int puzzleGame::solvable(void) //무작위로 배열된 퍼즐이 풀수있는지 판별하는 함수 
{
	int inv_count = 0, k = 0, temp, t_arr[MDIM*MDIM]; //반전된 숫자 배열 세는 변수 등을 선언

	if (x != MDIM - 1 || y != MDIM - 1) //먼저 빈칸의 위치를 맨 아래 맨 오른쪽으로 이동시킨다.
	{
		temp = map[MDIM - 1][MDIM - 1];
		map[MDIM - 1][MDIM - 1] = MDIM*MDIM-1;
		map[y][x] = temp;
		y = MDIM - 1;
		x = MDIM - 1;
	}

	for (int i = 0; i < MDIM; i++)
		for (int j = 0; j < MDIM; j++)
			t_arr[i * 4 + j] = map[i][j];
	//배열의 숫자들을 비교하기 위해 퍼즐 크기의 일차원 배열에 복사한다.

	for (int i = 0; i < MDIM* MDIM - 2; i++) 
		for (int j = i + 1; j < MDIM * MDIM - 1; j++)
			if (t_arr[i] > t_arr[j])
				inv_count++;
	//일차원 배열들의 값을 순차적으로 비교하며 반전된 수의 수를 센다.

	if (inv_count % 2 == 1) //만약 반전된 수의 수가 홀수 개이면 풀 수 없는 퍼즐이며 0을 리턴한다.
		return 0;
	else
		return 1; //짝수 개이면 풀 수 있는 퍼즐이며 1을 리턴한다.
}

string puzzleGame::Point2(string nums) const
{	//소숫점 두자리까지 출력하기 위한 함수이다.
	string temp; //임시 string temp를 설정한다.
	temp = nums.substr(0, nums.find(".") + 3);
	//문자열에서.find를 통해 .의 위치를 찾고 .substr을 통해 0번째 인덱스부터 .포함 3개의 문자열을
	//출력하면 소수점 두자리까지 출력하게 된다. 이를 temp에 저장후 temp를 리턴한다.
	return temp;
}