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

#ifndef h__thurs_control_dropdown__
#define h__thurs_control_dropdown__

#include <string>

#include "../thurs.control.hpp"
#include "../thurs.color.hpp"

#include "thurs.control.listbox.hpp"

namespace thurs {

  //This widget uses containment 
  class DropDown : public Control, public sigslot::has_slots<> {
  public:
    //Constructor. Duh.
    DropDown(uint32 id, Surface *surface);
    //Update and draw
    void update();
    //Set the currently used skin class
    void setSkinClass(const std::string& name);

    ///////////////////////////////////////////////////////////////////////////
    
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

    ///////////////////////////////////////////////////////////////////////////

    sigslot::signal2<int, int> OnSelect;

    ///////////////////////////////////////////////////////////////////////////

    //Access item
    ListBox::Item* const operator[](uint32 index) {
      return (*m_listbox)[index];
    }

    void handleSelection(int sender, int item);

  protected:
    ListBox *m_listbox;
    bool m_expanded;
    std::string m_caption;
    Skin::SkinClass* m_icon;
  private:
  };

}

#endif
