/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rh_printf_intern.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhaas <rhaas@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:55:46 by raphha            #+#    #+#             */
/*   Updated: 2025/09/18 11:35:39 by rhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RH_PRINTF_INTERN_H
# define RH_PRINTF_INTERN_H

# include <stdarg.h>
# include <stddef.h>

typedef enum e_fsflag
{
	lftjust = 0x01,
	forcesign = 0x02,
	spacesign = 0x04,
	altfmt = 0x08,
	lftpad0 = 0x10,
}	t_fsflag;

typedef struct s_fmtspec
{
	int		fsflags;
	size_t	fieldwidth;
	int		precision;
	char	convspec;
	int		is_valid;
	int		fmtstrlen;
}			t_fmtspec;

typedef struct s_outstr_fmtspec
{
	size_t	strlen;
	size_t	fieldwidth;
	int		zeropadsize;
	char	*hex;
	char	signc;
}				t_outstr_fmtspec;

typedef struct s_bytecount
{
	int	lastwrite;
	int	total;
}				t_bytecount;

int		rh_set_formatspec(const char **pfmt, t_fmtspec *fmtspec);
int		rh_actflx(t_fsflag flag, const t_fmtspec *const fmtspec);
int		rh_convspec_c(va_list *valst, const t_fmtspec *const fmtspec);
int		rh_convspec_s(va_list *valst, const t_fmtspec *const fmtspec);
int		rh_convspec_p(va_list *valst, t_fmtspec *const fmtspec);
int		rh_convspec_diu(va_list *valst, const t_fmtspec *const fmtspec);
int		rh_convspec_x(va_list *valst, t_fmtspec *const fmtspec);
int		rh_convspec_invalidfs(const t_fmtspec *const fmtspec);
int		rh_convspec_percent(void);

int		rh_write_outstr(
			const char *const str,
			const t_fmtspec *const fmtspec,
			const char signc);
int		padding_left(
			const t_fmtspec *const fmtspec,
			t_outstr_fmtspec *poutfs);
int		padding_right(const int outlen_sofar, const t_fmtspec *const fmtspec);
char	rh_signchar(const int n, const t_fmtspec *const fmtspec);

int		bytecount_eval(t_bytecount *const bcp);
int		try_write(
			t_bytecount *const pbc,
			const char *const str, const size_t len);

#endif //RH_PRINTF_INTERN_H
