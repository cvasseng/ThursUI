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

#ifndef h__thurs__types__
#define h__thurs__types__

#include <string>
#include <functional>
#include <initializer_list>

//Basic type definitions

namespace thurs {

	typedef char            int8;
  typedef unsigned char   uint8;
  typedef short           int16;
  typedef unsigned short  uint16;
  typedef int             int32;
  typedef unsigned int    uint32;
  typedef long            int64;
  typedef unsigned long   uint64;

  typedef struct {
  	std::string filename;
  	uint16 size;
  	bool bold;
  	bool italic;
  } Font;

  enum UIAction {
    AC_CLICK,
    AC_CHANGE,
    AC_MOUSE_DOWN,
    AC_MOUSE_UP,
    AC_MOUSE_IN,
    AC_MOUSE_OUT,  
    AC_FOCUS,
    AC_BLUR, 
  };
  
  enum VerticalAlignment {
    VA_CUSTOM,
    VA_CLIENT,
    VA_BOTTOM,
    VA_TOP,
    VA_CENTER
  };

  enum HorizontalAlignment {
    HA_CUSTOM,
    HA_CLIENT,
    HA_LEFT,
    HA_RIGHT,
    HA_CENTER
  };
  
  enum WidgetType {
    WT_BUTTON,
    WT_CHECKBOX,
    WT_DROPDOWN,
    WT_EDITBOX,
    WT_IMAGEGRID,
    WT_LABEL,
    WT_LISTBOX,
    WT_MLTEXT,
    WT_PROGRESSBAR,
    WT_RECTANGLE,
    WT_SKILLBAR,
    WT_SLIDER,
    WT_UNKNOWN
  };
  
  class Control;

  typedef std::function<void(Control * const)> ThursCallback;

  template<typename T> class Vector2 {
  public:
  	T x;
  	T y;

  	Vector2() {
  		x = T(0);
  		y = T(0);
  	}

  	Vector2(T _x, T _y) {
  		x = _x;
  		y = _y;
  	}

    inline Vector2<T> operator-(Vector2<T> other) {
      Vector2<T> n;
      n.x = x - other.x;
      n.y = y - other.y;
      return n;
    }

    inline Vector2<T> operator+(Vector2<T> other) {
      Vector2<T> n;
      n.x = x + other.x;
      n.y = y + other.y;
      return n;
    }
        

	};

  template<typename T> class Rect {
  public:
    T x;
    T y;
    T w;
    T h;

    Rect() {
      x = T(0);
      y = T(0);
      w = T(0);
      h = T(0);
    }
  };


	typedef Vector2<int16> Vector2s;
	typedef Vector2<int32> Vector2i;
	typedef Vector2<float> Vector2f;

}

#endif
