/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 11:26:58 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/30 15:49:50 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void ft_swap(t_request **frist, t_request **second)
{
    t_request *temp;

    temp = *frist;
    *frist = *second;
    *second = temp;
}

int has_priority(t_request *request1, t_request *request2, t_table *table)
{
    long deadline1;
    long deadline2;
    
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

void push_request(t_dongle *dongle, t_request *new_request, t_table *table)
{
    int pos;
    int parent;

    pos = dongle->len_requests;
    dongle->requests[pos] = new_request;
    pos = dongle->len_requests ++;
    while (pos > 0)
    {
        parent = (pos - 1) / 2;
        if (has_priority(dongle->requests[pos], dongle->requests[parent], table))
        {
            ft_swap(&dongle->requests[pos], &dongle->requests[parent]);
            pos = parent;   
        }
        else
            break;

    }   
}

void remove_top(t_dongle *dongle, t_request *new_request, t_table *table)
{
    int pos;
    int son_right;
    int son_left;
    t_request *request;

    request = dongle->requests[0];
    dongle->requests[0] = dongle->requests[dongle->len_requests - 1];
    dongle->len_requests --;
    
    son_left = 0;
    while (son_left < dongle->len_requests)
    {
        son_left = (pos - 1) / 2;
        if (has_priority(dongle->requests[pos], dongle->requests[parent], table))
        {
            ft_swap(&dongle->requests[pos], &dongle->requests[parent]);
            pos = parent;   
        }
        else
            break;

    }   
}