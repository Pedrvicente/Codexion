/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 19:25:35 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/29 09:58:32 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	error_exit(t_sim *sim, t_dongle *dongles, t_coder *coders)
{
	int	i;

	if (dongles)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			pthread_mutex_destroy(&dongles[i].mutex_dongle);
			pthread_cond_destroy(&dongles[i].cond_dongle);
			i++;
		}
		free(dongles);
	}
	if (sim)
	{
		pthread_mutex_destroy(&sim->mutex_log);
		free(sim);
	}
	if (coders)
		free(coders);
	return (1);
}

void	free_exit(t_sim *sim, t_dongle *dongles, t_coder *coders)
{
	error_exit(sim, dongles, coders);
}
