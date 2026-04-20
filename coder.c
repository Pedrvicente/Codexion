/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:09:49 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/20 17:57:56 by pedde-al         ###   ########.fr       */
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

void	*coder_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;
	while (coder->alive)
	{
		if (coder->id % 2 == 0)
			take_left_first(coder);
		else
			take_right_first(coder);
	}
}
