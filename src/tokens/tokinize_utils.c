/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokinize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mregrag <mregrag@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 23:05:33 by mregrag           #+#    #+#             */
/*   Updated: 2024/08/01 13:38:50 by mregrag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*extract_word(char **input)
{
	char	*start;
	size_t	len;
	char	quote;

	quote = 0;
	start = *input;
	while (**input)
	{
		if (!quote && is_operator(*input))
			break ;
		if (!quote && ft_isspace(**input))
			break ;
		if (ft_isquotes(**input))
		{
			if (!quote)
				quote = **input;
			else if (**input == quote)
				quote = 0;
		}
		(*input)++;
	}
	len = *input - start;
	return (ft_substr(start, 0, len));
}

int	add_split_tokens(t_token **tokens, char *expanded_word)
{
	char	**split_words;
	int		i;

	i = 0;
	split_words = ft_split(expanded_word, ' ');
	if (!split_words)
		return (0);
	while (split_words[i])
	{
		token_add_back(tokens, new_token(split_words[i], T_CMD));
		i++;
	}
	return (free(split_words), 1);
}

int	check_quotes(char **line)
{
	size_t	i;

	i = 0;
	while ((*line)[i])
	{
		if (ft_isquotes((*line)[i]) && !skip_quotes(*line, &i))
			return (print_error("minish", "unexpected EOF `", NULL, NULL), 0);
		else
			i++;
	}
	return (1);
}

t_type	get_operator_type(char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0)
		return (T_HERDOC);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (T_APPEND);
	if (*str == '<')
		return (T_IN);
	if (*str == '>')
		return (T_OUT);
	if (*str == '|')
		return (T_PIPE);
	else
		return (T_CMD);
	return (T_CMD);
}
