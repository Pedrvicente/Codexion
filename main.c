/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:56:50 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/23 10:36:59 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim		*sim;
	t_dongle	*dongles;
	t_coder		*coders;
	pthread_t	*coders_threads;
	pthread_t	monitor_thread;
	int			i;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (1);
	if (!validate_args(argc, argv) || !parse_args(argv, sim))
		return (error_exit(sim, NULL, NULL));
	dongles = init_dongles(sim->number_of_coders);
	if (!dongles)
		return (error_exit(sim, NULL, NULL));
	coders = init_coders(sim->number_of_coders, dongles, sim);
	if (!coders)
		return (error_exit(sim, dongles, NULL));
	sim->coders = coders;
	pthread_mutex_init(&sim->mutex_log, NULL);
	sim->start_time = get_time();
	sim->simulation_running = 1;
	coders_threads = malloc(sizeof(pthread_t) * sim->number_of_coders);
	if (!coders_threads)
		return (error_exit(sim, dongles, NULL));
	i = 0;
	pthread_create(&monitor_thread, NULL, monitor_routine, sim);
	while (i < sim->number_of_coders)
	{
		pthread_create(&coders_threads[i], NULL, coder_routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(coders_threads[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	free_exit(sim, dongles, coders);
	return (0);
}
