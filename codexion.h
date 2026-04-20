/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:13:45 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/20 16:52:24 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>

typedef struct s_sim
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_sim;

typedef struct s_dongle
{
	pthread_mutex_t		mutex_dongle;
	pthread_cond_t		cond_dongle;
	int					available;
	long				timestamp;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_times;
	int				alive;
	long			last_compile;
	struct s_dongle	*left_dongle;
	struct s_dongle	*right_dongle;
	t_sim			*sim;
}	t_coder;

int			validate_args(int argc, char **argv);
int			parse_args(char **argv, t_sim *sim);
int			ft_is_int(char *c);
t_dongle	*init_dongles(int n);
t_coder		*init_coders(int n, t_dongle *dongles, t_sim *sim);
int 		error_exit(t_sim *sim, t_dongle *dongles, t_coder *coders);


#endif
