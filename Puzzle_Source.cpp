#include "Puzzle_Header.h"
//분리컴파일된 헤더파일을 include한다.

int main(void)
{
	puzzleGame gameBot;

	gameBot.shuffle(50); //50번 섞는다

	gameBot.setCTime(clock());
	gameBot.setSTime(clock());
	//시작하기전 시간 초기화

	cout << gameBot; //게임 화면 출력
	gameBot.getCommand(); //입력, 퍼즐 완성 여부 확인

	while (gameBot.getAction()) //action이 Stop이면 반복문이 종료된다.
	{
			gameBot.puzzleMove(gameBot.getAction()); // 입력된 키에 따라 퍼즐을 옮긴다.
			gameBot.setCTime(clock()); //현재 시간을 저장한다.
			cout << gameBot; // 화면출력
			gameBot.getCommand(); //입력, 퍼즐 완성 여부 확인
	}
	rectangle(gameBot.getPmap(), Point(240, 140), Point(600, 0), Scalar(239, 248, 250), -1);
	putText(gameBot.getPmap(), "Game Over!", Point(250, 130), 0, 1, Scalar(101, 110, 119),4, LINE_AA);
	//게임 완성 혹은 강제 종료시 화면 우측 상단에 "Game Over!" 텍스트 입력 중복출력 방지 위해 사각형 그림
	cout << gameBot; //텍스트가 입력된 화면 출력
	waitKey(0); //키를 입력 받을 때까지 대기한다.

	return 0;
}