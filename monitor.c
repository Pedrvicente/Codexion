/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 09:32:40 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/28 18:08:49 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	check_burnout(t_sim *sim)
{
	int	i;
	int	j;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (get_time() - sim->coders[i].last_compile > sim->time_to_burnout)
		{
			log_state(&sim->coders[i], "burned out");
			sim->simulation_running = 0;
			j = 0;
			while (j < sim->number_of_coders)
			{
				pthread_mutex_lock(&sim->dongles[j].mutex_dongle);
				pthread_cond_broadcast(&sim->dongles[j].cond_dongle);
				pthread_mutex_unlock(&sim->dongles[j].mutex_dongle);
				sim->coders[j].alive = 0;
				j++;
			}
			sim->simulation_running = 0;
			return ;
		}
		i++;
	}
	return ;
}

void	check_compile_times(t_sim *sim)
{
	int	i;
	int	j;
	int	counter;

	i = 0;
	counter = 0;
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_times >= sim->number_of_compiles_required)
			counter += 1;
		if (counter == sim->number_of_coders)
		{
			j = 0;
			while (j < sim->number_of_coders)
			{
				sim->coders[j].alive = 0;
				j++;
			}
			sim->simulation_running = 0;
			return ;
		}
		i++;
	}
	return ;
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (sim->simulation_running)
	{
		check_burnout(sim);
		check_compile_times(sim);
	}
	return (NULL);
}
