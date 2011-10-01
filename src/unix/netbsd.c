/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "uv.h"

#include <assert.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/sysctl.h>

#include <unistd.h>
#include <time.h>

#undef NANOSEC
#define NANOSEC 1000000000


uint64_t uv_hrtime(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (ts.tv_sec * NANOSEC + ts.tv_nsec);
}


int uv_exepath(char* buffer, size_t* size) {
  uint32_t usize;
  int result;
  char* path;
  char* fullpath;
  int mib[4];
  size_t cb;
  pid_t mypid;

  if (!buffer || !size) {
    return -1;
  }

  mypid = getpid();
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC_ARGS;
  mib[2] = mypid;
  mib[3] = KERN_PROC_ARGV;

  cb = *size;
  if (sysctl(mib, 4, buffer, &cb, NULL, 0) < 0) {
    *size = 0;
    return -1;
  }
  *size = strlen(buffer);

  return 0;
}

double uv_get_free_memory(void) {
  struct uvmexp info;
  size_t size = sizeof(info);
  int which[] = {CTL_VM, VM_UVMEXP};

  if (sysctl(which, 2, &info, &size, NULL, 0) < 0) {
    return -1;
  }

  return (double) info.free * psysconf(_SC_PAGESIZE);
}

double uv_get_total_memory(void) {
#if defined(HW_PHYSMEM64)
  uint64_t info;
  int which[] = {CTL_HW, HW_PHYSMEM64};
#else
  unsigned int info;
  int which[] = {CTL_HW, HW_PHYSMEM};
#endif
  size_t size = sizeof(info);

  if (sysctl(which, 2, &info, &size, NULL, 0) < 0) {
    return -1;
  }

  return (double) info;
}

int uv_fs_event_init(uv_loop_t* loop,
                     uv_fs_event_t* handle,
                     const char* filename,
                     uv_fs_event_cb cb) {
  uv__set_sys_error(loop, ENOSYS);
  return -1;
}


void uv__fs_event_destroy(uv_fs_event_t* handle) {
  assert(0 && "implement me");
}
