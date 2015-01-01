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
#include "../../include/thurs/controls/thurs.control.rectangle.hpp"

namespace thurs {

  //Constructor. Duh.
  Rectangle::Rectangle(uint32 id, Surface *surface)  : Control(id, surface) {
    //Set default class
    setSkinClass("Rectangle");
    m_imageHandle = 0;
  }

  void Rectangle::setImage(const std::string& filename) {
    m_imageHandle = m_renderer->loadImage(filename);
  }
  
  //Update and draw
  void Rectangle::update() {
    //Do parent stuff
    Control::update();

    if (m_imageHandle > 0) {
      m_skinClass.Attr.imageHandle = m_imageHandle;
      m_skinClass.Attr.hasImage = true;
    }

    m_renderer->renderRect(m_skinClass.Attr, m_position + m_wposition, m_size); 
  }


}