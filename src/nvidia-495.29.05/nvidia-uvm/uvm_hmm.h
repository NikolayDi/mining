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

#ifndef _UVM_HMM_H_
#define _UVM_HMM_H_

#include "nvtypes.h"
#include "uvm_forward_decl.h"
#include "uvm_linux.h"



























#if UVM_IS_CONFIG_HMM()
    // Tells whether HMM is enabled for the given va_space.
    // If it is not enabled, all of the functions below are no-ops.
    bool uvm_hmm_is_enabled(uvm_va_space_t *va_space);

    // Self-explanatory name: reports if HMM is enabled system-wide.
    bool uvm_hmm_is_enabled_system_wide(void);

    // Initialize HMM for the given the va_space.
    // Locking: the va_space->va_space_mm.mm mmap_lock must be write locked
    // and the va_space lock must be held in write mode.
    NV_STATUS uvm_hmm_va_space_initialize(uvm_va_space_t *va_space);

    // Initialize HMM for the given the va_space for testing.
    // Bug 1750144: UVM: Add HMM (Heterogeneous Memory Management) support to
    // the UVM driver. Remove this when enough HMM functionality is implemented.
    NV_STATUS uvm_hmm_va_space_initialize_test(uvm_va_space_t *va_space);

    // Destroy any HMM state for the given the va_space.
    // Locking: va_space lock must be held in write mode.
    void uvm_hmm_va_space_destroy(uvm_va_space_t *va_space);

    // Find an existing HMM va_block.
    // This must be called with va_space read locked.
    NV_STATUS uvm_hmm_va_block_find(uvm_va_space_t *va_space,
                                    NvU64 addr,
                                    uvm_va_block_t **va_block_ptr);

    // Find or create a new HMM va_block.
    // This must be called with mmap_lock read locked and va_space read locked.
    //
    // Return NV_ERR_INVALID_ADDRESS if there is no VMA associated with the
    // address 'addr', or if any part of the UVM_VA_BLOCK_SIZE
    // naturally-aligned region containing addr is covered by a uvm_va_range_t
    // or the VMA does not have at least PROT_READ permission.
    NV_STATUS uvm_hmm_va_block_find_create(uvm_va_space_t *va_space,
                                           struct mm_struct *mm,
                                           NvU64 addr,
                                           uvm_va_block_t **va_block_ptr);

    // If an HMM va_block overlaps the given range, check to see if it can
    // be deleted; otherwise, return NV_ERR_ADDRESS_IN_USE.
    // Locking: the va_space must be write locked.
    NV_STATUS uvm_hmm_va_block_kill(uvm_va_space_t *va_space,
                                    NvU64 start,
                                    NvU64 end);

#else // UVM_IS_CONFIG_HMM()

    static bool uvm_hmm_is_enabled(uvm_va_space_t *va_space)
    {
        return false;
    }

    static bool uvm_hmm_is_enabled_system_wide(void)
    {
        return false;
    }

    static NV_STATUS uvm_hmm_va_space_initialize(uvm_va_space_t *va_space)
    {
        return NV_OK;
    }

    static NV_STATUS uvm_hmm_va_space_initialize_test(uvm_va_space_t *va_space)
    {
        return NV_WARN_NOTHING_TO_DO;
    }

    static void uvm_hmm_va_space_destroy(uvm_va_space_t *va_space)
    {
    }

    static NV_STATUS uvm_hmm_va_block_find(uvm_va_space_t *va_space,
                                           NvU64 addr,
                                           uvm_va_block_t **va_block_ptr)
    {
        return NV_ERR_INVALID_ADDRESS;
    }

    static NV_STATUS uvm_hmm_va_block_find_create(uvm_va_space_t *va_space,
                                                  struct mm_struct *mm,
                                                  NvU64 addr,
                                                  uvm_va_block_t **va_block_ptr)
    {
        return NV_ERR_INVALID_ADDRESS;
    }

    static NV_STATUS uvm_hmm_va_block_kill(uvm_va_space_t *va_space,
                                           NvU64 start,
                                           NvU64 end)
    {
        return NV_OK;
    }

#endif // UVM_IS_CONFIG_HMM()

#endif  // _UVM_HMM_H_
