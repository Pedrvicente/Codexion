/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:56:50 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/20 16:56:51 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
	t_sim		*sim;
	t_dongle	*dongles;
	t_coder		*coders;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (1);
	if (!validate_args(argc, argv) || !parse_args(argv, sim))
		return (error_exit(sim, NULL, NULL));
	dongles = init_dongles(sim->number_of_coders);
	if (!dongles)
		return (error_exit(sim, NULL, NULL));
	coders = init_coders(sim->number_of_coders, dongles, sim);
	if (!coders)
		return (error_exit(sim, dongles, NULL));
	error_exit(sim, dongles, coders);
	return (0);
}
