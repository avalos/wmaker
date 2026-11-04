/* icon_order_test.c - tests for the icon yard order helpers
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
#include <string.h>

#include "iconorder.h"

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

static void checkSlot(const char *name, const IconYard *yard, int x, int y,
		      int expected)
{
	check(name, iconorder_slot_at(yard, x, y), expected);
}

static void checkRow(const char *name, int from, int to,
		     const char *const expected[])
{
	char cells[5][2];
	char *items[5];
	int i;

	for (i = 0; i < 5; i++) {
		cells[i][0] = (char) ('A' + i);
		cells[i][1] = '\0';
		items[i] = cells[i];
	}

	iconorder_move_entry((void **) items, 5, from, to);

	for (i = 0; i < 5; i++) {
		if (strcmp(items[i], expected[i]) != 0) {
			printf("FAIL: %s (position %d is %s, expected %s)\n",
			       name, i, items[i], expected[i]);
			failures++;
			return;
		}
	}

	printf("PASS: %s\n", name);
}

static void testRowScenarios(void)
{
	static const char *const front[] = { "A", "D", "B", "C", "E" };
	static const char *const back[] = { "A", "C", "D", "B", "E" };
	static const char *const pastFront[] = { "E", "A", "B", "C", "D" };
	static const char *const pastBack[] = { "B", "C", "D", "E", "A" };
	static const char *const unchanged[] = { "A", "B", "C", "D", "E" };

	checkRow("drag toward the front gives A D B C E", 3, 1, front);
	checkRow("drag toward the back gives A C D B E", 1, 3, back);
	checkRow("drag past the front gives E A B C D", 4, 0, pastFront);
	checkRow("drag past the back gives B C D E A", 0, 4, pastBack);
	checkRow("the same slot gives no move", 2, 2, unchanged);
}

static void testSlotsAndWrapping(void)
{
	static const IconYard yard = {
		0, 0, 1, 1, 50, 3, 2, 0
	};

	checkSlot("point in the first slot", &yard, 0, 0, 0);
	checkSlot("point inside the first slot", &yard, 49, 49, 0);
	checkSlot("point in the last slot of a row", &yard, 149, 0, 2);
	checkSlot("point in the next row wraps", &yard, 0, 50, 3);
	checkSlot("point in the next row and column", &yard, 150, 50, 5);
	checkSlot("past the end of a row clamps to its last slot", &yard,
		  1000, 0, 2);
	checkSlot("past the start of a row clamps to its first slot", &yard,
		  -1000, 0, 0);
	checkSlot("past the end of a later row clamps inside that row", &yard,
		  1000, 50, 5);
}

static void testDirections(void)
{
	static const IconYard leftToRight = {
		0, 0, 1, 1, 50, 4, 2, 0
	};
	static const IconYard rightToLeft = {
		200, 0, -1, 1, 50, 4, 2, 0
	};
	static const IconYard bottomRow = {
		0, 200, 1, -1, 50, 4, 2, 0
	};
	static const IconYard topToBottom = {
		0, 0, 1, 1, 50, 2, 4, 1
	};
	static const IconYard bottomToTop = {
		0, 200, 1, -1, 50, 2, 4, 1
	};
	static const IconYard rightToLeftColumn = {
		50, 0, -1, 1, 50, 2, 4, 1
	};

	checkSlot("left-to-right yard, first slot", &leftToRight, 0, 0, 0);
	checkSlot("left-to-right yard, second column", &leftToRight, 50, 0, 1);
	checkSlot("left-to-right yard, second row", &leftToRight, 0, 50, 4);
	checkSlot("left-to-right yard, last slot", &leftToRight, 150, 50, 7);

	checkSlot("right-to-left yard, first slot", &rightToLeft, 200, 0, 0);
	checkSlot("right-to-left yard, second column", &rightToLeft, 199, 0, 1);
	checkSlot("right-to-left yard, column edge", &rightToLeft, 150, 0, 1);
	checkSlot("right-to-left yard, third column", &rightToLeft, 149, 0, 2);
	checkSlot("right-to-left yard, last column", &rightToLeft, 50, 0, 3);
	checkSlot("right-to-left yard, second row", &rightToLeft, 200, 50, 4);
	checkSlot("right-to-left yard, last slot", &rightToLeft, 149, 50, 6);

	checkSlot("bottom-row yard, first slot", &bottomRow, 0, 249, 0);
	checkSlot("bottom-row yard, top of the first slot", &bottomRow, 0, 200,
		  0);
	checkSlot("bottom-row yard, second row", &bottomRow, 0, 199, 4);
	checkSlot("bottom-row yard, bottom of the second row", &bottomRow, 0,
		  150, 4);

	checkSlot("top-to-bottom yard, first slot", &topToBottom, 0, 0, 0);
	checkSlot("top-to-bottom yard, second row", &topToBottom, 0, 50, 1);
	checkSlot("top-to-bottom yard, last row", &topToBottom, 0, 150, 3);
	checkSlot("top-to-bottom yard, past the last row", &topToBottom, 0, 200,
		  3);
	checkSlot("top-to-bottom yard, second column", &topToBottom, 50, 0, 4);
	checkSlot("top-to-bottom yard, last slot", &topToBottom, 50, 150, 7);

	checkSlot("bottom-to-top yard, first slot", &bottomToTop, 0, 200, 0);
	checkSlot("bottom-to-top yard, top of the first slot", &bottomToTop, 0,
		  249, 0);
	checkSlot("bottom-to-top yard, second row", &bottomToTop, 0, 199, 1);
	checkSlot("bottom-to-top yard, last row", &bottomToTop, 0, 50, 3);
	checkSlot("bottom-to-top yard, past the last row", &bottomToTop, 0, 49,
		  3);
	checkSlot("bottom-to-top yard, second column", &bottomToTop, 50, 200,
		  4);
	checkSlot("bottom-to-top yard, last slot", &bottomToTop, 50, 50, 7);

	checkSlot("right-to-left column yard, first slot",
		  &rightToLeftColumn, 50, 0, 0);
	checkSlot("right-to-left column yard, second column",
		  &rightToLeftColumn, 49, 0, 4);
	checkSlot("right-to-left column yard, last slot",
		  &rightToLeftColumn, 49, 150, 7);
}

static void testBand(void)
{
	static const IconYard horizontal = {
		0, 0, 1, 1, 50, 3, 5, 0
	};
	static const IconYard vertical = {
		0, 0, 1, 1, 50, 4, 2, 1
	};

	check("position in the run's row is on the rows",
	      iconorder_is_on_rows(&horizontal, 10, 10, 0, 2), 1);
	check("position one row above the run is on the rows",
	      iconorder_is_on_rows(&horizontal, 10, -50, 0, 2), 1);
	check("position one row below the run is on the rows",
	      iconorder_is_on_rows(&horizontal, 10, 50, 0, 2), 1);
	check("position two rows below the run is off the rows",
	      iconorder_is_on_rows(&horizontal, 10, 100, 0, 2), 0);
	check("position two rows above the run is off the rows",
	      iconorder_is_on_rows(&horizontal, 10, -100, 0, 2), 0);
	check("position in the last row of a taller run is on the rows",
	      iconorder_is_on_rows(&horizontal, 10, 50, 0, 4), 1);
	check("position one row past a taller run is on the rows",
	      iconorder_is_on_rows(&horizontal, 10, 100, 0, 4), 1);
	check("position two rows past a taller run is off the rows",
	      iconorder_is_on_rows(&horizontal, 10, 150, 0, 4), 0);

	check("vertical position in the run's column is on the rows",
	      iconorder_is_on_rows(&vertical, 10, 10, 0, 3), 1);
	check("vertical position one column past the run is on the rows",
	      iconorder_is_on_rows(&vertical, 100, 10, 0, 3), 1);
	check("vertical position two columns past the run is off the rows",
	      iconorder_is_on_rows(&vertical, 150, 10, 0, 3), 0);
	check("vertical position one column before the run is on the rows",
	      iconorder_is_on_rows(&vertical, -50, 10, 0, 3), 1);
	check("vertical position two columns before the run is off the rows",
	      iconorder_is_on_rows(&vertical, -100, 10, 0, 3), 0);
}

int main(void)
{
	testRowScenarios();
	testSlotsAndWrapping();
	testDirections();
	testBand();

	if (failures) {
		printf("%d test(s) failed\n", failures);
		return 1;
	}

	printf("All icon order tests passed\n");
	return 0;
}
