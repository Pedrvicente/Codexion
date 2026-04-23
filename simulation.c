/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 12:28:49 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/23 12:50:44 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	thread_loop(t_sim *sim)
{
	pthread_t	*coders_threads;
	int			i;

	coders_threads = malloc(sizeof(pthread_t) * sim->number_of_coders);
	if (!coders_threads)
		return (1);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_create(&coders_threads[i], NULL, coder_routine, &sim->coders[i]);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(coders_threads[i], NULL);
		i++;
	}
	free(coders_threads);
	return (0);
}

int	start_simulation(t_sim *sim)
{
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	monitor_thread;

	dongles = init_dongles(sim->number_of_coders);
	if (!dongles)
		return (error_exit(sim, NULL, NULL));
	coders = init_coders(sim->number_of_coders, dongles, sim);
	if (!coders)
		return (error_exit(sim, dongles, NULL));
	pthread_mutex_init(&sim->mutex_log, NULL);
	sim->coders = coders;
	sim->start_time = get_time();
	sim->simulation_running = 1;
	if (sim->number_of_coders == 1)
	{
		log_state(sim->coders, "burned out");
		return (error_exit(sim, NULL, NULL));
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, sim);
	thread_loop(sim);
	pthread_join(monitor_thread, NULL);
	free_exit(sim, dongles, coders);
	return (0);
}
