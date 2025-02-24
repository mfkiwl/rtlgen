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

#include "params.h"
#include "utils.h"

namespace sysvc {

ParamGeneric::ParamGeneric(GenValue *parent) {
    SCV_set_cfg_parameter(parent,
                          parent,
                          parent->getValue());
}

std::string ParamGeneric::genparam(GenValue *p) {
    std::string ret = "";

    if (SCV_is_sysc()) {
        ret += "static const ";
    } else if (SCV_is_sv()) {
        ret += "localparam ";
    } else {
        ret += "const ";
    }
    ret += p->getType() + " ";
    ret += p->getName() + " = ";
    ret += p->getStrValue() + ";";

    // One line comment
    if (p->getComment().size()) {
        while (ret.size() < 60) {
            ret += " ";
        }
        ret += "// " + p->getComment();
    }
    ret += "\n";
    return ret;
}

}
