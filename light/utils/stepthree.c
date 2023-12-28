#include "light.h"

int*
point()
{
	char	*text = get_text_string("17.txt");
	int		*ret = malloc(sizeof(int) * 2);
	ret[0] = getmaxy(stdscr) / 2;
	ret[1] = getmaxx(stdscr) / 2 + getmaxx(stdscr) / 4;

	flushinp();
	dynamic_writing(text, getmaxy(stdscr) / 2, getmaxx(stdscr) / 4 - strlen(text) / 2);
	mvwprintw(stdscr, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 + getmaxx(stdscr) / 4, ".");
	wrefresh(stdscr);
	usleep(400000);
	get_space(stdscr);
	return ret;
}

void
spiral(int coord[2])
{
	char	*text = get_text_string("17.txt");
	int		x = coord[1];
	int		y = coord[0];
	char	characters[2] = { '_', '|' };
	int		char_idx = 1;
	int		limit = 1;
	int		c = 0;
	int		dir = 0;

	while (1)
	{
		if ((y > -1 && y < getmaxy(stdscr)) && (x > -1 && x < getmaxx(stdscr)))
		{
			mvwprintw(stdscr, getmaxy(stdscr) / 2, getmaxx(stdscr) / 4 - strlen(text) / 2, "%s", text);
			mvwprintw(stdscr, y, x, "%c", characters[char_idx]);
			usleep(2000);
			wrefresh(stdscr);
		}
		c++;
		if (c == limit) {
				limit++;
			c = 0;
			char_idx = (char_idx) ? 0 : 1;
			dir = (dir + 1 > 3) ? 0 : dir + 1;
			if (!dir)
			{
				y++;
				c += 2;
			}
		}
		else
		{
			switch (dir) {
				case 0: y++; break ;
				case 1: x--; break ;
				case 2: y--; break ;
				case 3: x++; break ;
				default: break ;
			}
		}
		if (x == 0 && y == 0)
		{
			get_space(stdscr);
			break ;
		}
	}
	free(text);
}

void
box_in_dream(t_box_info **box, WINDOW **win, int line)
{
	char	**text = NULL;
	int		h = getmaxy(stdscr) / 3;
	int		w = getmaxx(stdscr) / 4;

	*box = init_box(*box, h, w, getmaxy(stdscr) / 2 - h / 2, getmaxx(stdscr) / 8, 1, (int)'*', (int)'*');
	*win = set_box(*box, 0);

	switch (line)
	{
		case 0: text = get_text("18.txt", *box); break ;
		case 1: text = get_text("19.txt", *box); break ;
		case 2: text = get_text("20.txt", *box); break ;
		case 3: text = get_text("21.txt", *box); break ;
		case 4: text = get_text("22.txt", *box); break ;
		default: text = NULL; break ;
	}
	if (text)
		formatted_dynamic_writing(text, *win, 0, 0);
}

void
leaving_doors(int coord[2], int limit, int l)
{
	int			x = coord[1];
	int			y = coord[0];
	char		characters[2] = { '_', '|' };
	int			char_idx = 1;
	int			c = 0;
	int			dir = 2;
	WINDOW		*win = NULL;
	t_box_info	*box = NULL;

	while (1)
	{
		if ((y > -1 && y < getmaxy(stdscr)) && (x > -1 && x < getmaxx(stdscr)))
			mvwprintw(stdscr, y, x, "%c", characters[char_idx]);
		c++;
		if (c == limit) {
			limit++;
			c = 0;
			char_idx = (char_idx) ? 0 : 1;
			dir = (dir + 1 > 3) ? 0 : dir + 1;
			if (!dir)
			{
				y++;
				c += 2;
			}
		}
		else
		{
			switch (dir) {
				case 0: y++; break ;
				case 1: x--; break ;
				case 2: y--; break ;
				case 3: x++; break ;
				default: break ;
			}
		}
		wrefresh(stdscr);
		if ((y == 0 && x == 0) || x < 0)
		{
			box_in_dream(&box, &win, l);
			get_space(stdscr);
			freewin(&box, &win);
			break ;
		}
	}
}

void
dream_one()
{
    int	*coord;
	int	limit = 4;
	int	c = 0;
	int	l = 0;

	coord = point();
	spiral(coord);
	while (1) {
		coord[0] += 2;
		coord[1] -= 1;
		leaving_doors(coord, limit, l++);
		limit += 6;
		c++;
		if (c == 5)
			break ;
	}
	free(coord);
}

void
random_dust(char *str)
{
	int	coord[2];

	coord[0] = rand() % strlen(str);
	coord[1] = rand() % strlen(str);
	str[coord[0]] = '.';
	str[coord[1]] = '.';
}

