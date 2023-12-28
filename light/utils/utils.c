#include "light.h"

void		die(char *str)
{
	endwin();
	write(2, str, strlen(str));
	exit(1);
}

int			file_length(const char *path)
{
	int		ret = 0;
	int		fd;
	char	n;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		die("Error opening file\n");
	while (read(fd, &n, 1))
		ret++;
	close(fd);
	return ret;
}

void		start_ncurses()
{
	const char	*str = "LOG:\n\nI'm here since the start. You had to walk a long road to read me.\n";

	write(1, str, strlen(str));
	setlocale(LC_ALL, "it_IT.UTF-8");
	initscr();
	if (!has_colors())
		die("This terminal does not support colors.\n");
	start_color();
	init_color(COLOR_WHITE, 999, 999, 999);
	init_color(COLOR_BLACK, 0, 0, 0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	wbkgd(stdscr, COLOR_PAIR(1));
	noecho();
}

void		create_border_window(t_box_info *box, int COLOR)
{
	box->border_window = newwin(box->height + 3, box->width + 4, box->y - 2, box->x - 2);
	refresh();
	if (COLOR)
		wbkgdset(box->border_window, COLOR_PAIR(COLOR));
	box(box->border_window, box->bh, box->bw);
	wrefresh(box->border_window);
}

WINDOW		*set_box(t_box_info *box, int COLOR)
{
	WINDOW	*ret;

	if (box->box)
		create_border_window(box, COLOR);
	ret = newwin(box->height, box->width, box->y, box->x);
	if (COLOR)
		wbkgdset(ret, COLOR_PAIR(COLOR));
	wrefresh(stdscr);
	return (ret);
}

void		write_to_box(WINDOW *win,int y, int x, char **str)
{
	for (int i = 0; str[i]; i++)
		mvwprintw(win, y++, x, "%s", str[i]);
	wrefresh(win);
	for (int i = 0; str[i]; i++)
		free(str[i]);
	free(str);
}

int			is_unicode(char *str)
{
	return ((mblen(str, MB_CUR_MAX) == 1) ? 0 : 1);
}

int			next_word_length(char *str)
{
	int	ret = 0;
	int	i = 0;
	int	uni = 0;
	
	if (!str)
		return (0);
	while (str[i] && str[i] != ' ' && str[i] != '\n')
	{
		ret++;
		if (is_unicode(&str[i++]))
			uni++;
	}
	return (ret - uni / 2);
}

char		**format_string(char *str, t_box_info *box)
{
	char	**ret;
	int		i = 0;
	int		j;
	int		z;
	int		c = 0;

	ret = malloc(sizeof(char*) * (box->height + 1));
	if (!ret)
		die("Malloc error\n");
	for (; i < box->height; i++)
	{
		ret[i] = malloc(sizeof(char) * (box->width + 1));
		if (!ret[i])
			die("Malloc error\n");
	}
	ret[i] = NULL;
	i = 0;
	j = 0;
	z = 0;
	while (str[z])
	{
		if (str[z] == ' ' && j == 0)
			z++;
		else if (str[z] != ' ' && str[z] != '\n' && next_word_length(&str[z]) < box->width - j)
		{
			for (int x = next_word_length(&str[z]); x; x--)
			{
				ret[i][j++] = str[z++];
				c++;
				if (!str[z])
				{
					ret[i][j] = 0;
					i++;
				}
			}
		}
		else if (str[z] != ' ' && str[z] != '\n' && next_word_length(&str[z]) >= box->width - j)
		{
			ret[i][j] = '\0';
			j = 0;
			i++;
			c = 0;
		}
		else if (str[z] == '\n')
		{
			for (int x = box->width - j; x; x--)
			{
				ret[i][j++] = ' ';
				c++;
			}
			z++;
		}
		else
		{
			ret[i][j++] = str[z++];
			c++;
		}
		if (c >= box->width)
		{
			ret[i][j] = '\0';
			j = 0;
			i++;
			c = 0;
		}
	}
	ret[i] = NULL;
	return (ret);
}

char		**get_text(char *filename, t_box_info *box)
{
	FILE	*stream;
	char	*tmp;
	char	**ret;
	char	*path;

	path = malloc(sizeof(char) * (strlen("text/") + strlen(filename) + 1));
	if (!path)
		die("Malloc error\n");
	strncpy(path, "text/", 6);
	strcat(path, filename);
	stream = fopen(path, "r");
	tmp = malloc(sizeof(char) * (file_length(path) + 1));
	if (!tmp)
		die("Malloc error\n");
	fread(tmp, 1, file_length(path), stream);
	tmp[file_length(path)] = '\0';
	free(path);
	fclose(stream);
	ret = format_string(tmp, box);
	return (ret);
}

char		*get_text_string(char *filename)
{
	FILE	*stream;
	char	*ret;
	char	*path;

	path = malloc(sizeof(char) * (strlen("text/") + strlen(filename) + 1));
	if (!path)
		die("Malloc error\n");
	strncpy(path, "text/", 6);
	strcat(path, filename);
	stream = fopen(path, "r");
	ret = malloc(sizeof(char) * (file_length(path) + 1));
	if (!ret)
		die("Malloc error\n");
	fread(ret, 1, file_length(path), stream);
	ret[file_length(path)] = '\0';
	free(path);
	fclose(stream);
	return (ret);
}

int			*search_test(char *str, char **matrix, int i, int j, int width)
{
	int	z = 0;
	int	*ret;
	int	saved[2] = { i, j };

	while (str[z] && str[z] == matrix[i][j])
	{
		z++;
		j++;
		if (j == width)
		{
			j = 0;
			i++;
			if (!matrix[i])
				return (NULL);
		}
	}
	if (!str[z])
	{
		ret = malloc(sizeof(int) * 2);
		if (!ret)
			die("Malloc error\n");
		ret[0] = saved[0];
		ret[1] = saved[1];
		return (ret);
	}
	return (NULL);
}

int			*search_in_matrix(char *str, char **matrix)
{
	int	*ret;

	for (int i = 0; matrix[i]; i++)
	{
		for (int j = 0; matrix[i][j]; j++)
		{
			if (matrix[i][j] == str[0])
			{
				ret = search_test(str, matrix, i, j, strlen(matrix[0]));
				if (ret)
					return (ret);
			}
		}
	}
	return (NULL);
}

t_box_info	*init_box(t_box_info *box, int height, int width, int y, int x, int box_bool, int bh, int bw)
{
	t_box_info	*ret;

	if (box)
	{
		if (box->border_window)
			delwin(box->border_window);
		free(box);
	}
	ret = malloc(sizeof(t_box_info));
	if (!ret)
		die("Malloc error\n");

	ret->height = height;
	ret->width = width;
	ret->y = y;
	ret->x = x;
	ret->box = box_bool;
	ret->bh = bh;
	ret->bw = bw;
	ret->border_window = NULL;

	return (ret);
}

void		check()
{
	int	maxX;
	int	maxY;

	getmaxyx(stdscr, maxY, maxX);
	if (maxY < 37 || maxX < 130)
	{
		if (maxY < 37 && maxX < 130)
		{
			curs_set(0);
			clear();
			const char *str = "make this window taller and wider, pls\n";
			mvwprintw(stdscr, maxY / 2, maxX / 2 - strlen(str) / 2, "%s", str);
			mvwprintw(stdscr, maxY / 2 + 1, maxX / 2 - strlen("recommended measures { 130 columns, 37 rows };"), "recommended measures { 130 columns, 37 rows };");
			wrefresh(stdscr);
			sleep(2);
			endwin();
			die((char*)str);
		}
		else if (maxY < 37)
		{
			curs_set(0);
			clear();
			const char *str = "make this window taller, pls\n";
			mvwprintw(stdscr, maxY / 2, maxX / 2 - strlen(str) / 2, "%s", str);
			mvwprintw(stdscr, maxY / 2 + 1, maxX / 2 - strlen("recommended measures { 130 columns, 37 rows };"), "recommended measures { 130 columns, 37 rows };");
			wrefresh(stdscr);
			sleep(2);
			endwin();
			die((char*)str);
		}
		else if (maxX < 130)
		{
			curs_set(0);
			clear();
			const char *str = "make this window wider, pls\n";
			mvwprintw(stdscr, maxY / 2, maxX / 2 - strlen(str) / 2, "%s", str);
			mvwprintw(stdscr, maxY / 2 + 1, maxX / 2 - strlen("recommended measures { 130 columns, 37 rows };"), "recommended measures { 130 columns, 37 rows };");
			wrefresh(stdscr);
			sleep(2);
			endwin();
			die((char*)str);
		}
	}
	clear();
}

void		freewin(t_box_info **box, WINDOW **win)
{
	if (*box)
	{
		if ((*box)->border_window)
			delwin((*box)->border_window);
		free(*box);
		*box = NULL;
	}
	if (win && *win) {
		delwin(*win);
		*win = NULL;
	}
}

void		get_space(WINDOW *win)
{
	attron(A_REVERSE);
	mvwprintw(win, getmaxy(stdscr) - 1, 0, "spazio");
	attroff(A_REVERSE);
	wrefresh(win);
	flushinp();
	while (getch() != ' ')
		;
	flushinp();
	clear();
}

void		get_space_right(WINDOW *win)
{
	attron(A_REVERSE);
	mvwprintw(win, getmaxy(stdscr) - 1, getmaxx(stdscr) - 6, "spazio");
	attroff(A_REVERSE);
	wrefresh(win);
	while (getch() != ' ')
		;
	clear();
}

size_t		total_strlen(char **str)
{
	int	ret = 0;

	for (int i = 0; str[i]; i++)
		ret += strlen(str[i]);
	return (ret);
}

size_t		get_line_length(char *str)
{
	int	ret = 0;

	while (str[ret] && str[ret] != '\n')
		ret++;
	return (ret);
}

size_t		count_strings(char *str)
{
	int	i = -1;
	int	ret = 0;

	if (!str)
		return (0);
	while (str[++i])
	{
		if (str[i] == '\n')
			ret++;
	}
	return (ret);
}

char		**get_strings(char *text)
{
	char	**ret;
	int		c = count_strings(text);
	int		j = 0;
	int		i = 0;

	ret = malloc(sizeof(char*) * (c + 1));
	if (!ret)
		die("Malloc error\n");
	for (; i < c; i++)
	{
		ret[i] = malloc(sizeof(char) * (get_line_length(&text[j])));
		if (!ret[i])
			die("Malloc error\n");
		strncpy(ret[i], &text[j], get_line_length(&text[j]) + 1);
		if (ret[i][get_line_length(&text[j])] == '\n')
			ret[i][get_line_length(&text[j])] = '\0';
		else
			ret[i][get_line_length(&text[j]) + 1] = '\0';
		j += get_line_length(&text[j]) + 1;
	}
	ret[i] = NULL;
	return (ret);
}

void		print_car(char **matrix)
{
	int			y;
	int			x;
	int			flag = 0;
	int			height = 0;

	
	x = 0;
	for (int i = 0; matrix[i]; i++)
		height++;
	y = getmaxy(stdscr) - 1 - height;
	x = getmaxx(stdscr) / 2;
	for (int i = 0; matrix[i]; i++)
	{
		for (int j = 0; matrix[i][j]; j++)
		{
			if ((matrix[i][j] == 32 && !flag) || matrix[i][j] == ',')
				x++;
			else if (matrix[i][j] > 0 && matrix[i][j] < 127)
			{
				mvprintw(y, x++, "%c", matrix[i][j]);
				flag = 1;
			}
			else
				unicode_rep(&matrix[i][j], y, x++);
			refresh();
		}
		flag = 0;
		y++;
		x = getmaxx(stdscr) / 2;
	}
}

double		distance(int y, int x, int y2, int x2)
{
	return (sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2)));
}

