/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-al <pedde-al@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:56:50 by pedde-al          #+#    #+#             */
/*   Updated: 2026/04/23 13:35:06 by pedde-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim		*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (1);
	if (!validate_args(argc, argv) || !parse_args(argv, sim))
		return (error_exit(sim, NULL, NULL));
	start_simulation(sim);
}
