/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:09:49 by pedde-al          #+#    #+#             */
/*   Updated: 2026/05/04 13:21:50 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_left_first(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	coder->arrival_time = get_time();
	wait_for_dongle(left, coder);
	if (!is_running(coder->sim))
		return ;
	log_state(coder, "has taken a dongle");
	wait_for_dongle(right, coder);
	if (!is_running(coder->sim))
		return ;
	log_state(coder, "has taken a dongle");
}

void	take_right_first(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	coder->arrival_time = get_time();
	wait_for_dongle(right, coder);
	if (!is_alive(coder))
		return ;
	log_state(coder, "has taken a dongle");
	wait_for_dongle(left, coder);
	if (!is_running(coder->sim))
		return ;
	log_state(coder, "has taken a dongle");
}

void	release_dongle(t_dongle *dongle, int dongle_cooldown)
{
	(void)dongle_cooldown;
	pthread_mutex_lock(&dongle->mutex_dongle);
	dongle->timestamp = get_time();
	dongle->available = 1;
	pthread_cond_broadcast(&dongle->cond_dongle);
	pthread_mutex_unlock(&dongle->mutex_dongle);
}

void	compile(t_coder *coder, t_dongle *left, t_dongle *right)
{
	if (coder->id % 2 == 0)
		take_left_first(coder);
	else
		take_right_first(coder);
	if (!is_running(coder->sim))
		return ;
	pthread_mutex_lock(&coder->sim->mutex_state);
	coder->compile_times += 1;
	coder->last_compile = get_time();
	pthread_mutex_unlock(&coder->sim->mutex_state);
	log_state(coder, "is compiling");
	precise_sleep(coder->sim, coder->sim->time_to_compile);
	if (coder->id % 2 == 0)
	{
		release_dongle(left, coder->sim->dongle_cooldown);
		release_dongle(right, coder->sim->dongle_cooldown);
	}
	else
	{
		release_dongle(right, coder->sim->dongle_cooldown);
		release_dongle(left, coder->sim->dongle_cooldown);
	}
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;

	coder = (t_coder *)arg;
	while (is_alive(coder) && is_running(coder->sim))
	{
		left = coder->left_dongle;
		right = coder->right_dongle;
		compile(coder, left, right);
		if (!is_running(coder->sim))
			return (NULL);
		log_state(coder, "is debugging");
		precise_sleep(coder->sim, coder->sim->time_to_debug);
		log_state(coder, "is refactoring");
		precise_sleep(coder->sim, coder->sim->time_to_refactor);
	}
	return (NULL);
}