void		moon()
{
	int	radius = 4;
	int	center[2] = { getmaxy(stdscr) / 6, getmaxx(stdscr) - getmaxx(stdscr) / 6 };

	for (int i = 0; i < getmaxy(stdscr); i++)
	{
		for (int j = 0; j < getmaxx(stdscr); j++)
		{
			if (distance(center[0], center[1], i, j) <= radius && abs(center[0] - i) < radius)
			{
				attron(A_REVERSE);
				mvprintw(i, j, " ");
				attroff(A_REVERSE);
			}
			else
				mvprintw(i, j, " ");
		}
	}
	wrefresh(stdscr);
}

void		cloud()
{
	int	limits[5][2] = { {1, 16}, {2, 8}, {3, 9}, {2, 12}, {2, 44} };
	int	k = 0;

	attron(A_DIM);
	attron(A_REVERSE);
	for (int i = 5; i < 10; i++)
	{
		for (int j = 0; j < getmaxx(stdscr); j++)
		{
			if (j > limits[k][0] && j < limits[k][1])
				mvprintw(i, j, " ");
		}
		k++;
	}
	attroff(A_DIM);
	attroff(A_REVERSE);
	wrefresh(stdscr);
}

///////// stairs utils //////////////////////////

void		draw_step_hor(int length, int y, int x)
{
	attron(A_REVERSE);
	for (int i = 0; i < length; i++)
	{
		mvwprintw(stdscr, y, x++, " ");
		wrefresh(stdscr);
		usleep(30000);
	}
	attroff(A_REVERSE);
}

