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
#include "../include/thurs/controls/thurs.controls.hpp"

#include <fstream>
#include <sstream>
#include <json/writer.h>

namespace thurs {

  Surface::Surface(Renderer *renderer, Input *input) : m_skin(renderer) {
    m_renderer = renderer;
    m_input = input;
    m_canvasSize.x = 1024;
    m_canvasSize.y = 1024;
    m_tooltipActive = false;
    m_focused = 0;
    m_focusedChild = 0;
    m_visible = true;
  } 
  
  bool Surface::serialize(Json::Value &root) {
    Json::Value childWidgets;
    
    
     for (auto it : m_controls) {
      Json::Value c;
      it->serialize(c);
      childWidgets[std::to_string(it->id())] = c;  
    }
    
    Json::Value childSurfaces(Json::arrayValue);
    
    for (auto it : m_children) {
      Json::Value s;
      it->serialize(s);
      childSurfaces.append(s);
    }
    
    root["children"] = childSurfaces;
    root["widgets"] = childWidgets;
    
    return true;
  }
  
  //Serialize the surface to file 
  bool Surface::serialize(const std::string& filename) {
    std::filebuf fb;
    fb.open(filename.c_str(),std::ios::out);
    std::ostream os(&fb);
    
    if (!os.good()) {
      return false;
    }
    
    Json::Value root;
    
    serialize(root);
    
    os << root;
    
    fb.close();
    
    return true;
  }
  
  bool Surface::unserialize(Json::Value &root) {    
    Json::Value widgets = root["widgets"];
    Json::Value::Members wids = widgets.getMemberNames();
    
    for (uint32 i = 0; i < wids.size(); i++) {
      Json::Value child = widgets[wids[i]];
      uint32 id = 0;
      Control* c = 0;
      
      std::stringstream ss(wids[i]);
      ss >> id;
      
      if (child.isMember("type")) {
        std::string t = child.get("type", WT_UNKNOWN).asString();
        uint32 type = 0;
        std::stringstream ss(t);
        ss >> type;
        
        WidgetType tp = WidgetType(type);
        
        if (tp == WT_BUTTON) {
          c = new Button(id, this);
        } else if (tp == WT_CHECKBOX) {
          c = new Checkbox(id, this);
        } else if (tp == WT_DROPDOWN) {
          c = new DropDown(id, this);
        } else if (tp == WT_EDITBOX) {
          c = new EditBox(id, this);
        } else if (tp == WT_IMAGEGRID) {
          c = new ImageGrid(id, this);
        } else if (tp == WT_LABEL) {
          c = new Label(id, this);
        } else if (tp == WT_LISTBOX) {
          c = new ListBox(id, this);
        } else if (tp == WT_PROGRESSBAR) {
          c = new ProgressBar(id, this);
        } else if (tp == WT_RECTANGLE) {
          c = new Rectangle(id, this);
        } else if (tp == WT_SKILLBAR) {
          c = new SkillBar(id, this);
        } else if (tp == WT_SLIDER) {
          c = new Slider(id, this);
        }
          
        if (c) {
          c->unserialize(child);
        }  
      }
    }
    
    Json::Value children = root["children"];
    for (uint32 i = 0; i < children.size(); i++) {
      Json::Value child = children[i];
      if (child["type"].asString() == "window") {
        thurs::Window* win = new thurs::Window(this);
        win->unserialize(child);
      }
    }
    
    return false;
  }
  
  //Unserialize the surface from file 
  bool Surface::unserialize(const std::string& filename) {
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(str, root)) {
      return false;
    }

    return unserialize(root);
  }

  void Surface::resize(const Vector2f& vec) {
    m_canvasSize = vec;
  }

  void Surface::resize(float w, float h) {
    m_canvasSize.x = w;
    m_canvasSize.y = h;
  }

  void Surface::_onUpdate() {
    //We need to check if the mouse is over the child
    if (m_focusedChild && m_focusedChild->mouseOver()) {
      m_input->disable();
    } else {
      m_input->enable();
    }

    //Update the children
    for (uint32 i = 0; i < m_controls.size(); i++) {
      if (!m_focused || m_focused != m_controls[i]) {
        m_controls[i]->update();
      }
    }

    if (m_focused) {
      m_focused->update();
    }
  }

  void Surface::doTooltip(const std::string& text) {
    m_tooltipActive = true;
    m_tooltipClass.setState(S_ACTIVE);
    m_tooltipPos = m_input->mouseCoords();
  } 

  void Surface::cancelTooltip() {
    m_tooltipClass.setState(S_NORMAL);
    m_tooltipActive = false;
    m_tooltipClass.reset();
  }

  const Vector2f& Surface::getSize() {
    return m_canvasSize;
  }


  void Surface::hide() {
    m_visible = false;
  }
  
  void Surface::show() {
    m_visible = true;
  }

  void Surface::updateAndRender(float pixelFormat) {
    if (!m_visible) {
      return;
    }

   // printf("W %f H %f\n", m_canvasSize.x, m_canvasSize.y);
    m_renderer->begin(m_canvasSize.x, m_canvasSize.y, pixelFormat);

    _onUpdate();

    //May need to rearrange things
    //This is a really shitty solution and needs to be patched.
    if (m_focused && m_controls.size() > 0 && m_controls[m_controls.size() - 1] != m_focused) {
      Control* ph = m_controls[m_controls.size() - 1];
      for (uint32 i = 0; i < m_controls.size(); i++) {
        if (m_controls[i] == m_focused) {
          m_controls[m_controls.size() - 1] = m_focused;
          m_controls[i] = ph;
          break;
        }
      }
    }

    if (m_tooltipActive) {
      m_renderer->renderRect(m_tooltipClass.Attr, Vector2f(m_tooltipPos.x, m_tooltipPos.y), Vector2f(100.f, 100.f));
      m_tooltipClass.update();
    }

    m_renderer->end();

    int32 focusedIndex = -1;

    m_moveFocusedChildToTop = false;

    for (uint32 i = 0; i < m_children.size(); i++) {
      if (!m_focusedChild || (m_focusedChild == m_children[i] || !m_focusedChild->m_visible)) {
        m_input->enable();
        focusedIndex = i;
      } else {
        m_input->disable();
      }

      m_children[i]->m_canvasSize = m_canvasSize;
      m_children[i]->updateAndRender(pixelFormat);
    }

    //If the focused child is not on the top of the stack, move it there.
    if ( m_children.size() > 1 && m_children.size() - 1 != focusedIndex) {
      Surface* p = m_children[m_children.size() - 1];
      m_children[m_children.size() - 1] = m_focusedChild;
      m_children[focusedIndex] = p;
    }

  }

  Skin* const Surface::skin() {
    return &m_skin;
  }

  //Returns the attached input handler
  Input* const Surface::input() {
    return m_input;
  }

  Renderer* const Surface::renderer() {
    return m_renderer;
  }

  bool Surface::reloadSkin() {
    if (m_skin.reload()) {

      for (uint32 i = 0; i < m_controls.size(); i++) {
        m_controls[i]->reloadSkinClass();
      }

      for (uint32 i = 0; i < m_children.size(); i++) {
        m_children[i]->reloadSkin();
      }

      return true;
    }
    return false;
  }

  bool Surface::loadSkin(const std::string& filename) {
    if (m_skin.load(filename)) {
      m_tooltipClass = m_skin.getClass("Tooltip");
      for (auto it : m_skin.Fonts) {
        m_renderer->addFont(it.name, it.filename);
      }
      return true;
    }
    return false;
  }

}
