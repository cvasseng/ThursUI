/******************************************************************************


Copyright (c) 2014, Chris Vasseng - github.com/cvasseng - cvasseng@gmail.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <thurs/thurs.hpp>
#include <thurs/renderers/thurs.renderer.nanovg.hpp>
#include <thurs/controls/thurs.controls.hpp>

thurs::Input input;

void errorcb(int error, const char* desc) {
  printf("GLFW error %d: %s\n", error, desc);
}

void scrollFunc(GLFWwindow* win, double x, double y) {

}

void textFunc(GLFWwindow* win, unsigned int codepoint) {
  input.injectTextInput(codepoint, 0);

}

void keyFunc(GLFWwindow* win, int key, int scancode, int action, int mods) {
  printf("%i\n", scancode);
  if (action == GLFW_PRESS) {
    input.injectKeyEvent(thurs::Input::KE_PRESS, scancode);
  } else if (action == GLFW_RELEASE) {
    input.injectKeyEvent(thurs::Input::KE_RELEASE, scancode);
  }
}

int main(int argc, char** argv) {
  GLFWwindow* window;
  int width, height, wwidth, wheight;
  double mouseX, mouseY;
  float pixelFormat = 1.f;

  if (!glfwInit()) {
    printf("Failed to init GLFW.");
    return -1;
  }

  glfwSetErrorCallback(errorcb);

  #ifndef _WIN32 // don't require this on win32, and works with more cards
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #endif

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

  window = glfwCreateWindow(1024, 1024, "Thurs Hello World", NULL, NULL);

  glfwMakeContextCurrent(window);
  
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("Could not init glew\n");
    return -1;
  }
  //glfwSetKeyCallback(window, keyCallback);

  glfwSwapInterval(0);
  glfwSetTime(0);

  glClearColor(1.f, 1.f, 1.f, 1.f);

  ////// START UI INIT ///////

  
  thurs::Renderer* renderer = new thurs::RendererNanoVG();
  renderer->init();
  thurs::Surface* surface = new thurs::Surface(renderer, &input);
  if (!surface->loadSkin("skin.json")) {
    printf("Failed to load skin\n");
  }
  
  surface->unserialize("ui.test.json");

  ///// END UI INIT //////

  glfwSetScrollCallback(window, scrollFunc);
  glfwSetCharCallback(window, textFunc);
  glfwSetKeyCallback(window, keyFunc);

   while (!glfwWindowShouldClose(window)) {
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetWindowSize(window, &wwidth, &wheight);
    glfwGetCursorPos(window, &mouseX, &mouseY);

    pixelFormat = width / (float)wwidth; 
    
    glViewport(0, 0, width, height);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //printf("win size %i %i\n", wwidth, wheight);

    //Inject input and render UI  
    input.injectMouseCoords(mouseX, mouseY);
    input.injectMouseButton(thurs::Input::MouseButton(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)));
    surface->resize(wwidth, wheight);
    surface->updateAndRender(pixelFormat);  

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;

}