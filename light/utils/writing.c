#include "light.h"

cchar_t	*characters = NULL;

int			unicode_rep_to_win(WINDOW* win, char *str, int y, int x)
{
	int			j = 0;
	int			char_len;
	cchar_t		c;

	char_len = mblen(str, MB_CUR_MAX);
	if (!memcmp(str, "à", char_len))
		j = 0;
	else if (!memcmp(str, "è", char_len))
		j = 1;
	else if (!memcmp(str, "é", char_len))
		j = 2;
	else if (!memcmp(str, "ì", char_len))
		j = 3;
	else if (!memcmp(str, "ò", char_len))
		j = 4;
	else if (!memcmp(str, "ù", char_len))
		j = 5;
	switch (j)
	{
		case 0:
		{
			const wchar_t	*w = L"à";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 1:
		{
			const wchar_t	*w = L"è";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 2:
		{
			const wchar_t	*w = L"é";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 3:
		{
			const wchar_t	*w = L"ì";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 4:
		{
			const wchar_t	*w = L"ò";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 5:
		{
			const wchar_t	*w = L"ù";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		default: return (0);
	}
	mvwadd_wch(win, y, x++, &c);
	return (char_len);
}

int			unicode_rep(char *str, int y, int x)
{
	int			j = 0;
	int			char_len;
	cchar_t		c;

	char_len = mblen(str, MB_CUR_MAX);
	if (!memcmp(str, "à", char_len))
		j = 0;
	else if (!memcmp(str, "è", char_len))
		j = 1;
	else if (!memcmp(str, "é", char_len))
		j = 2;
	else if (!memcmp(str, "ì", char_len))
		j = 3;
	else if (!memcmp(str, "ò", char_len))
		j = 4;
	else if (!memcmp(str, "ù", char_len))
		j = 5;
	switch (j)
	{
		case 0:
		{
			const wchar_t	*w = L"à";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 1:
		{
			const wchar_t	*w = L"è";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 2:
		{
			const wchar_t	*w = L"é";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 3:
		{
			const wchar_t	*w = L"ì";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 4:
		{
			const wchar_t	*w = L"ò";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		case 5:
		{
			const wchar_t	*w = L"ù";
			c.attr = 0;
			c.chars[0] = w[0];
			c.chars[1] = L'\0';
			break ;
		}
		default: return (0);
	}
	mvwadd_wch(stdscr, y, x++, &c);
	return (char_len);
}

int			unicode_rep_no_coord(char *str)
{
	int	i;
	int	char_len;

	i = 0;
	char_len = mblen(str, MB_CUR_MAX);
	if (!memcmp(str, "ù", char_len))
	{
		const wchar_t	*w = L"ù";
		cchar_t c;
		c.attr = 0;
		c.chars[0] = w[i];
		c.chars[1] = L'\0';
		wadd_wch(stdscr, &c);
		return (char_len);
	}
	return (0);
}

void		formatted_dynamic_writing_portion(char *str, char **text, t_box_info *box, WINDOW *win)
{
	int		*coord;
	int		time = 70000;

	coord = search_in_matrix(str, text);
	if (!coord)
	{
		write(2, "No string found\n", 16);
		return ;
	}
	for (int i = 0; str[i]; i++)
	{
		if (str[i] && str[i] == ' ')
			time = 40000;
		else
			time = 70000;
		if (str[i] > 0 && str[i] < 127)
			mvwprintw(win, coord[0], coord[1]++, "%c", str[i]);
		else
			i += unicode_rep(&str[i], coord[0], coord[1]++) - 1;
		if (coord[1] == box->width)
		{
			coord[1] = 0;
			coord[0]++;
		}
		wrefresh(win);
		usleep(time);
	}
	wrefresh(win);
	free(coord);
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
}

int			formatted_dynamic_writing(char **text, WINDOW *win, int y, int x)
{
	int	time = 70000;

	for (int i = 0; text[i]; i++)
	{
		for (int j = 0; text[i][j]; j++)
		{
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
		}
		x = 0;
		y++;
		wrefresh(win);
	}
	wrefresh(win);
	for (int i = 0; text[i]; i++)
		free(text[i]);
	free(text);
	return (y);
}

void		dynamic_writing(char *str, int y, int x)
{
	int	time = 70000;

	for (int i = 0; str[i] && str[i] != '\n'; i++)
	{
		if (str[i] && str[i] == ' ')
			time = 40000;
		else
			time = 70000;
		if (str[i] < 127 && str[i] > 0)
			mvwprintw(stdscr, y, x++, "%c", str[i]);
		else
			i += unicode_rep(&str[i], y, x++) - 1;
		wrefresh(stdscr);
		usleep(time);
	}
}

void		dynamic_writing_option_no_coord(char *str, int opt)
{
	int	time = 70000;

	attron(opt);
	for (int i = 0; str[i]; i++)
	{
		if (str[i] && str[i] == ' ')
			time = 40000;
		else
			time = 70000;
		if (str[i] < 127 && str[i] > 0)
			wprintw(stdscr, "%c", str[i]);
		else
			i += unicode_rep_no_coord(&str[i]) - 1;
		wrefresh(stdscr);
		usleep(time);
	}
	attroff(opt);
}
