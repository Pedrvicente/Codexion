/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 18:55:28 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/22 19:16:44 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
void	log_state(t_coder *coder, char *message)
{
	long			time;
	
	pthread_mutex_lock(&coder->sim->mutex_log);
	time = get_time() - coder->sim->start_time;
	printf("%ld %d %s\n", time, coder->id, message);
	pthread_mutex_unlock(&coder->sim->mutex_log);
}