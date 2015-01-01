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
#include "../../include/thurs/controls/thurs.control.progressbar.hpp"

namespace thurs {

  //Constructor. Duh.
  ProgressBar::ProgressBar(uint32 id, Surface *surface)  : Control(id, surface) {
    Min = 0;
    Max = 100;
    Value = 50;

    //Set default class
    setSkinClass("ProgressBar");
  }

  void ProgressBar::setSkinClass(const std::string& name) {
    Control::setSkinClass(name);

    //Find the subclasses that contain the styling
    m_background = m_skinClass.findSub("background");
    m_bar = m_skinClass.findSub("bar");
  }
   
  //Update and draw
  void ProgressBar::update() {
    //Do parent stuff
    Control::update();

    std::stringstream ss;
    ss << Value << "/" << Max;

    if (m_background && m_bar) {
      uint16 m = m_bar->Attr.margins;
      uint16 w = (((float)Value / (float)(Max))) * (m_size.x - (m * 2));
      m_renderer->renderRect(m_background->Attr, m_position + m_wposition, m_size);  
      m_renderer->renderRect(m_bar->Attr, m_wposition + m_position + Vector2f(m, m), Vector2f(w, m_size.y - (m * 2)));  
      m_renderer->renderText(m_bar->Attr, ss.str(), m_wposition + m_position, m_size);
    }
  }
}