void		draw_step_ver(int length, int y, int x)
{
	attron(A_REVERSE);
	for (int i = 0; i < length; i++)
	{
		mvwprintw(stdscr, y++, x, " ");
		wrefresh(stdscr);
		usleep(30000);
	}
	attroff(A_REVERSE);
}

int			*draw_stairs(int step_w, int step_h, int range)
{
	int	x, y;
	int	*ret;
	int	*tmp;

	x = 0;
	y = 5;
	ret = malloc(sizeof(int) * 6);
	if (!ret)
		die("Malloc error");
	tmp = ret;

	for (int i = 0; true; i++)
	{
		if (i % 2)
		{
			if (y >= range)
				break ;
			draw_step_ver(step_h, y, x);
			y += step_h;
			if (y > getmaxy(stdscr))
				break ;
		}
		else
		{
			draw_step_hor(step_w, y, x);
			*tmp = x + 1;
			tmp++;
			x += step_w;
			if (x > getmaxx(stdscr))
				break ;
		}
	}
	draw_step_ver(getmaxy(stdscr) - y, y, x);
	return (ret);
}

int			write_on_stairs(int range, int step_h, int *pts)
{
	int x, y;
	int	ret = 0;

	x = 0;
	y = 5 - 1;

	char	*text;
	FILE	*file;
	char	**strings;

	file = fopen("text/5.txt", "r");
	text = malloc(sizeof(char) * (file_length("text/5.txt") + 1));
	if (!text)
		die("Malloc error\n");
	fread(text, file_length("text/5.txt"), 1, file);
	text[file_length("text/5.txt")] = '\0';
	fclose(file);
	strings = get_strings(text);
	free(text);
	int	j = 0;
	while (strings[j])
	{
		if (j == 5)
			ret = y;
		x = pts[j];
		if (strlen(strings[j]) + 1 < (size_t)range)
			x += (range - strlen(strings[j])) / 2;
		for (int i = 0; strings[j][i]; i++)
		{
			if (strings[j][i] > 0 && strings[j][i] < 127)
				mvwprintw(stdscr, y, x++, "%c", strings[j][i]);
			else
				i += unicode_rep(&strings[j][i], y, x++) - 1;
			wrefresh(stdscr);
			usleep(30000);
		}
		usleep(1000000);
		y += step_h;
		j++;
	}
	for (int i = 0; strings[i]; i++)
		free(strings[i]);
	free(strings);
	return (ret + 2);
}

void		block_on_stairs(int step_w, int y)
{
	t_box_info	*box = NULL;
	WINDOW		*win = NULL;
	int			width = step_w * 6;

	box = init_box(box, getmaxy(stdscr) - y, width, y, 0, 0, 0, 0);
	win = set_box(box, 0);
	curs_set(0);
	wattron(win, A_REVERSE);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < getmaxy(stdscr) - y; j++)
			mvwprintw(win, j, i, " ");
		wrefresh(win);
		usleep(30000);
	}
	wrefresh(win);
	wattroff(win, A_REVERSE);
	char	*text;
	FILE	*file;

	file = fopen("text/6.txt", "r");
	text = malloc(sizeof(char) * (file_length("text/6.txt") + 1));
	fread(text, file_length("text/6.txt"), 1, file);
	text[file_length("text/6.txt")] = '\0';
	fclose(file);
	dynamic_writing(text, y + ((getmaxy(stdscr) - y) / 2), 3);
	free(text);
	freewin(&box, &win);
}


