//  CS718 Computer Graphics
//  project3_polygon
//  Shawn Yang
//  11/7/2018

#include <iostream>
#include <GLUT/glut.h>
#include <cmath>

GLint vert[100][2], points[100][2], vec1[2], vec2[2];
int width =400, height =600, n =0, m=0,type = GL_LINE_STRIP, v;
bool rubberbanding = false, antialiasing = false, is_polygon_finished=false;

double get_angle(GLint vec1[], GLint vec2[]){
  // angle = arccos( (vec1 * vec2)/(|vec1||vec2|) )
  double num=vec1[0]*vec2[0]+vec1[1]*vec2[1];
  double denom = sqrt(vec1[0]*vec1[0]+vec1[1]*vec1[1])
                * sqrt(vec2[0]*vec2[0]+vec2[1]*vec2[1]);
  return acos(num/denom);
}

bool is_clockwise(GLint vec1[], GLint vec2[]){
  // k = x1*y2 - y1*x2
  GLint k = vec1[0]*vec2[1] - vec1[1]*vec2[0];
  return k>0;
}

bool is_inside(GLint point[]){
  double TWO_PI = 6.283;
  double TOL= 0.001;
  double angle_sum=0;
  double angle;
  
  for (int i=0; i< n-1;i++){
    // calculate for the pair of vertex used for calculating angle and direction
    vec1[0] =vert[i][0]-point[0];
    vec1[1] =vert[i][1]-point[1];
    vec2[0] =vert[i+1][0] -point[0];
    vec2[1] =vert[i+1][1] -point[1];
    
    angle = get_angle(vec1, vec2);
    angle_sum = is_clockwise(vec1, vec2)? angle_sum+angle:angle_sum-angle;
  }
  angle_sum = abs(angle_sum);
  // is inside only if angle_sum fell within the set tolerance of TWO_PI
  return angle_sum + TOL > TWO_PI && angle_sum -TOL < TWO_PI;
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT);
  if (n == 1){
    glBegin(GL_POINTS);
    glVertex2iv(vert[0]);
    glEnd();
  }
  glBegin(GL_LINE_STRIP);
  for (int i=0; i<n; i++)
    glVertex2iv(vert[i]);
  glEnd();
  
  if (is_polygon_finished){
    glBegin(GL_POINTS);
    for (int i=0; i<m; i++){
      if (is_inside(points[i])) glColor3f(0, 0.5, 0);
      else glColor3f(1, 0, 0);
      glVertex2iv(points[i]);
    }
    glColor3f(1,1,0);
    glEnd();
  }
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  switch (key){
    case 'c':
      if (!is_polygon_finished){
        v=n++;
        vert[v][0] =vert[0][0];
        vert[v][1] = vert[0][1];
        is_polygon_finished=true;
      }
      break;
    
    case 'r':
      n=0;
      m=0;
      is_polygon_finished=false;
      break;
      
    case 'a':
      antialiasing = !antialiasing;
      if (antialiasing){
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
      }
      else{
        glDisable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
      }
      break;
  }
  glutPostRedisplay();
}

void mouse (int button, int state, int x, int y){
  switch (button){
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN){
        if (is_polygon_finished){
          v=m++;
          points[v][0]=x;
          points[v][1] = height-1-y;
          
        }
        else{
          v=n++;
          vert[v][0] =x;
          vert[v][1] = height-1-y;
          rubberbanding = true;
        }
      }
      else
        rubberbanding = false;
      glutPostRedisplay();
      break;
  }
}

void motion (int x, int y){
  if (rubberbanding){
    vert[v][0] = x;
    vert[v][1] = height -y -1;
    glutPostRedisplay();
  }
}



int main(int argc, char ** argv) {
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(50, 100);
  glutCreateWindow("shawn yang's proj3");
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1, 1, 0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  
  return 0;
}
