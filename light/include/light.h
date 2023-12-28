#ifndef UTILS_H
# define UTILS_H
# define NCURSES_WIDECHAR 1
# if __APPLE__
# include <ncurses.h>
# elif __linux__
# include <ncursesw/ncurses.h>
# endif
# include <locale.h>
# include <limits.h>
# include <wchar.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <time.h>
# include <sys/time.h>
# include <math.h>
# define DONTKNOW 0

extern cchar_t *characters;

/* Attributes
 *
 * A_NORMAL
 * A_REVERSE
 * A_INVIS
 * A_BLINK
 * A_DIM
 * A_STANDOUT
 * A_PROTECT
 * A_BOLD
 * A_ALTCHARSET
 * A_CHARTEXT
 * 
 *
 * COLOR_PAIR(n)
 * COLOR_BLACK
 * COLOR_RED
 * COLOR_GREEN
 * COLOR_YELLOW
 * COLOR_BLUE
 * COLOR_MAGENTA
 * COLOR_CYAN
 * COLOR_WHITE
 */

typedef	struct	s_box_info
{
	int		height;
	int		width;
	int		y;
	int		x;
	int		box;
	int		bh;
	int		bw;
	WINDOW	*border_window;
}				t_box_info;

typedef struct s_mirror_info
{
	t_box_info	*boxes[2];
	WINDOW		*wins[2];
	int			far_away;
	int			pause;
	char		**matrix;
	char		*text;
	int			idx;
	char		**tmp_text;
}				t_mirror_info;

typedef struct	s_road_info
{
	int		counter;
	char	**matrix;
	float	line;
	int		pts[4];
	int		line_y;
	int		diff;
	float	stars;
	int		costellation;
	float	y_off;
}				t_road_info;

enum	state
{
	MIRROR,
	ROAD,
	HALFSKY,
	SKY,
	ROADSKY,
	STEPTWO
};

void		die(char *str);
int			file_length(const char *path);
void		start_ncurses();
WINDOW		*set_box(t_box_info *box, int COLOR);
void		write_to_box(WINDOW *win, int y, int x, char **str);
char		**get_text(char *filename, t_box_info *box);
char		*get_text_string(char *filename);
void		dynamic_writing(char *str, int y, int x);
void		dynamic_writing_option_no_coord(char *str, int opt);
int			formatted_dynamic_writing(char **text, WINDOW *win, int y, int x);
int			formatted_dynamic_writing_no_free(char **text, t_box_info *box, WINDOW *win, int y, int x);
void		formatted_dynamic_writing_portion(char *str, char **text, t_box_info *box, WINDOW *win);
t_box_info	*init_box(t_box_info *box, int height, int width, int y, int x, int box_bool, int bh, int bw);
void		check();
void		freewin(t_box_info **box, WINDOW **win);
void		get_space(WINDOW *win);
void		get_space_right(WINDOW *win);
size_t		total_strlen(char **str);
void		intro();
void		intruder(t_box_info *box, WINDOW *win, int y);
void		stairs();
int			*search_in_matrix(char *str, char **matrix);
int			unicode_rep(char *str, int y, int x);
int			unicode_rep_to_win(WINDOW* win, char *str, int y, int x);
void		get_in_the_car();
int			*draw_stairs(int step_w, int step_h, int range);
int			write_on_stairs(int range, int step_h, int *pts);
void		block_on_stairs(int step_w, int y);
void		rain();
void		look_at_road();
char		**get_road_text(char *filename);
void		lighting_up();
void		smoke(int limits[2], int y, int x, int a, int c);
int			line_len(char *ptr);
char		**get_cigarette();
void		draw_cigarette(char **cigarette, int y, int x);
int			write_on_smoke(char *s);
void		steptwo(t_road_info *road_info);
void		triple_road(t_road_info *road_info);
void		moon();
char		**ft_split(char *s, char c);
void		proportional(int coords[12][2], int files_size[2], int height, int width);
void		print_road(WINDOW *win);
void		move_road_last(t_box_info *box, WINDOW *win);
int			staring();
void		last_street(int* star);
void		dream();
void		dream_one();
void		dream_two();
void		dream_three();
void		dream_four();
void		dream_five();
void		dream_six();
void		get_back();

extern int	star;

#endif
