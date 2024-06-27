/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mregrag <mregrag@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 21:45:53 by mregrag           #+#    #+#             */
/*   Updated: 2024/06/27 22:08:07 by mregrag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expansion_input(char *str, t_list *env)
{
	size_t	i;
	char	*ret;

	ret = strdup("");
	i = 0;
	if (ft_issamechar(str, '$') && (ft_strlen(str) % 2) == 0)
		return (ft_itoa(getpid()));
	while (str[i])
	{
		if (str[i] == '\'')
			ret = handle_single_quotes(ret, str, &i);
		else if (str[i] == '"')
			ret = handle_double_quotes(ret, str, &i, env);
		else if (str[i] == '$')
			ret = handle_dollar(ret, str, &i, env);
		else
			ret = handle_normal(ret, str, &i);
	}
	if (!ret)
		return (NULL);
	return (remov_quotes(ret));
}
