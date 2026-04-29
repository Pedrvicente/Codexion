/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:09:57 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/29 14:56:16 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	enqueue_fifo(t_dongle *dongle, t_coder *coder)
{
	t_request	tmp;

	dongle->queue[dongle->queue_size].coder_id = coder->id;
	dongle->queue[dongle->queue_size].arrival_time = coder->arrival_time;
	dongle->queue_size += 1;
	if (dongle->queue_size > 1)
	{
		if (dongle->queue[0].arrival_time > dongle->queue[1].arrival_time)
		{
			tmp = dongle->queue[0];
			dongle->queue[0] = dongle->queue[1];
			dongle->queue[1] = tmp;
		}
	}
}

void	enqueue_edf(t_dongle *dongle, t_coder *coder)
{
	t_request	tmp;

	dongle->queue[dongle->queue_size].coder_id = coder->id;
	dongle->queue[dongle->queue_size].deadline = coder->last_compile
		+ coder->sim->time_to_burnout;
	dongle->queue_size += 1;
	if (dongle->queue_size > 1)
	{
		if (dongle->queue[0].deadline > dongle->queue[1].deadline)
		{
			tmp = dongle->queue[0];
			dongle->queue[0] = dongle->queue[1];
			dongle->queue[1] = tmp;
		}
	}
}

void	enqueue(t_dongle *dongle, t_coder *coder)
{
	if (strcmp(coder->sim->scheduler, "fifo") == 0)
		enqueue_fifo(dongle, coder);
	else
		enqueue_edf(dongle, coder);
}

void	dequeue(t_dongle *dongle)
{
	if (dongle->queue_size <= 0)
		return ;
	dongle->queue[0] = dongle->queue[1];
	dongle->queue_size -= 1;
}

void	wait_for_dongle(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex_dongle);
	enqueue(dongle, coder);
	while ((!dongle->available || dongle->queue[0].coder_id != coder->id)
		&& coder->sim->simulation_running)
		pthread_cond_wait(&dongle->cond_dongle, &dongle->mutex_dongle);
	if (!coder->sim->simulation_running)
	{
		dequeue(dongle);
		pthread_mutex_unlock(&dongle->mutex_dongle);
		return ;
	}
	while (get_time() - dongle->timestamp < coder->sim->dongle_cooldown
		&& coder->sim->simulation_running)
	{
		pthread_mutex_unlock(&dongle->mutex_dongle);
		usleep(100);
		pthread_mutex_lock(&dongle->mutex_dongle);
	}
	dongle->available = 0;
	dequeue(dongle);
	pthread_mutex_unlock(&dongle->mutex_dongle);
}
