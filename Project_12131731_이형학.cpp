#pragma comment(lib,"legacy_stdio_definitions.lib")
#include <Windows.h>
#include <gl/glut.h>
#include <gl/GLAUX.H>
#include <stdio.h>
#define _USE_MATH_DEFINES /* ���̰� ����ϱ� ���ؼ� ���� */
#include <math.h>

GLUquadricObj* m_pQuadric; /* GLUQuadic ��ü ���� */

GLuint texName[86];
GLuint quaName[5];

GLfloat Tx = 0, Ty = 0, Tz = 0;

float eyex = 0; /* gluLookAt�Լ����� eyex������ �� ���� */
float eyey = 0; /* gluLookAt�Լ����� eyey������ �� ���� */
float eyez = 0; /* gluLookAt�Լ����� eyez������ �� ���� */
float lootat_r = 100; /* ��ǥ�� �߽ɿ��������� ������ */
float lootat_elevation = M_PI / 4; /* ��ǥ���߽ɿ��� �浵 */
float lootat_azimuth = M_PI / 4; /* ��ǥ���߽ɿ��� ���� */
float upy = 1; /* gluLooAt�Լ����� upy������ �� ���� */

float angle0 = 0; /* ���� ȸ���� ���� ���� ���� */
float angle1 = 0; /* �����������̾��� ȸ���� ���� ���� ���� */
float angle2 = 0; /* ȸ�������� ȸ���� ���� ���� ���� */

int mbutton = 0; /* ���콺 ����, ������ Ŭ���� �����ϱ� ���� ���� */
int start = 0; /* ���� ON���� Ÿ�̸�, ȸ������ ���� ���θ� ���� ���� */
int power = 1; /* ���� ON, OFF��带 �����ϱ� ���� ���� */
int timer = 0; /* ���ۺ��� Ÿ�̸ӿ� ����� Ÿ�̸ӿ� ���� ���� */

void init(void); /* �ʱ�ȭ �Լ� */
void draw(void); /* �׸��� �Լ� */
void draw_body(); /* ��ü �׸��� �Լ� */
void draw_door(); /* �� �׸��� �Լ� */
void draw_handle(); /* ���ۺ� �׸��� �Լ� */
void draw_dish(); /* ȸ������ �׸��� �Լ� */
void draw_button(); /* ���, ���۹�ư �׸��� �Լ� */
void In_light(); /* ��������ȿ�� �Լ� */
void draw_timer(); /* Ÿ�̸� �׸��� �Լ� */
void count_timer(int value); /* Ÿ�̸� ��� �Լ� */
void resize(int width, int height); /* Viewport �� projection transform�� ���α׷��� ó�� ���۵ɶ�
									resize�� �ʱ�ȭ �Ǵ� ������ ũ�Ⱑ ����ɶ� resize�Լ����� ���� */
void keyboard(unsigned char key, int x, int y); /* Ű���� ó�� �Լ� */
void idle(void); /* idle�Լ� */
void idle_stop(); /* idle��Ȱ��ȭ �Լ� */
void main_menu_function(int option); /* �˾��޴� �Լ� */
void mouse(int button, int state, int x, int y); /* ���콺 �Լ� */
void motion(int x, int y); /* ����Լ� */
void specialkeyboard(int key, int x, int y); /* Ư��Ű �Լ� */

int main(int argc, char** argv)
{
	/* Window �ʱ�ȭ */
	glutInit(&argc, argv); /* glutInit�� GLUT�� ���º����� �ʱ�ȭ�ϴ� ���Ұ�
						   ������ý����� ������ ó���� */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); /* glutInitDisplayMode�� ���÷��� ��带 ��Ÿ����.*/
	glutInitWindowSize(500, 500); /* glutInitWindowSize�� â�� ũ�⸦ ����*/
	glutInitWindowPosition(300, 300); /* glutInitWindowPosition�� �����쿡�� ����Ǵ� â�� ��ġ�� ���� */
	glutCreateWindow("12131731 ������"); /* glutCreateWindow�� ��ȣ���� ������ �������� ���� â�� ����.*/

	init();/* �ʱ�ȭ �Լ� */

	int submenu1;
	/* Popup menu ���� �� �߰� */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("ON", 1);
	glutAddMenuEntry("OFF", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON); /* �޴��� ���콺 ��ư�� �����Ѵ�. */

	/* Callback �Լ� ����(Callback�Լ��� ��� ������ �����ϴ� ����� �ڽ��� ������ ����ǰų� �Ǵ�
	�̺�Ʈ�� �߻��ҋ��� ����� ������ �̺�Ʈ�� ���� ��� ó���� �� �� �ֵ��� �����ϴ� �Լ�) */
	glutDisplayFunc(draw); /* draw() -> ���� �׸��� �Լ� */

	/* Callback �Լ� ��� */
	glutReshapeFunc(resize); /* â ũ�Ⱑ ����Ǵ°ſ� ���� �ݹ� �Լ�  */
	glutKeyboardFunc(keyboard); /* keyboard callback �Լ� */
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpecialFunc(specialkeyboard); /* �Լ�Ű, ����Ű �� Ư��Ű�� ���� �ݹ� �Լ� */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Looping ���� */
	glutMainLoop();

	return 0;
}