char*
get_lamp(int off, int b)
{
	char	*ret = NULL;

	switch (off)
	{
		case 0:
		{
			ret = malloc(sizeof(char) * (getmaxx(stdscr) / 6 + 1));
			memset(&ret[0], ' ', getmaxx(stdscr) / 6);
			ret[getmaxx(stdscr) / 6] = 0;
			ret[strlen(ret) / 2 - 1] = '|';
			ret[strlen(ret) / 2 + 1] = '|';
			break ;
		}
		case 1:
		{
			ret = malloc(sizeof(char) * (getmaxx(stdscr) / 6 + 1));
			memset(&ret[0], '_', getmaxx(stdscr) / 6);
			ret[0] = 32;
			ret[1] = 32;
			ret[getmaxx(stdscr) / 6] = 0;
			ret[strlen(ret) / 2 - 1] = '|';
			ret[strlen(ret) / 2 + 1] = '|';
			ret[strlen(ret) - 2] = 32;
			ret[strlen(ret) - 1] = 32;
			break ;
		}
		case 2:
		{
			ret = malloc(sizeof(char) * (getmaxx(stdscr) / 6 + 1));
			memset(&ret[0], '_', getmaxx(stdscr) / 6);
			ret[0] = 32;
			ret[1] = '/';
			ret[getmaxx(stdscr) / 6] = 0;
			ret[strlen(ret) - 2] = '\\';
			ret[strlen(ret) - 1] = 32;
			break ;
		}
		case 3:
		{
			ret = malloc(sizeof(char) * (getmaxx(stdscr) / 6 + 1));
			memset(&ret[0], '_', getmaxx(stdscr) / 6);
			ret[0] = '/';
			ret[getmaxx(stdscr) / 6] = 0;
			ret[strlen(ret) - 1] = '\\';
			break ;
		}
		default:
		{
			ret = malloc(sizeof(char) * (getmaxx(stdscr) / 6 + 1 - 4));
			memset(&ret[0], ' ', getmaxx(stdscr) / 6 - 4);
 			ret[getmaxx(stdscr) / 6 - 4] = 0;
			if (!b)
				random_dust(ret);
 			break ;
		}
	}
	return (ret);
}

void
lamp()
{
	int	off = 0;

	for (; off < getmaxy(stdscr); off++)
	{
			char *str = get_lamp(off, 0);
			if (off < 4)
				mvwprintw(stdscr, off, getmaxx(stdscr) - getmaxx(stdscr) / 3, "%s", str);
			else
				mvwprintw(stdscr, off, getmaxx(stdscr) - getmaxx(stdscr) / 3 + 2, "%s", str);
			free(str);
			if (off == 3)
				attron(A_REVERSE);
	}
	attroff(A_REVERSE);
	wrefresh(stdscr);
}

long
get_ms(struct timeval t1, struct timeval t2)
{
	long	ret;

	ret = (t2.tv_sec - t1.tv_sec) * 1000;
	ret += (t2.tv_usec - t1.tv_usec) / 1000;
	return ret;
}

int
formatted_and_lamp(char **text, WINDOW *win, int y, int x)
{
	int				time = 70000;
	struct timeval	t1, t2;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	lamp();
	for (int i = 0; text[i]; i++)
	{
		for (int j = 0; text[i][j]; j++)
		{
			if (get_ms(t1, t2) > 400)
			{
				lamp();
				gettimeofday(&t1, NULL);
				gettimeofday(&t2, NULL);
			}
			if (text[i][j] && text[i][j] == ' ')
				time = 40000;
			else
				time = 70000;
			if (text[i][j] > 0 && text[i][j] < 127)
				mvwprintw(win, y, x++, "%c", text[i][j]);
			else
				j += unicode_rep_to_win(win, &text[i][j], y, x++) - 1;
			wrefresh(win);
			usleep(time);
			gettimeofday(&t2, NULL);
		}
		x = 0;
		y++;
		wrefresh(win);
	}
	wrefresh(win);
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
	return 0;
}

void
dream_two()
{
	char			**str = NULL;
	WINDOW			*win = NULL;
	t_box_info		*box = NULL;
	int				write = 1;
	struct timeval	t1, t2;
	int				d = 0;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	box = init_box(box, getmaxy(stdscr) / 3, getmaxx(stdscr) / 3, getmaxy(stdscr) / 2 - getmaxy(stdscr) / 3 / 2, getmaxx(stdscr) / 4 - getmaxx(stdscr) / 3 / 2, 0, 0, 0);
	win = set_box(box, 0);
	str = get_text("24.txt", box);
	flushinp();
	timeout(0);
	curs_set(0);
    while (1)
    {
		if (write)
	        write = formatted_and_lamp(str, win, 0, 0);
		if (!write)
		{
			attron(A_REVERSE);
			mvwprintw(stdscr, getmaxy(stdscr) - 1, 0, "spazio");
			wrefresh(stdscr);
			attroff(A_REVERSE);
		}
		if (!write && get_ms(t1, t2) > 600)
		{
			lamp();
			gettimeofday(&t1, NULL);
			gettimeofday(&t2, NULL);
		}
		int c = getch();
		if (c == 32)
		{
			c = 0;
			d++;
			flushinp();
			write = 1;
			freewin(&box, &win);
			clear();
			if (d == 2)
				break ;
			box = init_box(box, getmaxy(stdscr) / 3, getmaxx(stdscr) / 3, getmaxy(stdscr) / 2 - getmaxy(stdscr) / 3 / 2, getmaxx(stdscr) / 4 - getmaxx(stdscr) / 3 / 2, 0, 0, 0);
			win = set_box(box, 0);
			str = get_text("25.txt", box);
		}
		gettimeofday(&t2, NULL);
    }
	curs_set(3);
}

