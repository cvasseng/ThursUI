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

#include "../../include/thurs/thurs.hpp"
#include "../../include/thurs/controls/thurs.control.slider.hpp"

namespace thurs {

  //Constructor. Duh.
  Slider::Slider(uint32 id, Surface *surface)  : Control(id, surface) {
    //Set default class
    setSkinClass("Slider");
    Min = 0;
    Max = 100;
    Value = 50;
    m_isDragging = false;

    m_size.y = 15;
  }

  void Slider::setSkinClass(const std::string& name) {
    Control::setSkinClass(name);

    //Find the subclasses that contain the styling
    m_knob = m_skinClass.findSub("knob");
    m_bar = m_skinClass.findSub("bar");
  }
   
  //Update and draw
  void Slider::update() {
    //Do parent stuff
    Control::update();

    float p = ( (float)Value / (float)Max ) * (m_size.x - m_size.y);

    Vector2s mc = m_input->mouseCoords();
    bool m = mc.x >= m_position.x + m_wposition.x + p && mc.x <= m_position.x + m_wposition.x + p + m_size.y && 
             mc.y >= m_position.y + m_wposition.y && mc.y <= m_position.y + m_wposition.y + m_size.y;

    //Handle dragging
    if (m && m_input->mouseDown() && !m_isDragging) {
      m_isDragging = true;
      m_preDrag = m_position.x + m_wposition.x + p;
      m_mdelta = mc;
    }

    if (m_isDragging) {
      if (m_input->mouseUp()) {
        m_isDragging = false;
      } else {
        Value = (((m_preDrag + (mc.x - m_mdelta.x)) - m_position.x - m_wposition.x) / (m_size.x - m_size.y)) * Max;
        if (Value < Min) Value = Min;
        if (Value > Max) Value = Max;
        OnChange(Value);
      }
    }

    //Draw bar
    if (m_bar) {
      m_renderer->renderRect(m_bar->Attr, m_position + m_wposition + Vector2f(0, m_size.y / 3.f), Vector2f(m_size.x, m_size.y / 4.f));  
    }

    //Draw knob
    if (m_knob) {
      std::stringstream valStr;
      valStr << Value;
      Vector2f pos = m_wposition + m_position + Vector2f(p, 0);
      m_renderer->renderRect(m_knob->Attr, pos, Vector2f(m_size.y, m_size.y));  
      m_renderer->renderText(m_knob->Attr, valStr.str(), m_position + m_wposition + Vector2f(0, m_size.y), Vector2f(m_size.x, 10));
    }
  }
}