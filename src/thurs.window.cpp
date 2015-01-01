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

    m_titlebarHeight = 40;

    m_skin = surface->m_skin;

    m_visible = true;
    m_collapsed = false;

    m_titlebarClass = 0;

    setSkinClass("Window");

    m_moving = false;
  }

  Window::~Window() {
    SurfaceMapIt it = m_owner->m_children.find(m_id);
    if (it != m_owner->m_children.end()) {
      m_owner->m_children.erase(it);
    }
  }

  void Window::hide() {
    m_visible = false;
    OnHide();
  }

  void Window::show() {
    m_visible = true;
    OnShow();
  }

  void Window::_onUpdate() {
    if (!m_visible) {
      return;
    }

    if (m_titlebarClass) {
      m_titlebarHeight = m_renderer->getTextHeight(m_titlebarClass->Attr, "Window Title");
    }

    Vector2s mc = m_input->mouseCoords();
    bool mt = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + m_titlebarHeight;
    bool mo = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + m_size.y;

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

        if (m_winPos.x < 0) m_winPos.x = 0;
        if (m_winPos.y < 0) m_winPos.y = 0;
        if (m_winPos.x + m_winSize.x > m_owner->m_size.x) m_winPos.x = m_owner->m_size.x - m_winSize.x;
        if (m_winPos.y + m_winSize.y > m_owner->m_size.y) m_winPos.y = m_owner->m_size.y - m_winSize.y;
      }
    }

    if ( (!m_collapsed && mo) || mt) {
      m_skinClass.setState(S_HOVER);
    } else {
      m_skinClass.setState(S_NORMAL);
    }

    //Render the frame
    if (!m_collapsed) {
      m_renderer->renderRect(m_skinClass.Attr, m_winPos, m_winSize + Vector2f(0, m_titlebarHeight));

      //Update the children
      for (ControlMapIt it = m_controls.begin(); it != m_controls.end(); it++) {
        it->second->setWPosition(m_winPos + Vector2f(0, m_titlebarHeight));
        it->second->update();
      }
    }

    if (m_titlebarClass) {
      //Render titlebar
      
      m_renderer->renderRect(m_titlebarClass->Attr, m_winPos, Vector2f(m_winSize.x, m_titlebarHeight));
      m_renderer->renderText(m_titlebarClass->Attr, "Window Title", m_winPos, Vector2f(m_winSize.x, m_titlebarHeight));
    }

    float s = m_titlebarHeight / 2.f;
    if (m_closeClass) {
      Vector2f pos = m_winPos + Vector2f(m_winSize.x - (s * 2.0f), s / 2.f);
      if (mc.x >= pos.x && mc.x <= pos.x + s && mc.y >= pos.y && mc.y <= pos.y + s && m_input->mouseDown()) {
        hide();
      }
      m_renderer->renderRect(m_closeClass->Attr, pos, Vector2f(s, s));
    }

    if (m_collapseClass) {
      Vector2f pos = m_winPos + Vector2f(m_winSize.x - (s * 4.0f), s / 2.f);
      if (mc.x >= pos.x && mc.x <= pos.x + s && mc.y >= pos.y && mc.y <= pos.y + s && m_input->mouseDown()) {
        //toggle collapse
        m_collapsed = !m_collapsed;
      }
      m_renderer->renderRect(m_collapseClass->Attr, pos, Vector2f(s, s));
    }

    m_skinClass.update();

  }

  void Window::setSkinClass(const std::string& name) {
    m_skinClass = m_skin.getClass(name);
    m_titlebarClass = m_skinClass.findSub("titlebar");
    m_closeClass = m_skinClass.findSub("closeIcon");
    m_collapseClass = m_skinClass.findSub("collapseIcon");
  }

}
