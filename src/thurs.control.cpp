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

#include "thurs.os.hpp"
#include "../include/thurs/thurs.hpp"

namespace thurs {

  /////////////////////////////////////////////////////////////////////////////

  Control::Control(uint32 id, Surface *surface) {
    m_surface = surface;
    m_input = surface->input();
    m_renderer = surface->renderer();
    m_canMove = false;
    m_canResize = false;
    m_isMoving = false;
    m_isResizing = false;
    m_id = id;
    m_mouseWasInside = false;
    m_acceptedDropType = 0;

    m_size.x = 100;
    m_size.y = 25;

    Surface::ControlMapIt it = m_surface->m_controls.find(id);
    if (it == m_surface->m_controls.end()) {
      m_surface->m_controls.insert(ControlMapPair(id, this));
    } else {
      //Fatal error..
    }

    m_noStateHandling = false;
  }

  Control::~Control() {
    Surface::ControlMapIt it = m_surface->m_controls.find(m_id);
    if (it != m_surface->m_controls.end()) {
      m_surface->m_controls.erase(m_id);
    } else {
      //Fatal error..
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  uint32 Control::id() {
    return m_id;
  }

  void Control::update() {
    bool mi = mouseInside();

    //HANDLE CONTROL MOVING
    if (m_canMove) {
      if (m_isMoving) {
        if (m_input->mouseUp()) {
          m_isMoving = false;
        } else {
          //New position is the initial position - current position
          m_position.x = m_initalPos.x + (m_input->mouseCoords().x - m_startMovePos.x);
          m_position.y = m_initalPos.y + (m_input->mouseCoords().y - m_startMovePos.y);
        }
      } else if (mi && m_input->mouseDown() && !m_isMoving) {
        m_startMovePos = m_input->mouseCoords();
        m_initalPos = m_position;
        m_isMoving = true;
        m_input->markHandled();
      }
    } 

    //HANDLE GENERIC EVENTS
    if (mi) {

      if (!m_mouseWasInside) {
        //on mouse over
        OnMouseIn(m_id);
        m_mouseWasInside = true;
        if (!m_noStateHandling) {
          m_skinClass.setState(S_HOVER);
        }
        m_mouseOverTime = getTime();
      }

      if (m_input->mouseClick()) {
        //focus + click
        OnClick(m_id);
      }

      if (m_input->mouseDown()) {
        //mouse down
         OnMouseDown(m_id);
         if (!m_noStateHandling) {
          m_skinClass.setState(S_ACTIVE);
        }
      }


      if (m_input->mouseUp()) {
        //Check if there's drop data associated with the mouse event
    
        //mouse up
        OnMouseUp(m_id);
        if (!m_noStateHandling) {
          m_skinClass.setState(S_HOVER);
        }
      }

      //Render tooltip
      if (Tooltip.size() > 0 && getTime() - m_mouseOverTime > 1000) {
        //m_renderer->renderText(m_tooltipSkinClass.Attr.textFill, 0, Tooltip, m_input->mouseCoords());
      }

    } else if (m_mouseWasInside) {
      //on mouse leave
      OnMouseOut(m_id);

      if (!m_noStateHandling) {
        m_skinClass.setState(S_NORMAL);
      }

      m_mouseWasInside = false;
    }

    m_skinClass.update();

  }

  /////////////////////////////////////////////////////////////////////////////

  bool Control::focused() {
    return false;
  }

  void Control::canMove(bool flag) {
    m_canMove = flag;
  }

  void Control::canResize(bool flag) {
    m_canResize = flag;
  }

  //Set the position of the control
  void Control::setPosition(const Vector2f& pos) {
    m_position = pos;
  }

  void Control::setPosition(Vector2f pos) {
    m_position = pos;
  }

  void Control::setPosition(uint16 x, uint16 y) {
    m_position.x = x;
    m_position.y = y;
  }

  //Set the size of the control
  void Control::setSize(const Vector2f& size) {
    m_size.x = size.x;
    m_size.y = size.y;
  }

  void Control::setSize(Vector2f size) {
    m_size.x = size.x;
    m_size.y = size.y;
  }

  void Control::setSize(uint16 w, uint16 h) {
    m_size.x = w;
    m_size.y = h;
  }

  void Control::setWPosition(const Vector2f& pos) {
    m_wposition = pos;
  }

  void Control::setSkinClass(const std::string& name) {
    m_skinClass = m_surface->m_skin.getClass(name);
    m_tooltipSkinClass = m_surface->m_skin.getClass("Tooltip");
    m_loadedClassName = name;
  }

  void Control::reloadSkinClass() {
    setSkinClass(m_loadedClassName);
  }

  /////////////////////////////////////////////////////////////////////////////

  bool Control::mouseInside() {
    Vector2s c = m_input->mouseCoords();
    return c.x >= (m_wposition.x + m_position.x) && 
           c.x <= (m_wposition.x + m_position.x) + m_size.x &&
           c.y >= (m_wposition.y + m_position.y) && 
           c.y <= (m_wposition.y + m_position.y) + m_size.y;
  }
}