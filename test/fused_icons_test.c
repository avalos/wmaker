/* fused_icons_test.c - tests for the fused window icon decisions
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

#include <stdio.h>

#include "fusedicons.h"

static int failures;

static void check(const char *name, int got, int expected)
{
	if (got == expected) {
		printf("PASS: %s\n", name);
	} else {
		printf("FAIL: %s (got %d, expected %d)\n", name, got, expected);
		failures++;
	}
}

static void testAppliesToWindow(void)
{
	check("fused icon applies to a managed window when enabled",
	      fusedIconAppliesToWindow(1, 0, 0, 0, 0), 1);
	check("no fused icon when the preference is off",
	      fusedIconAppliesToWindow(0, 0, 0, 0, 0), 0);
	check("no fused icon for a transient window",
	      fusedIconAppliesToWindow(1, 1, 0, 0, 0), 0);
	check("no fused icon for a client-drawn icon window",
	      fusedIconAppliesToWindow(1, 0, 1, 0, 0), 0);
	check("no fused icon when the window opts out",
	      fusedIconAppliesToWindow(1, 0, 0, 1, 0), 0);
	check("no fused icon for a window that cannot be minimized",
	      fusedIconAppliesToWindow(1, 0, 0, 0, 1), 0);
}

static void testHiddenOnWorkspace(void)
{
	check("fused icon hidden on another workspace",
	      fusedIconHiddenOnWorkspace(0, 0, 1, 0), 1);
	check("fused icon shown on its own workspace",
	      fusedIconHiddenOnWorkspace(0, 0, 1, 1), 0);
	check("fused icon shown on every workspace when omnipresent",
	      fusedIconHiddenOnWorkspace(1, 0, 1, 0), 0);
	check("fused icon shown on every workspace with sticky icons",
	      fusedIconHiddenOnWorkspace(0, 1, 1, 0), 0);
}

static void testBringsWindowHere(void)
{
	check("restore keeps the window workspace when fused",
	      fusedIconBringsWindowHere(1, 0), 0);
	check("shift-click brings the window to the current workspace",
	      fusedIconBringsWindowHere(1, 1), 1);
	check("restore moves the window when the preference is off",
	      fusedIconBringsWindowHere(0, 0), 1);
}

static void testPositionOnScreen(void)
{
	check("an icon position inside the screen is kept",
	      fusedIconPositionOnScreen(10, 10, 1024, 768), 1);
	check("an icon position on the top-left corner is kept",
	      fusedIconPositionOnScreen(0, 0, 1024, 768), 1);
	check("an icon position on the bottom-right corner is kept",
	      fusedIconPositionOnScreen(1023, 767, 1024, 768), 1);
	check("an icon position left of the screen is off-screen",
	      fusedIconPositionOnScreen(-1, 10, 1024, 768), 0);
	check("an icon position above the screen is off-screen",
	      fusedIconPositionOnScreen(10, -1, 1024, 768), 0);
	check("an icon position right of the screen is off-screen",
	      fusedIconPositionOnScreen(1024, 10, 1024, 768), 0);
	check("an icon position below the screen is off-screen",
	      fusedIconPositionOnScreen(10, 768, 1024, 768), 0);
}

static void testGroupShowsOnWorkspace(void)
{
	int workspaces[3] = { 0, 0, 2 };
	int omnipresent[3] = { 0, 0, 0 };

	check("group shows on a workspace with one of its windows",
	      fusedGroupShowsOnWorkspace(workspaces, omnipresent, 3, 0, 0), 1);
	check("group hidden on a workspace with none of its windows",
	      fusedGroupShowsOnWorkspace(workspaces, omnipresent, 3, 0, 1), 0);
	check("group shows on the workspace of a lone window",
	      fusedGroupShowsOnWorkspace(&workspaces[2], &omnipresent[2], 1, 0, 2), 1);

	omnipresent[0] = 1;
	check("group shows on every workspace with an omnipresent window",
	      fusedGroupShowsOnWorkspace(workspaces, omnipresent, 3, 0, 1), 1);

	check("group shows on every workspace with sticky icons",
	      fusedGroupShowsOnWorkspace(workspaces, omnipresent, 3, 1, 1), 1);

	check("group hidden everywhere with no windows",
	      fusedGroupShowsOnWorkspace(workspaces, omnipresent, 0, 0, 0), 0);
	check("group shown with no windows only when sticky icons are on",
	      fusedGroupShowsOnWorkspace(workspaces, omnipresent, 0, 1, 0), 1);
}

int main(void)
{
	testAppliesToWindow();
	testHiddenOnWorkspace();
	testBringsWindowHere();
	testPositionOnScreen();
	testGroupShowsOnWorkspace();

	if (failures) {
		printf("%d test(s) failed\n", failures);
		return 1;
	}

	printf("All fused icon tests passed\n");
	return 0;
}
