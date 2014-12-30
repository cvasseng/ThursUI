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

#ifndef h__thurs__skin__
#define h__thurs__skin__

#include <string>
#include <map>

#include "thurs.types.hpp"

namespace Json {
  class Value;
}

namespace thurs {

  enum SkinState {
    S_NORMAL,  
    S_HOVER,   
    S_ACTIVE, 
    S_FOCUS,
    S_COUNT, 
  };

  class Skin {
  public:

    ///////////////////////////////////////////////////////////////////////////

    //Think of it as a CSS class..
    class SkinClass {
    public:
      friend class Skin;
      struct Attributes {
        Color fill;
        Color stroke;
        Color textFill;
        Color textStroke;
        uint16 margins;

        bool hasFill;
        bool hasStroke;
        bool hasTextFill;
        bool hasTextStroke;
        bool hasMargins;
      };

      SkinClass();
      //Set the current state
      void setState(SkinState s);
      //Update the skin - does tweening etc.
      void update();
      //Reset the skin
      void reset();
      //Find a sub class
      SkinClass* const findSub(const std::string& name);

      Attributes Attr;
    protected:
      //Current state
      SkinState m_state;
      //The available states
      Attributes m_attributes[S_COUNT];
      //Sub classes
      std::map<std::string, SkinClass> m_subs;
    private:
    };

    typedef std::map<std::string, SkinClass> ClassMap;
    typedef ClassMap::iterator ClassMapIt;
    #define ClassMapPair(x, y) std::pair<std::string, SkinClass>(x, y)

    ///////////////////////////////////////////////////////////////////////////
    
    //Load skin
    bool load(const std::string& filename);
    //Get a class - this returns a copy
    SkinClass getClass(const std::string& name);
    //Get a class pointer - this returns the actual class.
    /* Useful for layout tools. */
    SkinClass* const getClassPtr(const std::string& name);
  protected:
    //Class map
    ClassMap m_classes;

    //Parse a json value as a skin class
    void jsonToStates(Json::Value &v, Skin::SkinClass& sc);
    void jsonToAttribute(Json::Value &v, Skin::SkinClass::Attributes& attr, Skin::SkinClass& sc);
  private:
  };


}

#endif