void
fill_boxes(t_box_info **box, WINDOW **win)
{
	char	**str = NULL;
	int		order[12] = { 0, 6, 12, 3, 5, 1, 7, 10, 14, 9, 2 };
	int		idx = 0;
	char	*files[12] = { "26.txt", "27.txt", "28.txt", "29.txt", "30.txt", "31.txt", "32.txt", "32b.txt", "33.txt", "34.txt", "34b.txt", NULL };
	int		f_idx = 0;
	int		written = 0;

	str = get_text(files[f_idx++], box[0]);
	while (1)
	{
		if (!written)
			written = formatted_dynamic_writing(str, win[order[idx]], 0, 0);
		sleep(1);
		if (files[f_idx] == NULL)
			break ;
		str = get_text(files[f_idx++], box[0]);
		idx++;
		written = 0;
	}
	clear();
}

void
dream_three()
{
	t_box_info	*box[16];
	WINDOW		*win[16];
	int			x = 4;
	int			y = 3;

	flushinp();
	for (int i = 0; i < 16; i++)
	{
		box[i] = NULL;
		win[i] = NULL;
		box[i] = init_box(box[i], getmaxy(stdscr) / 4 - 5, getmaxx(stdscr) / 4 - 5, y, x, 1, (int)'|', (int)'-');
		win[i] = set_box(box[i], 0);
		x += getmaxx(stdscr) / 4;
		if (x >= getmaxx(stdscr))
		{
			x = 4;
			y += getmaxy(stdscr) / 4; 
			for (int j = 0; j < getmaxx(stdscr); j++)
				mvwprintw(stdscr, y - 4, j, "=");
		}
	}
	wrefresh(stdscr);
	fill_boxes(&box[0], &win[0]);
	for (int i = 0; i < 16; i++)
		freewin(&box[i], &win[i]);
}

void
broken_lamp(int	b)
{
	int	off = 0;

	for (; off < getmaxy(stdscr); off++)
	{
			char *str = get_lamp(off, b);
			if (off < 4)
				mvwprintw(stdscr, off, getmaxx(stdscr) - getmaxx(stdscr) / 3, "%s", str);
			else
				mvwprintw(stdscr, off, getmaxx(stdscr) - getmaxx(stdscr) / 3 + 2, "%s", str);
			free(str);
			if (off == 3 && !b)
				attron(A_REVERSE);
	}
	attroff(A_REVERSE);
	wrefresh(stdscr);
}

int
formatted_and_broken_lamp(char **text, WINDOW *win, int y, int x)
{
	int				time = 70000;
	struct timeval	t1, t2;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	broken_lamp((int)rand() % 2);
	for (int i = 0; text[i]; i++)
	{
		for (int j = 0; text[i][j]; j++)
		{
			if (get_ms(t1, t2) > 400)
			{
				broken_lamp((int)rand() % 2);
				gettimeofday(&t1, NULL);
				gettimeofday(&t2, NULL);
			}
			if (text[i][j] && text[i][j] == ' ')
				time = 40000;
			else
				time = 70000;
			if (text[i][j] > 0 && text[i][j] < 127)
				mvwprintw(win, y, x++, "%c", text[i][j]);
			else
				j += unicode_rep_to_win(win, &text[i][j], y, x++) - 1;
			wrefresh(win);
			usleep(time);
			gettimeofday(&t2, NULL);
		}
		x = 0;
		y++;
		wrefresh(win);
	}
	wrefresh(win);
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
	return 0;
}

void
dream_four()
{
	t_box_info		*box = NULL;
	WINDOW			*win = NULL;
	char			**str = NULL;
	int				write = 1;
	struct timeval	t1, t2;
	int				d = 0;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	box = init_box(box, getmaxy(stdscr) / 3, getmaxx(stdscr) / 3, getmaxy(stdscr) / 2 - getmaxy(stdscr) / 3 / 2, getmaxx(stdscr) / 4 - getmaxx(stdscr) / 3 / 2, 0, 0, 0);
	win = set_box(box, 0);
	str = get_text("35.txt", box);
	timeout(0);
	curs_set(0);
	while (1)
	{
		if (write)
			write = formatted_and_broken_lamp(str, win, 0, 0);
		if (!write && get_ms(t1, t2) > 600)
		{
			broken_lamp(rand() % 2);
			gettimeofday(&t1, NULL);
			gettimeofday(&t2, NULL);
		}
		int c = getch();
		if (c == 32)
		{
			c = 0;
			flushinp();
			freewin(&box, &win);
			clear();
			if (d == 1)
				break ;
			box = init_box(box, getmaxy(stdscr) / 3, getmaxx(stdscr) / 3, getmaxy(stdscr) / 2 - getmaxy(stdscr) / 3 / 2, getmaxx(stdscr) / 4 - getmaxx(stdscr) / 3 / 2, 0, 0, 0);
			win = set_box(box, 0);
			str = get_text("36.txt", box);
			write = 1;
			d++;
		}
		gettimeofday(&t2, NULL);
	}
	curs_set(3);
}

