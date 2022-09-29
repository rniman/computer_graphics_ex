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
GLuint shaderID;		//���̴� ���α׷� �̸�
GLuint vertexShader;	//���ؽ� ���̴� ��ü
GLuint fragmentShader;	//�����׸�Ʈ ���̴� ��ü

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

	//���̴� �о�ͼ� ���̴� ���α׷� �����
	make_vertexShader();	//���ؽ� ���̴� �����
	make_fragementShader();	//�����׸�Ʈ ���̴� �����
	shaderID = make_shaderProgram();	//���̴� ���α׷� �����

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutMainLoop();
}

GLboolean make_vertexShader()
{
	GLchar* vertexSource; 

	//���ؽ� ���̴� �ڵ带 �о�ͼ� �����ϰ� �������� �ؾ���
	//filetobuf:��������� �Լ��� �ؽ�Ʈ�� �а� ���ڿ��� �����ϴ� �Լ�

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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl; 
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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return false;
	}
}

GLint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //���̴� ���α׷� �����

	//���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, vertexShader);

	//���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader);

	//���̴� ���α׷� ��ũ�ϱ�
	glLinkProgram(ShaderProgramID); 

	//���̴� ��ü�� ���̴� ���α׷��� ��ũ�����Ƿ� ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog); 
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return false;
	}

	//������� ���α׷� ����ϱ�
	//�������� ���̴� ���α׷��� ����� �ְ� ���� �Ѱ��� ���α׷��� ����Ϸ���
	//glUseProgram �Լ��� ȣ���Ͽ� ����� Ư�� ���α׷��� �����Ѵ�.
	//����ϱ� ������ ȣ�� �� �� �ִ�.
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

	glDrawArrays(GL_TRIANGLES, 0, 3); //������ �ϱ�:0�� �ε������� 1���� ���ؽ��� ����� �� �׸���

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

