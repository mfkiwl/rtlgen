#pragma once

#include <api_core.h>
#include "systemc_gen.h"

namespace sysvc {

SystemCGenerator::SystemCGenerator(AttributeType &cfg) {
    cfg_ = cfg;
    ns_.make_string("debugger");
    sz_ = cfg_["FileMaxLength"].to_int64();
    out_ = new char[sz_];
    pos_ = 0;

}

SystemCGenerator::~SystemCGenerator() {
    delete [] out_;
}


void SystemCGenerator::generate(ModuleObject *m) {
    generate_h(m);
}

void SystemCGenerator::generate_h(ModuleObject *m) {
    if (m->id == LIST_ID_MODULE) {
        pos_ = 0;
        generate_h_module(m);

        char outfile[4096];
        RISCV_sprintf(outfile, sizeof(outfile), "%s/%s.h",
            cfg_["OutputDir"].to_string(),
            m->name.to_string());
        write_file(outfile);
    }
}

void SystemCGenerator::generate_h_module(ModuleObject *m) {
    const char *mname = m->name.to_string();

    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_, "%s",
        "/*\n"
        " *  Copyright 2022 Sergey Khabarov, sergeykhbr@gmail.com\n"
        " *\n"
        " *  Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        " *  you may not use this file except in compliance with the License.\n"
        " *  You may obtain a copy of the License at\n"
        " *\n"
        " *      http://www.apache.org/licenses/LICENSE-2.0\n"
        " *\n"
        " *  Unless required by applicable law or agreed to in writing, software\n"
        " *  distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        " *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        " *  See the License for the specific language governing permissions and\n"
        " *  limitations under the License.\n"
        " */\n"
        " \n"
        "#pragma once\n"
        "\n"
        "#include <systemc.h>\n"
        "\n"
        );

    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_,
        "namespace %s {\n"
        "\n",
            ns_.to_string());

    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_,
        "SC_MODULE(%s) {\n",
            mname);

    // Input signal declaration
    IoObject *io = m->io;
    int width;
    while (io) {
        const char *ioname = io->name.to_string();

        if (io->dir == IO_DIR_INPUT) {
            add_string("    sc_in");
        } else if (io->dir == IO_DIR_OUTPUT) {
            add_string("    sc_out");
        }

        if (io->width.is_integer()) {
            width = io->width.to_int();
        } else {
            width = getParameterValue(io->width.to_string());
        }
        if (width == 1) {
            add_string("<bool> ");
        } else if (width <= 64) {
            add_string("<sc_uint<");
            if (io->width.is_integer()) {
                add_dec(width);
            } else {
                add_string(io->width.to_string());
            }
            add_string(">> ");
        } else {
            add_string("<sc_biguint<");
            if (io->width.is_integer()) {
                add_dec(width);
            } else {
                add_string(io->width.to_string());
            }
            add_string(">> ");
        }

        pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_,
            "%s;    // %s\n",
                ioname,
                io->comment.to_string());

        io = static_cast<IoObject *>(io->next);
    }

    
    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_, "%s",
        "};\n"
        "\n");


    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_, "\n}  // namespace %s\n",
        ns_.to_string());
}

void SystemCGenerator::add_string(const char *str) {
    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_, "%s", str);
}

void SystemCGenerator::add_dec(int v) {
    pos_ += RISCV_sprintf(&out_[pos_], sz_ - pos_, "%d", v);
}

void SystemCGenerator::write_file(const char *fname) {
    FILE *f = fopen(fname, "wb");
    if (f) {
        fwrite(out_, 1, pos_, f);
        fclose(f);
    } else {
        RISCV_printf("error: cannot open file %s\n", fname);
    }
}

} // namepace sysvc

