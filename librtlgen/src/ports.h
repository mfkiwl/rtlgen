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
             GenValue *width,
             const char *comment);

    int getWidth() { return static_cast<int>(width_->getValue()); }

    virtual std::string generate(EGenerateType v) = 0;
 protected:
    GenValue *width_;
};

class IoPortsStart : public GenObject {
 public:
    IoPortsStart(GenObject *parent)
        : GenObject(parent, ID_IO_START, "") {}
};

class IoPortsEnd : public GenObject {
 public:
    IoPortsEnd(GenObject *parent)
        : GenObject(parent, ID_IO_END, "") {}
};


class InPort : public PortObject {
 public:
    InPort(GenObject *parent, const char *name,
             GenValue *width, const char *comment)
        : PortObject(parent, ID_INPUT, name, width, comment) {}

    virtual std::string generate(EGenerateType v) override;
};

class OutPort : public PortObject {
 public:
    OutPort(GenObject *parent, const char *name,
             GenValue *width, const char *comment)
        : PortObject(parent, ID_OUTPUT, name, width, comment) {}

    virtual std::string generate(EGenerateType v) override;
};

}  // namespace sysvc
