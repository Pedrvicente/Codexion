/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:56:50 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/22 19:41:32 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
	t_sim		*sim;
	t_dongle	*dongles;
	t_coder		*coders;
	pthread_t	*threads;
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
	pthread_mutex_init(&sim->mutex_log, NULL);
	sim->start_time = get_time();
	threads = malloc(sizeof(pthread_t) * sim->number_of_coders);
	if (!threads)
		return (error_exit(sim, dongles, NULL));
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_create(&threads[i], NULL, coder_routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free_exit(sim, dongles, coders);
	return (0);
}
