#pragma once

#ifndef Puzzle_Header
#define Puzzle_Header

#include <iostream>
#include <ctime> //clock() 함수 사용
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

#define FSIZE 120 //숫자 그림 하나 크기
#define FSTARTX 30 //숫자 그림 시작 위치 x
#define FSTARTY 30 //숫자 그림 시작 위치 y
#define Psize 82 //조절되어야 할 숫자의 크기

#define ESC 27 //게임 종료
#define LEFT 2424832 //왼쪽 화살표 키 ASCII값
#define RIGHT 2555904 // 오른쪽 화살표 키 ASCII값
#define DOWN 2621440 //아래 화살표 키 ASCII값
#define UP 2490368 //위쪽 화살표 키 ASCII값

#define MDIM 4 //4x4 퍼즐맵의 크기
#define PDIM 7 //7x7 퍼즐의 크기
#define PNUM MDIM*MDIM // 네모칸의 수

//게임 Map 그리기 시작 위치: 필요하다면 출력 위치를 조정할 수 있음 
#define MAP_STARTX 117	
#define MAP_STARTY 206

enum COMMAND { Stop = 0, Left, Right, Down, Up, Non };
//action 에 대해 숫자 지정

class puzzleGame
{
public:
	puzzleGame(); //default 생성자: 클래스 객체 초기화 함수, 필요시에 매개변수를 추가함
	~puzzleGame();// 소멸자: 클래스 객체의 소멸전에 동적할당의 해제와 같은 객체의 정리를 위해 사용됨
	friend ostream & operator <<(ostream & outputStream, const puzzleGame& gmBot);
	// gameDrawd 멤버함수 대신에 << 연산자 오버로딩를 통해 사용하시오.
	void puzzleDraw(Mat Mapping, const Mat Puzzle[], int x, int y, int k) const;
	// 하나의 퍼즐을 그리는 멤버함수
	void puzzleMove(int moveKey); //퍼즐 이동 함수; 필요시에 매개변수 추가함
	void shuffle(int num);//퍼즐 맵 초기화; 50회 무작위 이동, 필요시에 인자 추가 할 수 있음
	void getCommand(void);//모든 퍼즐이 제 위치에 있거나 ESC키가 눌러졌으면 정수 0을 반환
						  // 방향키를 눌렀을 때는 해당 키값을 반환(LEFT, RIGHT, DOWN, UP)
						  //필요시에 매개변수 인자를 추가할 수 있음
	int getAction(void)	const { return action; }; //메인함수에서 action의 값을 읽는다.
	int solvable(void); //풀 수 있는 퍼즐인지 판별하는 함수
	void setCTime(clock_t ct) { cTime = ct; }; //cTime설정 인라인 함수
	void setSTime(clock_t st) { sTime = st; }; //sTime설정 인라인 함수
	string Point2(string nums) const; //소숫점 두자리까지 출력하기위한 함수
	Mat getPmap() {return this->Pmap; }; //메인함수에서 Pmap을 부를때 필요한 함수
private:

	Mat Puzzle_num, Pmap; //숫자 전체 맵 저장 Puzzle_num, 전체 배경맵 저장 Pmap
	Mat Num16[PNUM]; //1~15, 빈칸을 저장하기 위한 Num[16] 배열
	int **map; //map 동적할당위해 선언

	int action; //앞으로 동작 저장
	int x, y; //빈칸의 위치
	int moveNum; //이동회수
	clock_t sTime; //시작 시간
	clock_t cTime; //시작 시간
};
#endif // !Puzzle_Header
