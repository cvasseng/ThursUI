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

#include <fstream>
#include <json/json.h>

#include "../include/thurs/thurs.hpp"

namespace thurs {

  Skin::SkinClass::SkinClass() {
    Attr.margins = 2;
  }

  //Set the current state
  void Skin::SkinClass::setState(SkinState s) {
    //Start tweening
    Attr.fill.tween(m_attributes[s].fill);
    Attr.stroke.tween(m_attributes[s].stroke);
    Attr.textFill.tween(m_attributes[s].textFill);
    Attr.textStroke.tween(m_attributes[s].textStroke);

    for (ClassMapIt it = m_subs.begin(); it != m_subs.end(); it++) {
      it->second.setState(s);
    }

    m_state = s;
  }

  //Update the skin - does tweening etc.
  void Skin::SkinClass::update() {
    Attr.fill.update();
    Attr.stroke.update();
    Attr.textFill.update();
    Attr.textStroke.update();

    for (ClassMapIt it = m_subs.begin(); it != m_subs.end(); it++) {
      it->second.update();
    }
  }

  void Skin::SkinClass::reset() {
    Attr.fill = m_attributes[S_NORMAL].fill;
    Attr.stroke = m_attributes[S_NORMAL].stroke;
    Attr.textFill = m_attributes[S_NORMAL].stroke;
    Attr.textStroke = m_attributes[S_NORMAL].textStroke;
    Attr.margins = m_attributes[S_NORMAL].margins;

    for (ClassMapIt it = m_subs.begin(); it != m_subs.end(); it++) {
      it->second.reset();
    }
  }

  Skin::SkinClass* const Skin::SkinClass::findSub(const std::string& name) {
    ClassMapIt it = m_subs.find(name);
    if (it != m_subs.end()) {
      return &it->second;
    }
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////

  void Skin::jsonToAttribute(Json::Value &v, Skin::SkinClass::Attributes& attr, Skin::SkinClass& sc) {

    attr.hasFill = false;
    attr.fill = sc.m_attributes[S_NORMAL].fill;
    attr.hasStroke = false;
    attr.stroke = sc.m_attributes[S_NORMAL].stroke;
    attr.hasTextFill = false;
    attr.textFill = sc.m_attributes[S_NORMAL].textFill;
    attr.hasTextStroke = false;
    attr.textStroke = sc.m_attributes[S_NORMAL].textStroke;
    attr.hasMargins = false;
    attr.margins = sc.m_attributes[S_NORMAL].margins;

    if (v.isMember("fill")) {
      attr.fill = Color(v.get("fill", "255 255 255 255").asString());
    } 

    if (v.isMember("stroke")) {
      attr.stroke = Color(v.get("stroke", "255 255 255 255").asString());
    } 

    if (v.isMember("textFill")) {
      attr.textFill = Color(v.get("textFill", "255 255 255 255").asString());
    } 

    if (v.isMember("textStroke")) {
      attr.textStroke = Color(v.get("textStroke", "255 255 255 255").asString());
    } 

    if (v.isMember("margins")) {
      attr.margins = v.get("margins", "0").asInt();
      printf("margins is %i\n", attr.margins);
    }

  }

  void Skin::jsonToStates(Json::Value &v, Skin::SkinClass& sc) {
    //Try to find the states: NORMAL, HOVER, ACTIVE, FOCUS
    if (v.isMember("normal")) {
      jsonToAttribute(v["normal"], sc.m_attributes[S_NORMAL], sc);
    }

    if (v.isMember("hover")) {
      jsonToAttribute(v["hover"], sc.m_attributes[S_HOVER], sc);
    } else { //Copy normal state
      sc.m_attributes[S_HOVER] = sc.m_attributes[S_NORMAL];
    }
    
    if (v.isMember("active")) {
      jsonToAttribute(v["active"], sc.m_attributes[S_ACTIVE], sc);
    } else { //Copy normal state
      sc.m_attributes[S_ACTIVE] = sc.m_attributes[S_HOVER];
    }

    if (v.isMember("focus")) {
      jsonToAttribute(v["focus"], sc.m_attributes[S_FOCUS], sc);
    } else { //Copy focus state
      sc.m_attributes[S_FOCUS] = sc.m_attributes[S_NORMAL];
    }
  }

  //Load skin
  bool Skin::load(const std::string& filename) {
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(str, root)) {
      return false;
    }

    Json::Value::Members classes = root.getMemberNames();
    for (uint32 i = 0; i < classes.size(); i++) {
      std::string name = classes[i];
      Json::Value v = root[name];

      Skin::SkinClass sc;

      bool addP = false;

      //We need to figure out if we're dealing with sub classes.
      Json::Value::Members subs = v.getMemberNames();
      for (uint32 j = 0; j < subs.size(); j++) {
        if (subs[j] != "normal" && subs[j] != "hover" && subs[j] != "active" && subs[j] != "focus") {
          //Yup, this is a sub class.
          Skin::SkinClass sub;
          jsonToStates(v[subs[j]], sub);

          sc.m_subs.insert(ClassMapPair(subs[j], sub));

        } else {
          addP = true;
        }
      }

      //There's a valid state in the root
      if (addP) {
        jsonToStates(v, sc);
      }

      sc.reset();
      m_classes.insert(ClassMapPair(name, sc));
    }

    return true;
  }

  //Get a class - this returns a copy
  Skin::SkinClass Skin::getClass(const std::string& name) {
    SkinClass def;

    ClassMapIt it = m_classes.find(name);
    if (it != m_classes.end()) {
      return it->second;
    }

    return def;
  }

  //Get a class reference - this returns the actual class.
  /* Useful for layout tools. */
  Skin::SkinClass* const Skin::getClassPtr(const std::string& name) {
    ClassMapIt it = m_classes.find(name);
    if (it != m_classes.end()) {
      return &it->second;
    }
    return 0;
  }

}
