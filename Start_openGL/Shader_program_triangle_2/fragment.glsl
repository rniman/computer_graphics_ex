#version 330
//out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//FragColor: ����� ������ ������ ������ ���۷� ���� ��. 

//���ؽ� ���̴����Լ� ���� ����
in vec3 out_Color; 
//���� ���
out vec4 FragColor;

void main(void) 
{
	FragColor = vec4 (out_Color , 1.0);
}