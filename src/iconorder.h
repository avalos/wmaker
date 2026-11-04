/* iconorder.h - icon yard order helpers
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

#ifndef ICONORDER_H
#define ICONORDER_H

/*
 * Describes an icon yard without any X types.  The origin is the
 * coordinate of the first slot, and the steps are the direction the
 * yard takes (+1 or -1) along each axis.  The primary axis is the one
 * slots advance along before wrapping into a new row.
 */
typedef struct IconYard {
	int origin_x;
	int origin_y;
	int step_x;
	int step_y;
	int icon_size;
	int columns;
	int rows;
	int vertical;
} IconYard;

/* Slot under a point, wrapping into rows along the secondary axis. */
int iconorder_slot_at(const IconYard *yard, int x, int y);

/* Clamp a slot to the first and last slot of a kind's run. */
int iconorder_clamp_slot(int slot, int first, int last);

/* True while the point is within the run's rows plus one row either way. */
int iconorder_is_on_rows(const IconYard *yard, int x, int y, int first,
			 int last);

/* Move one entry, shifting the entries between the two positions. */
void iconorder_move_entry(void **items, int count, int from, int to);

#endif				/* ICONORDER_H */
