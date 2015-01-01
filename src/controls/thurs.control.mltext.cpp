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
#include <fstream>

#include "../../include/thurs/thurs.hpp"
#include "../../include/thurs/controls/thurs.control.mltext.hpp"

namespace thurs {

  //Constructor. Duh.
  MultiLineText::MultiLineText(uint32 id, Surface *surface)  : Control(id, surface), m_scrollbar(surface) {
    Caption = "BUTTON";
    //Set default class
    setSkinClass("MultiLineText");
  }
  
  //Update and draw
  void MultiLineText::update() {
    //Do parent stuff
    Control::update();

    Vector2s mc = m_input->mouseCoords();

    //Temporary
    m_actualHeight = 0.f;
    for (uint32 j = 0; j < m_lines.size(); j++) {
      m_actualHeight += m_lines[j].height;
    }

    m_renderer->renderRect(m_skinClass.Attr, m_position + m_wposition, m_size);  

    m_renderer->setScissor(m_position + m_wposition, m_size);
   // m_renderer->renderText(m_skinClass.Attr, Caption, m_position + m_wposition, m_size);

    float y = -(m_scrollbar.val() * (m_actualHeight - m_size.y));
    for (uint32 j = 0; j < m_lines.size(); j++) {

      Vector2f pos = m_position + m_wposition;
      pos.y += y;
      for (uint32 i = 0; i < m_lines[j].items.size(); i++) {
        TextEntry* entry = &m_lines[j].items[i];

        if (mc.x >= pos.x && mc.x <= pos.x + entry->width && mc.y >= pos.y && mc.y <= pos.y + entry->height) {
          if (entry->id.size() > 0 && m_input->mouseDown()) {
            //Click link
            OnLinkClick(id(), entry->id);
          }

          entry->skinClass.setState(S_HOVER);
        } else {
          entry->skinClass.setState(S_NORMAL);
        }

        entry->skinClass.update();
        m_renderer->renderText(entry->skinClass.Attr, entry->text, pos, m_size);
        pos.x += entry->width;
      }

      y += m_lines[j].height;

    }

    m_renderer->clearScissor();

    if (m_actualHeight > m_size.y) {
      m_scrollbar.update(Vector2f(10, m_size.y), Vector2f(m_position.x + m_wposition.x + (m_size.x - 10), m_position.y + m_wposition.y));
    }

  }

  //Append text
  void MultiLineText::appendText(const std::string& text) {
    std::string buffer;
    std::string className;
    std::string id;
    bool readingClass = false;

    if (m_lines.size() == 0) {
      TextLine l;
      m_lines.push_back(l);
    }

    TextLine *activeLine = &m_lines[m_lines.size() - 1];

    if (activeLine->items.size() == 0) {
      TextEntry entry;
      entry.skinClass = m_skinClass.findAndCpySub("default");
      activeLine->items.push_back(entry);
    }

    TextEntry *active = &activeLine->items[activeLine->items.size() - 1];


    std::stringstream ss(text);

    while(!ss.eof()) {
      ss >> buffer;

      if (buffer[0] == '[' && buffer[1] == '$' && buffer[buffer.size() - 2] == '$' && buffer[buffer.size()- 1] == ']') {
        //We need to parse the class name, and ID if it exists.
        id = "";
        className = buffer.substr(2, buffer.size() - 4);
        for (uint32 i = 0; i < buffer.size(); i++) {
          if (buffer[i] == '#') {
            className = buffer.substr(2, i - 2);
            id = buffer.substr(i + 1, buffer.size() - i - 3);
            break;
          } 
        }
        //printf("\nFound class name: %s id is: %s\n", className.c_str(), active->id.c_str());

        TextEntry te;
        te.skinClass = m_skinClass.findAndCpySub(className);
        activeLine->items.push_back(te);
        active = &activeLine->items[activeLine->items.size() - 1];
        active->id = id;
      } else {
        printf("%s ", buffer.c_str());
        //Check if the width of the buffer causes the line to overflow
        float bufferWidth = m_renderer->getTextWidth(active->skinClass.Attr, buffer + ' ');
        float bufferHeight = m_renderer->getTextHeight(active->skinClass.Attr, buffer);
       
        if (activeLine->width + bufferWidth > m_size.x - 10.f) {
          //Yup.
          TextLine l;
          TextEntry e;
          e.skinClass = active->skinClass;
          e.text = buffer + ' ';
          e.width = bufferWidth;
          e.height = bufferHeight;
          e.id = active->id;
          l.width = bufferWidth;
          l.height = bufferHeight;

          l.items.push_back(e);
          m_lines.push_back(l);

          activeLine = &m_lines[m_lines.size() - 1];
          active = &activeLine->items[activeLine->items.size() - 1];
        } else {
          //Nope. Just add it.
          active->height = bufferHeight;
          active->width += bufferWidth;
          active->text += buffer + ' ';
          activeLine->width += bufferWidth;
          activeLine->height = activeLine->height > bufferHeight ? activeLine->height : bufferHeight;

        }
      }
    }
  }

  void MultiLineText::appendLine(const std::string& text) {
    TextLine l;
    m_lines.push_back(l);
    appendText(text);
  }

  void MultiLineText::clear() {
    m_actualHeight = 0.f;
    m_lines.clear();
  }

  void MultiLineText::loadFromFile(const std::string& filename) {
    clear();
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    appendText(str);
  }


}