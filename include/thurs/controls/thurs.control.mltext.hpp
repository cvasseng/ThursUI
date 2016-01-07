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

#ifndef h__thurs_control_mltext__
#define h__thurs_control_mltext__

#include <string>
#include <vector>

#include "../thurs.control.hpp"
#include "../thurs.color.hpp"
#include "../thurs.scrollbar.hpp"

namespace thurs {

  /*
    Markup:
    
    [%classname%] bla bla bla [%otherclass%] 

  */

  class MultiLineText : public Control {
  public:

    ///////////////////////////////////////////////////////////////////////////

    struct TextEntry {
      std::string className;
      Skin::SkinClass skinClass;
      std::string id;
      std::string text;
      float width;
      float height;
      float y;

      TextEntry() {
        width = 0.f;
        height = 0.f;
        className = "default";
      }
    };

    struct TextLine {
      float width;
      float height;
      std::vector<TextEntry> items;

      TextLine() {
        width = 0.f;
        height = 0.f;
      }
    };

    ///////////////////////////////////////////////////////////////////////////

    //Constructor. Duh.
    MultiLineText(uint32 id, Surface *surface);
    //Update and draw
    void update();
    //Reload class
    void reloadSkinClass();
    //Type
    virtual WidgetType type() { return WT_MLTEXT; }

    ///////////////////////////////////////////////////////////////////////////

    //Append text
    void appendText(const std::string& text);
    //Append text as new line
    void appendLine(const std::string& text);
    //Clear it
    void clear();
    //Load contents from file
    void loadFromFile(const std::string& filename);

    ///////////////////////////////////////////////////////////////////////////
    
    sigslot::signal2<uint32, const std::string&> OnLinkClick; 

    ///////////////////////////////////////////////////////////////////////////

  protected:
    //Lines
    std::vector<TextLine> m_lines;
    //Actual height
    float m_actualHeight;
    //Scrollbar
    Scrollbar m_scrollbar;
  private: 
  };

}

#endif
