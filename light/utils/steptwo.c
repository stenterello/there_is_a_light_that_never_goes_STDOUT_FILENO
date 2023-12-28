#include "light.h"

void	dark(t_road_info *road_info)
{
	clear();
	attron(A_REVERSE);
	mvprintw(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
	attroff(A_REVERSE);
	wrefresh(stdscr);
	sleep(2);
	road_info->line++;
	mvprintw(getmaxy(stdscr) - getmaxy(stdscr) / 3, getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
	wrefresh(stdscr);
	flushinp();
	get_space(stdscr);
	road_info->line += 2;
}

int	triple_road_print(int offset)
{
	int	lines_dist = 6;
	int	x_point = getmaxx(stdscr) / 3;
	int	road_h = x_point / 2 - lines_dist;
	int	start_y = getmaxy(stdscr) - road_h;
	int	x = 0;
	int	moving_y = start_y + offset;
	int	idx = 0;

	for (int i = 0; i < 3; i++)
	{
		lines_dist = 6;
		for (int y = start_y; y < getmaxy(stdscr); y++)
		{
			for (int j = x; j < x + x_point; j++)
			{
				if (y == start_y)
					mvwprintw(stdscr, y, j, "_");
				else if (j == (x_point * i + x_point * (i + 1)) / 2 - lines_dist / 2)
					mvwprintw(stdscr, y, j, "/");
				else if (j == (x_point * i + x_point * (i + 1)) / 2 + lines_dist / 2)
					mvwprintw(stdscr, y, j, "\\");
				if (idx && moving_y != getmaxy(stdscr)) {
					if (y != start_y && (y == moving_y || y == moving_y + 1) && (j - x == x_point / 2 || j - x == x_point / 2 + 1))
						mvwprintw(stdscr, y, j, "|");
				} else if (!idx && moving_y != getmaxy(stdscr)) {
					if (y != start_y && (y == moving_y || y == moving_y - 1) && (j - x == x_point / 2 || j - x == x_point / 2 + 1))
						mvwprintw(stdscr, y, j, "|");
				} else {
					if (y != start_y && (y == moving_y || y == moving_y - 1) && (j - x == x_point / 2 || j - x == x_point / 2 + 1))
						mvwprintw(stdscr, y, j, "|");
				}
			}
			lines_dist += 2;
			idx = (idx % 2 == 0) ? idx + 1 : idx - 1;
		}
		x += x_point;
	}
	wrefresh(stdscr);
	return road_h;
}

void
write_text(t_road_info *road_info)
{
 	mvwprintw(stdscr, getmaxy(stdscr) / 3, getmaxx(stdscr) / 2 - (strlen(road_info->matrix[(int)road_info->line])) / 2, "%s", road_info->matrix[(int)road_info->line]);
	wrefresh(stdscr);
}

void	triple_road(t_road_info *road_info)
{
	int	offset = 1;
	int	i = 1;

	flushinp();
	for (;;)
	{
		moon();
		int bench = triple_road_print(offset);
		if (offset == bench + 2)
			offset = 0;
		if (i % 2 == 0)
			offset++;
		i = (i % 2 == 0) ? i - 1 : i + 1;
		write_text(road_info);
		usleep(70000);
		int c = getch();
		if (c == 32)
		{
			road_info->line++;
			c = 0;
			flushinp();
			if ((int)road_info->line == 28)
				break ;
		}
	}
}

int	add_rand()
{
	
	return round((-1+2*(((float)rand())/(2147483647))));
}

int
triple_road_print_trembling(int offset)
{
	int	lines_dist = 6;
	int	x_point = getmaxx(stdscr) / 3;
	int	road_h = x_point / 2 - lines_dist;
	int	start_y = getmaxy(stdscr) - road_h;
	int	x = 0;
	int	moving_y = start_y + offset;
	int	idx = 0;

	srand(time(NULL));
	for (int i = 0; i < 3; i++)
	{
		lines_dist = 6;
		for (int y = start_y; y < getmaxy(stdscr); y++)
		{
			for (int j = x; j < x + x_point; j++)
			{
				if (y == start_y)
					mvwprintw(stdscr, y, j + add_rand(), "_");
				else if (j == (x_point * i + x_point * (i + 1)) / 2 - lines_dist / 2)
					mvwprintw(stdscr, y, j + add_rand(), "/");
				else if (j == (x_point * i + x_point * (i + 1)) / 2 + lines_dist / 2)
					mvwprintw(stdscr, y, j + add_rand(), "\\");
				if (idx && moving_y != getmaxy(stdscr)) {
					if (y != start_y && (y == moving_y || y == moving_y + 1) && (j - x == x_point / 2 || j - x == x_point / 2 + 1))
						mvwprintw(stdscr, y, j + add_rand(), "|");
				} else if (!idx && moving_y != getmaxy(stdscr)) {
					if (y != start_y && (y == moving_y || y == moving_y - 1) && (j - x == x_point / 2 || j - x == x_point / 2 + 1))
						mvwprintw(stdscr, y, j + add_rand(), "|");
				} else {
					if (y != start_y && (y == moving_y || y == moving_y - 1) && (j - x == x_point / 2 || j - x == x_point / 2 + 1))
						mvwprintw(stdscr, y, j + add_rand(), "|");
				}
			}
			lines_dist += 2;
			idx = (idx % 2 == 0) ? idx + 1 : idx - 1;
		}
		x += x_point;
	}
	wrefresh(stdscr);
	return road_h;
}

void
triple_road_trembling(t_road_info *road_info)
{
	int	offset = 1;
	int	i = 1;

	flushinp();
	for (;;)
	{
		moon();
		int bench = triple_road_print_trembling(offset);
		if (offset == bench + 2)
			offset = 0;
		if (i % 2 == 0)
			offset++;
		i = (i % 2 == 0) ? i - 1 : i + 1;
		write_text(road_info);
		wrefresh(stdscr);
		usleep(70000);
		int c = getch();
		if (c == 32)
		{
			road_info->line++;
			c = 0;
			flushinp();
			if ((int)road_info->line == 31)
				break ;
		}
	}
}

void
stars2(t_road_info *road_info)
{
	int			coords[12][2] = { {5, 46}, {1, 18}, {1, 33}, {1, 38}, {2, 10}, {2, 17}, {2, 43}, {4, 9}, {4, 25}, {5, 51}, {5, 15}, {5, 42} };
	int			file_size[2] = { 7, 65 };
	
	proportional(coords, file_size, getmaxy(stdscr), getmaxx(stdscr));
	for (int i = 0; i < road_info->stars; i++)
		mvprintw(coords[i][0], coords[i][1], "*");
	wrefresh(stdscr);
}

void
stars2_trembling(t_road_info *road_info)
{
	int			coords[12][2] = { {5, 46}, {1, 18}, {1, 33}, {1, 38}, {2, 10}, {2, 17}, {2, 43}, {4, 9}, {4, 25}, {5, 51}, {5, 15}, {5, 42} };
	int			file_size[2] = { 7, 65 };
	
	proportional(coords, file_size, getmaxy(stdscr), getmaxx(stdscr));
	for (int i = 0; i < road_info->stars; i++)
		mvprintw(coords[i][0], coords[i][1] + add_rand(), "*");
	wrefresh(stdscr);
}

void
stars_on_black(t_road_info *road_info)
{
	int	trembling = 0;

	while (1)
	{
		clear();
		if (trembling)
			stars2_trembling(road_info);
		else
			stars2(road_info);
		mvprintw(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 -  strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
		wrefresh(stdscr);
		usleep(70000);
		int c = getch();
		if (c == 32)
		{
			road_info->line++;
			c = 0;
			flushinp();
			if ((int)road_info->line == 34)
				trembling = 1;
			if ((int)road_info->line == 35)
				break ;
		}
	}
}

void	steptwo(t_road_info *road_info)
{
	t_box_info	*box = NULL;
	WINDOW		*win = NULL;

	dark(road_info);
	triple_road(road_info);
	triple_road_trembling(road_info);
	road_info->line++;
	stars_on_black(road_info);
	clear();
	while (1) {
		mvprintw(getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
		wrefresh(stdscr);
		int c = getch();
		if (c == 32) {
			road_info->line++;
			c = 0;
			flushinp();
			break ;
		}
	}
	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0, 0, 0, 0);
	win = set_box(box, 0);
	move_road_last(box, win);
	freewin(&box, &win);
}

