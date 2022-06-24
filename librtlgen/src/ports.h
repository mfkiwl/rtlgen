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
#include "params.h"
#include "logic.h"
#include <iostream>

namespace sysvc {

/*
 * Input/Output ports of the modules
 */
class PortObject : public GenObject {
 public:
    PortObject(GenObject *parent,
             EIdType type,
             const char *name,
             Logic *wire,
             const char *comment);

    Logic *getWire() { return wire_; }
    int getWidth() { return static_cast<int>(wire_->getValue()); }
 protected:
    Logic *wire_;
};


class InPort : public PortObject {
 public:
    InPort(GenObject *parent, const char *name,
             Logic *wire, const char *comment="")
        : PortObject(parent, ID_INPUT, name, wire, comment) {}

    virtual std::string getType(EGenerateType v);
};

class OutPort : public PortObject {
 public:
    OutPort(GenObject *parent, const char *name,
             Logic *wire, const char *comment="")
        : PortObject(parent, ID_OUTPUT, name, wire, comment) {}

    virtual std::string getType(EGenerateType v);
};

}  // namespace sysvc
