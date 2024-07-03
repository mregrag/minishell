/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mregrag <mregrag@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 21:23:34 by mregrag           #+#    #+#             */
/*   Updated: 2024/07/04 00:21:22 by mregrag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_node	*new_node(t_type type)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = NULL;
	node->left = NULL;
	node->right = NULL;
	node->flag = 0;
	return (node);
}


void	free_tree(t_node *node)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	if (node->type == T_WORD && node->cmd)
	{
		while (node->cmd && node->cmd[i])
		{
			free(node->cmd[i]);
			i++;
		}
		free(node->cmd);
	}
	free_tree(node->left);
	free_tree(node->right);
	free(node);
}

t_node	*create_redire(t_token **tokens, t_token *tmp, t_env *env)
{
	t_node	*node;

	node = new_node((*tokens)->type);
	*tokens = (*tokens)->next->next;
	node->left = parse_redire(tokens, env);
	node->right = create_file(tmp->next, tmp->type, env);
	free(tmp);
	return (node);
}

void	creat_cmd(t_node *node, t_token **tokens, int count, t_env *env)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (i < count)
	{
		node->cmd[i] = expansion_input((*tokens)->value, env);
		tmp = *tokens;
		*tokens = (*tokens)->next;
		i++;
	}
	node->cmd[count] = NULL;
}
