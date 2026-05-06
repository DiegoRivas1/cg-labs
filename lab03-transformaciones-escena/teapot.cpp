#include <GL/freeglut.h>

// Ángulo para rotación
float angle = 0.0f;

void init() {
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f); // fondo oscuro
    glEnable(GL_DEPTH_TEST); // activar profundidad
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Cámara simple
    gluLookAt(2.5, 2.5, 2.5,   // posición cámara
              0.0, 0.0, 0.0,   // mira al origen
              0.0, 1.0, 0.0);  // eje up

    // Rotación
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // Color
    glColor3f(1.0f, 0.8f, 0.2f);

    // Tetera
    glutSolidTeapot(0.7);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, ratio, 0.1, 100.0);

    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
}

void idle() {
    angle += 0.1f; // animación
    if (angle > 360.0f) angle -= 360.0f;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    // Modo display
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Tamaño ventana
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tetera con FreeGLUT");

    init();

    // Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}