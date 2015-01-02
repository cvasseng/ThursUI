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
#include "../thurs.os.hpp"

#include "../../include/thurs/thurs.hpp"
#include "../../include/thurs/controls/thurs.control.editbox.hpp"

namespace thurs {

  //Constructor. Duh.
  EditBox::EditBox(uint32 id, Surface *surface)  : Control(id, surface), InputListener(surface->input()) {
    //Set default class
    m_cursorClass = 0;
    setSkinClass("EditBox");
    m_cursor = 0;
  }
  
  //Update and draw
  void EditBox::update() {
    //Do parent stuff
    Control::update();

    float cpos = m_renderer->getTextWidth(m_skinClass.Attr, Value.substr(0, m_cursor));

    m_renderer->setScissor(m_position + m_wposition, m_size);

    float scroll = 0.f;
    if (cpos > m_size.x) {
      scroll = (cpos - m_size.x);
    }

    Vector2f pos = m_position + m_wposition;
    pos.x -= scroll;

    m_renderer->renderRect(m_skinClass.Attr, m_position + m_wposition, m_size);  
    m_renderer->renderText(m_skinClass.Attr, Value, pos, m_size);

    if (getTime() - m_blinkTimer > 500) {
      m_blink = !m_blink;
      m_blinkTimer = getTime();
    }

    //Draw cursor
    if (m_cursorClass && m_blink && m_focus) {

      Vector2f cc = Vector2f(cpos, m_cursorClass->Attr.margins);
      if (cc.x > m_size.x) {
        cc.x = m_size.x - 1;
      }

      m_renderer->renderRect(m_cursorClass->Attr, m_position + m_wposition + cc, Vector2f(1, m_size.y - (m_cursorClass->Attr.margins * 2)));
    }

    m_renderer->clearScissor();
  }

  bool EditBox::OnKeyPress(uint32 charCode) {
    if (m_focus) {
      //Value += (char)charCode;
      //std::string i = (char*)charCode;
      Value.insert(m_cursor, std::string((char*)&charCode));
      m_cursor++;  
      return true;
    }
    return false;
  }

  bool EditBox::OnKeyDown(uint32 scancode) {
    if (scancode == 51) {
      printf("backspace\n");
        //Value += (char)charCode;
      if (Value.size() > m_cursor - 1) {
        m_cursor--;
        Value.erase(Value.begin() + m_cursor);
      }
    }
    if (scancode == 123) {
      m_cursor--;
      if (m_cursor < 0) {
        m_cursor = 0;
      }
    }
    if (scancode == 124) {
      m_cursor++;
      if (m_cursor > Value.size()) {
        m_cursor = Value.size();
      }
    }
  }

  void EditBox::setSkinClass(const std::string& name) {
    Control::setSkinClass(name);
    m_cursorClass = m_skinClass.findSub("cursor");
  }


}