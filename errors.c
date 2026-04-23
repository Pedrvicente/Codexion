/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 19:25:35 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/22 19:26:49 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	error_exit(t_sim *sim, t_dongle *dongles, t_coder *coders)
{
	if (sim)
		free(sim);
	if (coders)
		free(coders);
	if (dongles)
		free(dongles);
	return (1);
}

void	free_exit(t_sim *sim, t_dongle *dongles, t_coder *coders)
{
	free(sim);
	free(dongles);
	free(coders);
}