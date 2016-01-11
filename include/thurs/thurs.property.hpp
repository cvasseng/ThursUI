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

#include "thurs.types.hpp"
#include "thurs.color.hpp"

#include <string>
#include <sstream>
#include <functional>
#include <json/json.h>
#include <map>

namespace thurs {

  class PropertyCollection;

  class Property {
  public:
    Property() { m_name = "";}
    Property(const std::string& name);
    virtual ~Property();
    virtual void set(const std::string& value) = 0;
    virtual std::string toString() = 0;
    const std::string& name();
  protected:
    std::string m_name;
  private:
  };
  
  class PropertyCollection {
  public:
    PropertyCollection();
    virtual ~PropertyCollection();
    
    //Register a property with the collection 
    bool add(Property& pro);
    bool add(const std::string& name, Property& pro);
    //Set a property 
    bool set(const std::string& name, const std::string& value);
    //Read a value 
    template<typename T> T getAs(const std::string& name, const T& def = "");
    //Read a value 
    std::string get(const std::string& name, const std::string& def = "");
    //For each property 
    void forEach(std::function<void(const std::string, const std::string&)> &fn);
    //Set properties from JSON
    bool fromJSON(const Json::Value &v);   
    //Build JSON with the properties 
    bool toJSON(Json::Value &v);
  protected:
    //Properties 
    std::map<std::string, Property*> m_properties;
  private:  
  };
  
  template<typename T> class Property_t : public Property {
  public:
    Property_t(){}
    Property_t(const std::string& name) : Property(name) {}
    virtual ~Property_t() {}
    
    virtual void set(const std::string& value) {
      std::stringstream ss(value);
      ss >> m_value;
    }
    
    virtual void set(const T& val) {
      m_value = val;
    }
    
    virtual const T& get() {
      return m_value;
    }
    
    virtual std::string toString() {
      std::stringstream ss;
      ss << m_value;
      return ss.str();  
    }
    
    operator T () {
      return m_value;
    }
    
    void operator =(T v) {
      m_value = v;
    }
    
  protected:
    //The value 
    T m_value;
  private:
  };
  
  template<typename T> class PropertyEnum : public Property {
  public:
    PropertyEnum(){}
    PropertyEnum(const std::string& name) : Property(name) {}
    virtual ~PropertyEnum() {}
    
    virtual void set(const std::string& value) {
      std::stringstream ss(value);
      uint32 v;
      ss >> v;
      m_value = T(v);
    }
    
    virtual void set(const T& val) {
      m_value = val;
    }
    
    virtual const T& get() {
      return m_value;
    }
    
    virtual std::string toString() {
      std::stringstream ss;
      ss << m_value;
      return ss.str();  
    }
    
    operator T () {
      return m_value;
    }
    
    void operator =(T v) {
      m_value = v;
    }
    
  protected:
    //The value 
    T m_value;
  private:
  };
  
  class PropertyString : public Property {
  public:
    PropertyString(){}
    PropertyString(const std::string& name);
    virtual ~PropertyString();
    void set(const std::string& value);
    const std::string& get();
    std::string toString();
    
    operator std::string() { return m_value; }
    
    void operator =(std::string v) {
      m_value = v;
    }
  protected:
    std::string m_value;
  };
  
  class PropertyColor : public Property {
  public:
  protected:
    Color m_value;
  private:  
  };
  
  template <typename T> class PropertyVec2 : public Property, public Vector2<T> {
  public:
    // T x;
  	// T y;
    
    // PropertyVec2() {
    //   x = 0;
    //   y = 0;
    // }
    
    // PropertyVec2(T _x, T _y) {
    //   x = _x;
    //   y = _y;
    // }
    
    PropertyVec2(const std::string& name) : Property(name) {}
    virtual ~PropertyVec2() {}
    
    void set(const std::string& value) {
      std::stringstream ss(value);
      ss >> this->x >> this->y;
    }
    
    std::string toString() {
      std::stringstream ss;
      ss << this->x << " " << this->y;
      return ss.str();
    }
    
    // inline Vector2<T> operator-(Vector2<T> other) {
    //   Vector2<T> n;
    //   n.x = this->x - other.x;
    //   n.y = this->y - other.y;
    //   return n;
    // }

    // inline Vector2<T> operator+(Vector2<T> other) {
    //   Vector2<T> n;
    //   n.x = this->x + other.x;
    //   n.y = this->y + other.y;
    //   return n;
    // }

    void operator = (T v) {
      this->x = v.x;
      this->y = v.y;
    }
    
    void operator = (Vector2<T> v) {
      this->x = v.x;
      this->y = v.y;
    }
    
    void operator = (PropertyVec2<T> v) {
      this->x = v.x;
      this->y = v.y;
    }
    
  protected:
  };

  //Basic properties 
  typedef Property_t<bool>                  PropertyBool;
  typedef Property_t<int8>                  PropertyInt8;
  typedef Property_t<uint8>                 PropertyUInt8;
  typedef Property_t<int16>                 PropertyInt16;
  typedef Property_t<uint16>                PropertyUInt16;
  typedef Property_t<int32>                 PropertyInt32;
  typedef Property_t<uint32>                PropertyUInt32;
  typedef Property_t<int64>                 PropertyInt64;
  typedef Property_t<uint64>                PropertyUInt64;
  typedef Property_t<float>                 PropertyFloat;
  typedef Property_t<double>                PropertyDouble;
  typedef PropertyEnum<UIAction>            PropertyUIAction;
  typedef PropertyEnum<VerticalAlignment>   PropertyVAlignment;
  typedef PropertyEnum<HorizontalAlignment> PropertyHAlignment;
  typedef PropertyVec2<int16>               PropertyVec2s;
  typedef PropertyVec2<int32>               PropertyVec2i;
  typedef PropertyVec2<float>               PropertyVec2f;
  
  

}
