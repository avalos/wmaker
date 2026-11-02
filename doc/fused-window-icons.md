# Fused window icons

Window Maker can paint one fused icon per open window instead of one AppIcon per application, optionally collapsing
an application's many windows into one group icon. This document describes the preferences and the behaviour of that
fork.

## Preferences

The preferences live in the Window Maker defaults domain (`~/GNUstep/Defaults/WindowMaker` by default) and are exposed
under WPrefs → Expert. They are read at startup or configuration reload, so **restart Window Maker** after changing any
of them; a rebuilt package is not needed.

| Preference | Default | Effect | Restart needed |
| --- | --- | --- | --- |
| `FuseWindowIcons` | `YES` | On: one icon per window instead of per application, carrying the application actions and the window actions. Off: today's behaviour, one AppIcon per application and a miniwindow per minimized window. | Yes |
| `FuseWindowIconGrouping` | `YES` | On: group the windows of one application behind a single group icon once the count exceeds the threshold. Off: always show one icon per window, whatever the count. | Yes |
| `FuseWindowIconGroupThreshold` | `10` | Window count above which grouping replaces the per-window icons (`10` means an application with 10 windows still shows 10 icons). Values below `1` are clamped to `1`. | Yes |
