#include "stdafx.h"

// standard
#include <assert.h>
#include <math.h>
#include <stdlib.h>

// glut
#include <GL/glut.h>

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 0;
int cnt = 0;

//================================
// init
//================================
Point *p_cpoints, *p_points, *p_tan_points, *p_curve_points;
Vector prev_tan, prev_normal, XN, YN, ZN;
Matrix cur_mat = new float[16];

float ORG[3] = {0,0,0};
 
float XP[3] = {50,0,0}, XS[3] = {2,0,0},
      YP[3] = {0,50,0}, YS[3] = {0,2,0},
      ZP[3] = {0,0,50}, ZS[3] = {0,0,2};


int ctrl_len = 12;
int n_frame = 60;

void draw_axes(void){ 
    glPushMatrix ();
	glTranslatef (0, 0, 0);
    glLineWidth (2.0);
	glBegin (GL_LINES);
	glColor3f (1,0,0);  glVertex3fv (ORG);  glVertex3fv (XP);    // X axis is red.
	glColor3f (0,1,0);  glVertex3fv (ORG);  glVertex3fv (YP);    // Y axis is green.
	glColor3f (0,0,1);  glVertex3fv (ORG);  glVertex3fv (ZP);    // z axis is blue.
	glEnd();
    glPopMatrix ();
}

void draw_teapot(void){
	glPushMatrix();
	
	if (cnt == 0) {
		init_point(1, 0, 0, &prev_tan);
		init_point(0, 1, 0, &prev_normal);
		for (int i = 0; i < 16; ++ i) cur_mat[i] = IDENTITY_MAT[i];
	}

	glTranslatef (p_points[cnt].pos[0], p_points[cnt].pos[1], p_points[cnt].pos[2]);
	
	
	Vector nxt_norm;
    gen_rotate_vec(p_tan_points[cnt], p_curve_points[cnt], &nxt_norm);


	/* show tan line */
	glLineWidth (2.0);
	glBegin (GL_LINES);
	glColor3f (0,1,1);  glVertex3fv (ORG);  glVertex3fv (p_tan_points[cnt].pos);
	//glColor3f (1,0,1);  glVertex3fv (ORG);  glVertex3fv (nxt_norm.pos);
	glEnd();
	/* end */
	
	//printf("%f %f %f \n", prev_normal.pos[0], prev_normal.pos[1], prev_normal.pos[2]);
	Matrix m1 = new float[16];
	Matrix m2 = new float[16];
	/* cal matrix */
	gen_transform_matrix(XN, p_tan_points[cnt], p_curve_points[cnt], ZN, m1, m2, R_EULER);
	//glMultMatrixf(cur_mat);
	glMultMatrixf(m2);
	//glMultMatrixf(m1);
	//gen_4mul_mat(cur_mat, m1);


	/* next frame */
	//init_point_p(p_tan_points[cnt], &prev_tan);
	cnt += 1;
	cnt %= (ctrl_len - 3) * n_frame;
	
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
	GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
	GLfloat material_Se		= 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
	glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);

	
	glutSolidTeapot(1.5);
	
	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	/* show object axes */
	glLineWidth (2.0);
	glBegin (GL_LINES);
	glColor3f (1,0,0);  glVertex3fv (ORG);  glVertex3fv (XS);    // X axis is red.
	glColor3f (0,1,0);  glVertex3fv (ORG);  glVertex3fv (YS);    // Y axis is green.
	glColor3f (0,0,1);  glVertex3fv (ORG);  glVertex3fv (ZS);    // z axis is blue.
	glEnd();
	/* end */
	glPopMatrix();
}
void draw_line(void) {
    glPushMatrix ();
	glTranslatef (0, 0, 0);
    glLineWidth (2.0);
	glBegin (GL_LINES);
	float *orign, *next;
	for (int i = 0; i < (ctrl_len - 3) * n_frame - 1; ++ i) {
		next = (p_points[i + 1].pos);
		orign = (p_points[i].pos);
		glColor3f(1,0,0);  glVertex3fv(orign);  glVertex3fv(next);
	}
	glEnd();

	glBegin(GL_POINTS);
	for (int i = 0; i < ctrl_len; ++ i) {
		glColor3f(1.0, 1.0, 1.0);
		glVertex3fv(p_cpoints[i].pos); 
	}
    glEnd();

	
    glLineWidth (0.5);
	glBegin (GL_LINES);
	for (int i = 0; i < ctrl_len - 1; ++ i) {
		next = (p_cpoints[i + 1].pos);
		orign = (p_cpoints[i].pos);
		glColor3f(0.0, 1.0, 1.0);  glVertex3fv(orign);  glVertex3fv(next);
	}
	glEnd();

    glPopMatrix ();

}
void init( void ) {
	// init something before main loop...
	p_points = new Point[(ctrl_len - 3) * n_frame];
	p_tan_points = new Point[(ctrl_len - 3) * n_frame];
	p_curve_points = new Point[(ctrl_len - 3) * n_frame];
	p_cpoints = new Point[12];

	init_point(0, 0, 3, &p_cpoints[0]);
	init_point(-2, 3, 3, &p_cpoints[1]);
	init_point(-4, 5, 3, &p_cpoints[2]);
	init_point(-5, 6, 2, &p_cpoints[3]);
	init_point(-7, 6, 1, &p_cpoints[4]);
	init_point(-5, 7, 1, &p_cpoints[5]);
	init_point(-3, 7, 1, &p_cpoints[6]);
	init_point(2, 9, 1, &p_cpoints[7]);
	init_point(1, 7, 2, &p_cpoints[8]);
	init_point(4, 3, 2, &p_cpoints[9]);
	init_point(7, 4, 3, &p_cpoints[10]);
	init_point(9, 9, 3, &p_cpoints[11]);

	init_point(1, 0, 0, &prev_tan);
	init_point(0, 0, 1, &prev_normal);
	init_point(1, 0, 0, &XN);
	init_point(0, 1, 0, &YN);
	init_point(0, 0, 1, &ZN);

	for (int i = 0; i < 16; ++ i) cur_mat[i] = IDENTITY_MAT[i];
	gen_cr_line(p_points, p_tan_points, p_curve_points, p_cpoints, ctrl_len, n_frame, L_BSPLINE);
}

//================================
// update
//================================
void update( void ) {
	// do something before rendering...

	// rotation angle
	g_angle = ( g_angle + 5 ) % 360;
}

//================================
// render
//================================
void render( void ) {
	// clear buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(20.0f, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	draw_teapot();
	draw_line();
    draw_axes();
	
	
	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	g_screenWidth  = w;
	g_screenHeight = h;	
	
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;

	//update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}



//================================
// main
//================================
int main( int argc, char** argv ) {
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( argv[0] );

	
	glPointSize(3.0);

	// init
	init();
	
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 16, timer, 0 );
	
	// main loop
	glutMainLoop();

	return 0;
}