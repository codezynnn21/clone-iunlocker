#!/system/bin/sh
MODDIR=${0%/*}
LOG=/data/adb/iunlocker_codm/hide.log
mkdir -p /data/adb/iunlocker_codm
echo "[$(date '+%F %T')] service start" >> "$LOG"
resetprop -n ro.debuggable 0 2>/dev/null
resetprop -n ro.secure 1 2>/dev/null
resetprop -n ro.build.type user 2>/dev/null
resetprop -n ro.build.tags release-keys 2>/dev/null
resetprop -n ro.boot.verifiedbootstate green 2>/dev/null
echo "[$(date '+%F %T')] props applied" >> "$LOG"
