/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:09:49 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/20 18:15:47 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_left_first(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	pthread_mutex_lock(&left->mutex_dongle);
	while (!left->available)
		pthread_cond_wait(&left->cond_dongle, &left->mutex_dongle);
	left->available = 0;
	pthread_mutex_unlock(&left->mutex_dongle);
	pthread_mutex_lock(&right->mutex_dongle);
	while (!right->available)
		pthread_cond_wait(&right->cond_dongle, &right->mutex_dongle);
	right->available = 0;
	pthread_mutex_unlock(&right->mutex_dongle);
}

void	take_right_first(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	pthread_mutex_lock(&right->mutex_dongle);
	while (!right->available)
		pthread_cond_wait(&right->cond_dongle, &right->mutex_dongle);
	right->available = 0;
	pthread_mutex_unlock(&right->mutex_dongle);
	pthread_mutex_lock(&left->mutex_dongle);
	while (!left->available)
		pthread_cond_wait(&left->cond_dongle, &left->mutex_dongle);
	left->available = 0;
	pthread_mutex_unlock(&left->mutex_dongle);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex_dongle);
	dongle->available = 1;
	pthread_cond_broadcast(&dongle->cond_dongle);
	pthread_mutex_unlock(&dongle->mutex_dongle);
}

void	compile(t_coder *coder, t_dongle *left, t_dongle *right)
{
	if (coder->id % 2 == 0)
	{
		take_left_first(coder);
		usleep(coder->sim->time_to_compile * 1000);
		release_dongle(left);
		release_dongle(right);
	}
	else
	{
		take_right_first(coder);
		usleep(coder->sim->time_to_compile * 1000);
		release_dongle(right);
		release_dongle(left);
	}
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;

	coder = (t_coder *)arg;
	while (coder->alive)
	{
		left = coder->left_dongle;
		right = coder->right_dongle;
		compile(coder, left, right);
		usleep(coder->sim->time_to_debug * 1000);
		usleep(coder->sim->time_to_refactor * 1000);
	}
}
