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

#ifndef h__thurs_control_imagegrid__
#define h__thurs_control_imagegrid__

#include <string>
#include <vector>

#include "../thurs.control.hpp"
#include "../thurs.scrollbar.hpp"
#include "../thurs.skin.hpp"

namespace thurs {

  class ImageGrid : public Control {
  public:
    struct Entry {
      uint32 id;
      std::string image;
      uint32 imageHandle;
      Skin::SkinClass skinClass;
    };

    //Constructor. Duh.
    ImageGrid(uint32 id, Surface *surface);
    //Update and draw
    void update();

    //Add an image
    void addImage(uint32 id, const std::string& image);

  protected:
    //Number of images on the x axis
    uint16 m_imagesX;
    //Scrollbar
    Scrollbar m_scrollbar;
    //Ref class

    //Entries
    std::vector<Entry> m_entries;
  private:
  };

}

#endif
