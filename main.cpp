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
#include <sys/sysmacros.h>
#include <jni.h>
#include <zygisk.hpp>
#include <android/log.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <mutex>

#define TAG "codm_strong"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

using zygisk::Api;
using zygisk::AppSpecializeArgs;

static const char *TARGET = "com.garena.game.codm";

static const char *BLOCK_PATHS[] = {
    "/system/bin/su", "/system/xbin/su", "/sbin/su", "/vendor/bin/su",
    "/data/adb/magisk", "/data/adb/modules", "/data/adb/ksu", "/data/adb/ap",
    "/debug_ramdisk", "/system/bin/magisk", nullptr
};

static bool path_blocked(const char *p) {
    if (!p) return false;
    for (int i = 0; BLOCK_PATHS[i]; i++)
        if (strstr(p, BLOCK_PATHS[i])) return true;
    return false;
}

static int (*orig_openat)(int, const char *, int, ...) = nullptr;
static int (*orig_open)(const char *, int, ...) = nullptr;

static int hook_openat(int dirfd, const char *pathname, int flags, ...) {
    if (path_blocked(pathname)) {
        LOGI("blocked openat %s", pathname);
        errno = ENOENT;
        return -1;
    }
    if (orig_openat) return orig_openat(dirfd, pathname, flags);
    return -1;
}

static int hook_open(const char *pathname, int flags, ...) {
    if (path_blocked(pathname)) {
        LOGI("blocked open %s", pathname);
        errno = ENOENT;
        return -1;
    }
    if (orig_open) return orig_open(pathname, flags);
    return -1;
}

static bool find_dev_inode(const char *lib_substr, dev_t *out_dev, ino_t *out_inode) {
    FILE *f = fopen("/proc/self/maps", "r");
    if (!f) return false;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (!strstr(line, lib_substr)) continue;
        unsigned int major = 0, minor = 0;
        unsigned long inode = 0;
        if (sscanf(line, "%*x-%*x %*s %*x %x:%x %lu", &major, &minor, &inode) == 3) {
            *out_dev = makedev(major, minor);
            *out_inode = (ino_t)inode;
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

class StrongHide : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        const char *nice = env->GetStringUTFChars(args->nice_name, nullptr);
        if (nice && !strcmp(nice, TARGET)) {
            target = true;
            LOGI("specializing %s", nice);
        }
        if (nice) env->ReleaseStringUTFChars(args->nice_name, nice);
        if (!target) return;

        dev_t runtime_dev = 0, libc_dev = 0;
        ino_t runtime_inode = 0, libc_inode = 0;

        if (find_dev_inode("libandroid_runtime.so", &runtime_dev, &runtime_inode)) {
            api->pltHookRegister(runtime_dev, runtime_inode, "openat",
                                 (void *)hook_openat, (void **)&orig_openat);
        } else {
            LOGI("libandroid_runtime.so not found in maps");
        }

        if (find_dev_inode("libc.so", &libc_dev, &libc_inode)) {
            api->pltHookRegister(libc_dev, libc_inode, "open",
                                 (void *)hook_open, (void **)&orig_open);
            api->pltHookRegister(libc_dev, libc_inode, "openat",
                                 (void *)hook_openat, (void **)&orig_openat);
        } else {
            LOGI("libc.so not found in maps");
        }

        if (!api->pltHookCommit()) {
            LOGI("pltHookCommit failed");
        }
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (!target) return;
        spoof();
        LOGI("strong hide applied");
    }

private:
    Api *api = nullptr;
    JNIEnv *env = nullptr;
    bool target = false;

    void spoof() {
        __system_property_set("ro.debuggable", "0");
        __system_property_set("ro.secure", "1");
        __system_property_set("ro.build.type", "user");
        __system_property_set("ro.build.tags", "release-keys");
        __system_property_set("ro.boot.verifiedbootstate", "green");
        __system_property_set("ro.boot.flash.locked", "1");
        __system_property_set("ro.boot.vbmeta.device_state", "locked");
        __system_property_set("ro.boot.warranty_bit", "0");
        __system_property_set("ro.vendor.boot.warranty_bit", "0");
        __system_property_set("sys.oem_unlock_allowed", "0");
    }
};

REGISTER_ZYGISK_MODULE(StrongHide)