void
fill_boxes_instantly(t_box_info **box, WINDOW **win)
{
	char	**str = NULL;
	int		order[12] = { 0, 6, 12, 3, 5, 1, 7, 10, 14, 9, 2 };
	int		idx = 0;
	char	*files[12] = { "26.txt", "27.txt", "28.txt", "29.txt", "30.txt", "31.txt", "32.txt", "32b.txt", "33.txt", "34.txt", "34b.txt", NULL };
	int		f_idx = 0;

	str = get_text(files[f_idx++], box[0]);
	while (1)
	{
		write_to_box(win[order[idx]], 0, 0, str);
		flushinp();
		if (files[f_idx] == NULL)
			break ;
		str = get_text(files[f_idx++], box[0]);
		idx++;
	}
	clear();
}

int
get_box_y(int idx)
{
	switch (idx / 4)
	{
		case 0: return 3;
		case 1: return 3 + getmaxy(stdscr) / 4;
		case 2: return 3 + getmaxy(stdscr) / 4 * 2;
		default: return 3 + getmaxy(stdscr) / 4 * 3;
	}
}

int
get_box_x(int idx)
{
	switch (idx % 4)
	{
		case 0: return 4;
		case 1: return 4 + getmaxx(stdscr) / 4;
		case 2: return 4 + getmaxx(stdscr) / 4 * 2;
		default: return 4 + getmaxx(stdscr) / 4 * 3;
	}
}

void
fill_lightning_boxes(t_box_info **box, WINDOW **win)
{
	char	**str = get_text("38.txt", box[0]);
	int		order[9] = { 4, 11, 9, 13, 2, 8, 15, 1, 7 };
	int		idx = 0;
	char	*files[9] = { "38b.txt", "39.txt", "39b.txt", "39c.txt", "40.txt", "41.txt", "41b.txt", "41c.txt", NULL };
	int		f_idx = 0;
	int		written = 0;

	while (1)
	{
		if (!written)
		{
			wbkgdset(box[order[idx]]->border_window, COLOR_PAIR(2));
			wbkgdset(win[order[idx]], COLOR_PAIR(2));
			for (int i = 0; i <= box[order[idx]]->border_window->_maxy; i++)
			{
				for (int j = 0; j <= box[order[idx]]->border_window->_maxx; j++)
					mvwprintw(box[order[idx]]->border_window, i, j, " ");
			}
			for (int i = 0; i <= win[order[idx]]->_maxy; i++)
			{
				for (int j = 0; j <= win[order[idx]]->_maxx; j++)
					mvwprintw(win[order[idx]], i, j, " ");
			}
			wrefresh(box[order[idx]]->border_window);
			wrefresh(win[order[idx]]);
			written = formatted_dynamic_writing(str, win[order[idx]], 0, 0);
		}
		sleep(1);
		if (files[f_idx] == NULL)
			break ;
		str = get_text(files[f_idx++], box[0]);
		idx++;
		written = 0;
	}
}

void
dream_five()
{
	char	*str = get_text_string("37.txt");

	dynamic_writing(str, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - strlen(str) / 2);
	get_space(stdscr);

	t_box_info	*box[16];
	WINDOW		*win[16];
	int			x = 4;
	int			y = 3;

	flushinp();

	for (int i = 0; i < 16; i++)
	{
		box[i] = NULL;
		win[i] = NULL;
		box[i] = init_box(box[i], getmaxy(stdscr) / 4 - 5, getmaxx(stdscr) / 4 - 5, y, x, 1, (int)'|', (int)'-');
		win[i] = set_box(box[i], 0);
		x += getmaxx(stdscr) / 4;
		if (x >= getmaxx(stdscr))
		{
			x = 4;
			y += getmaxy(stdscr) / 4; 
			for (int j = 0; j < getmaxx(stdscr); j++)
				mvwprintw(stdscr, y - 4, j, "=");
		}
		wrefresh(stdscr);
	}
	fill_boxes_instantly(&box[0], &win[0]);
	fill_lightning_boxes(&box[0], &win[0]);
	for (int i = 0; i < 16; i++)
		freewin(&box[i], &win[i]);
	clear();
	str = get_text_string("42.txt");
	dynamic_writing(str, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2 - strlen(str) / 2);
	get_space(stdscr);
}

int
is_in(int lines[10], int n)
{
	for (int i = 0; i < 10; i++)
	{
		if (lines[i] == 0)
			return 0;
		if (lines[i] == n)
			return 1;
	}
	return 0;
}

int	get_random_line(int lines[10])
{
	int	rand_n = rand() % 10 + 1;

	while (is_in(lines, rand_n))
	{
		rand_n = rand() % 10 + 1;
	}
	int i = 0;
	for (; i < 10; i++)
	{
		if (lines[i] == 0)
		{
			lines[i] = rand_n;
			break ;
		}
	}
	return rand_n;
}

