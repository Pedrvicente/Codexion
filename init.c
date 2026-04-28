/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:19:22 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/28 17:10:38 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*init_dongles(int n)
{
	t_dongle	*dongles;
	int			i;

	i = 0;
	dongles = malloc(sizeof(t_dongle) * n);
	if (!dongles)
		return (NULL);
	while (i < n)
	{
		pthread_mutex_init(&dongles[i].mutex_dongle, NULL);
		pthread_cond_init(&dongles[i].cond_dongle, NULL);
		dongles[i].available = 1;
		dongles[i].timestamp = 0;
		dongles[i].queue_size = 0;
		i++;
	}
	return (dongles);
}

t_coder	*init_coders(int n, t_dongle *dongles, t_sim *sim)
{
	t_coder	*coders;
	int		i;

	i = 0;
	coders = malloc(sizeof(t_coder) * n);
	if (!coders)
		return (NULL);
	while (i < n)
	{
		coders[i].id = i + 1;
		coders[i].compile_times = 0;
		coders[i].alive = 1;
		coders[i].last_compile = sim->start_time;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % n];
		coders[i].sim = sim;
		i++;
	}
	return (coders);
}
