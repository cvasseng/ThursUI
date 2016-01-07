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

namespace thurs {
  
  Property::Property(const std::string& name) {
    m_name = name;
    
  }
  
  Property::~Property() {
    
  }
  
  const std::string& Property::name() {
    return m_name;
  }

  /////////////////////////////////////////////////////////////////////////////
  
  PropertyString::PropertyString(const std::string& name) : Property(name) {

  }
  
  PropertyString::~PropertyString() {
    
  }
  
  void PropertyString::set(const std::string& value) {
    m_value = value;
  }
  
  const std::string& PropertyString::get() {
    return m_value;
  }
  
  std::string PropertyString::toString() {
    return m_value;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  PropertyCollection::PropertyCollection() {
    
  }
  
  PropertyCollection::~PropertyCollection() {
    
  }

  //Register a property with the collection 
  bool PropertyCollection::add(Property& pro) {
    //printf("inserting prop %s\n", prop.name().c_str());
    auto it = m_properties.find(pro.name());
    if (it == m_properties.end()) {
      m_properties.insert(std::make_pair(pro.name(), &pro));
      return true;      
    }
  
    return false;
  }
  
  bool PropertyCollection::add(const std::string& name, Property& pro) {
    //printf("inserting prop %s\n", prop.name().c_str());
    auto it = m_properties.find(name);
    if (it == m_properties.end()) {
      m_properties.insert(std::make_pair(name, &pro));
      return true;      
    }
  
    return false;
  }
  
  //Set a property 
  bool PropertyCollection::set(const std::string& name, const std::string& value) {
    auto it = m_properties.find(name);
    if (it != m_properties.end()) {
      it->second->set(value);
    }
    
    return false;
  }
  
  //Read a value 
  template<typename T> T PropertyCollection::getAs(const std::string& name, const T& def) {
    return def;
  }
  
  //Read a value 
  std::string PropertyCollection::get(const std::string& name, const std::string& def) {
    return def;
  }
  
  //For each property 
  void PropertyCollection::forEach(std::function<void(const std::string, const std::string&)> &fn) {
    
  }
  
  //Set properties from JSON
  bool PropertyCollection::fromJSON(const Json::Value &v) {
    Json::Value::Members m = v.getMemberNames();
    for (uint32 i = 0; i < m.size(); i++) {
      set(m[i], v[m[i]].asString());
    }
    
    return true;
  }   
  
  bool PropertyCollection::toJSON(Json::Value &v) {
    for (auto it : m_properties) {
      v[it.first] = it.second->toString();
    }    
    return true;
  }
  
  /////////////////////////////////////////////////////////////////////////////

}
