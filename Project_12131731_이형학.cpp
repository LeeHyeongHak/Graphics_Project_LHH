#pragma comment(lib,"legacy_stdio_definitions.lib")
#include <Windows.h>
#include <gl/glut.h>
#include <gl/GLAUX.H>
#include <stdio.h>
#define _USE_MATH_DEFINES /* 파이값 사용하기 위해서 정의 */
#include <math.h>

GLUquadricObj* m_pQuadric; /* GLUQuadic 객체 선언 */

GLuint texName[86];
GLuint quaName[5];

GLfloat Tx = 0, Ty = 0, Tz = 0;

float eyex = 0; /* gluLookAt함수에서 eyex인자의 값 선언 */
float eyey = 0; /* gluLookAt함수에서 eyey인자의 값 선언 */
float eyez = 0; /* gluLookAt함수에서 eyez인자의 값 선언 */
float lootat_r = 100; /* 좌표계 중심에서부터의 반지름 */
float lootat_elevation = M_PI / 4; /* 좌표계중심에서 경도 */
float lootat_azimuth = M_PI / 4; /* 좌표계중심에서 위도 */
float upy = 1; /* gluLooAt함수에서 upy인자의 값 선언 */

float angle0 = 0; /* 문의 회전을 위한 각도 인자 */
float angle1 = 0; /* 수동조리다이얼의 회전을 위한 각도 인자 */
float angle2 = 0; /* 회전접시의 회전을 위한 각도 인자 */

int mbutton = 0; /* 마우스 왼쪽, 오른쪽 클릭을 구분하기 위한 인자 */
int start = 0; /* 전원 ON모드시 타이머, 회전접시 동작 여부를 위한 인자 */
int power = 1; /* 전원 ON, OFF모드를 구분하기 위한 인자 */
int timer = 0; /* 조작부의 타이머에 출력할 타이머에 대한 인자 */

void init(void); /* 초기화 함수 */
void draw(void); /* 그리기 함수 */
void draw_body(); /* 본체 그리기 함수 */
void draw_door(); /* 문 그리기 함수 */
void draw_handle(); /* 조작부 그리기 함수 */
void draw_dish(); /* 회전접시 그리기 함수 */
void draw_button(); /* 취소, 시작버튼 그리기 함수 */
void In_light(); /* 내부조명효과 함수 */
void draw_timer(); /* 타이머 그리기 함수 */
void count_timer(int value); /* 타이머 계산 함수 */
void resize(int width, int height); /* Viewport 및 projection transform을 프로그램이 처음 시작될때
									resize로 초기화 또는 윈도우 크기가 변경될때 resize함수에서 구현 */
void keyboard(unsigned char key, int x, int y); /* 키보드 처리 함수 */
void idle(void); /* idle함수 */
void idle_stop(); /* idle비활성화 함수 */
void main_menu_function(int option); /* 팝업메뉴 함수 */
void mouse(int button, int state, int x, int y); /* 마우스 함수 */
void motion(int x, int y); /* 모션함수 */
void specialkeyboard(int key, int x, int y); /* 특수키 함수 */

int main(int argc, char** argv)
{
	/* Window 초기화 */
	glutInit(&argc, argv); /* glutInit은 GLUT의 상태변수를 초기화하는 역할과
						   윈도우시스템의 세션을 처리함 */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); /* glutInitDisplayMode는 디스플레이 모드를 나타낸다.*/
	glutInitWindowSize(500, 500); /* glutInitWindowSize는 창의 크기를 결정*/
	glutInitWindowPosition(300, 300); /* glutInitWindowPosition은 윈도우에서 실행되는 창의 위치를 결정 */
	glutCreateWindow("12131731 이형학"); /* glutCreateWindow는 괄호안의 내용을 제목으로 갖는 창을 연다.*/

	init();/* 초기화 함수 */

	int submenu1;
	/* Popup menu 생성 및 추가 */
	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("ON", 1);
	glutAddMenuEntry("OFF", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON); /* 메뉴를 마우스 버튼에 부착한다. */

	/* Callback 함수 정의(Callback함수란 어떠한 정보를 관리하는 대상이 자신의 정보가 변경되거나 또는
	이벤트가 발생할떄의 변경된 정보나 이벤트에 따른 어떠한 처리를 할 수 있도록 제공하는 함수) */
	glutDisplayFunc(draw); /* draw() -> 실제 그리기 함수 */

	/* Callback 함수 등록 */
	glutReshapeFunc(resize); /* 창 크기가 변경되는거에 대한 콜백 함수  */
	glutKeyboardFunc(keyboard); /* keyboard callback 함수 */
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpecialFunc(specialkeyboard); /* 함수키, 방향키 등 특수키에 대한 콜백 함수 */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Looping 시작 */
	glutMainLoop();

	return 0;
}

