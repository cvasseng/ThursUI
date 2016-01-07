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

    surface->m_children.push_back(this);

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

    Title = "Window Title";

    m_moving = false;

    CanClose = true;
    CanMove = true;
    CanCollapse = true;
    
    Properties.add("canClose", CanClose);
    Properties.add("canMove", CanMove);
    Properties.add("canCollapse", CanCollapse);
    Properties.add("title", Title);
  }

  Window::~Window() {
    for (uint32 i = 0; i < m_owner->m_children.size(); i++) {
      if (m_owner->m_children[i] == this) {
        m_owner->m_children.erase(m_owner->m_children.begin() + i);
        return;
      }
    }

    //uh oh..
  }
  
  bool Window::serialize(Json::Value &root) {
    root["type"] = "window";
    Surface::serialize(root);
    //Add window properties to root here..  
    return Properties.toJSON(root["properties"]);
  }
  
  bool Window::unserialize(Json::Value &root) {
    Surface::unserialize(root);
    return Properties.fromJSON(root["properties"]);
  }

  void Window::setSize(float x, float y) {
    m_winSize.x = x;
    m_winSize.y = y;
  }
  
  void Window::setSize(const Vector2f& vec) {
    m_winSize = vec;
  }

  void Window::setPos(float x, float y) {
    m_winPos.x = x;
    m_winPos.y = y;
  }

  void Window::setPos(const Vector2f& vec) {
    m_winPos = vec;
  }

  void Window::hide() {
    m_visible = false;
    OnHide();
  }

  void Window::show() {
    m_visible = true;
    OnShow();
  }

  const Vector2f& Window::getSize() {
    return m_winSize;
  }

  bool Window::mouseOver() {
    if (m_titlebarClass) {
      m_titlebarHeight = m_renderer->getTextHeight(m_titlebarClass->Attr, Title) + 4;
    }

    Vector2s mc = m_input->mouseCoords();
    bool mt = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + m_titlebarHeight;
    bool mo = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + m_winSize.y + m_titlebarHeight;

    return (!m_collapsed && mo) || mt;
  }

  void Window::_onUpdate() {
    if (!m_visible) {
      return;
    }

    if (m_titlebarClass) {
      m_titlebarHeight = m_renderer->getTextHeight(m_titlebarClass->Attr, Title) + 4;
    }

    Vector2s mc = m_input->mouseCoords();
    bool mt = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + m_titlebarHeight;
    bool mo = mc.x >= m_winPos.x && mc.x <= m_winPos.x + m_winSize.x && mc.y >= m_winPos.y && mc.y <= m_winPos.y + m_winSize.y + m_titlebarHeight;

    if (mt) { //Mouse on titlebar?
      if (!m_moving && m_input->mouseDown()) {
        m_preMovePos = m_winPos;
        m_mdelta = mc;
        m_moving = true;
        
       // m_input->markHandled();
      }
    }

    if (((!m_collapsed && mo) || mt) && m_input->mouseDown()) {
      m_owner->m_moveFocusedChildToTop = true;
    }

    if (m_moving) {
      if (m_input->mouseUp()) {
        m_moving = false;
      } else {

        m_winPos.x = m_preMovePos.x + (mc.x - m_mdelta.x);
        m_winPos.y = m_preMovePos.y + (mc.y - m_mdelta.y);

        if (m_winPos.x < 0) m_winPos.x = 0;
        if (m_winPos.y < 0) m_winPos.y = 0;
        if (m_winPos.x + m_winSize.x > m_owner->m_canvasSize.x) m_winPos.x = m_owner->m_canvasSize.x - m_winSize.x;
        if (m_winPos.y + m_winSize.y > m_owner->m_canvasSize.y) m_winPos.y = m_owner->m_canvasSize.y - m_winSize.y;
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

      if (m_focused) {
        m_focused->setWPosition(m_winPos + Vector2f(0, m_titlebarHeight));
        m_focused->update();
      }

      //Right, so this is kinda fun - we need to iterate backwards through this map
      //and check if the mouse is on the widget. The moment we find a widget with the mouse
      //over it, we need to break the loop. In our actual rendering loop, this is the only
      //child that should have the input enabled
      
      int32 inputHogger = -1;
      for (int32 i = m_controls.size() - 1; i >= 0; i--) {
        m_controls[i]->setWPosition(m_winPos + Vector2f(0, m_titlebarHeight));
        if (m_controls[i]->mouseInside()) {
          inputHogger = i;
          break;
        }
      }

      //Update the children
      bool wasHandled = m_input->handled();
      for (uint32 i = 0; i < m_controls.size(); i++) {
        m_controls[i]->setWPosition(m_winPos + Vector2f(0, m_titlebarHeight));
        if (inputHogger == i) {
          m_input->enable();
         // m_controls[i]->setFocus(true);
        } else {
         // m_controls[i]->setFocus(false);
          m_input->disable();
        }
        m_controls[i]->update();
      }

      if (wasHandled) {
        m_input->disable();
      } else {
        m_input->enable();
      }

    }

    if (m_titlebarClass) {
      //Render titlebar
      m_renderer->renderRect(m_titlebarClass->Attr, m_winPos, Vector2f(m_winSize.x, m_titlebarHeight));
      m_renderer->renderText(m_titlebarClass->Attr, Title, m_winPos, Vector2f(m_winSize.x, m_titlebarHeight));
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

    //If mouse is on the window, swallow the input.
    //This way we only need to mess with
    if ( (!m_collapsed && mo) || mt) {
      m_owner->m_focusedChild = this;
    } else if (m_owner->m_focusedChild == this) {
      m_owner->m_focusedChild = 0;
    }

    m_skinClass.update();

  }

  void Window::setSkinClass(const std::string& name) {
    m_skinClass = m_skin.getClass(name);
    m_titlebarClass = m_skinClass.findSub("titlebar");
    m_closeClass = m_skinClass.findSub("closeIcon");
    m_collapseClass = m_skinClass.findSub("collapseIcon");
    m_skinClassName = name;
  }

  bool Window::reloadSkin() {
    if (Surface::reloadSkin()) {
      setSkinClass(m_skinClassName);
      return true;
    }
    return false;
  } 

}
