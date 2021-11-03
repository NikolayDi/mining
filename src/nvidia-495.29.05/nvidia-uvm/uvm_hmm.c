/*******************************************************************************
    Copyright (c) 2016-2021 NVIDIA Corporation

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be
        included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#include "uvm_hmm.h"
//
// You need all of the following in order to actually run HMM:
//
//     1) A version of Linux with make_device_exclusive_range() defined
//        (usually 5.14 or later), with config CONFIG_HMM_MIRROR and
//        CONFIG_DEVICE_PRIVATE set.
//
//     2) *Not* passing UVM Kernel module parameter "disable_hmm=1".
//
//     3) ATS must not be enabled
//
//     4) UvmInitialize() called without the UVM_INIT_FLAGS_DISABLE_HMM or
//        UVM_INIT_FLAGS_MULTI_PROCESS_SHARING_MODE flags
//
//     5) A system with Turing+ GPUs since GSP OpenRM is required and GPU
//        VA bits >= CPU VA bits. Otherwise, the GPU can't address all of
//        the CPU's virtual address space.
//
static bool disable_hmm = false;
module_param(disable_hmm, bool, 0444);
MODULE_PARM_DESC(disable_hmm,
                 "Force-disable HMM functionality in the UVM driver. "
                 "Default: false (i.e, HMM is potentially enabled). Ignored if "
                 "HMM is not supported in the driver, or if ATS settings "
                 "conflict with HMM.");


























































































































































































































































































































