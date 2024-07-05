/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokinize_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mregrag <mregrag@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 22:02:54 by mregrag           #+#    #+#             */
/*   Updated: 2024/07/02 00:02:17 by mregrag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token *new_token(char *value, t_type type)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return NULL;
    }
    token->type = type;
    token->next = NULL;
    return token;
}

void token_add_back(t_token **lst, t_token *new)
{
    t_token *last;

    if (!lst || !new)
        return;
    if (!*lst)
    {
        *lst = new;
        return;
    }
    last = *lst;
    while (last->next)
        last = last->next;
    last->next = new;
    free(new);

}


void clear_token(t_token **tokens)
{
    t_token *current;
    t_token *next;

    if (!tokens || !*tokens)
        return;

    current = *tokens;
    while (current)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }

    *tokens = NULL;
}


int ft_lstsize_token(t_token *lst)
{
    int count;

    count = 0;
    while (lst)
    {
        count++;
        lst = lst->next;
    }
    return count;
}