///////// get_in_the_car utils /////////////////

char		*convert_rain_coord(char *text)
{
	int		occ = 0;
	int		idx = 0;
	int		len = (int)strlen(text);
	char	*ret;

	ret = malloc(sizeof(char) * getmaxx(stdscr) + 1);
	if (!ret)
		die("Malloc error\n");
	for (int i = 0; text[i]; i++)
	{
		if (text[i] == '|')
			occ++;
	}
	int pos[occ];
	for (int i = 0; text[i]; i++)
	{
		if (text[i] == '|')
			pos[idx++] = i;
	}
	for (int i = 0; i < getmaxx(stdscr); i++)
		ret[i] = ' ';
	for (int i = 0; i < idx; i++)
	{
		pos[i] = pos[i] * getmaxx(stdscr) / len;
		ret[pos[i]] = '|';
	}
	ret[getmaxx(stdscr) - 1] = 0;
	return (ret);
}

char		**proportional_rain(char **text)
{
	char	**tmp;
	int		height = 0;
	char	*tmp2;

	for (int i = 0; text[i]; i++)
		height++;

	tmp = malloc(sizeof(char*) * (height + 1));
	if (!tmp)
		die("Malloc error\n");
	tmp2 = malloc(sizeof(char) * ((int)getmaxx(stdscr) + 1));
	if (!tmp2)
		die("Malloc error\n");
	for (int i = 0; i < height; i++)
	{
		memset(&tmp2[0], ' ', getmaxx(stdscr));
		tmp2[getmaxx(stdscr) - 1] = 0;
		tmp[i] = convert_rain_coord(text[i]);
	}
	tmp[height] = NULL;
	return (tmp);
}

char		**transform_in_car(char *text)
{
	char	**ret;
	char	*ptr = text;
	int		lines = 0;
	int		j = 0;
	int		k = 0;

	for (int i = 0; text[i]; i++)
	{
		if (text[i] == '\n')
			lines++;
	}

	ret = malloc(sizeof(char*) * (lines + 1));
	if (!ret)
		die("Malloc error\n");
	for (k = 0; k < lines; k++)
	{
		ret[k] = malloc(sizeof(char) * (getmaxx(stdscr) + 1));
		if (!ret[k])
			die("Malloc error\n");
		memset(&ret[k][0], 32, getmaxx(stdscr));
		while (*ptr && *ptr != '\n')
		{
			ret[k][j++] = *ptr;
			ptr++;
		}
		ret[k][j] = '\0';
		while (*ptr && *ptr == '\n')
			ptr++;
		j = 0;
	}
	ret[k] = NULL;
	return (ret);
}

void		rain()
{
	t_box_info	*box = NULL;
	WINDOW		*win = NULL;
	char		**rain_text = get_road_text("text/rain2.txt");
	char		**rain;
	char		**text = NULL;
	char		*car;
	char		*floor = NULL;
	char		**car_matrix;
	int			rain_height = 0;
	int			offset = 0;
	int			win_offset = 0;
	int			c = 0;
	int			eight = 96;
	char		*file = "";

	clear();
	rain = proportional_rain(rain_text);
	for (int i = 0; rain_text[i]; i++)
		free(rain_text[i]);
	free(rain_text);
	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr) / 5, getmaxy(stdscr) / 2 - getmaxy(stdscr) / 5 / 2 + win_offset, 10, 1, getmaxy(stdscr) / 5, getmaxx(stdscr) / 5);
	win = set_box(box, 0);
	floor = malloc(sizeof(char) * (getmaxx(stdscr) + 1));
	if (!floor)
		die("Malloc error\n");
	memset(&floor[0], '-', getmaxx(stdscr));
	floor[getmaxx(stdscr) - 1] = '\0';
	for (int i = 0; rain[i]; i++)
		rain_height++;
	offset = getmaxy(stdscr) - 1;
	car = get_text_string("7.txt");
	car_matrix = transform_in_car(car);
	free(car);
	cbreak();
	timeout(0);
	while (1)
	{
		for (int i = 0; rain[i] && i < getmaxy(stdscr) - 1; i++)
		{
			if (i == getmaxy(stdscr) - 2)
			{
				mvprintw(i, 0, "%s", floor);
				memset(&floor[0], '-', getmaxx(stdscr));
			}
			else if (i == getmaxy(stdscr) - 3)
			{
				for (int j = 0; j < getmaxx(stdscr); j++)
				{
					if (rain[i + offset][j] == '|')
						floor[j] = '*';
				}
				mvprintw(i, 0, "%s", rain[i + offset]);
			}
			else if (i + offset >= 0)
				mvprintw(i, 0, "%s", rain[i + offset]);
			else
				mvprintw(i, 0, "%s", rain[rain_height + offset]);
		}
		offset--;
		if (offset == 3)
			offset = getmaxy(stdscr) - 1;
		wrefresh(stdscr);
		print_car(car_matrix);
		for (int i = 0; i < getmaxx(stdscr); i++)
			mvprintw(getmaxy(stdscr) - 1, i, " ");
		wrefresh(stdscr);
		c = getch();
		if (c == 32)
		{
			eight++;
			switch (eight)
			{
				case 96: file = ""; break ;
				case 97: file = "8a.txt"; break ;
				case 98: file = "8b.txt"; break ;
				case 99: file = "8c.txt"; break ;
				default: file = NULL; break ;
			}
			win_offset += getmaxx(stdscr) / 15;
			if (!file)
				break ;
		}
		if (strlen(file))
		{
			freewin(&box, &win);
			box = init_box(box,
				getmaxy(stdscr) / 2,
				getmaxx(stdscr) / 5,
				getmaxy(stdscr) / 2 - getmaxy(stdscr) / 5 / 2,
				10 + win_offset,
				1,
				getmaxy(stdscr) / 5,
				getmaxx(stdscr) / 5);
			flushinp();
			win = set_box(box, 0);
			text = get_text(file, box);
			write_to_box(win, 0, 0, text);
			wrefresh(win);
		}
		usleep(100000);
	}
	for (int i = 0; car_matrix[i]; i++)
		free(car_matrix[i]);
	free(car_matrix);
	for (int i = 0; rain[i]; i++)
		free(rain[i]);
	free(rain);
}

