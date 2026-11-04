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

## Preview

A fused icon shows a preview of its window's content, captured while the window was visible. The preview is drawn in the
hover balloon that appears when the pointer rests on the icon; the balloon needs `MiniwindowPreviewBalloons = YES` to be
shown.

The preview refreshes on the events that change a window's visible content: when the window maps, gains or loses focus,
unmaps or is iconified, changes workspace, or changes title. Captures are coalesced so a burst of events produces a
single screenshot, and a window that is not viewable is skipped. The capture never blocks window operations.

When no capture has succeeded, the icon shows the application's icon image instead of a preview.

A preview shows the window's content at the moment it was captured. A part of a window that is covered by another window
when it is captured is not guaranteed to show that window's content: Window Maker does not use a compositor, so it can
only read what the X server exposes of the visible window.

## Grouping

When grouping is on, an application with more open windows than the threshold (default `10`, set by
`FuseWindowIconGroupThreshold`) shows one group icon in place of its per-window icons. The group icon is labelled with
the application name and the window count. Clicking it reveals every window of the application, restoring each to its own
workspace and focusing the last focused one; its menu and middle-button relaunch work like the other icons.

The group icon shows on each workspace where the application has at least one window, at the same position, and on no
other workspace. An omnipresent window counts for every workspace, and `sticky_icons` shows the group icon everywhere.

To turn grouping off, set `FuseWindowIconGrouping = NO`; per-window icons are then always shown, whatever the count.
