#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>


GLboolean make_vertexShader();
GLboolean make_fragementShader();
GLint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid initBuffer();

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

//�ﰢ�� ��ġ ��
const GLfloat triShape[3][3] = { { -0.5, -0.5, 0.0 },
								 { 0.5, -0.5, 0.0 },
								 { 0.0, 0.5, 0.0} };
//�ﰢ�� ������ ����
const GLfloat colors[3][3] = { { 1.0, 0.0, 0.0 },
							   { 0.0, 1.0, 0.0 },
							   { 0.0, 0.0, 1.0 } };

GLuint vao, vbo[2];


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
	
	shaderID = make_shaderProgram();	//���̴� ���α׷� �����
	initBuffer();


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
	make_vertexShader();	//���ؽ� ���̴� �����
	make_fragementShader();	//�����׸�Ʈ ���̴� �����

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

void initBuffer()
{
	//vao�����ϰ� �Ҵ�
	glGenVertexArrays(1, &vao);

	//vao�� ���ε��ϱ�
	glBindVertexArray(vao);

	//2���� vbo�� �����ϰ� �Ҵ�
	glGenBuffers(2, vbo);

	//1���� vbo�� Ȱ��ȭ�ϰ� ���ε��ϰ�, ���ؽ� �Ӽ�(��ǥ��) ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//��ǥ���� attribute�ε��� 0���� ����Ѵ�. ���ؽ��� 3*float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//attribute 0�� �ε��� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//2��° vbo�� Ȱ��ȭ�ϰ� ���ε��ϰ�, ���ؽ� �Ӽ�(����) ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//���� colors���� ���ؽ� ������ �����Ѵ�.
	//colors �迭�� ������: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, colors, GL_STATIC_DRAW);

	//������ attribute �ε��� 1���� ����Ѵ� : ���ؽ� �� 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

GLvoid drawScene()
{
	GLfloat rColor, gColor, bColor;

	rColor = gColor = bColor = 1.0;
	
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderID);
	
	//����� vao �ҷ�����
	glBindVertexArray(vao);

	//�ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

