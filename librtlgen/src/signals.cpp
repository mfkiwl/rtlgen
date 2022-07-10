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

#include "signals.h"
#include "utils.h"

namespace sysvc {

Signal::Signal(GenObject *parent, const char *name, const char *width,
    const char *comment) : Logic(width, name, "0", parent, comment) {
    id_ = ID_SIGNAL;
}

Signal::Signal(GenObject *parent, const char *name, GenValue *width, 
    const char *comment) : Logic(width, name, "0", parent, comment) {
    id_ = ID_SIGNAL;
}

Signal::Signal(GenObject *parent, const char *name, const char *width,
    const char *val, bool usereg, const char *comment)
    : Logic(width, name, val, parent, comment) {
    id_ = ID_SIGNAL;
    reg_ = usereg;
}


std::string Signal::getType() {
    std::string out = "";
    if (SCV_is_sysc()) {
        out += "sc_signal<" + Logic::getType() + ">";
    } else if (SCV_is_sv()) {
        out += Logic::getType();
    } else {
    }
    return out;
}

}
