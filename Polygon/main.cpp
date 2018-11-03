//
//  main.cpp
//  Polygon
//
//  Created by Shawn Yang on 11/3/18.
//  Copyright © 2018 Shawn Yang. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>


GLint vert[100][2];
int width =400, height =600, n =0, type = GL_LINE_STRIP, v;
bool rubberbanding = false, antialiasing = false;
void display(){
  glClear(GL_COLOR_BUFFER_BIT);
  if (n == 1 && (type ==GL_LINE_STRIP || type == GL_LINE_LOOP)){
    glBegin(GL_POINT);
    glVertex2iv(vert[0]);
    glEnd();
  }
  glBegin(type);
  for (int i=0; i<n; i++)
    glVertex2iv(vert[i]);
  glEnd();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  switch (key){
      
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
      
    case 'c':
      n=0;
      break;
    
    case 'l':
      type = GL_LINE_STRIP;
      break;
      
    case 'p':
      type = GL_LINE_LOOP;
      break;
    
    case 'v':
      type = GL_POINTS;
      break;
  }
  glutPostRedisplay();
}

int findVertex(int x, int y){
  int dx, dy;
  for (int i=0; i<n; i++){
    dx = vert[i][0]-x;
    dy = vert[i][1] - y;
    if (dx*dx+dy*dy<16)
      return i;
  }
  return -1;
}

void mouse (int button, int state, int x, int y){
  switch (button){
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN){
        v=n++;
        vert[v][0] =x;
        vert[v][1] = height-1-y;
        rubberbanding = true;
        glutPostRedisplay();
      }
      else
        rubberbanding = false;
      break;
      
    case GLUT_RIGHT_BUTTON:
      if (state == GLUT_DOWN && (v = findVertex(x, height-1-y))!=-1){
        if (glutGetModifiers()==GLUT_ACTIVE_CTRL){
          for (int i=v; i<n-1; i++){
            vert[i][0] =vert[i+1][0];
            vert[i][1] =vert[i+1][1];
          }
          n--;
        }else{
          vert[v][0]=x;
          vert[v][1] = height -1-y;
          rubberbanding = true;
        }
        glutPostRedisplay();
      }
      else
        rubberbanding=false;
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
