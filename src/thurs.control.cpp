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

  #define PCASE(x, y) case x: { y } break;

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
    m_doingTooltip = false;
    m_focus = false;
    m_visible = true;

    m_size.x = 100;
    m_size.y = 25;

    HAlign = HA_CUSTOM;
    VAlign = VA_CUSTOM;

    /*Surface::ControlMapIt it = m_surface->m_controls.find(id);
    if (it == m_surface->m_controls.end()) {
      m_surface->m_controls.insert(ControlMapPair(id, this));
    } else {
      //Fatal error..
    }
*/

    //So originally, this was a map. 
    //This is better as it allows for better focus management.
    //Also it allows for multiple elements to have the same ID.
    //Not that doing that is a good idea, but whatever.
    m_surface->m_controls.push_back(this);

    m_noStateHandling = false;

    //Tooltip = "I'm a tooltip!";
  }

  Control::~Control() {

    for (uint32 i = 0; i < m_surface->m_controls.size(); i++) {
      if (m_surface->m_controls[i] == this) {
        m_surface->m_controls.erase(m_surface->m_controls.begin());
        return;
      }
    }
    //Should not get to this point...
  }

  /////////////////////////////////////////////////////////////////////////////

  uint32 Control::id() {
    return m_id;
  }

  void Control::visible(bool flag) {
    m_visible = flag;
  }

  void Control::update() {
    if (!m_visible) {
      //return;
    }

    bool mi = mouseInside();

    switch(VAlign) {
      PCASE(VA_CLIENT,
        m_size.y = m_surface->getSize().y;
        m_position.y = 0;
      )
      PCASE(VA_BOTTOM,
        m_position.y = m_surface->getSize().y - m_size.y;
      )
      PCASE(VA_TOP,
        m_position.y = 0;
      )
      PCASE(VA_CENTER,    
        m_position.y = (m_surface->getSize().y / 2.f) - (m_size.y / 2.f);
      )
      PCASE(VA_CUSTOM,

      )
    };

    switch(HAlign) {
      PCASE(HA_CUSTOM,

      )
      PCASE(HA_CLIENT,
        m_position.x = 0;
        m_size.x = m_surface->getSize().x;
      )
      PCASE(HA_LEFT,
        m_position.x = 0;
      )
      PCASE(HA_RIGHT,
        m_position.x = m_surface->getSize().x - m_size.x;
      )
      PCASE(HA_CENTER,
        m_position.x = (m_surface->getSize().x / 2.f) - (m_size.x / 2.f);
      )
    };

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
        m_tooltipPos = m_input->mouseCoords();
      }

      if (m_input->mouseClick()) {
        //focus + click
        OnClick(m_id);
        m_focus = true;
       // m_surface->m_focused = this;
      }

      if (m_input->mouseDown()) {
        //mouse down
         OnMouseDown(m_id);
         if (!m_noStateHandling) {
          m_skinClass.setState(S_ACTIVE);
        }
      }

      if (m_tooltipPos.x != m_input->mouseCoords().x || m_tooltipPos.y != m_input->mouseCoords().y) {
        if (m_doingTooltip) {
          m_doingTooltip = false;
          m_surface->cancelTooltip();
        }
        m_tooltipPos = m_input->mouseCoords();
        m_mouseOverTime = getTime();
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
      if (!m_doingTooltip && Tooltip.size() > 0 && getTime() - m_mouseOverTime > 1000) {
        m_surface->doTooltip(Tooltip);
        m_doingTooltip = true;
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

  void Control::toFront() {
    m_surface->m_focused = this;
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
    if (!m_visible) return false;
    
    Vector2s c = m_input->mouseCoords();
    return c.x >= (m_wposition.x + m_position.x) && 
           c.x <= (m_wposition.x + m_position.x) + m_size.x &&
           c.y >= (m_wposition.y + m_position.y) && 
           c.y <= (m_wposition.y + m_position.y) + m_size.y;
  }
}