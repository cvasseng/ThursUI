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

#include "../include/thurs/thurs.hpp"

namespace thurs {

  Window::Window(Surface* surface) : Surface(surface->renderer(), surface->input()) {
    //ugly..
    static uint16 id = 0;
    m_id = ++id;

    surface->m_children.insert(SurfaceMapPair(m_id, this));
    m_owner = surface;

    m_winSize.x = 400;
    m_winSize.y = 440;

    m_winPos.x = 200;
    m_winPos.y = 200;

    m_skin = surface->m_skin;

    setSkinClass("Window");

    m_moving = false;
  }

  Window::~Window() {
    SurfaceMapIt it = m_owner->m_children.find(m_id);
    if (it != m_owner->m_children.end()) {
      m_owner->m_children.erase(it);
    }
  }

  void Window::_onUpdate() {
    Vector2s mc = m_input->mouseCoords();
    bool mt = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + 40;

    //Render the frame
    m_renderer->renderRect(m_skinClass.Attr.fill, m_winPos, m_winSize);
    m_renderer->renderText(m_skinClass.Attr.textFill, 0, "Window Title", m_winPos);

    if (mt) { //Mouse on titlebar?
      if (!m_moving && m_input->mouseDown()) {
        m_preMovePos = m_winPos;
        m_mdelta = mc;
        m_moving = true;
      }
    }

    if (m_moving) {
      if (m_input->mouseUp()) {
        m_moving = false;
      } else {
        m_winPos.x = m_preMovePos.x + (mc.x - m_mdelta.x);
        m_winPos.y = m_preMovePos.y + (mc.y - m_mdelta.y);
      }
    }

    //Update the children
    for (ControlMapIt it = m_controls.begin(); it != m_controls.end(); it++) {
      it->second->setWPosition(m_winPos);
      it->second->update();
    }

  }

  void Window::setSkinClass(const std::string& name) {
    m_skinClass = m_skin.getClass(name);
  }

}
