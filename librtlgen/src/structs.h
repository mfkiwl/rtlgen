// 
//  Copyright 2022 Sergey Khabarov, sergeykhbr@gmail.com
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// 

#pragma once

#include "genobjects.h"
#include "values.h"
#include <iostream>
#include <list>

namespace sysvc {

class StructObject : public GenObject {
 public:
    // Declare structure type and create on instance
    StructObject(GenObject *parent,
                 const char *type,
                 const char *name,
                 int idx = -1,
                 const char *comment="");

    /** GenObject generic methods */
    virtual std::string getType() { return type_; }
    virtual std::string getName() override;
    virtual void setZeroValue(const char *v) { zeroval_ = std::string(v); }
    virtual std::string getStrValue() override;
    virtual std::string generate();

 protected:
    virtual void registerCfgType(const char *name);
    virtual std::string generate_interface();
    virtual std::string generate_const_none();

    std::string type_;
    std::list<std::string> instances_;   // instance list
    int idx_;                            // array item index (-1) if not in an array
    std::string zeroval_;
};

}  // namespace sysvc
