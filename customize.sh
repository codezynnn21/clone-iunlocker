#!/system/bin/sh
SKIPUNZIP=0
ui_print "========================================"
ui_print "  iUnlocker GLTool"
ui_print "  GPU spoof + AC bypass hooks"
ui_print "========================================"
ui_print "- Zygisk module (arm64 + arm32)"
ui_print "- Target: com.garena.game.codm"
ui_print "- Enable Zygisk and add the game to DenyList"
ui_print "========================================"

# ABI check
case "$ARCH" in
  arm64|arm) ui_print "- ABI: $ARCH supported" ;;
  *) ui_print "! ABI $ARCH may not be supported" ;;
esac
