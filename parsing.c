/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:17:22 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/28 17:18:16 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_is_int(char *c)
{
	int	i;

	i = 0;
	if (!c[0])
		return (0);
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
		printf("Error: Wrong number of arguments\n");
		return (0);
	}
	while (i < 8)
	{
		if (!ft_is_int(argv[i]))
		{
			printf("Error %s: Argument must be an integer\n", argv[i]);
			return (0);
		}
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		printf("Error: Value must be fifo or edf\n");
		return (0);
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
	sim->dongle_cooldown = atoi(argv[7]);
	sim->scheduler = argv[8];
	if (sim->number_of_coders <= 0
    || sim->time_to_burnout <= 0
    || sim->time_to_compile <= 0
    || sim->time_to_debug < 0
    || sim->time_to_refactor < 0
    || sim->number_of_compiles_required <= 0
    || sim->dongle_cooldown < 0)
	{
		printf("Error: arguments out of valid range\n");
		return (0);
	}
	return (1);
}
