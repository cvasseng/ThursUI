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

  //Constructor
  SkinPart::SkinPart() {
    for (int8 i = 0; i < S_COUNT; i++) {
      m_states[i].active = false;
    }
  }
  
  //Set the current state
  void SkinPart::setState(SkinState s) {
    if (s == S_COUNT) {
      return;
    }

    m_state = s;
    
    if (m_states[s].active) {
      //Start tweening everything to the new state
      Active.fill.tween(m_states[s].fill);
      Active.stroke.tween(m_states[s].stroke);
      Active.text.tween(m_states[s].text);
    }
  }

  //Update the skin
  void SkinPart::update() {
    Active.fill.update();
    Active.stroke.update();
    Active.text.update();
  }

  void SkinPart::add(SkinState s, SkinPart::Atom props) {
    if (s != S_COUNT) {
      m_states[s] = props;
      m_states[s].active = true;
    }
  }

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

    Surface::ControlMapIt it = m_surface->m_controls.find(id);
    if (it == m_surface->m_controls.end()) {
      m_surface->m_controls.insert(ControlMapPair(id, this));
    } else {
      //Fatal error..
    }
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
         MouseIn();
         m_background.tween(Color(43, 102, 236));
         m_foreground.tween(Color(255));
         //We should tween the color here

          m_mouseWasInside = true;
      }

      if (m_input->mouseClick()) {
        //focus + click
        Clicked();
      }

      if (m_input->mouseDown()) {
        //mouse down
         MouseDown();
         m_background.tween(Color(43, 102, 136));
      }


      if (m_input->mouseUp()) {
        m_background.tween(Color(43, 102, 236));
        //mouse up
        MouseUp();
      }


    } else if (m_mouseWasInside) {
      //on mouse leave
      MouseOut();
      m_foreground.tween(Color(190));
      m_background.tween(Color(40));
      m_mouseWasInside = false;
    }

    m_background.update();
    m_foreground.update();

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
  void Control::setPosition(const Vector2s& pos) {
    m_position = pos;
  }

  void Control::setPosition(Vector2s pos) {
    m_position = pos;
  }

  void Control::setPosition(uint16 x, uint16 y) {
    m_position.x = x;
    m_position.y = y;
  }

  //Set the size of the control
  void Control::setSize(const Vector2s& size) {
    m_size.x = size.x;
    m_size.y = size.y;
  }

  void Control::setSize(Vector2s size) {
    m_size.x = size.x;
    m_size.y = size.y;
  }

  void Control::setSize(uint16 w, uint16 h) {
    m_size.x = w;
    m_size.y = h;
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