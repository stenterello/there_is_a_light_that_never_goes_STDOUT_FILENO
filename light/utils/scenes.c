#include "light.h" 

void		intro()
{
	dynamic_writing("da qui non passo ho bisogno di piu' ", getmaxy(stdscr) - 4, getmaxx(stdscr) / 2 - strlen("da qui non passo ho bisogno di piu' ") / 2);
	dynamic_writing_option_no_coord("spazio", A_REVERSE);
	curs_set(0);

	while (getch() != ' ')
		;
	clear();
	curs_set(1);
}

void		intruder(t_box_info *box, WINDOW *win, int y)
{
	char		**str = NULL;
	char		*one_str = NULL;
	int			coord[2];

	coord[0] = 0;
	coord[1] = getmaxx(stdscr) / 2;
	box = init_box(box, getmaxy(stdscr), getmaxx(stdscr) / 2, 0, coord[1], 0, 0, 0);
	win = set_box(box, 0);
	str = get_text("1.txt", box);
	y = formatted_dynamic_writing(str, win, 1, 0);
	sleep(1);
	str = get_text("2.txt", box);
	y = formatted_dynamic_writing(str, win, y + 2, 0);
	get_space(stdscr);
	clear();
	freewin(&box, &win);
	coord[0] = getmaxy(stdscr) / 4;
	coord[1] = 1;
	box = init_box(box, getmaxy(stdscr), getmaxx(stdscr) / 2, coord[0], coord[1], 0, 0, 0);
	win = set_box(box, 0);
	str = get_text("3.txt", box);
	y = formatted_dynamic_writing(str, win, 1, 0);
	freewin(&box, &win);
	coord[0] = getmaxy(stdscr) / 2 + getmaxy(stdscr) / 4 + 2;
	coord[1] = getmaxx(stdscr) / 4 - getmaxx(stdscr) / 3 / 2;
	if (getmaxy(stdscr) / 8 % 2)
		box = init_box(box, getmaxy(stdscr) / 8, getmaxx(stdscr) / 3, coord[0], coord[1], 1, 0, 0);
	else
		box = init_box(box, getmaxy(stdscr) / 8 + 1, getmaxx(stdscr) / 3, coord[0], coord[1], 1, 0, 0);
	win = set_box(box, 0);
	one_str = get_text_string("4.txt");
	coord[0] = coord[0] + box->height / 2;
	coord[1] = coord[1] - strlen(one_str) / 2 + box->width / 2;
	dynamic_writing(one_str, coord[0], coord[1]);
}

void		stairs()
{
	int	step_w;
	int	step_h;
	int	y;
	int	last_step;
	int	range;
	int	*x_coords;

	last_step = (getmaxy(stdscr) - 5) / 6;
	range = getmaxy(stdscr) - 5 - last_step;
	step_w = roundf(((float)(getmaxx(stdscr))) / 7);
	step_h = roundf(((float)(getmaxy(stdscr)) - 5 - last_step) / 6);
	x_coords = draw_stairs(step_w, step_h, range);
	y = write_on_stairs(step_w - 1, step_h, x_coords);
	block_on_stairs(step_w, y);
	free(x_coords);
	get_space_right(stdscr);
	clear();
}

void		get_in_the_car()
{
	rain();
	look_at_road();
}

void		lighting_up()
{
	int		limits[2];
	int		x, y; 
	char	**cigarette;
	int		c = 0;
	int		a = 0;
	char	*text;
	int		idx = 0;

	cigarette = get_cigarette();
	text = get_text_string("11.txt");
	limits[0] = getmaxx(stdscr) - getmaxx(stdscr) / 7;
	limits[1] = limits[0] + 6;
	x = getmaxx(stdscr) - strlen(cigarette[0]) - 5;
	y = getmaxy(stdscr) - 5;
	while (1)
	{
		draw_cigarette(cigarette, y, x);
		if (!(c % 10))
			a = (a == 0) ? 1 : 0;
		smoke(limits, y - 1, x, a, c);
		if (write_on_smoke(text))
		{
			free(text);
			text = NULL;
			char	*filename = NULL;
			switch (++idx)
			{
				case 0: filename = "11.txt"; break;
				case 1: filename = "12.txt"; break;
				case 2: filename = "13.txt"; break;
				case 3: filename = "14.txt"; break;
				case 4: filename = "15.txt"; break;
				default: break ;
			}
			if (!filename)
				break;
			text = get_text_string(filename);
		}
		usleep(70000);
		clear();
		c = (c == INT_MAX) ? 0 : c + 1;
	}
	for (int i = 0; cigarette[i]; i++)
		free(cigarette[i]);
	free(cigarette);
	clear();
}

int	star = 1;

int
staring()
{
	if (star)
		last_street(&star);
	return (star);
}
 
void
dream()
{
	dream_one();
	dream_two();
	dream_three();
	dream_four();
	dream_five();
	dream_six();
}