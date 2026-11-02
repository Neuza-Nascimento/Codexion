/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_heap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 11:26:58 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/11/02 18:29:42 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_swap(t_request *frist, t_request *second)
{
	t_request	temp;

	temp = *frist;
	*frist = *second;
	*second = temp;
}

static int	has_priority(t_request *request1, t_request *request2,
		t_table *table)
{
	long	deadline1;
	long	deadline2;

	if (table->scheduler == EDF)
	{
		deadline1 = request1->dealine;
		deadline2 = request2->dealine;
		if (deadline1 != deadline2)
			return (deadline1 < deadline2);
		return (request1->ticket < request2->ticket);
	}
	return (request1->ticket < request2->ticket);
}

static int	the_best_child(t_dongle *dongle, int pos, t_table *table)
{
	int	right;
	int	left;
	int	best;

	right = (2 * pos) + 2;
	left = (2 * pos) + 1;
	best = left;
	if (right >= dongle->len_requests)
		return (best);
	if (has_priority(&dongle->requests[right], &dongle->requests[left], table))
		best = right;
	return (best);
}

void	push_request(t_dongle *dongle, t_request *new_request, t_table *table)
{
	int	pos;
	int	parent;

	pos = dongle->len_requests;
	dongle->requests[pos] = *new_request;
	pos = dongle->len_requests++;
	while (pos > 0)
	{
		parent = (pos - 1) / 2;
		if (has_priority(&dongle->requests[pos], &dongle->requests[parent],
				table))
		{
			ft_swap(&dongle->requests[pos], &dongle->requests[parent]);
			pos = parent;
		}
		else
			break ;
	}
}

void	remove_top(t_dongle *dongle, t_table *table)
{
	int	pos;
	int	best;

	if (dongle->len_requests == 0)
		return ;
	dongle->requests[0] = dongle->requests[dongle->len_requests - 1];
	dongle->len_requests--;
	pos = 0;
	while ((2 * pos) + 1 < dongle->len_requests)
	{
		best = the_best_child(dongle, pos, table);
		if (has_priority(&dongle->requests[best], &dongle->requests[pos],
				table))
		{
			ft_swap(&dongle->requests[pos], &dongle->requests[best]);
			pos = best;
		}
		else
			break ;
	}
}
