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

class EventHandler : public sigslot::has_slots<> {
  public:
    EventHandler(thurs::Surface* root, thurs::Slider* slider, thurs::ProgressBar* pbar) {
      m_root = root;
      m_slider = slider;
      m_bar = pbar;
    }

    void SliderChange(int v) {
      m_bar->Value = v;
    }

    void ReloadSkins(unsigned int fromid) {
      printf("HANDLER LOL\n");
      m_root->reloadSkin();
    }
  protected:
    //Root surface
    thurs::Surface* m_root;
    thurs::Slider* m_slider;
    thurs::ProgressBar* m_bar;
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

  
  thurs::Renderer* renderer = new thurs::RendererNanoVG();
  renderer->init();
  thurs::Surface* surface = new thurs::Surface(renderer, &input);
  if (!surface->loadSkin("skin.json")) {
    printf("Failed to load skin\n");
  }

  

  thurs::Window* win = new thurs::Window(surface);
  thurs::Window* win2 = new thurs::Window(surface);
  win2->setPos(0, 0);

  thurs::Rectangle* rect = new thurs::Rectangle(213, surface);
  rect->HAlign = thurs::Control::HA_RIGHT;
  rect->VAlign = thurs::Control::VA_BOTTOM;
  rect->setImage("logo.png");
  rect->setSize(64, 64);

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
  
  btn3->HAlign = thurs::Control::HA_CENTER;
 // btn3->VAlign = thurs::Control::VA_CLIENT;

  thurs::ProgressBar *pbar = new thurs::ProgressBar(2, win);
  pbar->setSize(380, 25);
  pbar->setPosition(10, 40);

  thurs::ProgressBar *pbar2 = new thurs::ProgressBar(4, win2);
  pbar2->setSize(380, 25);
  pbar2->setPosition(10, 10);
  pbar2->setSkinClass("XPBar");

  thurs::MultiLineText *mt = new thurs::MultiLineText(19, win2);
  mt->setPosition(10, 45);
  mt->setSize(380, 100);
  mt->loadFromFile("multitext.txt");

  thurs::DropDown *dd = new thurs::DropDown(22, win2);
  dd->setPosition(10, 155);
  dd->setSize(380, 25);
 // dd->addItem(0, "Hello world");
   for (int i = 0; i < 99; i++) {
    std::stringstream ss;
    ss << "Item #" << i;
    dd->addItem(i, ss.str());
  }

  thurs::Button *btn5 = new thurs::Button(43, win2);
  btn5->setSize(380, 25);
  btn5->setPosition(10, 190);
  btn5->Caption = "YET ANOTHER BUTTON";

  thurs::Label *lbl = new thurs::Label(47, win2);
  lbl->setSize(380, 25);
  lbl->setPosition(10, 225);
  lbl->Caption = "Edit Box";

  thurs::EditBox *eb = new thurs::EditBox(45, win2);
  eb->setSize(380, 25);
  eb->setPosition(10, 250);

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

  for (int i = 0; i < 10; i++) {
    std::stringstream ss;
    ss << "Img #" << i;
    igrid->addImage(i, "logo.png", ss.str());
  }

  EventHandler handler(surface, slider, pbar);
  btn3->OnMouseDown.connect(&handler, &EventHandler::ReloadSkins);
  slider->OnChange.connect(&handler, &EventHandler::SliderChange);
  //btn5->OnMouseDown.connect(&handler, &EventHandler::ReloadSkins);

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

  delete btn;
  delete win;
  delete surface;
  delete renderer;

  glfwTerminate();

  return 0;

}