void
disorder()
{
	char	*str = get_text_string("43.txt");
	char	*files[8] = { "44.txt", "45.txt", "46.txt", "46b.txt", "47.txt", "47b.txt", "47c.txt", NULL };
	int		idx = 0;
	int		lines[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int		y = 0;

	dynamic_writing(str, getmaxy(stdscr) / 3, getmaxx(stdscr) / 2 - strlen(str) / 2);
	while (1)
	{
		if (!files[idx])
			break ;
		str = get_text_string(files[idx++]);
		y = get_random_line(lines);
		dynamic_writing(str, getmaxy(stdscr) / 2 + y, getmaxx(stdscr) / 2 - strlen(str) / 2);
		sleep(1);
	}
	clear();
}

typedef struct	s_fall_coord
{
	int	y;
	int	x;
}		t_fall_coord;

int
get_higher_box(t_fall_coord coord[100])
{
	int	ret = getmaxy(stdscr);

	for (int i = 0; i < 100; i++)
	{
		if (coord[i].y != 0 && coord[i].y < ret)
			ret = coord[i].y;
	}
	return ret;
}

void
add_box(t_fall_coord coord[100])
{
	for (int i = 0; i < 100; i++) {
		if (coord[i].x == 0 && coord[i].y == 0) {
			coord[i].y = 1;
			return ;
		}
	}
}

void
falling_boxes(t_fall_coord coord[100], t_box_info *box, WINDOW *win)
{
	wclear(win);
	wattron(win, A_REVERSE);
	for (int i = 0; i < 100; i++) {
		if (coord[i].x != 0 || coord[i].y != 0)
		{
			if (coord[i].x == 0)
				coord[i].x = rand() % box->width;
			mvwprintw(win, coord[i].y, coord[i].x, " ");
			coord[i].y++;
		}
		if (coord[i].y == getmaxy(stdscr))
		{
			coord[i].x = 0;
			coord[i].y = 0;
		}
	}
	if (get_higher_box(coord) > 3)
		add_box(coord);
	wattroff(win, A_REVERSE);
}

int
formatted_and_falling_boxes(char **text, WINDOW *win, int y, int x)
{
	int				time = 70000;
	struct timeval	t1, t2;
	t_fall_coord	coord[100];
	t_box_info		*fall_box = NULL;
	WINDOW			*fall_win = NULL;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	fall_box = init_box(fall_box, getmaxy(stdscr), getmaxx(stdscr) / 5, 0, getmaxx(stdscr) - getmaxx(stdscr) / 4, 0, 0, 0);
	fall_win = set_box(fall_box, 0);
	for (int i = 0; i < 100; i++) {
		coord[i].x = 0;
		coord[i].y = 0;
	}
	for (int i = 0; text[i]; i++)
	{
		for (int j = 0; text[i][j]; j++)
		{
			if (get_ms(t1, t2) > 100)
			{
				falling_boxes(coord, fall_box, fall_win);
				wrefresh(fall_win);
				gettimeofday(&t1, NULL);
				gettimeofday(&t2, NULL);
			}
			if (text[i][j] && text[i][j] == ' ')
				time = 40000;
			else
				time = 70000;
			if (text[i][j] > 0 && text[i][j] < 127)
				mvwprintw(win, y, x++, "%c", text[i][j]);
			else
				j += unicode_rep_to_win(win, &text[i][j], y, x++) - 1;
			wrefresh(win);
			usleep(time);
			gettimeofday(&t2, NULL);
		}
		x = 0;
		y++;
		wrefresh(win);
	}
	wrefresh(win);
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
	timeout(0);
	flushinp();
	int	c = getch();
	while (c != 32)
	{
		if (get_ms(t1, t2) > 100)
		{
			falling_boxes(coord, fall_box, fall_win);
			wrefresh(fall_win);
			gettimeofday(&t1, NULL);
			gettimeofday(&t2, NULL);
		}
		attron(A_REVERSE);
		mvwprintw(stdscr, getmaxy(stdscr) - 1, 0, "spazio");
		attroff(A_REVERSE);
		wrefresh(stdscr);
		usleep(time);
		gettimeofday(&t2, NULL);
		c = getch();
	}
	freewin(&fall_box, &fall_win);
	clear();
	return 0;
}

void
pile()
{
	t_box_info	*pile_box = NULL;
	WINDOW		*pile_win = NULL;
	int			span = 1;
	int			x = 0;

	pile_box = init_box(pile_box, 20, 40, getmaxy(stdscr) - getmaxy(stdscr) / 4, getmaxx(stdscr) / 2 - 20, 0, 0, 0);
	pile_win = set_box(pile_box, 0);
	wattron(pile_win, A_REVERSE);
	for (int i = 0; i < pile_box->height; i++) {
		x = pile_box->width / 2 - span / 2;
		for (int j = 0; j < span; j++) {
			mvwprintw(pile_win, i, x++, " ");
		}
		span += 2;
	}
	wattroff(pile_win, A_REVERSE);
	wrefresh(pile_win);

	char	*str = get_text_string("48a.txt");

	dynamic_writing(str, 10, getmaxx(stdscr) / 2 - strlen(str) / 2);
	get_space(stdscr);
	clear();
}

void
insert_stair()
{
	int	w = getmaxx(stdscr) / 5;
	int	offset = getmaxy(stdscr);
	int	window_height = getmaxy(stdscr) / 3 + 4;
	int	pegs[3] = { getmaxy(stdscr) + 4, getmaxy(stdscr) + 9, getmaxy(stdscr) + 14 };

	for (; offset > getmaxy(stdscr) - window_height; offset--) {
		for (int i = 0; i < window_height; i++) {
			for (int j = 0; j < w; j++) {
				if (getmaxy(stdscr) - window_height + i > offset) {
					switch (j) {
						case 0:
						case 1:	mvwprintw(stdscr, getmaxy(stdscr) - window_height + i, getmaxx(stdscr) / 2 - w / 2 + j, "|"); break ;
						default: {
							if (j == w - 1 || j == w - 2)
								mvwprintw(stdscr, getmaxy(stdscr) - window_height + i, getmaxx(stdscr) / 2 - w / 2 + j, "|");
							else if (getmaxy(stdscr) - window_height + i == pegs[0] || getmaxy(stdscr) - window_height + i == pegs[1] || getmaxy(stdscr) - window_height + i == pegs[2])
								mvwprintw(stdscr, getmaxy(stdscr) - window_height + i, getmaxx(stdscr) / 2 - w / 2 + j, "=");
							else
								mvwprintw(stdscr, getmaxy(stdscr) - window_height + i, getmaxx(stdscr) / 2 - w / 2 + j, " ");
							break ;
						}
					}
				}
			}
			wrefresh(stdscr);

		}
		pegs[0]--;
		pegs[1]--;
		pegs[2]--;
		usleep(200000);
	}
}

void
stair(t_fall_coord coord[20])
{
	t_box_info	*fake_box = NULL;
	t_box_info	*box2 = NULL;
	t_box_info	*box = NULL;
	WINDOW		*win = NULL;
	WINDOW		*win2 = NULL;

	fake_box = init_box(fake_box, 10, getmaxx(stdscr) / 4, 0, 0, 0, 0, 0);
	char	**str = get_text("48f.txt", fake_box);

	box2 = init_box(box2, getmaxy(stdscr) / 2 + 2, getmaxx(stdscr) / 2 + 4, getmaxy(stdscr) / 4 - 1, getmaxx(stdscr) / 4 - 2, 1, 0, 0);
	win2 = set_box(box2, 0);
	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, getmaxy(stdscr) / 4, getmaxx(stdscr) / 4, 0, 0, 0);
	win = set_box(box, 0);
	for (int i = 0; i < 7; i++)
		mvwprintw(win, coord[i].y, coord[i].x, "*");
	wrefresh(win2);
	wrefresh(win);
	for (int i = 0; i < 7; i++)
		mvwprintw(win, coord[i].y, coord[i].x, "*");
	wrefresh(win);
	wrefresh(win2);
	insert_stair();
	freewin(&box, &win);
	freewin(&box2, &win2);
	freewin(&fake_box, NULL);
	dynamic_writing(str[0], getmaxy(stdscr) - 3, getmaxx(stdscr) - getmaxx(stdscr) / 3);
	get_space(stdscr);
}

