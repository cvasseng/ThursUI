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

#include "../thurs.os.hpp"
#include "../../include/thurs/thurs.hpp"
#include "../../include/thurs/controls/thurs.control.skillbar.hpp"

namespace thurs {

  //Constructor. Duh.
  SkillBar::SkillBar(uint32 id, Surface *surface)  : Control(id, surface), m_scrollbar(surface) {
    //Set default class
    setSkinClass("SkillBar");

    m_size.x = 400;
    m_size.y = 50;
    //Start out with a 10 slot bar - one for each number + 0
    setSlotCount(10);
  }

  void SkillBar::setSlotCount(uint16 c) {
    m_imagesX = c;
    m_entries.clear();
    for (uint32 i = 0; i < c; i++) {
      Entry en;
      en.coolingDown = false;
      en.activationTime = 0;
      en.meta.cooldownMS = 1000;
      en.meta.id = i;

      en.skinClass = m_skinClass.findAndCpySub("item");
      en.cooldownClass = m_skinClass.findAndCpySub("cooldown");

      std::stringstream ss;
      ss << (i + 1);
      en.meta.caption = ss.str();

      m_entries.push_back(en);
    }
  }
  
  //Update and draw
  void SkillBar::update() {
    //Do parent stuff
    Control::update();

    Vector2s mc = m_input->mouseCoords();

    //The width of each image should be so that we can fit m_imagesX onto it.
    float w = (m_size.x - 10) / m_imagesX;


    m_renderer->renderRect(m_skinClass.Attr, m_position + m_wposition, m_size);  

    m_renderer->setScissor(m_position + m_wposition, m_size);
    
    float height = (m_entries.size() / m_imagesX) * w;
    float scroll = m_scrollbar.val() * (height - m_size.y);

    if (height < m_size.y) {
      w = m_size.x / m_imagesX;
      height = (m_entries.size() / m_imagesX) * w;
    }

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
          //Should maybe check cooldown here
          OnUse(m_entries[i].meta);
          doSkill(m_entries[i].meta.id);
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

      //Draw the cooldown thing
      if (m_entries[i].coolingDown) {
        float progress = (float)(getTime() - m_entries[i].activationTime) / (float)m_entries[i].meta.cooldownMS;
        if (progress > 1.f) {
          m_entries[i].coolingDown = false;
        } else {
          //Draw it.
          m_renderer->renderRect(m_entries[i].cooldownClass.Attr, x + m, y + m, w - (m * 2), (w - (m * 2)) * (1.f - progress));
        }
        printf("%f %i %i\n", progress, m_entries[i].activationTime, m_entries[i].meta.cooldownMS);
      }

      if (m_entries[i].meta.caption.size() > 0) {
       // m_renderer->renderText(m_entries[i].skinClass.Attr, m_entries[i].meta.caption, Vector2f(x, y), Vector2f(w - (m * 2), w - (m * 2)));
        m_renderer->renderText(m_entries[i].skinClass.Attr, m_entries[i].meta.caption, x, y + height, w, m_size.y - height);
      }
      

      m_entries[i].skinClass.update();

      x += w;
    }

    if (height > m_size.y) {
      m_scrollbar.update(Vector2f(10, m_size.y), Vector2f(m_position.x + m_wposition.x + (m_size.x - 10), m_position.y + m_wposition.y));
    }

    m_renderer->clearScissor();

  }

  void SkillBar::doSkill(uint32 id, bool ignoreCooldown) {
    for (uint32 i = 0; i < m_entries.size(); i++) {
      if (m_entries[i].meta.id == id && (!m_entries[i].coolingDown || ignoreCooldown)) {
        m_entries[i].coolingDown = true;
        m_entries[i].activationTime = getTime();
        return;
      }
    }
  }

  void SkillBar::set(uint32 slot, uint32 id, const std::string& image, const std::string& caption) {
    if (slot < m_entries.size()) {
      m_entries[slot].coolingDown = false;
      m_entries[slot].activationTime = 0;
      m_entries[slot].image = image;

      m_entries[slot].meta.id = id;
      m_entries[slot].meta.cooldownMS = 1000;
      m_entries[slot].meta.caption = caption;

      if (image.size() > 0) {
        m_entries[slot].imageHandle = m_renderer->loadImage(image);
      } 
      
    }

  }


}