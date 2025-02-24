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

#include "modules.h"
#include "defparams.h"
#include "ports.h"
#include "signals.h"
#include "utils.h"
#include "files.h"
#include "structs.h"
#include "regs.h"
#include "operations.h"
#include "array.h"
#include "funcs.h"

namespace sysvc {

std::string ModuleObject::generate_sysc_h_reg_struct(bool negedge) {
    std::string out = "";
    std::string ln = "";
    bool twodim = false;        // if 2-dimensional register array, then do not use reset function
    out += "    struct " + getType();
    if (!negedge) {
        out += "_registers";
    } else {
        out += "_nregisters";
    }
    out += " {\n";
    for (auto &p: getEntries()) {
        if (!negedge && !p->isReg()) {
            continue;
        }
        if (negedge && !p->isNReg()) {
            continue;
        }
        ln = "        ";
        if (p->isSignal()) {
            // some structure are not defined as a signal but probably should be
            ln += "sc_signal<";
        }
        ln += p->getType();
        if (p->isSignal()) {
            ln += ">";
        }
        ln += " " + p->getName();
        if (p->getDepth()) {
            twodim = true;
            ln += "[" + p->getStrDepth() + "]";
        }
        ln += ";";
        if (p->getComment().size()) {
            while (ln.size() < 60) {
                ln += " ";
            }
            ln += "// " + p->getComment();
        }
        out += ln + "\n";
    }
    if (!negedge) {
        out += "    } v, r;\n";
    } else {
        out += "    } nv, nr;\n";
    }
    out += "\n";
    // Reset function only if no two-dimensial signals
    if (!twodim) {
        out += "    void " + getType();
        if (!negedge) {
            out += "_r_reset(" + getType() + "_registers &iv) {\n";
        } else {
            out += "_nr_reset(" + getType() + "_nregisters &iv) {\n";
        }
        for (auto &p: entries_) {
            if (!negedge && !p->isReg()) {
                continue;
            }
            if (negedge && !p->isNReg()) {
                continue;
            }
            out += "        iv." + p->getName() + " = ";
            out += p->getStrValue();
            out += ";\n";
        }
        out += "    }\n";
        out += "\n";
    }
    return out;
}

std::string ModuleObject::generate_sysc_h_struct() {
    std::string out = "";
    int tcnt = 0;
    for (auto &p: entries_) {
        if (p->getId() != ID_STRUCT_DEF) {
            continue;
        }
        if (p->isVector()) {
            out += Operation::addspaces();
            out += "typedef ";
            if (p->isVector()) {
                out += "sc_vector<";
            }
            if (p->isSignal()) {
                out += "sc_signal<";
            }
            out += p->generate();
            if (p->isSignal()) {
                out += ">";
            }
            if (p->isVector()) {
                out += ">";
            }
            out += " " + p->getType() + ";\n";
        } else {
            out += p->generate();
        }
        tcnt++;
    }
    if (tcnt) {
        out += "\n";
        tcnt = 0;
    }
    // Register structure definition
    if (isRegProcess() && isCombProcess()) {
        out += generate_sysc_h_reg_struct(false);
    }
    if (isNRegProcess() && isCombProcess()) {
        out += generate_sysc_h_reg_struct(true);
    }
    return out;
}

std::string ModuleObject::generate_sysc_h() {
    std::string out = "";
    std::string ln;
    std::string text = "";
    std::list<GenObject *> tmpllist;
    std::list<GenObject *> argslist;
    int tcnt = 0;

    getTmplParamList(tmpllist);
    if (tmpllist.size()) {
        ln = "template<";
        for (auto &e: tmpllist) {
            if (e != tmpllist.front()) {
                ln += "         ";
            }
            ln += e->getType() + " " + e->getName() + " = " + e->getStrValue();
            if (e != tmpllist.back()) {
                ln += ",";
            } else {
                ln += ">";
            }
            if (e->getComment().size()) {
                while (ln.size() < 60) {
                    ln += " ";
                }
                ln += "// " + e->getComment();
            }
            out += ln + "\n";
            ln = "";
        }
    }

    tcnt = 0;
    out += "SC_MODULE(" + getType() + ") {\n";
    out += " public:\n";

    // Input/Output signal declaration
    for (auto &p: entries_) {
        if (!p->isInput() && !p->isOutput()) {
            if (p->getId() == ID_COMMENT) {
                text = "    " + p->generate();
            } else {
                text = "";
            }
            continue;
        }
        ln = "";
        ln += "    ";
        if (p->isVector()) {
            ln += "sc_vector<";
        }
        if (p->isInput()) {
            ln += "sc_in<";
        } else {
            ln += "sc_out<";
        }
        if (p->isVector()) {
            // element of vector to form sc_vector<sc_in<type>> and
            // do not use vector type name
            ln += p->generate();
        } else {
            ln += p->getType();
        }
        ln += ">";
        if (p->isVector()) {
            ln += ">";
        }
        ln += " " + p->getName() + ";";
        if (p->getComment().size()) {
            while (ln.size() < 60) {
                ln += " ";
            }
            ln += "// " + p->getComment();
        }
        if (text.size()) {
            out += text;
        }
        out += ln + "\n";
        text = "";
    }

    // Process declaration:
    out += "\n";
    bool hasProcess = false;
    for (auto &p: entries_) {
        if (p->getId() != ID_PROCESS) {
            continue;
        }
        if (p->getName() == "registers") {
            continue;
        }
        out += "    void " + p->getName() + "();\n";
        hasProcess = true;
    }
    if (isRegProcess()) {
        out += "    void registers();\n";
        hasProcess = true;
    }
    if (isNRegProcess()) {
        out += "    void nregisters();\n";
        hasProcess = true;
    }
    if (hasProcess) {
        out += "\n";
        out += "    SC_HAS_PROCESS(" + getType() + ");\n";
    }


    out += "\n";
    // Constructor declaration:
    std::string space1 = "    " + getType() + "(";
    out += space1 + "sc_module_name name";
    if (getAsyncReset() && getEntryByName("async_reset") == 0) {
        ln = "";
        while (ln.size() < space1.size()) {
            ln += " ";
        }
        ln += "bool async_reset";           // Mandatory generic parameter
        out += ",\n" + ln;
    }
    for (auto &p: entries_) {
        if (p->getId() != ID_DEF_PARAM) {
            continue;
        }
        out += ",\n";
        ln = "";
        while (ln.size() < space1.size()) {
            ln += " ";
        }
        ln += p->getType();
        ln += " " + p->getName();
        out += ln;
    }
    out += ");\n";
    // Destructor declaration
    if (isSubModules()) {
        out += "    virtual ~" + getType() + "();\n";
    }
    out += "\n";

    // VCD generator
    if (isVcd()) {
        out += "    void generateVCD(sc_trace_file *i_vcd, sc_trace_file *o_vcd);\n";
    }

    out += "\n";
    out += " private:\n";

    // Generic parameter local storage:
    tcnt = 0;
    if (getAsyncReset() && getEntryByName("async_reset") == 0) {
        out += "    " + (new Logic())->getType() + " async_reset_;\n";
        tcnt++;
    }
    for (auto &p: entries_) {
        if (p->getId() == ID_DEF_PARAM){
            out += "    " + p->getType() + " " + p->getName() + "_;\n";
            tcnt++;
        } else if (p->getId() == ID_PARAM && p->isGenericDep() && tmpllist.size() == 0) {
            // No underscore symbol
            out += "    " + p->getType() + " " + p->getName() + ";\n";
            tcnt++;
        }
    }
    if (tcnt) {
        out += "\n";
        tcnt = 0;
    }

    // Local paramaters visible inside of module
    GenObject *prev = 0;
    Operation::set_space(1);
    for (auto &p: entries_) {
        if (p->getId() != ID_PARAM) {
            prev = 0;
            if (p->getId() == ID_COMMENT) {
                prev = p;
            }
            continue;
        }
        if (!p->isLocal()) {
            prev = 0;
            continue;
        }
        if (p->isGenericDep() && tmpllist.size() == 0) {
            prev = 0;
            continue;
        }
        if (p->isString()) {
            prev = 0;
            continue;
        } else {
            if (prev) {
                out += Operation::addspaces() + prev->generate();
            }
            out += Operation::addspaces() + "static const " + p->getType() + " " + p->getName();
            out += " = " + p->getStrValue() + ";\n";
        }
        tcnt++;
        prev = 0;
    }
    if (tcnt) {
        out += "\n";
        tcnt = 0;
    }

    // Functions declaration:
    tcnt = 0;
    for (auto &p: entries_) {
        if (p->getId() != ID_FUNCTION) {
            continue;
        }
        tcnt++;
        ln = "    " + p->getType();
        ln += " " + p->getName();
        out += ln + "(";
        tcnt = 0;
        argslist.clear();
        static_cast<FunctionObject *>(p)->getArgsList(argslist);
        for (auto &io: argslist) {
            if (tcnt++) {
                out += ", ";
            }
            out += io->getType() + " " + io->getName();
        }
        out += ");\n";
    }
    if (tcnt) {
        out += "\n";
        tcnt = 0;
    }

    // struct definitions
    Operation::set_space(1);
    out += generate_sysc_h_struct();

    // Signals list
    text = "";
    for (auto &p: getEntries()) {
        if (p->isInput() || p->isOutput()) {
            text = "";
            continue;
        }
        if (p->getName() == "") {
            // ignore typedef
            continue;
        }
        if (p->isReg() || p->isNReg()
            || (!p->isSignal()
                && p->getId() != ID_VALUE
                && p->getId() != ID_STRUCT_INST
                && p->getId() != ID_ARRAY_DEF
                && p->getId() != ID_VECTOR)) {
            if (p->getId() == ID_COMMENT) {
                text += "    " + p->generate();
            } else {
                text = "";
            }
            continue;
        }
        if (p->getId() == ID_ARRAY_DEF) {
            if (p->getItem()->getId() == ID_MODULE_INST) {
                text = "";
                continue;
            }
        }
        if (text.size()) {
            out += text;
            text = "";
        }
        ln = "    ";
        if (!p->isTypedef()) {
            if (p->isVector()) {
                ln += "sc_vector<";
            }
            if (p->isSignal()) {
                ln += "sc_signal<";
            }
        }
        ln += p->getType();
        if (!p->isTypedef()) {
            if (p->isSignal()) {
                ln += ">";
            }
            if (p->isVector()) {
                ln += ">";
            }
        }
        ln += " " + p->getName();
        if (p->getDepth() && !p->isVector()) {
            ln += "[" + p->getStrDepth() + "]";
        }
        ln += ";";
        if (p->getComment().size()) {
            while (ln.size() < 60) {
                ln += " ";
            }
            ln += "// " + p->getComment();
        }
        out += ln + "\n";
        tcnt++;
    }
    for (auto &p: getEntries()) {
        if (p->getId() != ID_FILEVALUE) {
            continue;
        }
        out += "    FILE *" + p->getName() + ";\n";
        tcnt++;
    }
    if (tcnt) {
        out += "\n";
        tcnt = 0;
    }

    // Sub-module list
    for (auto &p: entries_) {
        if (p->getId() == ID_MODULE_INST) {
            out += "    " + p->getType();
            out += generate_sysc_template_param(p);
            out += " *" + p->getName() + ";\n";
            tcnt ++;
        } else if (p->getId() == ID_ARRAY_DEF) {
            if (p->getItem()->getId() == ID_MODULE_INST) {
                out += "    " + p->getType();
                out += generate_sysc_template_param(p->getItem());
                out += " *" + p->getName();
                out += "[" + p->getStrDepth() + "];\n";
            }
            tcnt ++;
        }
    }
    if (tcnt) {
        out += "\n";
        tcnt = 0;
    }


    out += 
        "};\n"
        "\n";

    // Templates only. Generated in h-file
    if (tmpllist.size()) {
        out += generate_sysc_cpp();
    }
    return out;
}

std::string ModuleObject::generate_sysc_proc_registers(bool clkpos) {
    std::string out = "";
    std::string xrst = "";
    std::string src = "v";
    std::string dst = "r";

    out += generate_sysc_template_f_name();
    if (clkpos) {
        out += "::registers() {\n";
    } else {
        out += "::nregisters() {\n";
        src = "nv";
        dst = "nr";
    }
    Operation::set_space(Operation::get_space() + 1);
    if (getResetPort()) {
        if (isCombProcess()) {
            out += Operation::reset(dst.c_str(), 0, this, xrst);
        }
        out += " else {\n";
        Operation::set_space(Operation::get_space() + 1);
    }
    if (isCombProcess()) {
        out += Operation::copyreg(dst.c_str(), src.c_str(), this);
    }
    if (getResetPort()) {
        Operation::set_space(Operation::get_space() - 1);
        out += Operation::addspaces();
        out += "}\n";
    }
    for (auto &e: getEntries()) {
        if (e->getId() != ID_PROCESS
            || e->getName() != "registers") {
            continue;
        }
        out += "\n";
        for (auto &r: e->getEntries()) {
            out += r->generate();
        }
    }
    Operation::set_space(Operation::get_space() - 1);
    out += Operation::addspaces();
    out += "}\n";
    out += "\n";
    return out;
}

std::string ModuleObject::generate_sysc_sensitivity(std::string prefix,
                                                    std::string name, 
                                                    GenObject *obj) {
    std::string ret = "";
    bool prefix_applied = true;
    if (obj->getId() == ID_STRUCT_DEF
        || obj->getId() == ID_MODULE_INST
        || obj->getId() == ID_PROCESS
        || obj->isOutput()) {
        return ret;
    }
    if (obj->getId() == ID_ARRAY_DEF) {
        // Check when array stores module instantiation:
        if (obj->getItem()->getId() == ID_MODULE_INST) {
            return ret;
        }
    }

    for (int i = 0; i < prefix.size(); i++) {
        if (i >= name.size()
            || prefix.c_str()[i] != name.c_str()[i]) {
            prefix_applied = false;
            break;
        }
    }

    if ((obj->isSignal() && obj->getParent()->getId() != ID_ARRAY_DEF)    // signal is a part of array not a child structure
        || obj->getId() == ID_ARRAY_DEF
        || obj->getId() == ID_STRUCT_INST) {
        if (name.size()) {
            name += ".";
        }
        name += obj->getName();
        if (obj->isReg() && !prefix_applied) {
            name = prefix + name;
        }
        if (obj->isNReg() && !prefix_applied) {
            // @warning: Structure is not supported
            name = "n" + prefix + name;
        }
    }

    if (obj->isInput() && obj->getName() != "i_clk") {
        ret += Operation::addspaces();
        if (obj->isVector() || obj->getItem()->getId() == ID_VECTOR) {
            ret += "for (int i = 0; i < " + obj->getItem()->getStrDepth() + "; i++) {\n";
            Operation::set_space(Operation::get_space() + 1);
            ret += Operation::addspaces();
            ret += "sensitive << " + obj->getName() + "[i];\n";
            Operation::set_space(Operation::get_space() - 1);
            ret += Operation::addspaces();
            ret += "}\n";
        } else {
            ret += "sensitive << " + obj->getName() + ";\n";
        }
    } else if  (obj->isSignal()) {
        ret += Operation::addspaces();
        if (obj->isVector() || obj->getId() == ID_ARRAY_DEF) {
            ret += "for (int i = 0; i < " + obj->getStrDepth() + "; i++) {\n";
            Operation::set_space(Operation::get_space() + 1);
            ret += Operation::addspaces();
            ret += "sensitive << " + name + "[i];\n";
            Operation::set_space(Operation::get_space() - 1);
            ret += Operation::addspaces();
            ret += "}\n";
        } else {
            ret += "sensitive << " + name + ";\n";
        }
    } else if (obj->getId() == ID_ARRAY_DEF
        && (obj->getItem()->getId() != ID_VALUE || obj->getItem()->isSignal())) {
        // ignore value (not signals) declared in module scope
        name += "[i]";
        ret += Operation::addspaces();
        ret += "for (int i = 0; i < " + obj->getStrDepth() + "; i++) {\n";
        GenObject *item = obj->getItem();
        Operation::set_space(Operation::get_space() + 1);
        if (item->getEntries().size() == 0) {
            ret += generate_sysc_sensitivity(prefix, name, item);
        } else {
            for (auto &s: item->getEntries()) {
                ret += generate_sysc_sensitivity(prefix, name, s);
            }
        }
        Operation::set_space(Operation::get_space() - 1);
        ret += Operation::addspaces();
        ret += "}\n";
    } else if (obj->getId() == ID_STRUCT_INST) {
        for (auto &s: obj->getEntries()) {
            ret += generate_sysc_sensitivity(prefix, name, s);
        }
    } else {
        for (auto &s: obj->getEntries()) {
            ret += generate_sysc_sensitivity(prefix, name, s);
        }
    }
    return ret;
}

std::string ModuleObject::generate_sysc_vcd_entries(std::string name1, std::string name2, GenObject *obj) {
    std::string ret = "";
    bool prefix_applied = true;
    if (obj->getId() == ID_STRUCT_DEF
        || obj->getId() == ID_MODULE_INST
        || obj->getId() == ID_PROCESS
        || obj->isVector()
        || obj->getItem()->getId() == ID_VECTOR) {
        return ret;
    }

    if (!isTop()
        && (obj->getName() == "i_clk" || obj->getName() == "i_nrst")) {
        return ret;
    }


    if (name1.size() < 2
        || name1.c_str()[0] != 'r' || name1.c_str()[1] != '.') {
        prefix_applied = false;
    }

    if (obj->getParent()->getId() == ID_ARRAY_DEF) {
        name1 += "[";
        if (obj->getSelector()) {
            name1 += obj->getSelector()->getName();
        } else {
            name1 += obj->getName();
        }
        name1 += "]";
    } else if ((obj->isSignal() && obj->getParent()->getId() != ID_ARRAY_DEF)    // signal is a part of array not a child structure
        || obj->getId() == ID_ARRAY_DEF
        || obj->getId() == ID_STRUCT_INST) {
        if (name1.size()) {
            name1 += ".";
            name2 += "_";
        }
        name1 += obj->getName();
        name2 += obj->getName();
        if (obj->isReg() && !prefix_applied) {
            name1 = "r." + name1;
            name2 = ".r_" + name2;
        }
        if (obj->isNReg() && !prefix_applied) {
            name1 = "nr." + name1;
            name2 = ".nr_" + name2;
        }
    }

    if (!obj->isVcd()) {
        // skip it
    } else if (obj->isInput() || obj->isOutput()) {
        ret += Operation::addspaces();
        ret += "sc_trace(o_vcd, " + obj->getName() + ", " + obj->getName() + ".name());\n";
    } else if (obj->getId() == ID_ARRAY_DEF && (obj->isReg() || obj->isNReg())) {
        (*obj->getEntries().begin())->setSelector(new ParamI32D(0, "i", "0"));
        name2 += "%d";
        ret += Operation::addspaces();
        ret += "for (int i = 0; i < " + obj->getStrDepth() + "; i++) {\n";
        std::list<GenObject *>::iterator it = obj->getEntries().begin();
        Operation::set_space(Operation::get_space() + 1);

        ret += Operation::addspaces();
        ret += "char tstr[1024];\n";
        ret += generate_sysc_vcd_entries(name1, name2, *it);

        Operation::set_space(Operation::get_space() - 1);
        ret += Operation::addspaces();
        ret += "}\n";
    } else if (obj->isSignal() && (obj->isReg() || obj->isNReg())) {
        ret += Operation::addspaces();
        if (obj->getParent()->getId() == ID_ARRAY_DEF
            || obj->getParent()->getParent()->getId() == ID_ARRAY_DEF) {
            ret += "RISCV_sprintf(tstr, sizeof(tstr), \"%s" + name2 + "\", pn.c_str(), i);\n";
            ret += Operation::addspaces() + "sc_trace(o_vcd, " + name1 + ", tstr);\n";
        } else {
            ret += "sc_trace(o_vcd, " + name1 + ", pn + \"" + name2 + "\");\n";
        }
    } else {
        for (auto &s: obj->getEntries()) {
            ret += generate_sysc_vcd_entries(name1, name2, s);
        }
    }

    return ret;
}

std::string ModuleObject::generate_sysc_template_param(GenObject *p) {
    std::string ret = "";
    int tcnt = 0;
    std::list<GenObject *> tmpllist;

    static_cast<ModuleObject *>(p)->getTmplParamList(tmpllist);
    if (tmpllist.size()) {
        ret += "<";
        for (auto &e: tmpllist) {
            if (tcnt++) {
                ret += ", ";
            }
            ret += e->getStrValue();
        }
        ret += ">";
    }
    return ret;
}

std::string ModuleObject::generate_sysc_template_f_name(const char *rettype) {
    std::string ret = "";
    int tcnt = 0;
    std::list<GenObject *> tmpllist;

    getTmplParamList(tmpllist);
    if (tmpllist.size()) {
        ret += Operation::addspaces();
        ret += "template<";
        for (auto &e: tmpllist) {
            if (tcnt++) {
                ret += ", ";
            }
            ret += e->getType() + " " + e->getName();
        }
        ret += ">\n";
    }
    tcnt = 0;

    ret += Operation::addspaces();
    ret += std::string(rettype);
    if (rettype[0]) {
        ret += " ";
    }
    ret += getType();
    if (tmpllist.size()) {
        ret += "<";
        for (auto &e: tmpllist) {
            if (tcnt++) {
                ret += ", ";
            }
            ret += e->getName();
        }
        ret += ">";
    }
    return ret;
}

std::string ModuleObject::generate_sysc_param_strings() {
    std::string ret = "";
    int tcnt = 0;
    for (auto &p: getEntries()) {
        if (p->getId() != ID_PARAM) {
            continue;
        }
        if (!static_cast<GenValue *>(p)->isLocal()) {
            continue;
        }
        if (p->getType() != "std::string") {
            continue;
        }
        ret += "static " + p->getType() + " " + p->getName();
        ret += " = " + p->getStrValue() + ";\n";

        tcnt++;
    }
    for (auto &p: getEntries()) {
        if (p->getId() != ID_ARRAY_STRING) {
            continue;
        }
        ret += "static " + p->getType() + " " + p->getName() + "[" + p->getStrDepth() +"]";
        ret += " = {\n";
        for (auto &e: p->getEntries()) {
            ret += "    \"" + e->getName() + "\"";
            if (e != p->getEntries().back()) {
                ret += ",";
            }
            ret += "\n";
        }
        ret += "};\n";
        tcnt++;
    }
    if (tcnt) {
        ret += "\n";
    }
    return ret;
}

std::string ModuleObject::generate_sysc_constructor() {
    std::string ret = "";
    std::string ln = "";
    std::list<GenObject *> tmpllist;
    int tcnt = 0;

    getTmplParamList(tmpllist);
    if (tmpllist.size()) {
        ret += Operation::addspaces();
        ret += "template<";
        for (auto &e: tmpllist) {
            if (tcnt++) {
                ret += ", ";
            }
            ret += e->getType() + " " + e->getName();
        }
        ret += ">\n";
    }
    tcnt = 0;

    ret += Operation::addspaces();
    std::string space1 = getType();
    if (tmpllist.size()) {
        space1 += "<";
        for (auto &e: tmpllist) {
            if (tcnt++) {
                space1 += ", ";
            }
            space1 += e->getName();
        }
        space1 += ">";
    }
    space1 += "::" + getType() + "(";
    ret += space1 + "sc_module_name name";
    if (getAsyncReset() && getEntryByName("async_reset") == 0) {
        ln = "";
        while (ln.size() < space1.size()) {
            ln += " ";
        }
        ln += "bool async_reset";           // Mandatory generic parameter
        ret += ",\n" + ln;
    }
    for (auto &p: entries_) {
        if (p->getId() != ID_DEF_PARAM) {
            continue;
        }
        ret += ",\n";
        ln = "";
        while (ln.size() < space1.size()) {
            ln += " ";
        }
        ln += p->getType();
        ln += " " + p->getName();
        ret += ln;
    }
    ret += ")\n";
    ret += "    : sc_module(name)";
    // Input/Output signal declaration
    tcnt = 0;
    for (auto &p: getEntries()) {
        if (!p->isInput() && !p->isOutput()) {
            continue;
        }
        ret += ",\n    " + p->getName() + "(\"" + p->getName() + "\"";
        if (p->getItem()->isVector()) {
            ret += ", " + p->getItem()->getStrDepth();
        }
        ret += ")";
    }
    // Signal Vectors also should be initialized
    for (auto &p: getEntries()) {
        if (p->isInput() || p->isOutput()) {
            continue;
        }
        if (!p->isVector() || p->getName() == "") {
            continue;
        }
        ret += ",\n    " + p->getName() + "(\"" + p->getName() + "\"";
        ret += ", " + p->getStrDepth();
        ret += ")";
    }
    ret += " {\n";
    ret += "\n";
    // local copy of the generic parameters:
    if (getAsyncReset() && getEntryByName("async_reset") == 0) {
        ret += "    async_reset_ = async_reset;\n";
    }
    for (auto &p: entries_) {
        if (p->getId() == ID_DEF_PARAM) {
            ret += "    " + p->getName() + "_ = " + p->getName() + ";\n";
        }
        if (p->getId() == ID_PARAM && p->isGenericDep() && tmpllist.size() == 0) {
            ret += "    " + p->getName() + " = " + p->getStrValue() + ";\n";
        }
    }

    // Sub-module instantiation
    Operation::set_space(1);
    ret += generate_sysc_submodule_nullify();
    for (auto &p: entries_) {
        if (p->getId() != ID_OPERATION) {
            continue;
        }
        ret += p->generate();
        ret += "\n";
    }

    // Process sensitivity list:
    std::list<GenObject *> objlist;
    std::string prefix1 = "r.";
    for (auto &p: entries_) {
        if (p->getId() != ID_PROCESS) {
            continue;
        }
        if (p->getName() == "registers") {
            continue;
        }
        ret += "\n";
        ret += "    SC_METHOD(" + p->getName() + ");\n";

        ln = std::string("");
        Operation::set_space(1);
        ret += generate_sysc_sensitivity(prefix1, ln, this);
    }
    if (isRegProcess()) {
        ret += "\n";
        ret += "    SC_METHOD(registers);\n";
        if (getResetPort()) {
            ret += "    sensitive << " + getResetPort()->getName() + ";\n";
        }
        ret += "    sensitive << " + getClockPort()->getName() + ".pos();\n";
    }

    if (isNRegProcess()) {
        ret += "\n";
        ret += "    SC_METHOD(nregisters);\n";
        if (getResetPort()) {
            ret += "    sensitive << " + getResetPort()->getName() + ";\n";
        }
        ret += "    sensitive << " + getClockPort()->getName() + ".neg();\n";
    }
    ret += "}\n";
    ret += "\n";
    return ret;
}

std::string ModuleObject::generate_sysc_submodule_nullify() {
    std::string ret = "";
    int icnt = 0;

    for (auto &p: entries_) {
        if (p->getId() == ID_MODULE_INST) {
            ret += Operation::addspaces() + "" + p->getName() + " = 0;\n";
            icnt++;
        } else if (p->getId() == ID_ARRAY_DEF) {
            if (p->getItem()->getId() == ID_MODULE_INST) {
                icnt++;
                ret += Operation::addspaces();
                ret += "for (int i = 0; i < " + p->getStrDepth() + "; i++) {\n";
                Operation::set_space(Operation::get_space() + 1);
                ret += Operation::addspaces() + p->getName() + "[i] = 0;\n";
                Operation::set_space(Operation::get_space() - 1);
                ret += Operation::addspaces() + "}\n";
            }
        }
    }
    if (icnt) {
        ret += "\n";
    }
    return ret;
}


std::string ModuleObject::generate_sysc_destructor() {
    std::string ret = "";

    ret += generate_sysc_template_f_name("");
    ret += "::~" + getType() + "() {\n";
    Operation::set_space(Operation::get_space() + 1);
    for (auto &p: entries_) {
        if (p->getId() == ID_MODULE_INST) {
            ret += Operation::addspaces() + "if (" + p->getName() + ") {\n";
            Operation::set_space(Operation::get_space() + 1);
            ret += Operation::addspaces() + "delete " + p->getName() + ";\n";
            Operation::set_space(Operation::get_space() - 1);
            ret += Operation::addspaces() + "}\n";
        } else if (p->getId() == ID_ARRAY_DEF) {
            if (p->getItem()->getId() == ID_MODULE_INST) {
                ret += Operation::addspaces();
                ret += "for (int i = 0; i < " + p->getStrDepth() + "; i++) {\n";
                Operation::set_space(Operation::get_space() + 1);
                ret += Operation::addspaces() + "if (" + p->getName() + "[i]) {\n";
                Operation::set_space(Operation::get_space() + 1);
                ret += Operation::addspaces() + "delete " + p->getName() + "[i];\n";
                Operation::set_space(Operation::get_space() - 1);
                ret += Operation::addspaces() + "}\n";
                Operation::set_space(Operation::get_space() - 1);
                ret += Operation::addspaces() + "}\n";
            }
        }
    }
    Operation::set_space(Operation::get_space() - 1);
    ret += "}\n";
    ret += "\n";
    return ret;
}

std::string ModuleObject::generate_sysc_vcd() {
    std::string ret = "";
    std::string ln = "";
    std::string ln2 = "";

    ret += generate_sysc_template_f_name();
    ret += "::generateVCD(sc_trace_file *i_vcd, sc_trace_file *o_vcd) {\n";
    Operation::set_space(Operation::get_space() + 1);
    if (isRegProcess() && isCombProcess()) {
        ret += Operation::addspaces() + "std::string pn(name());\n";
    }
    ret += Operation::addspaces() + "if (o_vcd) {\n";
    Operation::set_space(Operation::get_space() + 1);
    ret += generate_sysc_vcd_entries(ln, ln2, this);
    Operation::set_space(Operation::get_space() - 1);
    ret += Operation::addspaces() + "}\n";
    ret += "\n";

    // Sub modules:
    for (auto &p: entries_) {
        if (p->getId() == ID_MODULE_INST && p->isVcd()) {
            ret += Operation::addspaces() + "if (" + p->getName() + ") {\n";
            Operation::set_space(Operation::get_space() + 1);
            ret += Operation::addspaces() + p->getName() + "->generateVCD(i_vcd, o_vcd);\n";
            Operation::set_space(Operation::get_space() - 1);
            ret += Operation::addspaces() + "}\n";
        } else if (p->getId() == ID_ARRAY_DEF) {
            if (p->getItem()->getId() == ID_MODULE_INST && p->isVcd()) {
                ret += Operation::addspaces();
                ret += "for (int i = 0; i < " + p->getStrDepth() + "; i++) {\n";
                Operation::set_space(Operation::get_space() + 1);
                ret += Operation::addspaces() + "if (" + p->getName() + "[i]) {\n";
                Operation::set_space(Operation::get_space() + 1);
                ret += Operation::addspaces();
                ret += p->getName() + "[i]->generateVCD(i_vcd, o_vcd);\n";
                Operation::set_space(Operation::get_space() - 1);
                ret += Operation::addspaces() + "}\n";
                Operation::set_space(Operation::get_space() - 1);
                ret += Operation::addspaces() + "}\n";
            }
        }
    }
    Operation::set_space(Operation::get_space() - 1);
    ret += Operation::addspaces() + "}\n";
    ret += "\n";
    return ret;
}

std::string ModuleObject::generate_sysc_cpp() {
    std::string out = "";
    std::string ln;
    std::string text = "";

    // static strings
    Operation::set_space(0);
    out += generate_sysc_param_strings();

    // Constructor
    Operation::set_space(0);
    out += generate_sysc_constructor();

    // Destructor:
    if (isSubModules()) {
        Operation::set_space(0);
        out += generate_sysc_destructor();
    }

    // generateVCD function
    if (isVcd()) {
        Operation::set_space(0);
        out += generate_sysc_vcd();
    }

    // Functions
    for (auto &p: entries_) {
        if (p->getId() != ID_FUNCTION) {
            continue;
        }
        Operation::set_space(0);
        out += generate_sysc_func(p);
        out += "\n";
    }

    // Process
    for (auto &p: entries_) {
        if (p->getId() != ID_PROCESS) {
            continue;
        }
        if (p->getName() == "registers") {
            continue;
        }
        Operation::set_space(0);
        out += generate_sysc_proc(p);
    }

    Operation::set_space(0);
    if (isRegProcess()) {
        out += generate_sysc_proc_registers(true);
    }
    if (isNRegProcess()) {
        out += generate_sysc_proc_registers(false);
    }
    return out;
}

std::string ModuleObject::generate_sysc_func(GenObject *func) {
    std::string ret = "";
    ret += generate_sysc_template_f_name(func->getType().c_str()) + "::";
    ret += static_cast<FunctionObject *>(func)->generate();
    return ret;
}

std::string ModuleObject::generate_sysc_proc(GenObject *proc) {
    std::string ret = "";
    std::string ln;
    int tcnt = 0;

    ret += generate_sysc_template_f_name();
    ret += "::" + proc->getName() + "() {\n";
    
    // process variables declaration
    tcnt = 0;
    for (auto &e: proc->getEntries()) {
        ln = "";
        if (e->getId() == ID_VALUE
            || e->getId() == ID_STRUCT_INST
            || e->getId() == ID_VECTOR) {
            ln += "    " + e->getType() + " " + e->getName();
        } else if (e->getId() == ID_ARRAY_DEF) {
            ln += "    " + e->getType() + " " + e->getName();
            ln += "[";
            ln += e->getStrDepth();
            ln += "]";
        } else {
            continue;
        }
        tcnt++;
        ln += ";";
        if (e->getComment().size()) {
            while (ln.size() < 60) {
                ln += " ";
            }
            ln += "// " + e->getComment();
        }
        ret += ln + "\n";
    }
    if (tcnt) {
        ret += "\n";
        tcnt = 0;
    }

    // nullify all local variables to avoid latches:
    GenObject *arritem;
    for (auto &e: proc->getEntries()) {
        if (e->getId() == ID_VALUE
            || (e->getId() == ID_STRUCT_INST && e->getStrValue().size())) {
            ret += "    " + e->getName() + " = ";
            if (e->getStrValue().size()) {
                ret += e->getStrValue();
            } else {
                ret += "0";
            }
            ret += ";";
            tcnt++;
        } else if (e->getId() == ID_ARRAY_DEF) {
            ret += "    for (int i = 0; i < " + e->getStrDepth() + "; i++) {\n";
            ret += "        " + e->getName() + "[i] = ";
            arritem = e->getItem();
            if (arritem->getId() == ID_STRUCT_INST && arritem->getStrValue().size() == 0) {
                SHOW_ERROR("todo: %s", "crawl through sub-structure element");
            }
            ret += arritem->getStrValue();
            ret += ";\n";
            ret += "    }";
        } else {
            continue;
        }
        ret += "\n";
    }
    if (tcnt) {
        tcnt = 0;
        ret += "\n";
    }

    if (isRegProcess()) {
        Operation::set_space(1);
        ret += Operation::copyreg("v", "r", this);
        tcnt++;
    }
    if (isNRegProcess()) {
        Operation::set_space(1);
        ret += Operation::copyreg("nv", "nr", this);
        tcnt++;
    }
    if (tcnt) {
        tcnt = 0;
        ret += "\n";
    }

    // Generate operations:
    Operation::set_space(1);
    for (auto &e: proc->getEntries()) {
        if (e->getId() != ID_OPERATION) {
            continue;
        }
        ret += e->generate();
    }

    ret += "}\n";
    ret += "\n";
    return ret;
}

}