void
star_painting()
{
	t_box_info		*box = NULL;
	WINDOW			*win = NULL;
	t_box_info		*box2 = NULL;
	WINDOW			*win2 = NULL;
	t_fall_coord	coord[20];

	curs_set(0);
	box2 = init_box(box2, getmaxy(stdscr) / 2 + 2, getmaxx(stdscr) / 2 + 4, getmaxy(stdscr) / 4 - 1, getmaxx(stdscr) / 4 - 2, 1, 0, 0);
	win2 = set_box(box2, 0);
	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, getmaxy(stdscr) / 4, getmaxx(stdscr) / 4, 1, 0, 0);
	win = set_box(box, 0);
	coord[0].x = box->width / 4;
	coord[0].y = 2;
	coord[1].x = box->width - box->width / 4;
	coord[1].y = 4;
	coord[2].x = box->width / 2 + 2;
	coord[2].y = box->height / 2 - 1;
	coord[3].x = box->width / 4 - 5;
	coord[3].y = box->height - box->height / 4;
	coord[4].x = box->width - 3;
	coord[4].y = box->height - 2;
	coord[5].x = 1;
	coord[5].y = 4;
	coord[6].x = box->width / 2 - 1;
	coord[6].y = box->height - 1;
	for (int i = 0; i < 7; i++)
		mvwprintw(win, coord[i].y, coord[i].x, "*");
	wrefresh(win);

	t_box_info	*fake_box = NULL;
	fake_box = init_box(fake_box, 10, getmaxx(stdscr) - 5, 0, 0, 0, 0, 0);
	char	**str = get_text("48b.txt", fake_box);

	formatted_dynamic_writing(str, stdscr, getmaxy(stdscr) - 4, getmaxx(stdscr) / 2 - strlen(str[0]) / 2);
	str = get_text("48c.txt", fake_box);
	formatted_dynamic_writing(str, stdscr, getmaxy(stdscr) - 2, getmaxx(stdscr) / 2 - strlen(str[0]) / 2);
	get_space(stdscr);
	wclear(stdscr);
	freewin(&box2, &win2);
	box2 = init_box(box2, getmaxy(stdscr) / 2 + 2, getmaxx(stdscr) / 2 + 4, getmaxy(stdscr) / 4 - 1, getmaxx(stdscr) / 4 - 2, 1, 0, 0);
	win2 = set_box(box2, 0);
	freewin(&box, &win);
	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, getmaxy(stdscr) / 4, getmaxx(stdscr) / 4, 0, 0, 0);
	win = set_box(box, 0);
	for (int i = 0; i < 7; i++)
		mvwprintw(win, coord[i].y, coord[i].x, "*");
	wrefresh(win2);
	wrefresh(win);
	str = get_text("48d.txt", fake_box);
	formatted_dynamic_writing(str, stdscr, getmaxy(stdscr) - 6, getmaxx(stdscr) / 2 - strlen(str[0]) / 2);
	str = get_text("48e.txt", fake_box);
	formatted_dynamic_writing(str, stdscr, getmaxy(stdscr) - 5, getmaxx(stdscr) / 2 - strlen(str[0]) / 2);
	get_space(stdscr);
	stair(coord);
	freewin(&box, &win);
	freewin(&box2, &win2);
	freewin(&fake_box, NULL);
	curs_set(1);
}

