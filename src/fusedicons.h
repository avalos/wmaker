/* fusedicons.h - decisions for the fused window icons
 *
 *  Window Maker window manager
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WMAKER_FUSEDICONS_H
#define WMAKER_FUSEDICONS_H

int fusedIconAppliesToWindow(int fuse_window_icons, int is_transient,
			     int net_handle_icon, int no_appicon,
			     int no_miniaturizable);

int fusedIconHiddenOnWorkspace(int omnipresent, int sticky_icons,
			       int window_workspace, int current_workspace);

int fusedIconBringsWindowHere(int fuse_window_icons,
			      int bring_to_current_workspace);

int fusedGroupShowsOnWorkspace(const int *window_workspaces,
			       const int *window_omnipresent,
			       int count, int sticky_icons, int workspace);

int fusedIconPositionOnScreen(int x, int y, int screen_width, int screen_height);

#endif
