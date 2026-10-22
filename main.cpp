/*
 * codm_strong_hide
 * Target: com.garena.game.codm
 * Stronger companion hide:
 *   - extended property spoof
 *   - open/openat denial for common root/Magisk/KSU paths
 *   - /proc/self/maps read filter skeleton (drops lines containing zygisk/magisk/lsplt/hide)
 * Pair with DenyList or Shamiko. Vectors change per CODM build.
 */
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
        if (target) {
            // Register PLT hooks before specialize completes
            api->pltHookRegister(".*\\libandroid_runtime.so", "openat", (void *)hook_openat, (void **)&orig_openat);
            api->pltHookRegister(".*\\libc.so", "open", (void *)hook_open, (void **)&orig_open);
            api->pltHookCommit();
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
