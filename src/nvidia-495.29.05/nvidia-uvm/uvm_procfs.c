/*******************************************************************************
    Copyright (c) 2015-2018 NVIDIA Corporation

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

#include "uvm_global.h"
#include "uvm_procfs.h"
#include "uvm_gpu.h"

#include "nv-procfs.h"
#include "uvm_linux.h"

#define UVM_PROC_DIR_NAME "driver/nvidia-uvm"
#define UVM_PROC_GPUS_DIR_NAME "gpus"
#define UVM_PROC_CPU_DIR_NAME "cpu"

#if defined(CONFIG_PROC_FS)
  // This parameter enables additional debug procfs entries. It's enabled by
  // default for debug and develop builds and disabled for release builds.
  int uvm_enable_debug_procfs = UVM_IS_DEBUG() || UVM_IS_DEVELOP();
  module_param(uvm_enable_debug_procfs, int, S_IRUGO);
  MODULE_PARM_DESC(uvm_enable_debug_procfs, "Enable debug procfs entries in /proc/" UVM_PROC_DIR_NAME);
#else
  int uvm_enable_debug_procfs = 0;
#endif

static struct proc_dir_entry *uvm_proc_dir;
static struct proc_dir_entry *uvm_proc_gpus;
static struct proc_dir_entry *uvm_proc_cpu;

NV_STATUS uvm_procfs_init()
{
    if (!uvm_procfs_is_enabled())
        return NV_OK;

    uvm_proc_dir = NV_CREATE_PROC_DIR(UVM_PROC_DIR_NAME, NULL);
    if (uvm_proc_dir == NULL)
        return NV_ERR_OPERATING_SYSTEM;

    uvm_proc_gpus = NV_CREATE_PROC_DIR(UVM_PROC_GPUS_DIR_NAME, uvm_proc_dir);
    if (uvm_proc_gpus == NULL)
        return NV_ERR_OPERATING_SYSTEM;

    uvm_proc_cpu = NV_CREATE_PROC_DIR(UVM_PROC_CPU_DIR_NAME, uvm_proc_dir);
    if (uvm_proc_cpu == NULL)
        return NV_ERR_OPERATING_SYSTEM;

    return NV_OK;
}

void uvm_procfs_exit()
{
    uvm_procfs_destroy_entry(uvm_proc_dir);
}

// TODO: Bug 1767237: Copied from nv-procfs.c. Refactor it out to
//       nv-procfs-common.c.
static void procfs_destroy_entry_with_root(struct proc_dir_entry *entry, struct proc_dir_entry *delimiter)
{
#if defined(NV_PROC_REMOVE_PRESENT)
    proc_remove(entry);
#else
    while (entry) {
        struct proc_dir_entry *next = entry->next;
        if (entry->subdir)
            procfs_destroy_entry_with_root(entry->subdir, delimiter);
        remove_proc_entry(entry->name, entry->parent);
        if (entry == delimiter)
            break;
        entry = next;
    }
#endif
}

void uvm_procfs_destroy_entry(struct proc_dir_entry *entry)
{
    procfs_destroy_entry_with_root(entry, entry);
}

struct proc_dir_entry *uvm_procfs_get_gpu_base_dir()
{
    return uvm_proc_gpus;
}

struct proc_dir_entry *uvm_procfs_get_cpu_base_dir()
{
    return uvm_proc_cpu;
}

int uvm_procfs_open_callback(void)
{
    // Defer PM lock acquisition until the respective read() callback
    // is invoked, to ensure the lock is acquired and released by
    // the same thread.  Else the lock tracking validation code must
    // be disabled for this lock, which is undesirable.
    // See bug 2594854 for additional information.

    return 0;
}

void uvm_procfs_close_callback(void)
{
    // The PM lock is acquired/released in the read() callback.  See
    // uvm_procfs_open_callback().
}
