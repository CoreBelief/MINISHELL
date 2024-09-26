

// void	variable_exp_dollar(t_token *token, char *str, t_shell *shell)
// {
// 	int		i;
// 	char	*expansion;
// 	char	*new_str;

// 	i = 0;
// 	expansion = NULL;
// 	new_str = ft_strdup("");
// 	//error handling
// 	if (str[i] == '$' && str[i + 1])
// 	{
// 		if (str[i + 1] == '?')
// 		{
// 			expansion = var_exp_exit(&i, shell);
// 		}
// 		else
// 		{
// 			i++;
// 			expansion = variable_exp(str, &i, shell);
// 			if (!expansion)
// 			{
// 				printf("expansion variable not found\n");
// 				// free (token->content);
// 				// token->content = "";
// 				// return ;
// 			}
// 		}
// 		new_str = append_str(new_str, expansion);
// 		new_str = append_str(new_str, &str[i]);
// 		token->content = new_str;
// 		// printf("token->cont %s\n", token->content);
// 		free (str);
// 	}
// }