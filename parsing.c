/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:17:22 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/20 16:52:01 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int error_exit(t_sim *sim, t_dongle *dongles, t_coder *coders)
{
	if (sim)
		free(sim);
	if (coders)
		free(coders);
	if (dongles)
		free(dongles);
	return (1);
}

int	ft_is_int(char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (c[i] < '0' || c[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	validate_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 9)
	{
		printf("Error: not enough arguments\n");
		return (0);
	}
	while (i < 8)
	{
		if (!ft_is_int(argv[i]))
		{
			printf("Error %s: Argument must be and integer\n", argv[i]);
			return (0);
		}
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0  && strcmp(argv[8], "edf") != 0)
	{
		printf("Error: Value must be fifo or edf\n");
		return (1);
	}
	return (1);
}

int	parse_args(char **argv, t_sim *sim)
{
	sim->number_of_coders = atoi(argv[1]);
	sim->time_to_burnout = atoi(argv[2]);
	sim->time_to_compile = atoi(argv[3]);
	sim->time_to_debug = atoi(argv[4]);
	sim->time_to_refactor = atoi(argv[5]);
	sim->number_of_compiles_required = atoi(argv[6]);
	sim->dongle_cooldown= atoi(argv[7]);
	sim->scheduler = argv[8];
	return (1);
}
