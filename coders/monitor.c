/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 09:32:40 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/30 14:20:11 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop_all(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->mutex_state);
	sim->simulation_running = 0;
	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].alive = 0;
		i++;
	}
	pthread_mutex_unlock(&sim->mutex_state);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex_dongle);
		pthread_cond_broadcast(&sim->dongles[i].cond_dongle);
		pthread_mutex_unlock(&sim->dongles[i].mutex_dongle);
		i++;
	}
}

void	check_burnout(t_sim *sim)
{
	int		i;
	long	last;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->mutex_state);
		last = sim->coders[i].last_compile;
		pthread_mutex_unlock(&sim->mutex_state);
		if (get_time() - last > sim->time_to_burnout)
		{
			stop_all(sim);
			log_state(&sim->coders[i], "burned out");
			return ;
		}
		i++;
	}
}

void	check_compile_times(t_sim *sim)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	pthread_mutex_lock(&sim->mutex_state);
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_times >= sim->number_of_compiles_required)
			counter += 1;
		i++;
	}
	pthread_mutex_unlock(&sim->mutex_state);
	if (counter == sim->number_of_coders)
		stop_all(sim);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (sim->simulation_running)
	{
		check_burnout(sim);
		check_compile_times(sim);
		usleep(500);
	}
	return (NULL);
}
