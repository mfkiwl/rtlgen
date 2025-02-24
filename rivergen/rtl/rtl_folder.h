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

#include <api.h>
#include "ambalib/ambalib_folder.h"
#include "techmap/techmap_folder.h"
#include "riverlib/riverlib_folder.h"
#include "misclib/misclib_folder.h"
#include "riscv_soc.h"


class rtl_folder : public FolderObject {
  public:
    rtl_folder(GenObject *parent) :
        FolderObject(parent, "rtl"),
        ambalib(this),
        techmap(this),
        riverlib(this),
        misclib(this),
        riscv_soc_file_(this) {}

 protected:
    // subfolders:
    ambalib_folder ambalib;
    techmap_folder techmap;
    riverlib_folder riverlib;
    misclib_folder misclib;
    // files
    riscv_soc_file riscv_soc_file_;
};
