/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkoualil <mkoualil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 21:22:11 by mregrag           #+#    #+#             */
/*   Updated: 2024/07/10 22:32:02 by mkoualil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_node	*parse_command(t_token **tokens, t_env *env)
{
	t_node	*node;
	t_token	*tmp;
	int		count;
	int		i;

	node = new_node(T_WORD);
	if (!node)
		return (NULL);
	count = ft_lstsize_token(*tokens);
	node->cmd = malloc(sizeof(char *) * (count + 1));
	if (!node->cmd)
		return (NULL);
	i = 0;
	while (i < count)
	{
		node->cmd[i] = expansion_input((*tokens)->value, env);
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free_token(tmp);
		i++;
	}
	node->cmd[count] = NULL;
	return (node);
}

t_node	*parse_file(t_token *token, t_type type, t_env *env)
{
	t_node	*node;

	node = new_node(type);
	if (!node)
		return (NULL);
	node->cmd = malloc(sizeof(char *) * 2);
	if (!node->cmd)
		return (NULL);
	if (type == T_IN || type == T_APPEND || type == T_OUT)
	{
		node->cmd[0] = expansion_input(token->value, env);
		free(token->value);
	}
	else if (type == T_HERDOC)
		node->cmd[0] = token->value;
	node->cmd[1] = NULL;
	free(token);
	return (node);
}

t_node    *create_redire(t_token **tokens, t_token *tmp, t_env *env)
{
    t_node    *node;

    node = new_node((*tokens)->type);
    *tokens = (*tokens)->next->next;
    node->left = parse_redire(tokens, env);
    node->right = parse_file(tmp->next, tmp->type, env);
    free(tmp);
    return (node);
}
t_node    *parse_redire(t_token **tokens, t_env *env)
{
    t_token    *current;
    t_token    *redir_token;
    t_node    *node;

    if (!*tokens)
        return (NULL);
    current = *tokens;
    if ((*tokens)->type >= T_IN && (*tokens)->type <= T_HERDOC)
        return (create_redire(tokens, current, env));
    while (current && current->next)
    {
        if (is_redirection(current->next->type))
        {
            node = new_node(current->next->type);
            redir_token = current->next;
            current->next = redir_token->next->next;
            node->left = parse_redire(tokens, env);
            node->right = parse_file(redir_token->next, redir_token->type, env);
            free_token(redir_token);
            return (node);
        }
        current = current->next;
    }
    return (parse_command(tokens, env));
}

t_node	*parse_expression(t_token **tokens, t_env *env)
{
	t_token	*tmp;
	t_token	*next_token;
	t_node	*node;

	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->type == T_PIPE)
		{
			node = new_node(T_PIPE);
			(*tokens)->next = NULL;
			node->left = parse_redire(&tmp, env);
			node->right = parse_expression(&(next_token->next), env);
			free_token(next_token);
			return (node);
		}
		*tokens = next_token;
	}
	return (parse_redire(&tmp, env));
}

t_node	*parse_tokens(t_token **tokens, t_env *env)
{
	if (!tokens || !*tokens)
		return (NULL);
	if (!check_syntax(*tokens))
	{
		clear_tokens(tokens);
		return (NULL);
	}
	return (parse_expression(tokens, env));
}
