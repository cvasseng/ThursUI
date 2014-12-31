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
#include "../include/thurs/thurs.scrollbar.hpp"

namespace thurs {

  Scrollbar::Scrollbar(Surface* surface) {
    m_input = surface->input();
    m_renderer = surface->renderer();
    m_skin = surface->skin();
    m_scrollVal = 0.f;
    m_scrolling = false;
    m_bodyClass = 0;
    m_barClass = 0;
    setClass("Scrollbar");
  }

  //Update the current value - handles mouse interactions
  const float& Scrollbar::update(Vector2f size, Vector2f pos) {
    if (!m_barClass || !m_bodyClass) {
      return m_scrollVal;
    }

    //Calculate the pos
    float y = pos.y + (m_scrollVal * (size.y - size.x));

    Vector2s mc = m_input->mouseCoords();
    bool mo = mc.x >= pos.x && mc.x <= pos.x + size.x && mc.y >= y && mc.y <= pos.y + y + size.y;

    if (mo && m_input->mouseDown() && !m_scrolling) {
        m_scrolling = true;
        m_mdelta = mc;
        m_initialVal = y;
    }

    if (m_scrolling) {
      if (m_input->mouseUp()) {
        m_scrolling = false;
      } else {
        float ny = (m_initialVal + (mc.y - m_mdelta.y)) - pos.y;

        m_scrollVal = ny / (size.y - size.x);
        if (m_scrollVal < 0.f) m_scrollVal = 0.f;
        if (m_scrollVal > 1.f) m_scrollVal = 1.f;
      }
    }

    //Draw it
    m_renderer->renderRect(m_bodyClass->Attr, pos, size);
    m_renderer->renderRect(m_barClass->Attr, Vector2f(pos.x, y), Vector2f(size.x, size.x));

    return m_scrollVal;
  }

  //Return the current scroll position
  const float& Scrollbar::val() {
    return m_scrollVal;
  }

  //Set the skin class
  void Scrollbar::setClass(const std::string& name) {
    m_skinClass = m_skin->getClass(name);
    m_bodyClass = m_skinClass.findSub("body");
    m_barClass = m_skinClass.findSub("bar");
  }
}
