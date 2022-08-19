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
#include "mem/mem_folder.h"
#include "lrunway.h"
#include "tagmem.h"
#include "tagmemnway.h"
#include "tagmemcoupled.h"
#include "mpu.h"
#include "cache_top.h"

class cache_folder : public FolderObject {
 public:
    cache_folder(GenObject *parent) :
        FolderObject(parent, "cache"),
        mem_folder_(this),
        lrunway_(this),
        tagmem_(this),
        tagmemnway_(this),
        tagmemcoupled_(this),
        mpu_(this),
        cache_top_(this) {}

 protected:
    // subfolders:
    mem_folder mem_folder_;
    // files
    lrunway_file lrunway_;
    tagmem_file tagmem_;
    tagmemnway_file tagmemnway_;
    tagmemcoupled_file tagmemcoupled_;
    mpu_file mpu_;
    cache_top_file cache_top_;
};
