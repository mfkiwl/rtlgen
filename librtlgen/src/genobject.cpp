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

#include "genobjects.h"

namespace sysvc {

GenObject::GenObject(GenObject *parent, EIdType id,
                     const char *name, const char *comment) {
    id_ = id;
    parent_ = parent;
    reg_ = false;
    name_ = std::string(name);
    comment_ = std::string(comment);
    if (parent_) {
        parent_->add_entry(this);
    }
}

std::string GenObject::getFullPath() {
    std::string ret = "";
    if (parent_) {
        ret = parent_->getFullPath();
    }
    return ret;
}

std::string GenObject::getFile() {
    std::string ret = "";
    if (getId() == ID_FILE) {
        return getName();
    } else if (parent_) {
        return parent_->getFile();
    }
    return ret;
}

void GenObject::add_entry(GenObject *p) {
    entries_.push_back(p);
}

void GenObject::getSignals(std::list<GenObject *> &objlist) {
    if (getId() == ID_STRUCT_DEF) {
        // Only signal instance
        return;
    }
    if (getId() == ID_SIGNAL) {
        objlist.push_back(this);
    }
    for (auto &e: entries_) {
        if (e->getId() == ID_STRUCT_DEF) {
            continue;
        }
        e->getSignals(objlist);
    }
}

std::string GenObject::getFullName(EGenerateType v, GenObject *top) {
    std::string ret = getName();
    GenObject *p = this;
    if (top != p) {
        do {
            p = p->getParent();
            ret = p->getName() + p->getNameSpliter(v) + ret;
        } while (p != top);
    }
    return ret;
}

std::string GenObject::getNameSpliter(EGenerateType v) {
    std::string ret = std::string(".");
    if (getId() == ID_ARRAY_ITEM) {
        ret = std::string("].");
    } else if (getId() == ID_ARRAY_DEF) {
        ret = std::string("[");
    }
    return ret;
}

}