int*
create_pegs(int height)
{
	int	*ret = malloc(sizeof(height));
	if (!ret)
		die("Malloc error\n");
	ret[0] = -9;
	int	f = -9;

	for (int i = 1; i < height; i++) {
		f -= 9;
		ret[i] = f;
	}
	return ret;
}

int*
create_lines(int *pegs, int height)
{
	int	idx = 1;
	int	*ret = malloc(sizeof(int) * height);
	if (!ret)
		die("Malloc error\n");

	for (int i = 0; i < height; i++) {
		ret[i] = pegs[idx] - 1;
		idx += 2;
	}
	return ret;
}

int
is_peg(int i, int *pegs, int height)
{
	for (int j = 0; j < height; j++) {
		if (pegs[j] == i)
			return 1;
	}
	return 0;
}

void
increment_pegs(int *pegs, int height)
{
	for (int i = 0; i < height; i++) {
		pegs[i] += 1;
	}
}

void
increment_lines(int *lines, int height)
{
	for (int i = 0; i < height; i++) {
		lines[i] += 1;
	}
}

typedef struct	s_star
{
	int	x;
	int	y;
}				t_star;

void
add_star(t_star stars[200])
{
	int	i = 0;

	for (; stars[i].x != -1 && stars[i].y != -1 && i != 200; i++) {
	}
	if (i == 200)
		return ;
	stars[i].x = rand() % getmaxx(stdscr);
	stars[i].y = rand() % getmaxy(stdscr);
}

void
paint_stars(t_star stars[200])
{
	for (int i = 0; i < 200; i++) {
		mvwprintw(stdscr, stars[i].y, stars[i].x, "*");
	}
	wrefresh(stdscr);
}

void
add_written(int *written)
{
	for (int i = 0; written[i] != -2; i++) {
		if (written[i] == -1) {
			written[i] = 1;
			return ;
		}
	}
}

int*
get_visible_lines(int *lines, int *written, int height)
{
	int	*ret = malloc(sizeof(int) * 3);
	int	j = 0;

	ret[0] = -1;
	ret[1] = -1;
	ret[2] = -1;
	
	for (int i = 0; i < height; i++) {
		if (lines[i] >= 0 && lines[i] < getmaxy(stdscr)) {
			ret[j++] = lines[i];
			if (lines[i] == 0)
				(*written) += 1;
		}
	}
	return ret;
}

int
write_lines(int lines[50], char **text, int *written, WINDOW *win, int text_height)
{
	int	*visible = get_visible_lines(lines, written, text_height);


	if (visible[0] != -1 && visible[1] != -1 && visible[2] != -1 && *written < text_height) {
		mvwprintw(win, visible[2], win->_maxx / 2 - strlen(text[*written]) / 2, "%s", text[*written]);
		mvwprintw(win, visible[1], win->_maxx / 2 - strlen(text[*written - 1]) / 2, "%s", text[*written - 1]);
		mvwprintw(win, visible[0], win->_maxx / 2 - strlen(text[*written - 2]) / 2, "%s", text[*written - 2]);
	}
	else if (visible[0] != -1 && visible[1] != -1 && *written < text_height) {
		mvwprintw(win, visible[1], win->_maxx / 2 - strlen(text[*written]) / 2, "%s", text[*written]);
		mvwprintw(win, visible[0], win->_maxx / 2 - strlen(text[*written - 1]) / 2, "%s", text[*written - 1]);
	}
	else if (visible[0] != -1 && *written < text_height) {
		mvwprintw(win, visible[0], win->_maxx / 2 - strlen(text[*written]) / 2, "%s", text[*written]);
	}
	if (*written > 0 && visible[0] == -1 && visible[1] == -1 && visible[2] == -1)
	{
		free(visible);
		return -1;
	}
	free(visible);
	return 0;
}

