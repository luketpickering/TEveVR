#include <iostream>

#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description){
    std::cout << description << std::endl;
}

int ZRot = 0;

static void key_callback(GLFWwindow* window, int key,
  int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
     ZRot = (ZRot + 2)%360;
  }
  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
     ZRot = (ZRot - 2)%360;
  }
}


int WindowLoop(){

  if (!glfwInit()){
    return EXIT_FAILURE;
  }

  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

  if(!window){
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)){

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(float(ZRot), 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

   glfwDestroyWindow(window);

  glfwTerminate();
  return EXIT_SUCCESS;
}
