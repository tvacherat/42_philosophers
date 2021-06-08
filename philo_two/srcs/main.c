/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvachera <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 12:07:06 by tvachera          #+#    #+#             */
/*   Updated: 2021/06/07 17:19:12 by tvachera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	main(int argc, char **argv)
{
	t_pars			pars;
	sem_t			*print;
	int				ret;

	ret = 0;
	if (argc < 5 || argc > 6 || !check_args(argc, argv, &pars))
	{
		write(2, "Wrong arguments\n", 16);
		return (1);
	}
	if (!pars.nb_meals)
	{
		printf("Everyone ate well, even though no one ate\n");
		return (0);
	}
	sem_unlink("print");
	print = sem_open("print", O_CREAT, S_IRWXU, 1);
	if (!launch_threads(&pars, print))
		ret = 1;
	sem_close(print);
	sem_unlink("print");
	return (ret);
}
