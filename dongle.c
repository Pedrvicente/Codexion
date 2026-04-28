/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 13:39:06 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/28 16:45:19 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cooldown(t_dongle *dongle, int dongle_cool)
{
	while (get_time() - dongle->timestamp < dongle_cool)
		usleep(100);
}
