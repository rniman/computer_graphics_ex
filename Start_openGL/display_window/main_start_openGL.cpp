//오픈지엘 라이브러리를 이용해서 윈도우 창을 띄우자

#include <iostream>
#include "gl/glew.h"
#include "gl/freeglut.h"
#include "gl/freeglut_ext.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void main(int argc, char** argv)	//윈도우 생성하고 콜백함수 설정
{
	//윈도우 생성
	glutInit(&argc, argv);							//glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//디스플레이 모드설정
	glutInitWindowPosition(100, 100);				//윈도우의 위치 지정
	glutInitWindowSize(250, 250);					//윈도우 크기 지정
	glutCreateWindow("Example1");					//윈도우 생성(윈도우 이름)

	//GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)                      // glew 초기화 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);						//출력함수의 지정
	glutReshapeFunc(Reshape);						//다시 그리기함수 지정
	glutMainLoop();									//이벤트 처리 시작
}

GLvoid drawScene()					//콜백함수-> 그리기 콜백함수
{
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);			//바탕색 지정
	glClear(GL_COLOR_BUFFER_BIT);					// 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현
	//그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();		
}

GLvoid Reshape(int w, int h)		//콜백함수-> 다시 그리기 콜백함수
{
	glViewport(0, 0, w, h); 
}