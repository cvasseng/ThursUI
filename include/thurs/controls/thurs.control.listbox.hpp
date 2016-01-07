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

#ifndef h__thurs__listbox__
#define h__thurs__listbox__

#include <vector>
#include <string>

#include "../thurs.skin.hpp"
#include "../thurs.control.hpp"
#include "../thurs.color.hpp"
#include "../thurs.scrollbar.hpp"

namespace thurs {

  class ListBox : public Control {
  public:
    ///////////////////////////////////////////////////////////////////////////

    struct Item {
      uint32 id;
      std::string title;
    };

    struct ItemFull {
      Skin::SkinClass skinClass;
      Item meta;
    };

    ///////////////////////////////////////////////////////////////////////////

    //emits Item:id and the selected index
    sigslot::signal2<int, int> OnSelect;

    ///////////////////////////////////////////////////////////////////////////

    //Constructor. Duh.
    ListBox(uint32 id, Surface *surface);
    //Update and draw
    void update();
    //Set the currently used skin class
    void setSkinClass(const std::string& name);
    //Add item
    void addItem(uint32 id, const std::string& title);
    //Get the number of items
    uint32 size();
    //Get the current selected item index
    uint32 selectedIndex();
    //Clear the list
    void clear();
    //Remove item
    bool remItem(uint32 index);
    //Type
    virtual WidgetType type() { return WT_LISTBOX; }

    ///////////////////////////////////////////////////////////////////////////

    //Access item
    Item* const operator[](uint32 index) {
      if (index < m_items.size()) {
        return &m_items[index].meta;
      }
      return 0;
    }
  protected:
    Skin::SkinClass* m_bodyClass;
    std::vector<ItemFull> m_items;
    int32 m_selItem;
    Scrollbar m_scrollbar;
 
  private:
  };


}

#endif
