/* iconorder.c - icon yard order helpers
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

#include "iconorder.h"

static int floor_div(int value, int divisor)
{
	int quotient = value / divisor;

	if (value % divisor != 0 && (value < 0) != (divisor < 0))
		quotient--;

	return quotient;
}

/*
 * The origin is the near edge of slot 0 (its left or top edge), so a
 * yard that runs in the negative direction has slot k covering
 * (origin - k * size, origin - (k - 1) * size].
 */
static int axis_index(int coordinate, int origin, int step, int size)
{
	if (step < 0)
		return floor_div(origin - coordinate + size - 1, size);

	return floor_div(coordinate - origin, size);
}

static int primary_count(const IconYard *yard)
{
	return yard->vertical ? yard->rows : yard->columns;
}

static int yard_secondary(const IconYard *yard, int x, int y)
{
	if (yard->vertical)
		return axis_index(x, yard->origin_x, yard->step_x,
				  yard->icon_size);

	return axis_index(y, yard->origin_y, yard->step_y, yard->icon_size);
}

static int yard_primary(const IconYard *yard, int x, int y)
{
	if (yard->vertical)
		return axis_index(y, yard->origin_y, yard->step_y,
				  yard->icon_size);

	return axis_index(x, yard->origin_x, yard->step_x, yard->icon_size);
}

int iconorder_slot_at(const IconYard *yard, int x, int y)
{
	int primary;
	int secondary;

	if (!yard || yard->icon_size <= 0 || yard->columns <= 0 ||
	    yard->rows <= 0)
		return 0;

	primary = iconorder_clamp_slot(yard_primary(yard, x, y), 0,
				       primary_count(yard) - 1);
	secondary = yard_secondary(yard, x, y);

	return secondary * primary_count(yard) + primary;
}

int iconorder_clamp_slot(int slot, int first, int last)
{
	int swap;

	if (last < first) {
		swap = first;
		first = last;
		last = swap;
	}

	if (slot < first)
		return first;
	if (slot > last)
		return last;

	return slot;
}

int iconorder_is_on_rows(const IconYard *yard, int x, int y, int first,
			 int last)
{
	int rows;
	int secondary;
	int first_row;
	int last_row;
	int swap;

	if (!yard || yard->icon_size <= 0 || yard->columns <= 0 ||
	    yard->rows <= 0)
		return 0;

	if (last < first) {
		swap = first;
		first = last;
		last = swap;
	}

	rows = primary_count(yard);
	secondary = yard_secondary(yard, x, y);
	first_row = floor_div(first, rows);
	last_row = floor_div(last, rows);

	return secondary >= first_row - 1 && secondary <= last_row + 1;
}

void iconorder_move_entry(void **items, int count, int from, int to)
{
	void *entry;
	int step;
	int i;

	if (!items || count <= 0)
		return;
	if (from < 0 || from >= count || to < 0 || to >= count ||
	    from == to)
		return;

	entry = items[from];
	step = from < to ? 1 : -1;
	for (i = from; i != to; i += step)
		items[i] = items[i + step];
	items[to] = entry;
}