void
clean_text(char **old_text)
{
	for (int i = 0; old_text[i]; i++) {
		if (old_text[i][strlen(old_text[i]) - 1] == ' ')
		{
			int	j = strlen(old_text[i]) - 1;
			for (; old_text[i][j] == ' '; j--) {
			}
			old_text[i][j + 1] = '\0';
		}
	}
}

t_star*
moving_stair()
{
	t_box_info		*box = NULL;
	WINDOW			*win = NULL;
	int				offset = 0;
	int				*pegs;
	int				*lines;
	struct timeval	t1;
	struct timeval	t2;
	t_star			*stars;
	char			**text = NULL;
	int				text_height = 0;
	int				written = -1;
	t_box_info		*fake_box = NULL;


	curs_set(0);
	stars = malloc(sizeof(t_star) * 200);
	box = init_box(box, getmaxy(stdscr), getmaxx(stdscr) / 3, 0, getmaxx(stdscr) / 3, 0, 0, 0);
	win = set_box(box, 0);
	fake_box = init_box(fake_box, getmaxy(stdscr), getmaxx(stdscr) / 3 - 4, 0, getmaxx(stdscr) / 3, 0, 0, 0);
	text = get_text("49.txt", fake_box);
	clean_text(text);
	for (int i = 0; text[i]; i++)
		text_height++;
	freewin(&fake_box, NULL);
	pegs = create_pegs(text_height * 2);
	lines = create_lines(pegs, text_height);
	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	for (int i = 0; i < 200; i++) {
		stars[i].x = -1;
		stars[i].y = -1;
	}

	while (1) {
		if (get_ms(t1, t2) > 800)
			add_star(stars);
		paint_stars(stars);
		for (int i = 0; i < box->height; i++) {
			if (i < offset) {
				for (int j = 0; j < box->width; j++) {
					switch (j) {
						case 0:
						case 1:	mvwprintw(win, i, j, "|"); break ;
						default: {
							if (j == box->width - 1 || j == box->width - 2)
								mvwprintw(win, i, j, "|");
							else if (is_peg(i, pegs, text_height * 2))
								mvwprintw(win, i, j, "=");
							else
								mvwprintw(win, i, j, " ");
							break ;
						}
					}
				}
			}
			wrefresh(win);
		}
		if (write_lines(lines, text, &written, win, text_height) == -1)
		{
			wrefresh(win);
			break ;
		};
		wrefresh(win);
		increment_pegs(pegs, text_height * 2);
		increment_lines(lines, text_height);
		usleep(200000);
		if (offset < box->height + 1)
			offset++;
		gettimeofday(&t2, NULL);
	}
	offset = 0;
	for (; offset < getmaxy(stdscr); offset++) {
		clear();
		paint_stars(stars);
		for (int i = 0; i < box->height; i++) {
			if (i > offset) {
				for (int j = 0; j < box->width; j++) {
					switch (j) {
						case 0:
						case 1:	mvwprintw(win, i, j, "|"); break ;
						default: {
							if (j == box->width - 1 || j == box->width - 2)
								mvwprintw(win, i, j, "|");
							break ;
						}
					}
				}
			}
		}
		wrefresh(win);
		usleep(200000);
	}
	for (int i = 0; text[i]; i++) {
		free(text[i]);
	}
	free(text);
	freewin(&box, &win);
	free(pegs);
	free(lines);
	curs_set(1);
	return stars;
}

void
sitting(t_star *stars)
{
	struct timeval	t1;
	struct timeval	t2;
	t_box_info		*fake_box = NULL;
	char			**text = NULL;
	int				idx = 0;
	int				y = getmaxy(stdscr) / 2;
	int				x = getmaxx(stdscr) - getmaxx(stdscr) / 3;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);
	fake_box = init_box(fake_box, getmaxy(stdscr) / 2, getmaxx(stdscr) / 3, getmaxy(stdscr) / 2, getmaxx(stdscr) - getmaxx(stdscr) / 3, 0, 0, 0);
	text = get_text("50.txt", fake_box);

	paint_stars(stars);
	while (text[idx])
	{
		dynamic_writing(text[idx], y++, x);
		idx++;
	}
	get_space(stdscr);
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
	freewin(&fake_box, NULL);
}

void
ending()
{
	t_star	*stars = moving_stair();
	sitting(stars);
	free(stars);
	clear();
	curs_set(0);
	mvwprintw(stdscr, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, "*");
	wrefresh(stdscr);
	sleep(2);
	mvwprintw(stdscr, getmaxy(stdscr) / 2 + 2, getmaxx(stdscr) / 2 - strlen("Here I am") / 2, "Here I am.");
	wrefresh(stdscr);
	sleep(2);
	get_space(stdscr);
	curs_set(1);
}

void
dream_six()
{
	t_box_info	*box = NULL;
	WINDOW		*win = NULL;
	char		**str = NULL;

	disorder();
	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr) / 2, getmaxy(stdscr) / 3, getmaxx(stdscr) / 8, 0, 0, 0);
	win = set_box(box, 0);
	str = get_text("48.txt", box);
	formatted_and_falling_boxes(str, win, 0, 0);
	pile();
	star_painting();
	ending();
}