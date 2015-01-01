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
#include "../../include/thurs/controls/thurs.control.imagegrid.hpp"

namespace thurs {

  //Constructor. Duh.
  ImageGrid::ImageGrid(uint32 id, Surface *surface)  : Control(id, surface), m_scrollbar(surface) {
    //Set default class
    setSkinClass("ImageGrid");

    m_imagesX = 5;
  }

  void ImageGrid::setImageCountAlongX(uint16 c) {
    m_imagesX = c;
  }
  
  //Update and draw
  void ImageGrid::update() {
    //Do parent stuff
    Control::update();

    Vector2s mc = m_input->mouseCoords();

    //The width of each image should be so that we can fit m_imagesX onto it.
    float w = (m_size.x - 10) / m_imagesX;


    m_renderer->renderRect(m_skinClass.Attr, m_position + m_wposition, m_size);  

    m_renderer->setScissor(m_position + m_wposition, m_size);
    
    float height = (m_entries.size() / m_imagesX) * w;
    float scroll = m_scrollbar.val() * (height - m_size.y);

    float y = m_position.y + m_wposition.y - w - scroll;
    float x = m_position.x + m_wposition.x;
    for (uint32 i = 0; i < m_entries.size(); i++) {
      if (i % m_imagesX == 0) {
        y += w;
        x = m_position.x + m_wposition.x;
      }
     
      if (mouseInside() && mc.x >+ x && mc.x <+ x + w && mc.y >+ y && mc.y <+ y + w) {
        m_entries[i].skinClass.setState(S_HOVER);
        if (m_input->mouseDown()) {
          OnSelect(m_entries[i].meta);
        }
      } else {
        m_entries[i].skinClass.setState(S_NORMAL);
      }

      //Draw image
      uint32 m = m_entries[i].skinClass.Attr.margins;
      if (m_entries[i].image.size() > 0) {
        m_entries[i].skinClass.Attr.hasImage = true;
        m_entries[i].skinClass.Attr.imageHandle = m_entries[i].imageHandle;
      }
      m_renderer->renderRect(m_entries[i].skinClass.Attr, Vector2f(x + m, y + m), Vector2f(w - (m * 2), w - (m * 2)));

      if (m_entries[i].meta.caption.size() > 0) {
        m_renderer->renderText(m_entries[i].skinClass.Attr, m_entries[i].meta.caption, Vector2f(x, y), Vector2f(w - (m * 2), w - (m * 2)));
      }

      m_entries[i].skinClass.update();

      x += w;
    }

    m_scrollbar.update(Vector2f(10, m_size.y), Vector2f(m_position.x + m_wposition.x + (m_size.x - 10), m_position.y + m_wposition.y));

    m_renderer->clearScissor();

  }

  void ImageGrid::addImage(uint32 id, const std::string& image, const std::string& caption) {
    Entry en;
    en.meta.id = id;
    en.meta.caption = caption;
    en.image = image;
    if (en.image.size() > 0) {
      en.imageHandle = m_renderer->loadImage(en.image);
    }
    en.skinClass = m_skinClass.findAndCpySub("item");

    m_entries.push_back(en);
  }


}