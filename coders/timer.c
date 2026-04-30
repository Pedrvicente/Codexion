/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 18:55:28 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/30 12:19:52 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	is_running(t_sim *sim)
{
	int	running;
	
	pthread_mutex_lock(&sim->mutex_state);
	running = sim->simulation_running;
	pthread_mutex_unlock(&sim->mutex_state);
	return (running);
}

int	is_alive(t_coder *coder)
{
	int	alive;
	
	pthread_mutex_lock(&coder->sim->mutex_state);
	alive = coder->alive;
	pthread_mutex_unlock(&coder->sim->mutex_state);
	return (alive);
}

void	precise_sleep(t_sim *sim, long duration)
{
	long	start;
	long	remaining;

	start = get_time();
	while (get_time() - start < duration)
	{
		if (!is_running(sim))
			return ;
		remaining = duration - (get_time() - start);
		if (remaining > 10)
			usleep(5000);
		else
			usleep(500);
	}
}

void	log_state(t_coder *coder, char *message)
{
	pthread_mutex_lock(&coder->sim->mutex_log);
	if (coder->sim->simulation_running || strcmp(message, "burned out") == 0)
		printf("%ld %d %s\n",
			get_time() - coder->sim->start_time, coder->id, message);
	pthread_mutex_unlock(&coder->sim->mutex_log);
}
