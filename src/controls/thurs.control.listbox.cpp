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
#include "../../include/thurs/controls/thurs.control.listbox.hpp"

namespace thurs {

  //Constructor. Duh.
  ListBox::ListBox(uint32 id, Surface *surface)  : Control(id, surface), m_scrollbar(surface) {
    //Set default class
    setSkinClass("ListBox");
    m_size.y = 80;
    m_selItem = -1;
  }

  //Add item
  void ListBox::addItem(uint32 id, const std::string& title) {
    ItemFull itm;
    itm.skinClass = m_skinClass.findAndCpySub("item");
    itm.meta.id = id;
    itm.meta.title = title;
    m_items.push_back(itm);
  }

  //Get the number of items
  uint32 ListBox::size() {
   return  m_items.size();
  }

  void ListBox::setSkinClass(const std::string& name) {
    Control::setSkinClass(name);

    //Find the subclasses that contain the styling
    m_bodyClass = m_skinClass.findSub("body");
  }

  uint32 ListBox::selectedIndex() {
    return m_selItem;
  }

  void ListBox::clear() {
    m_items.clear();
  }

  bool ListBox::remItem(uint32 index) {
    if (index < m_items.size()) {
      m_items.erase(m_items.begin() + index);
      return true;
    }
    return false;
  }
   
  //Update and draw
  void ListBox::update() {
    //Do parent stuff
    Control::update();

    Vector2s mc = m_input->mouseCoords();
    bool ison = mouseInside();

    if (!m_bodyClass) {
      return;
    }

    //Render the body
    m_renderer->renderRect(m_bodyClass->Attr, m_position + m_wposition, m_size);  

    m_renderer->setScissor(m_position + m_wposition, m_size);

    float height = m_items.size() * 20;
    float scroll = m_scrollbar.val() * (height - m_size.x);

    //Render the items
    for (uint32 i = 0; i < m_items.size(); i++) {
      float y = (i * 20);

      //Check if the mouse is over this item
      if (ison && mc.x < m_position.x + m_wposition.x + m_size.x -10 && mc.y >= m_position.y + m_wposition.y + y - scroll && mc.y <= m_position.y + m_wposition.y + y + 20 - scroll) {
        if (m_input->mouseUp()) {
          m_items[i].skinClass.setState(S_ACTIVE);
          m_selItem = i;
          OnSelect(m_items[i].meta.id, m_selItem);
        } else if (m_selItem != i) {
          m_items[i].skinClass.setState(S_HOVER);
        }
      } else if (m_selItem != i) {
        m_items[i].skinClass.setState(S_NORMAL);
      }

      m_items[i].skinClass.update();
      m_renderer->renderRect(m_items[i].skinClass.Attr, m_position + m_wposition + Vector2f(0, y - scroll), Vector2f(m_size.x, 20));
      m_renderer->renderText(m_items[i].skinClass.Attr, m_items[i].meta.title, m_position + m_wposition + Vector2f(0,  y - scroll + 10));
    }

    m_scrollbar.update(Vector2f(10, m_size.y), Vector2f(m_position.x + m_wposition.x + (m_size.x - 10), m_position.y + m_wposition.y));

    m_renderer->clearScissor();

  }
}