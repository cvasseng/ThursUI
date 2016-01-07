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

#include "../../include/thurs/thurs.hpp"
#include "../../include/thurs/controls/thurs.control.checkbox.hpp"

namespace thurs {

  //Constructor. Duh.
  Checkbox::Checkbox(uint32 id, Surface *surface)  : 
    Control(id, surface),
    Caption("title") 
  {
    Caption = "Check me!";
    Checked = false;
    //Set default class
    m_size.y = 15;
    setSkinClass("Checkbox");

    m_noStateHandling = true;
    Properties.add(Caption);
  }

  void Checkbox::setSkinClass(const std::string& name) {
    Control::setSkinClass(name);

    m_frame = m_skinClass.findSub("background");
    m_markClass = m_skinClass.findSub("mark");
  }
   
  //Update and draw
  void Checkbox::update() {
    //Do parent stuff
    Control::update();

    if (!m_frame || !m_markClass) {
      return;
    }

    //Handle state changes for the check mark
    if (mouseInside()) {
      if (!Checked) {
        m_skinClass.setState(S_HOVER);
      }
      if (m_input->mouseUp()) {
        Checked = !Checked;
        OnChange(id(), Checked);
        if (Checked) {
          m_skinClass.setState(S_ACTIVE);
        } else {
          m_skinClass.setState(S_NORMAL);
        }
      }
    } else if (!Checked) {
      m_skinClass.setState(S_NORMAL);
    }

    //This component is a bit special as it will only utilize m_size.y.

    //Frame:
    m_renderer->renderRect(m_frame->Attr, m_cposition.x, m_cposition.y, m_size.y, m_size.y);

    uint32 margins = m_markClass->Attr.margins;
    m_renderer->renderRect(m_markClass->Attr, m_cposition.x + margins, m_cposition.y + margins, m_size.y - (margins * 2), m_size.y - (margins * 2));  

    //Text
    m_renderer->renderText(m_markClass->Attr, Caption, m_cposition.x + m_size.y + 5, m_cposition.y, m_size.y, m_size.y);
  
  }


}