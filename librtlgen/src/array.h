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
#include "signals.h"
#include "utils.h"
#include "params.h"
#include "modules.h"
#include <iostream>
#include <list>

namespace sysvc {

template<class T>
class ArrayObject : public GenObject {
 public:
    ArrayObject(GenObject *parent,
                const char *type,
                const char *name,
                const char *depth,
                const char *comment="")
    : GenObject(parent, type, ID_ARRAY_DEF, name, comment) {
        setStrDepth(depth);
    }

    virtual std::string getType() override {
        if (type_.size()) {
            return type_;
        }
        return getItem(0)->getType();
    }

    virtual std::string generate() override {
        std::string ret = "";
        if (SCV_is_sv()) {
            ret += "typedef " + getItem()->getType() + " " + getType();
            ret += "[0: " + getStrDepth() + "-1];\n";
        } else if (SCV_is_sysc()) {
            ret += "typedef sc_vector<sc_signal<" + getItem()->getType() + ">> ";
            ret += getType() + ";\n";
        }
        return ret;
    }

    // No need to redfine operator '->' because we use this object directly
    virtual GenObject *getItem() override { return arr_[0]; }
    virtual GenObject *getItem(int idx) override { return arr_[idx]; }
    virtual GenObject *getItem(const char *name) override { return getItem()->getItem(name); }
    T *operator->() const { return arr_[0]; }
    T *operator->() { return arr_[0]; }
    T *Item() { return arr_[0]; }
 protected:
    T **arr_;
};

// T = signal or logic
template<class T>
class WireArray : public ArrayObject<T> {
 public:
    WireArray(GenObject *parent, const char *name, const char *width,
        const char *depth, bool reg=false, const char *comment="")
        : ArrayObject<T>(parent, "", name, depth, comment) {
        char tstr[64];
        int d = GenObject::getDepth();
        GenObject::reg_ = reg;
        ArrayObject<T>::arr_ = new T *[d];
        for (int i = 0; i < d; i++) {
            RISCV_sprintf(tstr, sizeof(tstr), "%d", i);
            ArrayObject<T>::arr_[i] = new T(this, tstr, width, "0");
        }
    }
    virtual bool isSignal() override { return ArrayObject<T>::arr_[0]->isSignal(); }
};

template<class T>
class TStructArray : public ArrayObject<T> {
    public:
    TStructArray(GenObject *parent, const char *type, const char *name,
        const char *depth, const char *comment="")
        : ArrayObject<T>(parent, type, name, depth, comment) {
        int d = GenObject::getDepth();
        ArrayObject<T>::arr_ = new T *[d];
        char tstr[64];
        for (int i = 0; i < d; i++) {
            RISCV_sprintf(tstr, sizeof(tstr), "%d", i);
            ArrayObject<T>::arr_[i] = new T(this, tstr);
        }
    }
};

template<class T>
class ModuleArray : public ArrayObject<T> {
 public:
    ModuleArray(GenObject *parent, const char *name, const char *depth, const char *comment="")
        : ArrayObject<T>(parent, "", name, depth, comment) {
        int d = GenObject::getDepth();
        ArrayObject<T>::arr_ = new T *[d];
        char tstr[64];
        for (int i = 0; i < d; i++) {
            RISCV_sprintf(tstr, sizeof(tstr), "%s%d", name, i);
            ArrayObject<T>::arr_[i] = new T(this, tstr);
        }
    }
    virtual void changeTmplParameter(const char *name, const char *val) {
        ArrayObject<T>::getItem(name)->setStrValue(val);
    }
};

class StringArray : public ArrayObject<ParamString> {
 public:
    StringArray(GenObject *parent, const char *name, const char *depth, const char *comment="")
        : ArrayObject<ParamString>(parent, "", name, depth, comment) {
        id_ = ID_ARRAY_STRING;
        int d = GenObject::getDepth();
        ArrayObject<ParamString>::arr_ = new ParamString *[d];
        char tstr[64];
        for (int i = 0; i < d; i++) {
            RISCV_sprintf(tstr, sizeof(tstr), "%s%d", name, i);
            ArrayObject<ParamString>::arr_[i] = new ParamString(this, tstr, "");
        }
    }
};

}  // namespace sysvc
