#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>


GLboolean make_vertexShader();
GLboolean make_fragementShader();
GLint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLint width, height;
GLuint shaderID;		//세이더 프로그램 이름
GLuint vertexShader;	//버텍스 세이더 객체
GLuint fragmentShader;	//프래그먼트 세이더 객체

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fopen_s(&fptr, file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}

int main(int argc, char** argv)
{
	width = 500;
	height = 500;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example");

	glewExperimental = GL_TRUE; 
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShader();	//버텍스 세이더 만들기
	make_fragementShader();	//프래그먼트 세이더 만들기
	shaderID = make_shaderProgram();	//세이더 프로그램 만들기

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutMainLoop();
}

GLboolean make_vertexShader()
{
	GLchar* vertexSource; 

	//버텍스 세이더 코드를 읽어와서 저장하고 컴파일을 해야함
	//filetobuf:사용자정의 함수로 텍스트를 읽고 문자열에 저장하는 함수

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result); 
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl; 
		return false;
	}
}

GLboolean make_fragementShader()
{
	GLchar* fragmentSource;

	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return false;
	}
}

GLint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //세이더 프로그램 만들기

	//세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, vertexShader);

	//세이더 프로그램에 프래그먼트 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader);

	//세이더 프로그램 링크하기
	glLinkProgram(ShaderProgramID); 

	//세이더 객체를 세이더 프로그램에 링크했으므로 세이더 객체 자체는 삭제 가능
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog); 
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}

	//만들어진 프로그램 사용하기
	//여러개의 세이더 프로그램을 만들수 있고 그중 한개의 프로그램을 사용하려면
	//glUseProgram 함수를 호출하여 사용할 특정 프로그램을 지정한다.
	//사용하기 직전에 호출 할 수 있다.
	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}

GLvoid drawScene()
{
	GLfloat rColor, gColor, bColor;

	rColor = gColor = 1.0;
	bColor = 1.0;
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderID);

	glDrawArrays(GL_TRIANGLES, 0, 3); //렌더링 하기:0번 인덱스에서 1개의 버텍스를 사용해 점 그리기

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