void		init_road_info(t_road_info *road_info, char *filename, t_box_info *box)
{
	road_info->counter = 0;
	road_info->line = 0;
	road_info->matrix = get_road_text(filename);
	road_info->pts[0] = -2;
	road_info->pts[1] = -1;
	road_info->pts[2] = -(box->height / 2 + 4);
	road_info->pts[3] = -(box->height / 2 + 3);
	road_info->diff = (road_info->pts[1] * -1 + road_info->pts[2]) / 2 - 1;
	road_info->line_y = road_info->pts[1] - (road_info->diff);
	road_info->stars = 0;
	road_info->costellation = 0;
	road_info->y_off = 0;
}

void		init_mirror_info(t_mirror_info *mirror_info, int speed, char *filename)
{
	int	first_win_height = getmaxy(stdscr) / 8;
	
	mirror_info->pause = speed;
	mirror_info->far_away = 0;
	mirror_info->idx = 0;
	mirror_info->boxes[0] = NULL;
	mirror_info->boxes[1] = NULL;
	mirror_info->boxes[0] = init_box(mirror_info->boxes[0], getmaxy(stdscr) / 8, getmaxx(stdscr) / 3, getmaxy(stdscr) / 3, getmaxx(stdscr) / 2 - getmaxx(stdscr) / 6, 0, 0, 0);
	mirror_info->boxes[1] = init_box(mirror_info->boxes[1], first_win_height - 1, getmaxx(stdscr) / 3, getmaxy(stdscr) / 3 + 1, getmaxx(stdscr) / 2 - getmaxx(stdscr) / 6, 1, 0, 0);
	mirror_info->text = get_text_string(filename);
	mirror_info->matrix = format_string(mirror_info->text, mirror_info->boxes[0]);
}

char		**get_road_text(char *filename)
{
	char	**ret;
	char	tmp[100];
	int		fd;
	int		height = 0;
	char	c;
	int		j = 0;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		die("Error opening file\n");
	while (read(fd, &c, 1) > 0)
	{
		if (c == '\n')
			height++;
	}
	close(fd);
	ret = malloc(sizeof(char*) * (height + 1));
	if (!ret)
		die("Malloc error\n");
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		die("Error opening file\n");
	height = 0;
	while (read(fd, &tmp[j], 1) > 0)
	{
		if (tmp[j] == '\n')
		{
			ret[height] = malloc(sizeof(char) * (j + 1));
			if (!ret[height])
				die("Malloc error\n");
			if (j > 0)
			{
				strncpy(ret[height], tmp, j);
				ret[height][j] = '\0';
			}
			else
				ret[height][0] = 0;
			j = 0;
			height++;
		}
		else
			j++;
	}
	ret[height] = NULL;
	return (ret);
}

void		print_road(WINDOW *win)
{
	int			offset = getmaxx(stdscr) / 3 - 4;

	moon();
	for (int i = 0; i < getmaxy(stdscr) / 2; i++)
	{
		for (int j = 0; j < getmaxx(stdscr); j++)
		{
			if (!i)
				mvwprintw(win, i, j, "_");
			else if (j == getmaxx(stdscr) / 2 - offset / 2)
				mvwprintw(win, i, j, "/");
			else if (j == getmaxx(stdscr) / 2 + offset / 2)
				mvwprintw(win, i, j, "\\");
			else
				mvwprintw(win, i, j, " ");
		}
		offset += 4;
	}
	wrefresh(win);
}

void		write_until(int idx, char **text, WINDOW *win)
{
	int		c = 0;
	int		i = -1;
	int		j = 0;
	char	*tmp;

	while (text[++i] && c < idx)
	{
		if (c + strlen(text[i]) < (size_t)idx)
		{
			c += strlen(text[i]);
			mvwprintw(win, i, j, "%s", text[i]);
		}
		else
		{
			tmp = malloc(sizeof(char) * (idx - c + 1));
			if (!tmp)
				die("Malloc error");
			memset(&tmp[0], 0, idx - c + 1);
			strncpy(tmp, text[i], idx - c);
			mvwprintw(win, i, j, "%s", tmp);
			free(tmp);
			c = idx;
		}
	}
	wrefresh(win);
}

void		mirror(t_mirror_info *mirror_info, int *waiting)
{
	mirror_info->wins[1] = set_box(mirror_info->boxes[1], 0);
	wbkgd(mirror_info->wins[1], COLOR_PAIR(1));
	wrefresh(mirror_info->wins[1]);
	mirror_info->wins[0] = set_box(mirror_info->boxes[0], 0);
	wbkgd(mirror_info->wins[0], COLOR_PAIR(2));
	if (mirror_info->far_away >= mirror_info->pause)
	{
		write_until(mirror_info->idx, mirror_info->matrix, mirror_info->wins[0]);
		mirror_info->far_away = 0;
		if ((size_t)mirror_info->idx < strlen(mirror_info->text))
			mirror_info->idx++;
		else if (*waiting == 0)
		{
			*waiting = 1;
			flushinp();
		}
	}
}