/* 사용자 초기화 함수*/
void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); /* 배경을 흰색으로 만듦 */
	glClearDepth(1.0); /* depth 초기화 */
	glFrontFace(GL_CW); /* 도형을 CW기준으로 그림 */
	glEnable(GL_DEPTH_TEST); /* Depth 활성화 */
	glShadeModel(GL_SMOOTH); /* 쉐이더모드를 SMOOTH로 설정 */

	/* Material Properties 설정 */
	GLfloat ambient_Material[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse_Material[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular_Material[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Material);
	glMateriali(GL_FRONT, GL_SHININESS, 256);

	glEnable(GL_COLOR_MATERIAL); /* COLOR MATERIAL 활성화 */
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); /* glColor값 활성화 */

	/* 외부 조명(0번 조명)의 light 설정 */
	GLfloat ambient0[] = { 0.2f, 0.2f, 0.2f, 1.0f }; /* LIGHT0의 ambient light값의 행렬  */
	GLfloat diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* LIGHT0의 difuuse light값의 행렬 */
	GLfloat specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* LIGHT0의 specular light값의 행렬 */

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0); /* LIGHT0의 ambient light값을 설정 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0); /* LIGHT0의 diffuse lihgt값을 설정 */
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0); /* LIGHT0의 specular light값을 설정 */

	glEnable(GL_LIGHTING); /* Lighting mode의 활성화 */
	glEnable(GL_LIGHT0); /* LIGHT0의 광원을 킴 */

	/* 내부 조명(1번조명)의 light 설정 */
	GLfloat ambient1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; /* LIGHT1의 ambient light값의 행렬  */
	GLfloat diffuse1[] = { 1.0f, 0.6f, 0.2f, 1.0f }; /* LIGHT1의 difuuse light값의 행렬 */
	GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* LIGHT1의 specular light값의 행렬 */

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1); /* LIGHT1의 ambient light값을 설정 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1); /* LIGHT1의 diffuse lihgt값을 설정 */
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1); /* LIGHT1의 specular light값을 설정 */

	/* 텍스처 초기화 */
	glGenTextures(86, texName); /* 텍스처 객체를 저장하는 텍스처 아이디 생성 */
	AUX_RGBImageRec * pRGBImage;


	/* 뒤쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	pRGBImage = auxDIBImageLoadA("뒤쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	pRGBImage = auxDIBImageLoadA("뒤쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	pRGBImage = auxDIBImageLoadA("뒤쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	pRGBImage = auxDIBImageLoadA("뒤쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	pRGBImage = auxDIBImageLoadA("뒤쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[5]);
	pRGBImage = auxDIBImageLoadA("뒤쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 위쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[6]);
	pRGBImage = auxDIBImageLoadA("위쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[7]);
	pRGBImage = auxDIBImageLoadA("위쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[8]);
	pRGBImage = auxDIBImageLoadA("위쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[9]);
	pRGBImage = auxDIBImageLoadA("위쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[10]);
	pRGBImage = auxDIBImageLoadA("위쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[11]);
	pRGBImage = auxDIBImageLoadA("위쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 왼쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[12]);
	pRGBImage = auxDIBImageLoadA("왼쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[13]);
	pRGBImage = auxDIBImageLoadA("왼쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[14]);
	pRGBImage = auxDIBImageLoadA("왼쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[15]);
	pRGBImage = auxDIBImageLoadA("왼쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[16]);
	pRGBImage = auxDIBImageLoadA("왼쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[17]);
	pRGBImage = auxDIBImageLoadA("왼쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 오른쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[18]);
	pRGBImage = auxDIBImageLoadA("오른쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[19]);
	pRGBImage = auxDIBImageLoadA("오른쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[20]);
	pRGBImage = auxDIBImageLoadA("오른쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[21]);
	pRGBImage = auxDIBImageLoadA("오른쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[22]);
	pRGBImage = auxDIBImageLoadA("오른쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[23]);
	pRGBImage = auxDIBImageLoadA("오른쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 아래쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[24]);
	pRGBImage = auxDIBImageLoadA("아래쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[25]);
	pRGBImage = auxDIBImageLoadA("아래쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[27]);
	pRGBImage = auxDIBImageLoadA("아래쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[26]);
	pRGBImage = auxDIBImageLoadA("아래쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[28]);
	pRGBImage = auxDIBImageLoadA("아래쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[29]);
	pRGBImage = auxDIBImageLoadA("아래쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 도어위쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[30]);
	pRGBImage = auxDIBImageLoadA("도어위쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[31]);
	pRGBImage = auxDIBImageLoadA("도어위쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[32]);
	pRGBImage = auxDIBImageLoadA("도어위쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[33]);
	pRGBImage = auxDIBImageLoadA("도어위쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[34]);
	pRGBImage = auxDIBImageLoadA("도어위쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[35]);
	pRGBImage = auxDIBImageLoadA("도어위쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 도어왼쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[65]);
	pRGBImage = auxDIBImageLoadA("도어왼쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[66]);
	pRGBImage = auxDIBImageLoadA("도어왼쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[67]);
	pRGBImage = auxDIBImageLoadA("도어왼쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[68]);
	pRGBImage = auxDIBImageLoadA("도어왼쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[69]);
	pRGBImage = auxDIBImageLoadA("도어왼쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[70]);
	pRGBImage = auxDIBImageLoadA("도어왼쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 도어오른쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[71]);
	pRGBImage = auxDIBImageLoadA("도어오른쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[72]);
	pRGBImage = auxDIBImageLoadA("도어오른쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[73]);
	pRGBImage = auxDIBImageLoadA("도어오른쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[74]);
	pRGBImage = auxDIBImageLoadA("도어오른쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[75]);
	pRGBImage = auxDIBImageLoadA("도어오른쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[76]);
	pRGBImage = auxDIBImageLoadA("도어오른쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/* 도어아래쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[77]);
	pRGBImage = auxDIBImageLoadA("도어아래쪽-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[78]);
	pRGBImage = auxDIBImageLoadA("도어아래쪽-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[79]);
	pRGBImage = auxDIBImageLoadA("도어아래쪽-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[80]);
	pRGBImage = auxDIBImageLoadA("도어아래쪽-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[81]);
	pRGBImage = auxDIBImageLoadA("도어아래쪽-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[82]);
	pRGBImage = auxDIBImageLoadA("도어아래쪽-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 도어아래쪽 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[83]);
	pRGBImage = auxDIBImageLoadA("도어투명-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[84]);
	pRGBImage = auxDIBImageLoadA("도어투명-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 조작부 폴리곤 텍스처 매핑 저장 */
	glBindTexture(GL_TEXTURE_2D, texName[36]);
	pRGBImage = auxDIBImageLoadA("조작부-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[37]);
	pRGBImage = auxDIBImageLoadA("조작부-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[38]);
	pRGBImage = auxDIBImageLoadA("조작부-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[39]);
	pRGBImage = auxDIBImageLoadA("조작부-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[40]);
	pRGBImage = auxDIBImageLoadA("조작부-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[41]);
	pRGBImage = auxDIBImageLoadA("조작부-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 타이머(0~10초) 폴리곤 */
	glBindTexture(GL_TEXTURE_2D, texName[42]);
	pRGBImage = auxDIBImageLoadA("타이머0.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[43]);
	pRGBImage = auxDIBImageLoadA("타이머1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[44]);
	pRGBImage = auxDIBImageLoadA("타이머2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[45]);
	pRGBImage = auxDIBImageLoadA("타이머3.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[46]);
	pRGBImage = auxDIBImageLoadA("타이머4.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[47]);
	pRGBImage = auxDIBImageLoadA("타이머5.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[48]);
	pRGBImage = auxDIBImageLoadA("타이머6.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[49]);
	pRGBImage = auxDIBImageLoadA("타이머7.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[50]);
	pRGBImage = auxDIBImageLoadA("타이머8.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[51]);
	pRGBImage = auxDIBImageLoadA("타이머9.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[52]);
	pRGBImage = auxDIBImageLoadA("타이머10.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[85]);
	pRGBImage = auxDIBImageLoadA("타이머-OFF.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 취소버튼 폴리곤 */
	glBindTexture(GL_TEXTURE_2D, texName[53]);
	pRGBImage = auxDIBImageLoadA("취소버튼-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[54]);
	pRGBImage = auxDIBImageLoadA("취소버튼-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[55]);
	pRGBImage = auxDIBImageLoadA("취소버튼-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[56]);
	pRGBImage = auxDIBImageLoadA("취소버튼-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[57]);
	pRGBImage = auxDIBImageLoadA("취소버튼-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[58]);
	pRGBImage = auxDIBImageLoadA("취소버튼-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 확인버튼 폴리곤 */
	glBindTexture(GL_TEXTURE_2D, texName[59]);
	pRGBImage = auxDIBImageLoadA("확인버튼-왼쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[60]);
	pRGBImage = auxDIBImageLoadA("확인버튼-오른쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[61]);
	pRGBImage = auxDIBImageLoadA("확인버튼-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[62]);
	pRGBImage = auxDIBImageLoadA("확인버튼-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[63]);
	pRGBImage = auxDIBImageLoadA("확인버튼-앞쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName[64]);
	pRGBImage = auxDIBImageLoadA("확인버튼-뒤쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); /* 폴리곤의 원래 색상은 무시하고 텍스처로 덮음 */
	glEnable(GL_TEXTURE_2D);/* 텍스처 활성화 */

	m_pQuadric = gluNewQuadric(); /* GLUQuadic 객체 생성 */

	gluQuadricTexture(m_pQuadric, GL_TRUE); /* 텍스처 매핑 좌표 생성 */
	glGenTextures(5, quaName); /* 텍스처 객체를 저장하는 텍스처 아이디 생성 */


	/* 다이얼 폴리곤 */
	glBindTexture(GL_TEXTURE_2D, quaName[0]);
	pRGBImage = auxDIBImageLoadA("다이얼-실린더.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, quaName[1]);
	pRGBImage = auxDIBImageLoadA("다이얼-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, quaName[2]);
	pRGBImage = auxDIBImageLoadA("다이얼-아래쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	/* 회전접시 폴리곤 */
	glBindTexture(GL_TEXTURE_2D, quaName[3]);
	pRGBImage = auxDIBImageLoadA("회전접시-실린더.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, quaName[4]);
	pRGBImage = auxDIBImageLoadA("회전접시-위쪽면.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glEnable(GL_BLEND); /* BLEND효과 활성화*/
}


/* 그리기 함수 */
void draw(void)
{
	eyex = lootat_r * sin(lootat_elevation) * sin(lootat_azimuth); /* gluLookAt함수에서 eyex인자의 값에 해당하는 식 */
	eyey = lootat_r * cos(lootat_elevation); /* gluLookAt함수에서 eyey인자의 값에 해당하는 식 */
	eyez = lootat_r * sin(lootat_elevation) * cos(lootat_azimuth);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/* 화면을 깨끗하게 지우기 */

	glMatrixMode(GL_MODELVIEW); /* glMatrixMode함수는 행렬모드를 설정,
								GL_MODELVIEW모드는 3D공간에 물체의 배치를 수학적으로 표현한 행렬이다.
								(실제 화면상의 물체에 대한 좌표점을 설정하기 위한 행렬) */
	glLoadIdentity(); /* glLoadIdentity함수는 단위행렬을 곱한다. 즉, 행렬을 초기화 */
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upy, 0); /* gluLookAt함수는 카메라의 위치를 결정하는 함수 */

	glPushMatrix();
	glTranslatef(Tx, Ty, Tz); /* 전자레인지 이동 */
	In_light(); /* 내부조명 */
	draw_timer(); /* 타이머 폴리곤 */
	draw_body(); /* 본체 폴리곤 */
	draw_handle(); /* 조작부 폴리곤 */
	draw_dish(); /* 회전접시 폴리곤 */
	draw_button(); /* 취소,시작버튼 폴리곤 */
	draw_door(); /* 문 폴리곤 생성 */
	glPopMatrix();

	glFlush(); /* glFush는 gl명령이 완전히 실행되기까지 기다린 후
			   쌓여있는 명령들을 모두 비우고 즉시 실행이 되도록 한다. */
	glutSwapBuffers(); /* Back buffer를 front buffer로 바꿈 */
}


/* 본체(위,옆,뒤,아랫면) 그리기 함수 */
void draw_body() {
	glPushMatrix();
	/* 뒤쪽 폴리곤 구현 */
	glPushMatrix();
	/* 뒤쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[0]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glEnd();

	/* 뒤쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[1]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 12.5f, -15.0f);
	glEnd();

	/* 뒤쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[2]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -12.5f, -15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, -15.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glEnd();

	/* 뒤쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[3]); /* 텍스처 바인딩 수행 */;
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 12.5f, -15.0f);
	glEnd();

	/* 뒤쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[4]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, -12.0f);
	glEnd();

	/* 뒤쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[5]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, -15.0f);
	glEnd();
	glPopMatrix();

	/* 위쪽 폴리곤 구현 */
	glPushMatrix();
	/* 위쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[6]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 12.5f, 15.0f);
	glEnd();

	/* 위쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[7]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 12.5f, -12.0f);
	glEnd();

	/* 위쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[9]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glEnd();

	/* 위쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[8]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 12.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 12.5f, -12.0f);
	glEnd();

	/* 위쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[10]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 9.5f, 15.0f);
	glEnd();

	/* 위쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[11]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 9.5f, -12.0f);
	glEnd();
	glPopMatrix();

	/* 왼쪽 폴리곤 구현 */
	glPushMatrix();
	/* 왼쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[12]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, 9.5f, 15.0f);
	glEnd();

	/* 왼쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[13]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-22.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-22.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-22.0f, 9.5f, -12.0f);
	glEnd();

	/* 왼쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[14]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-22.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-22.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glEnd();

	/* 왼쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[15]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-22.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, 9.5f, -12.0f);
	glEnd();

	/* 왼쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[16]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-22.0f, -9.5f, 15.0f);
	glEnd();

	/* 왼쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[17]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(-22.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-22.0f, -9.5f, -12.0f);
	glEnd();
	glPopMatrix();

	/* 오른쪽 폴리곤 구현 */
	glPushMatrix();
	/* 오른쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[18]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(12.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(12.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(12.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(12.0f, 9.5f, 15.0f);
	glEnd();

	/* 오른쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[19]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, 9.5f, -12.0f);
	glEnd();

	/* 오른쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[20]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(12.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(12.0f, -9.5f, 15.0f);
	glEnd();

	/* 오른쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[21]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(12.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(12.0f, 9.5f, -12.0f);
	glEnd();

	/* 오른쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[22]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(12.0f, 9.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(12.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -9.5f, 15.0f);
	glEnd();

	/* 오른쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[23]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(12.0f, 9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(12.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -9.5f, -12.0f);
	glEnd();
	glPopMatrix();

	/* 아래쪽 폴리곤 구현 */
	glPushMatrix();
	/* 아래쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[24]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, -9.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -9.5f, 15.0f);
	glEnd();

	/* 아래쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[25]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -9.5f, -12.0f);
	glEnd();

	/* 아래쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[26]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(-25.0f, -12.5f, -12.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -12.5f, -12.0f);
	glTexCoord2f(0, 0); glVertex3f(25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -12.5f, 15.0f);
	glEnd();

	/* 아래쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[27]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, -12.0f);
	glTexCoord2f(1, 0); glVertex3f(-25.0f, -9.5f, -12.0f);
	glEnd();

	/* 아래쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[28]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 1); glVertex3f(-25.0f, -9.5f, 15.0f);
	glTexCoord2f(0, 0); glVertex3f(-25.0f, -12.5f, 15.0f);
	glTexCoord2f(1, 0); glVertex3f(25.0f, -12.5f, 15.0f);
	glEnd();

	/* 아래쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[29]); /* 텍스처 바인딩 수행 */
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


/* 문 그리기 함수 */
void draw_door() {
	/* 문 폴리곤 구현 */
	glPushMatrix();
	glTranslatef(-25.0f, -12.5f, 15.0f);
	glRotatef(angle0, 0, 1, 0);
	glPushMatrix();
	/* 문의 위쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[30]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glEnd();

	/* 문의 위쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[31]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 25.0f, 0.0f);
	glEnd();

	/* 문의 위쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[33]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glEnd();

	/* 문의 위쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[32]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 25.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 25.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 25.0f, 0.0f);
	glEnd();

	/* 문의 위쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[34]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(40.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 20.0f, 3.0f);
	glEnd();

	/* 문의 위쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[35]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 20.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* 문의 왼쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[65]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 20.0f, 3.0f);
	glEnd();

	/* 문의 왼쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[66]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 20.0f, 0.0f);
	glEnd();

	/* 문의 왼쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[67]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glEnd();

	/* 문의 왼쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[68]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 20.0f, 0.0f);
	glEnd();

	/* 문의 왼쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[69]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(0.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 5.0f, 3.0f);
	glEnd();

	/* 문의 왼쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[70]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 5.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* 문의 오른쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[71]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(35.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(35.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 20.0f, 3.0f);
	glEnd();

	/* 문의 오른쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[72]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 20.0f, 0.0f);
	glEnd();

	/* 문의 오른쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[73]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glEnd();

	/* 문의 오른쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[74]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(35.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 20.0f, 0.0f);
	glEnd();

	/* 문의 오른쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[75]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(35.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 5.0f, 3.0f);
	glEnd();

	/* 문의 오른쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[76]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(35.0f, 20.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(35.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 5.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* 문의 아래쪽 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[77]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 5.0f, 3.0f);
	glEnd();

	/* 문의 아래쪽 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[78]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 5.0f, 0.0f);
	glEnd();

	/* 문의 아래쪽 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[80]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(40.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();

	/* 문의 아래쪽 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[79]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();

	/* 문의 아래쪽 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[81]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 1); glTexCoord2f(0, 1); glVertex3f(0.0f, 5.0f, 3.0f);
	glTexCoord2f(0, 0); glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 0.0f, 3.0f);
	glEnd();

	/* 문의 아래쪽 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[82]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(40.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(40.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	/* 문의 투명한 폴리곤의 앞면*/
	glBindTexture(GL_TEXTURE_2D, texName[83]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(35.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 20.0f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 5.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(35.0f, 5.0f, 3.0f);
	glEnd();

	/* 문의 투명한 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[84]); /* 텍스처 바인딩 수행 */
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


/* 조작부 및 수동 조리 다이얼 그리기 함수 */
void draw_handle() {
	/* 조작부 폴리곤 구현 */
	glPushMatrix();
	glTranslatef(15.0f, -12.5f, 15.0f);
	/* 조작부 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[36]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glEnd();

	/* 조작부 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[37]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(10.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 25.0f, 0.0f);
	glEnd();

	/* 조작부 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[39]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(10.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();

	/* 조작부 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[38]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(10.0f, 25.0f, 3.0f);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 25.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(0.0f, 25.0f, 0.0f);
	glEnd();

	/* 조작부 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[40]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 3.0f);
	glEnd();

	/* 조작부 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[41]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(10.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex3f(0.0f, 25.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();

	/* 수동 조리 다이얼 폴리곤 */
	glPushMatrix();
	glTranslatef(20.0f, -5.0f, 18.0f);
	glRotatef(angle1, 0, 0, 1);
	/* 수동 조리 다이얼 폴리곤의 실린더 */
	glBindTexture(GL_TEXTURE_2D, quaName[0]); /* 텍스처 바인딩 수행 */
	gluCylinder(m_pQuadric, 2, 2, 2, 50, 50);
	/* 수동 조리 다이얼 폴리곤의 윗면 */
	glTranslatef(0, 0, 2);
	glBindTexture(GL_TEXTURE_2D, quaName[1]); /* 텍스처 바인딩 수행 */
	gluDisk(m_pQuadric, 0, 2, 50, 50);
	/* 수동 조리 다이얼 폴리곤의 아랫면 */
	glTranslatef(0, 0, -2);
	glBindTexture(GL_TEXTURE_2D, quaName[2]); /* 텍스처 바인딩 수행 */
	gluDisk(m_pQuadric, 0, 2, 50, 50);
	glPopMatrix();
}


/* 회전접시 그리기 함수 */
void draw_dish() {
	/* 회전접시 폴리곤 */
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(-5.5f, 0.0f, -9.5f);
	glRotatef(angle2, 0, 0, 1);
	glPushMatrix();
	/* 회전접시 폴리곤의 실린더 */
	glBindTexture(GL_TEXTURE_2D, quaName[3]); /* 텍스처 바인딩 수행 */
	gluCylinder(m_pQuadric, 12, 12, 1, 50, 50);
	/* 회전접시 폴리곤의 윗면 */
	glTranslatef(0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, quaName[4]); /* 텍스처 바인딩 수행 */
	gluDisk(m_pQuadric, 0, 12, 50, 50);
	/* 회전접시 폴리곤의 아랫면 */
	glTranslatef(0, 0, -1);
	glBindTexture(GL_TEXTURE_2D, quaName[5]); /* 텍스처 바인딩 수행 */
	gluDisk(m_pQuadric, 0, 12, 50, 50);
	glPopMatrix();
	glPopMatrix();
}


/* 취소, 시작버튼 그리기 함수 */
void draw_button() {
	glPushMatrix();
	glTranslatef(15.0f, -12.5f, 15.0f); /* 취소, 시작버튼 위치 조정 */
	/* 취소버튼 */
	/* 취소버튼 폴리곤의 왼쪽면 */
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texName[53]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(2.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(2.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(2.0f, 4.2f, 3.5f);
	glEnd();

	/* 취소버튼 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[54]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 4.2f, 3.0f);
	glEnd();

	/* 취소버튼 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[56]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(2.0f, 3.4f, 3.5f);
	glEnd();

	/* 취소버튼 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[55]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(2.0f, 4.2f, 3.0f);
	glEnd();

	/* 취소버튼 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[57]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(2.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glEnd();

	/* 취소버튼 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[58]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(2.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(2.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.0f);
	glEnd();
	glPopMatrix();

	/* 시작버튼 */
	glPushMatrix();
	/* 시작버튼 폴리곤의 왼쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[59]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glEnd();

	/* 시작버튼 폴리곤의 오른쪽면 */
	glBindTexture(GL_TEXTURE_2D, texName[60]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(8.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(8.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 3.4f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(8.0f, 4.2f, 3.0f);
	glEnd();

	/* 시작버튼 폴리곤의 아랫면*/
	glBindTexture(GL_TEXTURE_2D, texName[62]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(5.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 1); glVertex3f(8.0f, 3.4f, 3.0f);
	glTexCoord2f(0, 0); glVertex3f(8.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glEnd();

	/* 시작버튼 폴리곤의 윗면 */
	glBindTexture(GL_TEXTURE_2D, texName[61]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(8.0f, 4.2f, 3.5f);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 4.2f, 3.0f);
	glTexCoord2f(1, 0); glVertex3f(5.0f, 4.2f, 3.0f);
	glEnd();

	/* 시작버튼 폴리곤의 앞면 */
	glBindTexture(GL_TEXTURE_2D, texName[63]); /* 텍스처 바인딩 수행 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(8.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 1); glVertex3f(5.0f, 4.2f, 3.5f);
	glTexCoord2f(0, 0); glVertex3f(5.0f, 3.4f, 3.5f);
	glTexCoord2f(1, 0); glVertex3f(8.0f, 3.4f, 3.5f);
	glEnd();

	/* 시작버튼 폴리곤의 뒷면*/
	glBindTexture(GL_TEXTURE_2D, texName[64]); /* 텍스처 바인딩 수행 */
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


/* 내부조명 효과 함수 */
void In_light() {
	glPushMatrix();
	/* 내부조명의 위치 조정 */
	glTranslatef(-5.0f, 0.0f, 0.0f);
	GLfloat light_position1[] = { 0, 0, 0, 1.0 }; /* LIGHT1조명의 위치를 좌표계의 중심으로 만들어주는 행렬 */
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	/* power가 0일때 즉, ON모드일때 내부조명 활성화 */
	if (power == 0) {
		glEnable(GL_LIGHT1);
	}
	/* OFF모드일때 내부조명 비활성화 */
	else if (power == 1) {
		glDisable(GL_LIGHT1);
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); /* 컬러 초기화 */
	glPopMatrix();
}


/* 타이머 폴리곤 그리기 함수 */
void draw_timer()
{
	glPushMatrix();
	glTranslatef(15.0f, -12.5f, 15.0f);
	/* 타이머 폴리곤의 앞면 */
	/* timer 변수에 따라 타이머에 표시될 텍스처가 달라짐 */
	if (power == 0) {
		if (timer == 1)
			glBindTexture(GL_TEXTURE_2D, texName[43]); /* 타이머1으로 매핑 */
		else if (timer == 2)
			glBindTexture(GL_TEXTURE_2D, texName[44]); /* 타이머2으로 매핑 */
		else if (timer == 3)
			glBindTexture(GL_TEXTURE_2D, texName[45]); /* 타이머3으로 매핑 */
		else if (timer == 4)
			glBindTexture(GL_TEXTURE_2D, texName[46]); /* 타이머4으로 매핑 */
		else if (timer == 5)
			glBindTexture(GL_TEXTURE_2D, texName[47]); /* 타이머5으로 매핑 */
		else if (timer == 6)
			glBindTexture(GL_TEXTURE_2D, texName[48]); /* 타이머6으로 매핑 */
		else if (timer == 7)
			glBindTexture(GL_TEXTURE_2D, texName[49]); /* 타이머7으로 매핑 */
		else if (timer == 8)
			glBindTexture(GL_TEXTURE_2D, texName[50]); /* 타이머8으로 매핑 */
		else if (timer == 9)
			glBindTexture(GL_TEXTURE_2D, texName[51]); /* 타이머9으로 매핑 */
		else if (timer == 10)
			glBindTexture(GL_TEXTURE_2D, texName[52]); /* 타이머10으로 매핑 */
		else
			glBindTexture(GL_TEXTURE_2D, texName[42]); /* 타이머0으로 매핑 */
	}
	else glBindTexture(GL_TEXTURE_2D, texName[85]); /* 타이머 OFF로 매핑 */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(7.5f, 23.0f, 3.01f);
	glTexCoord2f(0, 1); glVertex3f(2.5f, 23.0f, 3.01f);
	glTexCoord2f(0, 0); glVertex3f(2.5f, 21.0f, 3.01f);
	glTexCoord2f(1, 0); glVertex3f(7.5f, 21.0f, 3.01f);
	glEnd();

	glPopMatrix();
}

/* 타이머를 계산하는 함수 */
void count_timer(int value)
{
	/* power가 ON일때 작동도록 설정 */
	if (power == 0) {
		if (timer > 10) timer = 10; /* timer가 10보다 크면 10으로 만듦*/
		else if (timer < 0) timer = 0; /* timer가 0보다 작으면 0으로 만듦 */
		/* timer가 0이되면 전자레인지 작동이 중단됨 */
		else if (timer == 0)
		{
			start = 0; /* 작동이 중단된 상태 */
			glutIdleFunc(idle_stop); /* idle함수 비활성화 */
		}
		else {
			/* 작동하는 상태에서는 timer가 지속적으로 1씩 감소하도록 설정 */
			if (value > 0 && start == 1 ) {
				glutTimerFunc(1000, count_timer, 1); /* 1초마다 count_timer함수 호출 */
				timer--; /* timer값 1씩 감소 */
			}
		}
	}
}

/* 투영 공간 생성 */
/* Viewport 및 projection transform을 프로그램이 처음 시작될때 resize로 초기화
또는 윈도우 크기가 변경될때 resize함수에서 구현 */
void resize(int width, int height)
{
	glViewport(0, 0, width, height); /* Viewport T.F 설정 */
	glMatrixMode(GL_PROJECTION); /* GL_Projection(투영)은 뒤이어 나타나는 행렬 명령이 투영행렬에 영향을 줌 */
	glLoadIdentity(); /* 행렬 초기화 */
	gluPerspective(45, (float)width / (float)height, 1, 500); /* Projection T.F 설정 */
	glMatrixMode(GL_MODELVIEW); /* GL_MODELVIEW(물체이동)는 뒤이어 나타나는 행렬 명령이 모델뷰행렬에 영향을 줌 */
}

/* 키보드입력 처리함수 */
void keyboard(unsigned char key, int x, int y)
{
	/* '1'입력시 도어가 최대 90까지 열림 */
	if (key == '1') {
		if (angle0 <= -90) angle0 = -90;
		else angle0 -= 1;
	}
	/* '2'입력시 도어가 0도까지 닫힘 */
	else if (key == '2') {
		if (angle0 >= 0) angle0 = 0;
		else angle0 += 1;
	}
	/* q입력시 수동 다이얼이 오른쪽으로 회전 및 timer를 10으로 증가*/
	else if (key == 'q') {
		angle1 -= 70;
		if (power == 0) {
			timer = 10;
		}
	}
	/* w입력시 수동 다이얼이 왼쪽으로 회전 및 timer를 0으로 감소 */
	else if (key == 'w') {
		angle1 += 70;
		timer = 0;
	}
	/* e입력시 회전접시 수동으로 왼쪽으로 회전 */
	else if (key == 'e') {
		angle2 -= 5;
	}
	/* r입력시 회전접시 수동으로 오른쪽으로 회전 */
	else if (key == 'r') {
		angle2 += 5;
	}
	/* z입력시 회전접시 자동회전 및 타이머 감소 */
	else if (key == 'z') {
		start = 1; /* 작동이 시작된 상태를 의미 */
		/* power가 on이고 timer가 0보다 클때만 작동하도록 설정 */
		if (power == 0 && timer > 0) {
			glutTimerFunc(1000, count_timer, 1); /* 1초마다 timer가 1씩 줄도록함 */
			glutIdleFunc(idle); /* idle함수 적용 */
		}
	}
	/* x입력시 타이머 임시 중단 및 회전접시의 회전 중단 */
	else if (key == 'x') {
		start = 0; /* 작동이 중단된 상태를 의미 */
		glutIdleFunc(idle_stop); /* idle 비활성화 */
	}
	glutPostRedisplay();
}

/* idle함수 */
void idle(void)
{
	if (timer > 0)
		angle2 += 1.0; /* 회전접시가 회전하도록 한다 */
	else timer = 0;
	glutPostRedisplay(); /* glutDisplayFunc에 등록된 함수를 호출시켜서 창을 다시 그려준다. */
}


/* idle함수를 비활성화 하기위한 idle_stop함수 */
void idle_stop()
{
	glutPostRedisplay();
}


/* 팝업메뉴 클릭시 발생하는 함수 */
void main_menu_function(int option)
{
	/* 팝업메뉴 ON 입력시 */
	if (option == 1) {
		power = 0; /* power인자가 0이라는 것은 ON인 상태 */
	}
	/* 팝업메뉴 OFF 입력시 */
	else if (option == 2) {
		power = 1; /* power인자가 1이라는 것은 OFF인 상태 */
	}
	glutPostRedisplay();
}


int temp_x = 0; /* 현재 클릭한 x축 좌표 저장 */
int temp_y = 0; /* 현재 클릭한 y축 좌표 저장 */

void mouse(int button, int state, int x, int y)
{
	/* 마우스 왼쪽 버튼 눌렀을때 mbutton을 1로 바꿈 */
	if (button == GLUT_LEFT_BUTTON) mbutton = 1;
	/* 마우스 오른쪽 버튼 눌렀을때 mbutton을 2로 바꿈 */
	else if (button == GLUT_RIGHT_BUTTON) mbutton = 2;
}

void motion(int x, int y)
{
	if (mbutton == 1) {
		if (temp_x < x) {
			lootat_azimuth -= M_PI / 180; /* 카메라의 경도를 감소 */
		}
		else if (temp_x > x) {
			lootat_azimuth += M_PI / 180; /* 카메라의 경도를 증가 */
		}
		if (y > temp_y) {
			lootat_elevation -= M_PI / 180; /* 카메라 위도 증가*/
			if (upy == 1 && lootat_elevation < 0) /* centery가 1이면서 위도가 0도보다 작을때 */
			{
				upy = -1; lootat_elevation += 2 * M_PI;
			} /* upy를 -1로 바꾸고 현재 위도에 2파이만큼 더한다. */
			  /* upy가 -1이면서 위도가 파이보다 작을때  upy를 1로 바꾼다. */
			else if (upy == -1 && lootat_elevation < M_PI) upy = 1;
		}
		else if (y < temp_y) {
			lootat_elevation += M_PI / 180; /* 카메라 위도 감소 */
											/* upy가 1이면서 위도가 파이보다 클때 upy를 -1로 바꾼다. */
			if (upy == 1 && lootat_elevation > M_PI) { upy = -1; }
			/* upy가 -1이면서 위도가 2파이보다 클때 upy를 1로 바꾸고 현재 위도에 2파이를 뺀다 */
			else if (upy == -1 && lootat_elevation > 2 * M_PI) { upy = 1; lootat_elevation -= 2 * M_PI; }
		}
	}
	else if (mbutton == 2) {
		if (temp_x < x) {
			lootat_r += 1; /* r값을 늘려서 줌 아웃 기능 */
		}
		else if (temp_x > x) {
			lootat_r -= 1; /* r값을 줄여서 줌 인 기능 */
		}
	}
	temp_x = x; /* 현재 클릭한 x축 좌표 저장 */
	temp_y = y; /* 현재 클릭한 y축 좌표 저장 */
	glutPostRedisplay();
}

/* 특수키에 대한 콜백 함수 */
void specialkeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) /* 왼쪽 방향키를 눌렀을때 */
	{
		Tx -= 5; /* 전자레인지를 왼쪽으로 이동 */
	}
	else if (key == GLUT_KEY_RIGHT) /* 오른쪽 방향키를 눌렀을때 */
	{
		Tx += 5; /* 전자레인지를 오른쪽으로 이동 */
	}
	else if (key == GLUT_KEY_UP) /* 위쪽 방향키를 눌렀을때 */
	{
		Ty += 5; /* 전자레인지를 위쪽으로 이동 */
	}
	else if (key == GLUT_KEY_DOWN) /* 아래쪽 방향키를 눌렀을때 */
	{
		Ty -= 5; /* 전자레인지를 아래쪽으로 이동 */
	}
	else if (key == GLUT_KEY_PAGE_UP) /* PAGE_UP을 눌렀을때 */
	{
		Tz += 5; /* 전자레인지를 앞쪽으로 이동 */
	}
	else if (key == GLUT_KEY_PAGE_DOWN) /* PAGE_DOWN을 눌렀을때 */
	{
		Tz -= 5; /* 전자레인지를 뒷쪽으로 이동 */
	}
	glutPostRedisplay(); /* glutDisplayFunc에 등록된 함수를 호출시켜서 창을 다시 그려준다. */
}