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
#include "../../include/thurs/controls/thurs.control.dropdown.hpp"

namespace thurs {

  //Constructor. Duh.
  DropDown::DropDown(uint32 id, Surface *surface)  : Control(id, surface) {
    //Set default class
    setSkinClass("DropDown");
    m_expanded = false;

    m_listbox = new ListBox(10000 + id, surface);

    m_listbox->OnSelect.connect(this, &DropDown::handleSelection);
  }
  
  //Update and draw
  void DropDown::update() {
    //Do parent stuff
    Control::update();

    if (mouseInside() && m_input->mouseDown()) {
      m_expanded = !m_expanded;
      if (m_expanded) {
        m_listbox->toFront();
      }
    }

    Vector2s mc = m_input->mouseCoords();

    /*if (m_expanded && !mouseInside() && 
        mc.x >= m_position.x + m_wposition.x && mc.x <= m_position.x + m_wposition.x + m_size.x &&
        mc.y >= m_position.y + m_wposition.y && mc.y <= m_position.y + m_wposition.y + m_size.y + 50.f
       ) {
      m_expanded = false;
    }*/

    m_renderer->renderRect(m_skinClass.Attr, m_position + m_wposition, m_size);  
    m_renderer->renderText(m_skinClass.Attr, m_caption, m_position + m_wposition, m_size);

    if (m_icon) {
      uint32 margin = m_icon->Attr.margins;
      Vector2f p = m_position + m_wposition;
      p.x += (m_size.x - m_size.y) - margin;
      p.y += margin;
      m_renderer->renderRect(m_icon->Attr, p, Vector2f(m_size.y - (margin * 2.f), m_size.y - (margin * 2.f)));
    }

    m_listbox->visible(m_expanded);
    if (m_expanded) {
      m_listbox->setWPosition(m_wposition);
      m_listbox->setPosition(m_position.x, m_position.y + m_size.y + 2);
      m_listbox->setSize(m_size.x, 100.f);
    }
  }

  void DropDown::setSkinClass(const std::string& name) {
    Control::setSkinClass(name);
    m_icon = m_skinClass.findSub("icon");
  }

  //Handle selection
  void DropDown::handleSelection(int sender, int item) {
    m_caption = (*m_listbox)[m_listbox->selectedIndex()]->title;
    OnSelect(id(), item);
    m_expanded = false;
  }

  //Add item
  void DropDown::addItem(uint32 id, const std::string& title) {
    m_listbox->addItem(id, title);
  }

  //Get the number of items
  uint32 DropDown::size() {
    return m_listbox->size();
  }
  
  //Get the current selected item index
  uint32 DropDown::selectedIndex() {
    return m_listbox->selectedIndex();
  }

  //Clear the list
  void DropDown::clear() {
    m_listbox->clear();
  }

  //Remove item
  bool DropDown::remItem(uint32 index) {
    return m_listbox->remItem(index);
  }


}