void		write_on_road(t_road_info *road_info, WINDOW *win, t_box_info *box, int b, enum state *status)
{
	int	x;

	if (road_info->pts[0] > 0)
		mvwprintw(win, road_info->pts[0], getmaxx(stdscr) / 2, "||");
	if (road_info->pts[1] > 0)
		mvwprintw(win, road_info->pts[1], getmaxx(stdscr) / 2, "||");
	if (road_info->pts[2] > 0)
		mvwprintw(win, road_info->pts[2], getmaxx(stdscr) / 2, "||");
	if (road_info->pts[3] > 0)
		mvwprintw(win, road_info->pts[3], getmaxx(stdscr) / 2, "||");
	if (b)
	{
		x = getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2;
		if (road_info->line_y > 0)
			mvwprintw(win, road_info->line_y, x, "%s", road_info->matrix[(int)road_info->line]);
		if (road_info->line_y >= box->height)
		{
			road_info->line++;
			road_info->line_y = -2;
			if (road_info->line == 3 && status != NULL)
				*status += 1;
		}
	}
	wrefresh(win);
	wrefresh(stdscr);
}

void		proportional(int coords[12][2], int files_size[2], int height, int width)
{
	for (int i = 0; i < 12; i++)
	{
		coords[i][0] = coords[i][0] * height / files_size[0];
		coords[i][1] = coords[i][1] * width / files_size[1];
	}
}