/* ����� �ʱ�ȭ �Լ�*/
void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); /* ����� ������� ���� */
	glClearDepth(1.0); /* depth �ʱ�ȭ */
	glFrontFace(GL_CW); /* ������ CW�������� �׸� */
	glEnable(GL_DEPTH_TEST); /* Depth Ȱ��ȭ */
	glShadeModel(GL_SMOOTH); /* ���̴���带 SMOOTH�� ���� */

	/* Material Properties ���� */
	GLfloat ambient_Material[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse_Material[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular_Material[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Material);
	glMateriali(GL_FRONT, GL_SHININESS, 256);

	glEnable(GL_COLOR_MATERIAL); /* COLOR MATERIAL Ȱ��ȭ */
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); /* glColor�� Ȱ��ȭ */

	/* �ܺ� ����(0�� ����)�� light ���� */
	GLfloat ambient0[] = { 0.2f, 0.2f, 0.2f, 1.0f }; /* LIGHT0�� ambient light���� ���  */
	GLfloat diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* LIGHT0�� difuuse light���� ��� */
	GLfloat specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* LIGHT0�� specular light���� ��� */

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0); /* LIGHT0�� ambient light���� ���� */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0); /* LIGHT0�� diffuse lihgt���� ���� */
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0); /* LIGHT0�� specular light���� ���� */

	glEnable(GL_LIGHTING); /* Lighting mode�� Ȱ��ȭ */
	glEnable(GL_LIGHT0); /* LIGHT0�� ������ Ŵ */

	/* ���� ����(1������)�� light ���� */
	GLfloat ambient1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; /* LIGHT1�� ambient light���� ���  */
	GLfloat diffuse1[] = { 1.0f, 0.6f, 0.2f, 1.0f }; /* LIGHT1�� difuuse light���� ��� */
	GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* LIGHT1�� specular light���� ��� */

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1); /* LIGHT1�� ambient light���� ���� */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1); /* LIGHT1�� diffuse lihgt���� ���� */
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1); /* LIGHT1�� specular light���� ���� */

	/* �ؽ�ó �ʱ�ȭ */
	glGenTextures(86, texName); /* �ؽ�ó ��ü�� �����ϴ� �ؽ�ó ���̵� ���� */
	AUX_RGBImageRec * pRGBImage;


	/* ���� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	pRGBImage = auxDIBImageLoadA("����-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	pRGBImage = auxDIBImageLoadA("����-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[5]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ���� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[6]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[7]);
	pRGBImage = auxDIBImageLoadA("����-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[8]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[9]);
	pRGBImage = auxDIBImageLoadA("����-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[10]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[11]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ���� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[12]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[13]);
	pRGBImage = auxDIBImageLoadA("����-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[14]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[15]);
	pRGBImage = auxDIBImageLoadA("����-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[16]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[17]);
	pRGBImage = auxDIBImageLoadA("����-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ������ ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[18]);
	pRGBImage = auxDIBImageLoadA("������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[19]);
	pRGBImage = auxDIBImageLoadA("������-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[20]);
	pRGBImage = auxDIBImageLoadA("������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[21]);
	pRGBImage = auxDIBImageLoadA("������-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[22]);
	pRGBImage = auxDIBImageLoadA("������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[23]);
	pRGBImage = auxDIBImageLoadA("������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* �Ʒ��� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[24]);
	pRGBImage = auxDIBImageLoadA("�Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[25]);
	pRGBImage = auxDIBImageLoadA("�Ʒ���-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[27]);
	pRGBImage = auxDIBImageLoadA("�Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[26]);
	pRGBImage = auxDIBImageLoadA("�Ʒ���-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[28]);
	pRGBImage = auxDIBImageLoadA("�Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[29]);
	pRGBImage = auxDIBImageLoadA("�Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* �������� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[30]);
	pRGBImage = auxDIBImageLoadA("��������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[31]);
	pRGBImage = auxDIBImageLoadA("��������-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[32]);
	pRGBImage = auxDIBImageLoadA("��������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[33]);
	pRGBImage = auxDIBImageLoadA("��������-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[34]);
	pRGBImage = auxDIBImageLoadA("��������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[35]);
	pRGBImage = auxDIBImageLoadA("��������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ������� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[65]);
	pRGBImage = auxDIBImageLoadA("�������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[66]);
	pRGBImage = auxDIBImageLoadA("�������-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[67]);
	pRGBImage = auxDIBImageLoadA("�������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[68]);
	pRGBImage = auxDIBImageLoadA("�������-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[69]);
	pRGBImage = auxDIBImageLoadA("�������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[70]);
	pRGBImage = auxDIBImageLoadA("�������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ��������� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[71]);
	pRGBImage = auxDIBImageLoadA("���������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[72]);
	pRGBImage = auxDIBImageLoadA("���������-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[73]);
	pRGBImage = auxDIBImageLoadA("���������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[74]);
	pRGBImage = auxDIBImageLoadA("���������-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[75]);
	pRGBImage = auxDIBImageLoadA("���������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[76]);
	pRGBImage = auxDIBImageLoadA("���������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/* ����Ʒ��� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[77]);
	pRGBImage = auxDIBImageLoadA("����Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[78]);
	pRGBImage = auxDIBImageLoadA("����Ʒ���-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[79]);
	pRGBImage = auxDIBImageLoadA("����Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[80]);
	pRGBImage = auxDIBImageLoadA("����Ʒ���-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[81]);
	pRGBImage = auxDIBImageLoadA("����Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[82]);
	pRGBImage = auxDIBImageLoadA("����Ʒ���-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ����Ʒ��� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[83]);
	pRGBImage = auxDIBImageLoadA("��������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[84]);
	pRGBImage = auxDIBImageLoadA("��������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ���ۺ� ������ �ؽ�ó ���� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[36]);
	pRGBImage = auxDIBImageLoadA("���ۺ�-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[37]);
	pRGBImage = auxDIBImageLoadA("���ۺ�-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[38]);
	pRGBImage = auxDIBImageLoadA("���ۺ�-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[39]);
	pRGBImage = auxDIBImageLoadA("���ۺ�-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[40]);
	pRGBImage = auxDIBImageLoadA("���ۺ�-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[41]);
	pRGBImage = auxDIBImageLoadA("���ۺ�-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* Ÿ�̸�(0~10��) ������ */
	glBindTexture(GL_TEXTURE_2D, texName[42]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�0.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[43]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[44]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[45]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�3.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[46]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�4.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[47]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�5.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[48]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�6.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[49]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�7.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[50]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�8.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[51]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�9.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[52]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�10.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[85]);
	pRGBImage = auxDIBImageLoadA("Ÿ�̸�-OFF.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ��ҹ�ư ������ */
	glBindTexture(GL_TEXTURE_2D, texName[53]);
	pRGBImage = auxDIBImageLoadA("��ҹ�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[54]);
	pRGBImage = auxDIBImageLoadA("��ҹ�ư-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[55]);
	pRGBImage = auxDIBImageLoadA("��ҹ�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[56]);
	pRGBImage = auxDIBImageLoadA("��ҹ�ư-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[57]);
	pRGBImage = auxDIBImageLoadA("��ҹ�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[58]);
	pRGBImage = auxDIBImageLoadA("��ҹ�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* Ȯ�ι�ư ������ */
	glBindTexture(GL_TEXTURE_2D, texName[59]);
	pRGBImage = auxDIBImageLoadA("Ȯ�ι�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[60]);
	pRGBImage = auxDIBImageLoadA("Ȯ�ι�ư-�����ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[61]);
	pRGBImage = auxDIBImageLoadA("Ȯ�ι�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[62]);
	pRGBImage = auxDIBImageLoadA("Ȯ�ι�ư-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[63]);
	pRGBImage = auxDIBImageLoadA("Ȯ�ι�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[64]);
	pRGBImage = auxDIBImageLoadA("Ȯ�ι�ư-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); /* �������� ���� ������ �����ϰ� �ؽ�ó�� ���� */
	glEnable(GL_TEXTURE_2D);/* �ؽ�ó Ȱ��ȭ */

	m_pQuadric = gluNewQuadric(); /* GLUQuadic ��ü ���� */

	gluQuadricTexture(m_pQuadric, GL_TRUE); /* �ؽ�ó ���� ��ǥ ���� */
	glGenTextures(5, quaName); /* �ؽ�ó ��ü�� �����ϴ� �ؽ�ó ���̵� ���� */


	/* ���̾� ������ */
	glBindTexture(GL_TEXTURE_2D, quaName[0]);
	pRGBImage = auxDIBImageLoadA("���̾�-�Ǹ���.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, quaName[1]);
	pRGBImage = auxDIBImageLoadA("���̾�-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, quaName[2]);
	pRGBImage = auxDIBImageLoadA("���̾�-�Ʒ��ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* ȸ������ ������ */
	glBindTexture(GL_TEXTURE_2D, quaName[3]);
	pRGBImage = auxDIBImageLoadA("ȸ������-�Ǹ���.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, quaName[4]);
	pRGBImage = auxDIBImageLoadA("ȸ������-���ʸ�.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glEnable(GL_BLEND); /* BLENDȿ�� Ȱ��ȭ*/
}


/* �׸��� �Լ� */
void draw(void)
{
	eyex = lootat_r * sin(lootat_elevation) * sin(lootat_azimuth); /* gluLookAt�Լ����� eyex������ ���� �ش��ϴ� �� */
	eyey = lootat_r * cos(lootat_elevation); /* gluLookAt�Լ����� eyey������ ���� �ش��ϴ� �� */
	eyez = lootat_r * sin(lootat_elevation) * cos(lootat_azimuth);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/* ȭ���� �����ϰ� ����� */

	glMatrixMode(GL_MODELVIEW); /* glMatrixMode�Լ��� ��ĸ�带 ����,
								GL_MODELVIEW���� 3D������ ��ü�� ��ġ�� ���������� ǥ���� ����̴�.
								(���� ȭ����� ��ü�� ���� ��ǥ���� �����ϱ� ���� ���) */
	glLoadIdentity(); /* glLoadIdentity�Լ��� ��������� ���Ѵ�. ��, ����� �ʱ�ȭ */
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upy, 0); /* gluLookAt�Լ��� ī�޶��� ��ġ�� �����ϴ� �Լ� */

	glPushMatrix();
	glTranslatef(Tx, Ty, Tz); /* ���ڷ����� �̵� */
	In_light(); /* �������� */
	draw_timer(); /* Ÿ�̸� ������ */
	draw_body(); /* ��ü ������ */
	draw_handle(); /* ���ۺ� ������ */
	draw_dish(); /* ȸ������ ������ */
	draw_button(); /* ���,���۹�ư ������ */
	draw_door(); /* �� ������ ���� */
	glPopMatrix();

	glFlush(); /* glFush�� gl����� ������ ����Ǳ���� ��ٸ� ��
			   �׿��ִ� ��ɵ��� ��� ���� ��� ������ �ǵ��� �Ѵ�. */
	glutSwapBuffers(); /* Back buffer�� front buffer�� �ٲ� */
}


/* ��ü(��,��,��,�Ʒ���) �׸��� �Լ� */
void draw_body() {
	glPushMatrix();
	/* ���� ������ ���� */
	glPushMatrix();
	/* ���� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[0]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glEnd();

	/* ���� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[1]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 12.5f, -15.0f);
	glEnd();

	/* ���� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[2]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -12.5f, -15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, -15.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glEnd();

	/* ���� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[3]); /* �ؽ�ó ���ε� ���� */;
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 12.5f, -15.0f);
	glEnd();

	/* ���� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[4]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, -12.0f);
	glEnd();

	/* ���� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[5]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, -15.0f);
	glEnd();
	glPopMatrix();

	/* ���� ������ ���� */
	glPushMatrix();
	/* ���� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[6]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 12.5f, 15.0f);
	glEnd();

	/* ���� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[7]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 12.5f, -12.0f);
	glEnd();

	/* ���� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[9]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glEnd();

	/* ���� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[8]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 12.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 12.5f, -12.0f);
	glEnd();

	/* ���� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[10]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 9.5f, 15.0f);
	glEnd();

	/* ���� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[11]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 9.5f, -12.0f);
	glEnd();
	glPopMatrix();

	/* ���� ������ ���� */
	glPushMatrix();
	/* ���� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[12]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 9.5f, 15.0f);
	glEnd();

	/* ���� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[13]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-22.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-22.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-22.0f, 9.5f, -12.0f);
	glEnd();

	/* ���� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[14]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-22.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-22.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glEnd();

	/* ���� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[15]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-22.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 9.5f, -12.0f);
	glEnd();

	/* ���� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[16]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-22.0f, -9.5f, 15.0f);
	glEnd();

	/* ���� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[17]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-22.0f, -9.5f, -12.0f);
	glEnd();
	glPopMatrix();

	/* ������ ������ ���� */
	glPushMatrix();
	/* ������ �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[18]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(12.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(12.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(12.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(12.0f, 9.5f, 15.0f);
	glEnd();

	/* ������ �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[19]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 9.5f, -12.0f);
	glEnd();

	/* ������ �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[20]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(12.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(12.0f, -9.5f, 15.0f);
	glEnd();

	/* ������ �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[21]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(12.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(12.0f, 9.5f, -12.0f);
	glEnd();

	/* ������ �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[22]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(12.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(12.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -9.5f, 15.0f);
	glEnd();

	/* ������ �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[23]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(12.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(12.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -9.5f, -12.0f);
	glEnd();
	glPopMatrix();

	/* �Ʒ��� ������ ���� */
	glPushMatrix();
	/* �Ʒ��� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[24]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -9.5f, 15.0f);
	glEnd();

	/* �Ʒ��� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[25]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -9.5f, -12.0f);
	glEnd();

	/* �Ʒ��� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[26]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, 15.0f);
	glEnd();

	/* �Ʒ��� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[27]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -9.5f, -12.0f);
	glEnd();

	/* �Ʒ��� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[28]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, 15.0f);
	glEnd();

	/* �Ʒ��� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[29]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, -12.0f);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}


/* �� �׸��� �Լ� */
void draw_door() {
	/* �� ������ ���� */
	glPushMatrix();
	glTranslatef(-25.0f, -12.5f, 15.0f);
	glRotatef(angle0, 0, 1, 0);
	glPushMatrix();
	/* ���� ���� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[30]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glEnd();

	/* ���� ���� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[31]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 25.0f, 0.0f);
	glEnd();

	/* ���� ���� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[33]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glEnd();

	/* ���� ���� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[32]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 25.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 25.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 25.0f, 0.0f);
	glEnd();

	/* ���� ���� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[34]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(40.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 20.0f, 3.0f);
	glEnd();

	/* ���� ���� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[35]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 20.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* ���� ���� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[65]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 20.0f, 3.0f);
	glEnd();

	/* ���� ���� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[66]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 20.0f, 0.0f);
	glEnd();

	/* ���� ���� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[67]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glEnd();

	/* ���� ���� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[68]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 20.0f, 0.0f);
	glEnd();

	/* ���� ���� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[69]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 5.0f, 3.0f);
	glEnd();

	/* ���� ���� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[70]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 5.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* ���� ������ �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[71]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(35.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(35.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 20.0f, 3.0f);
	glEnd();

	/* ���� ������ �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[72]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 20.0f, 0.0f);
	glEnd();

	/* ���� ������ �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[73]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glEnd();

	/* ���� ������ �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[74]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(35.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 20.0f, 0.0f);
	glEnd();

	/* ���� ������ �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[75]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(35.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 5.0f, 3.0f);
	glEnd();

	/* ���� ������ �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[76]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(35.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(35.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 5.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* ���� �Ʒ��� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[77]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 5.0f, 3.0f);
	glEnd();

	/* ���� �Ʒ��� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[78]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 5.0f, 0.0f);
	glEnd();

	/* ���� �Ʒ��� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[80]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();

	/* ���� �Ʒ��� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[79]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();

	/* ���� �Ʒ��� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[81]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 0.0f, 3.0f);
	glEnd();

	/* ���� �Ʒ��� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[82]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* ���� ������ �������� �ո�*/
	glBindTexture(GL_TEXTURE_2D, texName[83]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glEnd();

	/* ���� ������ �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[84]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}


/* ���ۺ� �� ���� ���� ���̾� �׸��� �Լ� */
void draw_handle() {
	/* ���ۺ� ������ ���� */
	glPushMatrix();
	glTranslatef(15.0f, -12.5f, 15.0f);
	/* ���ۺ� �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[36]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glEnd();

	/* ���ۺ� �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[37]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(10.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 25.0f, 0.0f);
	glEnd();

	/* ���ۺ� �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[39]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(10.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();

	/* ���ۺ� �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[38]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 25.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 25.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 25.0f, 0.0f);
	glEnd();

	/* ���ۺ� �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[40]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 3.0f);
	glEnd();

	/* ���ۺ� �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[41]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	/* ���� ���� ���̾� ������ */
	glPushMatrix();
	glTranslatef(20.0f, -5.0f, 18.0f);
	glRotatef(angle1, 0, 0, 1);
	/* ���� ���� ���̾� �������� �Ǹ��� */
	glBindTexture(GL_TEXTURE_2D, quaName[0]); /* �ؽ�ó ���ε� ���� */
	gluCylinder(m_pQuadric, 2, 2, 2, 50, 50);
	/* ���� ���� ���̾� �������� ���� */
	glTranslatef(0, 0, 2);
	glBindTexture(GL_TEXTURE_2D, quaName[1]); /* �ؽ�ó ���ε� ���� */
	gluDisk(m_pQuadric, 0, 2, 50, 50);
	/* ���� ���� ���̾� �������� �Ʒ��� */
	glTranslatef(0, 0, -2);
	glBindTexture(GL_TEXTURE_2D, quaName[2]); /* �ؽ�ó ���ε� ���� */
	gluDisk(m_pQuadric, 0, 2, 50, 50);
	glPopMatrix();
}


/* ȸ������ �׸��� �Լ� */
void draw_dish() {
	/* ȸ������ ������ */
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(-5.5f, 0.0f, -9.5f);
	glRotatef(angle2, 0, 0, 1);
	glPushMatrix();
	/* ȸ������ �������� �Ǹ��� */
	glBindTexture(GL_TEXTURE_2D, quaName[3]); /* �ؽ�ó ���ε� ���� */
	gluCylinder(m_pQuadric, 12, 12, 1, 50, 50);
	/* ȸ������ �������� ���� */
	glTranslatef(0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, quaName[4]); /* �ؽ�ó ���ε� ���� */
	gluDisk(m_pQuadric, 0, 12, 50, 50);
	/* ȸ������ �������� �Ʒ��� */
	glTranslatef(0, 0, -1);
	glBindTexture(GL_TEXTURE_2D, quaName[5]); /* �ؽ�ó ���ε� ���� */
	gluDisk(m_pQuadric, 0, 12, 50, 50);
	glPopMatrix();
	glPopMatrix();
}


/* ���, ���۹�ư �׸��� �Լ� */
void draw_button() {
	glPushMatrix();
	glTranslatef(15.0f, -12.5f, 15.0f); /* ���, ���۹�ư ��ġ ���� */
	/* ��ҹ�ư */
	/* ��ҹ�ư �������� ���ʸ� */
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texName[53]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(2.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(2.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(2.0f, 4.2f, 3.5f);
	glEnd();

	/* ��ҹ�ư �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[54]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 4.2f, 3.0f);
	glEnd();

	/* ��ҹ�ư �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[56]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(2.0f, 3.4f, 3.5f);
	glEnd();

	/* ��ҹ�ư �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[55]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(2.0f, 4.2f, 3.0f);
	glEnd();

	/* ��ҹ�ư �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[57]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(2.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glEnd();

	/* ��ҹ�ư �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[58]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(2.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.0f);
	glEnd();
	glPopMatrix();

	/* ���۹�ư */
	glPushMatrix();
	/* ���۹�ư �������� ���ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[59]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glEnd();

	/* ���۹�ư �������� �����ʸ� */
	glBindTexture(GL_TEXTURE_2D, texName[60]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(8.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(8.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(8.0f, 4.2f, 3.0f);
	glEnd();

	/* ���۹�ư �������� �Ʒ���*/
	glBindTexture(GL_TEXTURE_2D, texName[62]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(8.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(8.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glEnd();

	/* ���۹�ư �������� ���� */
	glBindTexture(GL_TEXTURE_2D, texName[61]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(8.0f, 4.2f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 4.2f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 4.2f, 3.0f);
	glEnd();

	/* ���۹�ư �������� �ո� */
	glBindTexture(GL_TEXTURE_2D, texName[63]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(8.0f, 3.4f, 3.5f);
	glEnd();

	/* ���۹�ư �������� �޸�*/
	glBindTexture(GL_TEXTURE_2D, texName[64]); /* �ؽ�ó ���ε� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(8.0f, 3.4f, 3.0f);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}


/* �������� ȿ�� �Լ� */
void In_light() {
	glPushMatrix();
	/* ���������� ��ġ ���� */
	glTranslatef(-5.0f, 0.0f, 0.0f);
	GLfloat light_position1[] = { 0, 0, 0, 1.0 }; /* LIGHT1������ ��ġ�� ��ǥ���� �߽����� ������ִ� ��� */
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	/* power�� 0�϶� ��, ON����϶� �������� Ȱ��ȭ */
	if (power == 0) {
		glEnable(GL_LIGHT1);
	}
	/* OFF����϶� �������� ��Ȱ��ȭ */
	else if (power == 1) {
		glDisable(GL_LIGHT1);
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); /* �÷� �ʱ�ȭ */
	glPopMatrix();
}


/* Ÿ�̸� ������ �׸��� �Լ� */
void draw_timer()
{
	glPushMatrix();
	glTranslatef(15.0f, -12.5f, 15.0f);
	/* Ÿ�̸� �������� �ո� */
	/* timer ������ ���� Ÿ�̸ӿ� ǥ�õ� �ؽ�ó�� �޶��� */
	if (power == 0) {
		if (timer == 1)
			glBindTexture(GL_TEXTURE_2D, texName[43]); /* Ÿ�̸�1���� ���� */
		else if (timer == 2)
			glBindTexture(GL_TEXTURE_2D, texName[44]); /* Ÿ�̸�2���� ���� */
		else if (timer == 3)
			glBindTexture(GL_TEXTURE_2D, texName[45]); /* Ÿ�̸�3���� ���� */
		else if (timer == 4)
			glBindTexture(GL_TEXTURE_2D, texName[46]); /* Ÿ�̸�4���� ���� */
		else if (timer == 5)
			glBindTexture(GL_TEXTURE_2D, texName[47]); /* Ÿ�̸�5���� ���� */
		else if (timer == 6)
			glBindTexture(GL_TEXTURE_2D, texName[48]); /* Ÿ�̸�6���� ���� */
		else if (timer == 7)
			glBindTexture(GL_TEXTURE_2D, texName[49]); /* Ÿ�̸�7���� ���� */
		else if (timer == 8)
			glBindTexture(GL_TEXTURE_2D, texName[50]); /* Ÿ�̸�8���� ���� */
		else if (timer == 9)
			glBindTexture(GL_TEXTURE_2D, texName[51]); /* Ÿ�̸�9���� ���� */
		else if (timer == 10)
			glBindTexture(GL_TEXTURE_2D, texName[52]); /* Ÿ�̸�10���� ���� */
		else
			glBindTexture(GL_TEXTURE_2D, texName[42]); /* Ÿ�̸�0���� ���� */
	}
	else glBindTexture(GL_TEXTURE_2D, texName[85]); /* Ÿ�̸� OFF�� ���� */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(7.5f, 23.0f, 3.01f);
	glTexCoord2f(0, 1); glVertex3f(2.5f, 23.0f, 3.01f);
	glTexCoord2f(0, 0); glVertex3f(2.5f, 21.0f, 3.01f);
	glTexCoord2f(1, 0); glVertex3f(7.5f, 21.0f, 3.01f);
	glEnd();

	glPopMatrix();
}

/* Ÿ�̸Ӹ� ����ϴ� �Լ� */
void count_timer(int value)
{
	/* power�� ON�϶� �۵����� ���� */
	if (power == 0) {
		if (timer > 10) timer = 10; /* timer�� 10���� ũ�� 10���� ����*/
		else if (timer < 0) timer = 0; /* timer�� 0���� ������ 0���� ���� */
		/* timer�� 0�̵Ǹ� ���ڷ����� �۵��� �ߴܵ� */
		else if (timer == 0)
		{
			start = 0; /* �۵��� �ߴܵ� ���� */
			glutIdleFunc(idle_stop); /* idle�Լ� ��Ȱ��ȭ */
		}
		else {
			/* �۵��ϴ� ���¿����� timer�� ���������� 1�� �����ϵ��� ���� */
			if (value > 0 && start == 1 ) {
				glutTimerFunc(1000, count_timer, 1); /* 1�ʸ��� count_timer�Լ� ȣ�� */
				timer--; /* timer�� 1�� ���� */
			}
		}
	}
}

/* ���� ���� ���� */
/* Viewport �� projection transform�� ���α׷��� ó�� ���۵ɶ� resize�� �ʱ�ȭ
�Ǵ� ������ ũ�Ⱑ ����ɶ� resize�Լ����� ���� */
void resize(int width, int height)
{
	glViewport(0, 0, width, height); /* Viewport T.F ���� */
	glMatrixMode(GL_PROJECTION); /* GL_Projection(����)�� ���̾� ��Ÿ���� ��� ����� ������Ŀ� ������ �� */
	glLoadIdentity(); /* ��� �ʱ�ȭ */
	gluPerspective(45, (float)width / (float)height, 1, 500); /* Projection T.F ���� */
	glMatrixMode(GL_MODELVIEW); /* GL_MODELVIEW(��ü�̵�)�� ���̾� ��Ÿ���� ��� ����� �𵨺���Ŀ� ������ �� */
}

/* Ű�����Է� ó���Լ� */
void keyboard(unsigned char key, int x, int y)
{
	/* '1'�Է½� ��� �ִ� 90���� ���� */
	if (key == '1') {
		if (angle0 <= -90) angle0 = -90;
		else angle0 -= 1;
	}
	/* '2'�Է½� ��� 0������ ���� */
	else if (key == '2') {
		if (angle0 >= 0) angle0 = 0;
		else angle0 += 1;
	}
	/* q�Է½� ���� ���̾��� ���������� ȸ�� �� timer�� 10���� ����*/
	else if (key == 'q') {
		angle1 -= 70;
		if (power == 0) {
			timer = 10;
		}
	}
	/* w�Է½� ���� ���̾��� �������� ȸ�� �� timer�� 0���� ���� */
	else if (key == 'w') {
		angle1 += 70;
		timer = 0;
	}
	/* e�Է½� ȸ������ �������� �������� ȸ�� */
	else if (key == 'e') {
		angle2 -= 5;
	}
	/* r�Է½� ȸ������ �������� ���������� ȸ�� */
	else if (key == 'r') {
		angle2 += 5;
	}
	/* z�Է½� ȸ������ �ڵ�ȸ�� �� Ÿ�̸� ���� */
	else if (key == 'z') {
		start = 1; /* �۵��� ���۵� ���¸� �ǹ� */
		/* power�� on�̰� timer�� 0���� Ŭ���� �۵��ϵ��� ���� */
		if (power == 0 && timer > 0) {
			glutTimerFunc(1000, count_timer, 1); /* 1�ʸ��� timer�� 1�� �ٵ����� */
			glutIdleFunc(idle); /* idle�Լ� ���� */
		}
	}
	/* x�Է½� Ÿ�̸� �ӽ� �ߴ� �� ȸ�������� ȸ�� �ߴ� */
	else if (key == 'x') {
		start = 0; /* �۵��� �ߴܵ� ���¸� �ǹ� */
		glutIdleFunc(idle_stop); /* idle ��Ȱ��ȭ */
	}
	glutPostRedisplay();
}

/* idle�Լ� */
void idle(void)
{
	if (timer > 0)
		angle2 += 1.0; /* ȸ�����ð� ȸ���ϵ��� �Ѵ� */
	else timer = 0;
	glutPostRedisplay(); /* glutDisplayFunc�� ��ϵ� �Լ��� ȣ����Ѽ� â�� �ٽ� �׷��ش�. */
}


/* idle�Լ��� ��Ȱ��ȭ �ϱ����� idle_stop�Լ� */
void idle_stop()
{
	glutPostRedisplay();
}


/* �˾��޴� Ŭ���� �߻��ϴ� �Լ� */
void main_menu_function(int option)
{
	/* �˾��޴� ON �Է½� */
	if (option == 1) {
		power = 0; /* power���ڰ� 0�̶�� ���� ON�� ���� */
	}
	/* �˾��޴� OFF �Է½� */
	else if (option == 2) {
		power = 1; /* power���ڰ� 1�̶�� ���� OFF�� ���� */
	}
	glutPostRedisplay();
}


int temp_x = 0; /* ���� Ŭ���� x�� ��ǥ ���� */
int temp_y = 0; /* ���� Ŭ���� y�� ��ǥ ���� */

void mouse(int button, int state, int x, int y)
{
	/* ���콺 ���� ��ư �������� mbutton�� 1�� �ٲ� */
	if (button == GLUT_LEFT_BUTTON) mbutton = 1;
	/* ���콺 ������ ��ư �������� mbutton�� 2�� �ٲ� */
	else if (button == GLUT_RIGHT_BUTTON) mbutton = 2;
}

void motion(int x, int y)
{
	if (mbutton == 1) {
		if (temp_x < x) {
			lootat_azimuth -= M_PI / 180; /* ī�޶��� �浵�� ���� */
		}
		else if (temp_x > x) {
			lootat_azimuth += M_PI / 180; /* ī�޶��� �浵�� ���� */
		}
		if (y > temp_y) {
			lootat_elevation -= M_PI / 180; /* ī�޶� ���� ����*/
			if (upy == 1 && lootat_elevation < 0) /* centery�� 1�̸鼭 ������ 0������ ������ */
			{
				upy = -1; lootat_elevation += 2 * M_PI;
			} /* upy�� -1�� �ٲٰ� ���� ������ 2���̸�ŭ ���Ѵ�. */
			  /* upy�� -1�̸鼭 ������ ���̺��� ������  upy�� 1�� �ٲ۴�. */
			else if (upy == -1 && lootat_elevation < M_PI) upy = 1;
		}
		else if (y < temp_y) {
			lootat_elevation += M_PI / 180; /* ī�޶� ���� ���� */
											/* upy�� 1�̸鼭 ������ ���̺��� Ŭ�� upy�� -1�� �ٲ۴�. */
			if (upy == 1 && lootat_elevation > M_PI) { upy = -1; }
			/* upy�� -1�̸鼭 ������ 2���̺��� Ŭ�� upy�� 1�� �ٲٰ� ���� ������ 2���̸� ���� */
			else if (upy == -1 && lootat_elevation > 2 * M_PI) { upy = 1; lootat_elevation -= 2 * M_PI; }
		}
	}
	else if (mbutton == 2) {
		if (temp_x < x) {
			lootat_r += 1; /* r���� �÷��� �� �ƿ� ��� */
		}
		else if (temp_x > x) {
			lootat_r -= 1; /* r���� �ٿ��� �� �� ��� */
		}
	}
	temp_x = x; /* ���� Ŭ���� x�� ��ǥ ���� */
	temp_y = y; /* ���� Ŭ���� y�� ��ǥ ���� */
	glutPostRedisplay();
}

/* Ư��Ű�� ���� �ݹ� �Լ� */
void specialkeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) /* ���� ����Ű�� �������� */
	{
		Tx -= 5; /* ���ڷ������� �������� �̵� */
	}
	else if (key == GLUT_KEY_RIGHT) /* ������ ����Ű�� �������� */
	{
		Tx += 5; /* ���ڷ������� ���������� �̵� */
	}
	else if (key == GLUT_KEY_UP) /* ���� ����Ű�� �������� */
	{
		Ty += 5; /* ���ڷ������� �������� �̵� */
	}
	else if (key == GLUT_KEY_DOWN) /* �Ʒ��� ����Ű�� �������� */
	{
		Ty -= 5; /* ���ڷ������� �Ʒ������� �̵� */
	}
	else if (key == GLUT_KEY_PAGE_UP) /* PAGE_UP�� �������� */
	{
		Tz += 5; /* ���ڷ������� �������� �̵� */
	}
	else if (key == GLUT_KEY_PAGE_DOWN) /* PAGE_DOWN�� �������� */
	{
		Tz -= 5; /* ���ڷ������� �������� �̵� */
	}
	glutPostRedisplay(); /* glutDisplayFunc�� ��ϵ� �Լ��� ȣ����Ѽ� â�� �ٽ� �׷��ش�. */
}