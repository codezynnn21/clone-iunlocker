/*
 * codm_strong_hide
 * Target: com.garena.game.codm
 * Stronger companion hide:
 *   - extended property spoof
 *   - open/openat denial for common root/Magisk/KSU paths
 *   - /proc/self/maps read filter skeleton (drops lines containing zygisk/magisk/lsplt/hide)
 * Pair with DenyList or Shamiko. Vectors change per CODM build.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <zygisk.hpp>
#include <android/log.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <vector>
#include <mutex>

#define TAG "codm_strong"

using zygisk::Api;
using zygisk::AppSpecializeArgs;

static const char *TARGET = "com.garena.game.codm";

static const char *BLOCK_PATHS[] = {
    "/system/bin/su", "/system/xbin/su", "/sbin/su", "/vendor/bin/su",
    "/data/adb/magisk", "/data/adb/modules", "/data/adb/ksu", "/data/adb/ap",
    "/debug_ramdisk", "/system/bin/magisk", nullptr
};

// Zygisk API v5+ matches ELFs by device/inode instead of the old regex.
// List every known location; the wrong-arch and missing paths fail stat()
// so a single table works for both arm64-v8a and armeabi-v7a builds.
static const char *ANDROID_RUNTIME_PATHS[] = {
    "/apex/com.android.art/lib64/libandroid_runtime.so",
    "/apex/com.android.art/lib/libandroid_runtime.so",
    "/system/lib64/libandroid_runtime.so",
    "/system/lib/libandroid_runtime.so",
    nullptr
};

static const char *LIBC_PATHS[] = {
    "/apex/com.android.runtime/lib64/bionic/libc.so",
    "/apex/com.android.runtime/lib/bionic/libc.so",