void		write_on_halfsky(t_road_info *road_info, int *waiting)
{
	mvprintw(getmaxy(stdscr) / 4, getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
	wrefresh(stdscr);
	*waiting = 1;
}

void		stars(t_road_info *road_info)
{
	int			coords[12][2] = { {5, 46}, {1, 18}, {1, 33}, {1, 38}, {2, 10}, {2, 17}, {2, 43}, {4, 9}, {4, 25}, {5, 51}, {5, 15}, {5, 42} };
	int			file_size[2] = { 7, 65 };
	
	proportional(coords, file_size, getmaxy(stdscr) / 2, getmaxx(stdscr) - getmaxx(stdscr) / 8);
	for (int i = 0; i < road_info->stars; i++)
		mvprintw(coords[i][0] + road_info->y_off, coords[i][1], "*");
	wrefresh(stdscr);
}

void		write_on_sky(t_road_info *road_info)
{
	stars(road_info);
	mvprintw(getmaxy(stdscr) / 4, getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
	wrefresh(stdscr);
}

void		write_on_roadsky(t_road_info *road_info)
{
	if (!strncmp("e arrivare fino a te", road_info->matrix[(int)road_info->line], strlen("e arrivare fino a te")))
		attron(A_REVERSE);
	mvprintw(getmaxy(stdscr) - getmaxy(stdscr) / 4, getmaxx(stdscr) / 2 - strlen(road_info->matrix[(int)road_info->line]) / 2, "%s", road_info->matrix[(int)road_info->line]);
	if (!strncmp("e arrivare fino a te", road_info->matrix[(int)road_info->line], strlen("e arrivare fino a te")))
		attroff(A_REVERSE);
	for (int j = 0; j < getmaxx(stdscr); j++)
		mvprintw(getmaxy(stdscr) / 2, j, "_");
	wrefresh(stdscr);
	stars(road_info);
}

void		update_central_line(int *pts, int height)
{
	if (pts[0] == height + 1)
	{
		pts[0] = -2;
		pts[1] = -1;
	}
	if (pts[2] == height + 1)
	{
		pts[2] = -2;
		pts[3] = -1;
	}
}

void		get_line_y(t_road_info *road_info)
{
	if (road_info->pts[1] < 0 && road_info->pts[3] < 0 && road_info->pts[1] > road_info->pts[3])
		road_info->line_y = road_info->pts[1] - road_info->diff;
	else if (road_info->pts[1] < 0 && road_info->pts[3] < 0 && road_info->pts[3] > road_info->pts[1])
		road_info->line_y = road_info->pts[3] - road_info->diff;
	else if (road_info->pts[1] < 0)
		road_info->line_y = road_info->pts[1] - road_info->diff;
	else if (road_info->pts[3] < 0)
		road_info->line_y = road_info->pts[3] - road_info->diff;
	else
		road_info->line_y = -5;
}

void
move_road_last(t_box_info *box, WINDOW *win)
{
	int				speed = 70000;
	int				i = 0;
	int				c = 0;
	t_road_info		road_info;
	int				line = 0;
	int				d = 0;

	road_info.pts[0] = -2;
	road_info.pts[1] = -1;
	road_info.pts[2] = -(box->height / 2 + 4);
	road_info.pts[3] = -(box->height / 2 + 3);
	road_info.diff = (road_info.pts[1] * -1 + road_info.pts[2]) / 2 - 1;
	road_info.line_y = road_info.pts[1] - (road_info.diff);
	cbreak();
	timeout(0);
	curs_set(0);
	while (1)
	{
		update_central_line(road_info.pts, box->height);
		print_road(win);
		if (road_info.pts[0] > 0)
			mvwprintw(win, road_info.pts[0], getmaxx(stdscr) / 2, "||");
		if (road_info.pts[1] > 0)
			mvwprintw(win, road_info.pts[1], getmaxx(stdscr) / 2, "||");
		if (road_info.pts[2] > 0)
			mvwprintw(win, road_info.pts[2], getmaxx(stdscr) / 2, "||");
		if (road_info.pts[3] > 0)
			mvwprintw(win, road_info.pts[3], getmaxx(stdscr) / 2, "||");
		if (line && d == 1)
			mvwprintw(stdscr, getmaxy(stdscr) / 3, getmaxx(stdscr) / 2 - strlen("non c'è paura più bella.") / 2, "non c'è paura più bella.");
		else if (line && d == 2)
			mvwprintw(stdscr, getmaxy(stdscr) / 3, getmaxx(stdscr) / 2 - strlen(">_A me, non è rimasta che quella.") / 2, ">_A me, non è rimasta che quella.");
		wrefresh(win);
		wrefresh(stdscr);

		if (i % 3 == 0)
		{
			road_info.pts[0]++;
			road_info.pts[1]++;
			road_info.pts[2]++;
			road_info.pts[3]++;
			road_info.line_y++;
			i = 0;
		}
		usleep(speed);
		clear();
		i++;
		c = getch();
		if (c == 32) {
			if (!line || d == 1) {
				line = 1;
				d++;
			}
			else
				break ;
		}
	}
}

void		move_road(t_box_info *box, WINDOW *win)
{
	int				speed = 70000;
	int				c = 0;
	int				i = 0;
	enum state		status = MIRROR;
	t_mirror_info	mirror_info;
	t_road_info		road_info;
	int				waiting = 0;
	int				mirror_line = 0;

	init_mirror_info(&mirror_info, speed, "9a.txt");
	init_road_info(&road_info, "text/10.txt", box);
	cbreak();
	timeout(0);
	curs_set(0);
	while (1)
	{
		update_central_line(road_info.pts, box->height);
		print_road(win);
		
		if (i % 3 == 0)
		{
			road_info.pts[0]++;
			road_info.pts[1]++;
			road_info.pts[2]++;
			road_info.pts[3]++;
			if (status == ROAD)
				road_info.line_y++;
			if (status == SKY)
			{
				if (road_info.stars < 12)
					road_info.stars += 0.3;
				road_info.line += 0.075;
			}
			if (status == ROADSKY)
			{
				road_info.line += 0.075;
				road_info.y_off += 0.5;
			}
			i = 0;
		}
		switch (status)
		{
			case MIRROR: write_on_road(&road_info, win, box, 0, &status); mirror(&mirror_info, &waiting); break ;
			case ROAD: write_on_road(&road_info, win, box, 1, &status); break ;
			case HALFSKY: write_on_road(&road_info, win, box, 1, &status); write_on_halfsky(&road_info, &waiting); break ;
			case SKY: write_on_road(&road_info, win, box, 0, &status); write_on_sky(&road_info); break ;
			case ROADSKY: write_on_road(&road_info, win, box, 0, &status); write_on_roadsky(&road_info); break ;
			default: break ;
		}
		if (waiting)
		{
			c = getch();
			if (status == MIRROR)
			{
				attron(A_REVERSE);
				mvprintw(getmaxy(stdscr) - 1, 0, "spazio");
				attroff(A_REVERSE);
				wrefresh(stdscr);
			}
			switch (c)
			{
				case 32:
				{
					if (status == MIRROR)
					{
						for (int k = 0; mirror_info.matrix[k]; k++)
							free(mirror_info.matrix[k]);
						free(mirror_info.matrix);
						free(mirror_info.text);
						mirror_line++;
						if (mirror_line == 3)
						{
							freewin(&mirror_info.boxes[0], &mirror_info.wins[0]);
							freewin(&mirror_info.boxes[1], &mirror_info.wins[1]);
							status++;
						}
						else
						{
							switch (mirror_line)
							{
								case 1: mirror_info.text = get_text_string("9b.txt"); break ;
								case 2: mirror_info.text = get_text_string("9c.txt"); break ;
								default: break ;
							}
							mirror_info.idx = 0;
							mirror_info.matrix = format_string(mirror_info.text, mirror_info.boxes[0]);
						}
					}
					else
						status++;
					if (status == SKY)
						road_info.line++;
					waiting = 0;
					c = 0;
					get_line_y(&road_info);
					break ;
				}
				default: break ;
			}
		}
		if (status == SKY && !strlen(road_info.matrix[(int)road_info.line]))
		{
			status++;
			clear();
		}
		else if (status == ROADSKY && !strlen(road_info.matrix[(int)road_info.line]) && !strncmp(road_info.matrix[(int)road_info.line + 1], "sonno sciocco", strlen("sonno sciocco")))
		{
			status++;
			road_info.line++;
		}
		usleep(speed);
		if (status == STEPTWO)
			break ;
		mirror_info.far_away += speed;
		clear();
		i++;
	}
	steptwo(&road_info);
	curs_set(1);
	wrefresh(stdscr);
	for (int k = 0; road_info.matrix[k]; k++)
		free(road_info.matrix[k]);
	free(road_info.matrix);
}

void		look_at_road()
{
	t_box_info	*box = NULL;
	WINDOW		*win = NULL;

	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0, 0, 0, 0);
	win = set_box(box, 0);
	move_road(box, win);
	wrefresh(stdscr);
	freewin(&box, &win);
}

//////////// lighting up utils ////////////////////

char		**get_cigarette()
{
	char	**ret;
	char	*tmp;
	char	*ptr;

	tmp = get_text_string("cigarette.txt");
	ptr = tmp;
	ret = malloc(sizeof(char*) * (3));
	if (!ret)
		die("Malloc error\n");
	for (int i = 0; i < 2; i++)
	{
		ret[i] = malloc(sizeof(char) * (line_len(ptr) + 1));
		if (!ret[i])
			die("Malloc error\n");
		strncpy(ret[i], ptr, line_len(ptr) + 1);
		ret[i][line_len(ptr)] = '\0';
		ptr += line_len(ptr) + 1;
	}
	ret[2] = NULL;
	free(tmp);
	return (ret);
}

void		draw_cigarette(char **cigarette, int y, int x)
{
	for (int i = 0; cigarette[i]; i++)
		mvprintw(y++, x, "%s", cigarette[i]);
	wrefresh(stdscr);
}

int			line_len(char *ptr)
{
	int	ret = 0;
	int	i = 0;

	while (ptr[i] && ptr[i] != '\n')
	{
		ret++;
		i++;
	}
	return (ret);
}

void		smoke(int limits[2], int y, int x, int a, int c)
{
	int	add = 1;
	
	if (a)
	{
		x -= 1;
		add = -1;
	}
	curs_set(0);
	attron(A_DIM);
	attron(A_REVERSE);
	for (; y > -1 && c--; y--)
	{
		mvprintw(y, x, " ");
		if (x + add < limits[0] || x + add > limits[1])
			add *= -1;
		x += add;
	}
	attroff(A_DIM);
	attroff(A_REVERSE);
	wrefresh(stdscr);
	curs_set(1);
}

int	arr_length(char **s)
{
	int	i = 0;
	for (; s[i]; i++)
		;
	return i;
}

int		write_on_smoke(char *s)
{
	t_box_info	*box = NULL;
	t_box_info	*fake_box = NULL;
	WINDOW		*win = NULL;
	static int	c = 0;
	static int	b = 0;
	int			y = getmaxy(stdscr);
	char		**text;

	box = init_box(box, getmaxy(stdscr), getmaxx(stdscr) / 2, 0, 0, 0, 0, 0);
	fake_box = init_box(fake_box, getmaxy(stdscr), getmaxx(stdscr) / 2 - box->width / 4, 0, 0, 0, 0, 0);
	win = set_box(box, 0);
	text = format_string(s, fake_box);
	for (int i = 0; text[i]; i++)
	{
		mvwprintw(win, getmaxy(win) / 8 - arr_length(text) / 2 + i, getmaxx(win) / 2 - strlen(text[i]) / 2, "%s", text[i]);
	}
	for (int i = 0; i < c / 2 && y > -1; i++)
	{
		for (int j = 0; j < getmaxx(stdscr) / 2; j++)
		{
			if (i < b)
				mvwprintw(win, y, j, " ");
			else if (i % 2 && (j % 6 < 3 || j % 6 == 4))
				mvwprintw(win, y, j, " ");
			else if (i % 2 && j % 6 == 3)
				mvwprintw(win, y, j, ")");
			else if (i % 2 && j % 6 == 5)
				mvwprintw(win, y, j, "(");
			else if (j % 6 == 1 || (j % 6 > 2 && j % 6 < 6))
				mvwprintw(win, y, j, " ");
			else if (!(j % 6))
				mvwprintw(win, y, j, ")");
			else if (j % 6 == 2)
				mvwprintw(win, y, j, "(");				
		}
		y--;
		if (!y)
			b++;
		if (b == getmaxy(stdscr))
		{
			b = 0;
			c = 0;
			return 1;
		}
	}
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
	wrefresh(win);
	freewin(&box, &win);
	if (fake_box)
	{
		if (fake_box->border_window)
			delwin(fake_box->border_window);
		free(fake_box);
	}
	c++;
	return 0;
}

void
last_street(int* star)
{
	t_box_info		*box = NULL;
	WINDOW			*win = NULL;
	int				speed = 70000;
	int				i = 0;
	t_road_info		road_info;
	int				c = 0;
	int				idx = 0;


	box = init_box(box, getmaxy(stdscr) / 2, getmaxx(stdscr), getmaxy(stdscr) / 2, 0, 0, 0, 0);
	win = set_box(box, 0);
	init_road_info(&road_info, "text/16.txt", box);
	cbreak();
	timeout(0);
	curs_set(0);
	flushinp();
	while (1)
	{
		update_central_line(road_info.pts, box->height);
		print_road(win);
		
		if (i % 3 == 0)
		{
			road_info.pts[0]++;
			road_info.pts[1]++;
			road_info.pts[2]++;
			road_info.pts[3]++;
			road_info.line_y++;
			i = 0;
		}
		write_on_road(&road_info, win, box, 0, NULL);
		write_on_sky(&road_info);
		c = getch();
		if (c == 32)
		{
			road_info.line++;
			idx++;
			c = 0;
			if (idx == 5)
			{
				clear();
				break ;
			}
		}
		usleep(speed);
		clear();
		i++;
	}
	curs_set(1);
	wrefresh(stdscr);
	for (int k = 0; road_info.matrix[k]; k++)
		free(road_info.matrix[k++]);
	free(road_info.matrix);
	*star = 0;
}

void
loggg()
{
	const char*	str = "I used to talk with you, but the words suddenly stopped to sound in your mind - so I decided to remain here, just waiting.\n\
\rI did not know when, or even if, you would have listened to what I had to tell you, so I have hidden myself under a black blanket to not be seen by you.\n\
\rSo many times I felt the need to speak to you the words I had inside - but now that you're here, I feel like nothing has to be spoken.\n\
\rBecause we needed so much time to realize there's no me and you. You covered yourself under a black blanket, splitting my face in two.\n\n\n";

	write(1, str, strlen(str));
}


void
get_back()
{
	loggg();
}