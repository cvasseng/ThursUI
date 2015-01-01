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

#include <GLFW/glfw3.h>

#include <thurs/thurs.hpp>
#include <thurs/renderers/thurs.renderer.nanovg.hpp>
#include <thurs/controls/thurs.controls.hpp>

void errorcb(int error, const char* desc) {
  printf("GLFW error %d: %s\n", error, desc);
}

void window_size_callback(GLFWwindow* window, int width, int height) {

}

class EventHandler : public sigslot::has_slots<> {
  public:
    EventHandler(thurs::Surface* root) {
      m_root = root;
    }

    void ReloadSkins(unsigned int fromid) {
      printf("HANDLER LOL\n");
      m_root->reloadSkin();
    }
  protected:
    //Root surface
    thurs::Surface* m_root;
  private:
};


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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  #endif

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

  window = glfwCreateWindow(1024, 1024, "Thurs Hello World", NULL, NULL);

  glfwMakeContextCurrent(window);
  //glfwSetKeyCallback(window, keyCallback);

  glfwSwapInterval(0);
  glfwSetTime(0);

  glClearColor(1.f, 1.f, 1.f, 1.f);

  ////// START UI INIT ///////

  thurs::Input input;
  thurs::Renderer* renderer = new thurs::RendererNanoVG();
  renderer->init();
  thurs::Surface* surface = new thurs::Surface(renderer, &input);
  if (!surface->loadSkin("skin.json")) {
    printf("Failed to load skin\n");
  }

  EventHandler handler(surface);

  thurs::Window* win = new thurs::Window(surface);
  thurs::Window* win2 = new thurs::Window(surface);
  win2->setPos(0, 0);

  win->Title = "My Super Window";
  win2->Title = "I Super Window Too";

  thurs::Button *btn = new thurs::Button(1, win);
  btn->setSize(100, 25);
  btn->setPosition(10, 10);
  btn->Caption = "OK";
  

  thurs::Button *btn2 = new thurs::Button(100, win);
  btn2->setSize(100, 25);
  btn2->setPosition(120, 10);
  btn2->setSkinClass("ButtonCancel");
  btn2->Caption = "CANCEL";

  thurs::Button *btn3 = new thurs::Button(1, surface);
  btn3->setPosition(10, 10);
  btn3->Caption = "RELOAD SKIN";
  btn3->OnMouseDown.connect(&handler, &EventHandler::ReloadSkins);
  btn3->HAlign = thurs::Control::HA_CENTER;
 // btn3->VAlign = thurs::Control::VA_CLIENT;

  thurs::ProgressBar *pbar = new thurs::ProgressBar(2, win);
  pbar->setSize(380, 25);
  pbar->setPosition(10, 40);

  thurs::Slider *slider = new thurs::Slider(3, win);
  slider->setSize(380, 15);
  slider->setPosition(10, 70);

  thurs::ListBox *box = new thurs::ListBox(4, win);
  box->setSize(380, 80);
  box->setPosition(10, 100);
  box->addItem(0, "Hello");
  box->addItem(1, "World");
  box->addItem(2, "How");
  box->addItem(3, "Is");
  box->addItem(4, "It");
  box->addItem(5, "Going?");

  //Add some more entries
  for (int i = 0; i < 99; i++) {
    std::stringstream ss;
    ss << "Item #" << i;
    box->addItem(i, ss.str());
  }

  thurs::Checkbox *cbox = new thurs::Checkbox(5, win);
  cbox->setPosition(10, 190);

  thurs::ImageGrid *igrid = new thurs::ImageGrid(6, win);
  igrid->setPosition(10, 220);
  igrid->setSize(380, 200);

  for (int i = 0; i < 30; i++) {
    std::stringstream ss;
    ss << "Img #" << i;
    igrid->addImage(i, "logo.png", ss.str());
  }

  ///// END UI INIT //////

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

  delete btn;
  delete win;
  delete surface;
  delete renderer;

  glfwTerminate();

  return 0;

}