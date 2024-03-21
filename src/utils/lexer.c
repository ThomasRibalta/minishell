#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/minishell.h"

// Fonction pour créer un nouveau token
Token* createToken(TokenType type, const char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->value = value != NULL ? strdup(value) : NULL;
    token->next = NULL;
    return token;
}

// Fonction pour ajouter un token à la fin de la liste
void appendToken(Token** head, TokenType type, const char* value) {
    Token* newToken = createToken(type, value);
    if (*head == NULL) {
        *head = newToken;
    } else {
        Token* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newToken;
    }
}

// Fonction pour libérer la liste des tokens
void freeTokens(Token* head) {
    Token* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        if (tmp->value != NULL) free(tmp->value);
        free(tmp);
    }
}

// Fonction pour afficher les tokens (pour démonstration)
void printTokens(const Token* head) {
    while (head != NULL) {
        printf("Token Type: %d, Value: %s\n", head->type, head->value ? head->value : "NULL");
        head = head->next;
    }
}

int in_special_zone(char *input, int i) 
{
    int j;
    int open_double_quote;
    int open_single_quote;
    int open_parenthesis;

    j = 0;
    open_double_quote = 0;
    open_single_quote = 0;
    open_parenthesis = 0;
    while (input[j] && j != i)
    {
        if (input[j] == '"') 
        {
            if (!open_single_quote && !open_parenthesis) 
            {
                if (!open_double_quote)
                    open_double_quote++;
                else
                    open_double_quote--;
            }
        } 
        else if (input[j] == '\'') 
        {
            if (!open_double_quote && !open_parenthesis) 
            {
                if (!open_single_quote)
                    open_single_quote++;
                else
                    open_single_quote--;
            }
        }
        else if (input[j] == '(') 
        {
            if (!open_double_quote && !open_single_quote) 
            {
                if (!open_parenthesis)
                    open_parenthesis++;
                else
                    open_parenthesis--;
            }
        }
        else if (input[j] == ')') 
        {
            if (!open_double_quote && !open_single_quote) 
            {
                if (!open_parenthesis)
                    open_parenthesis++;
                else
                    open_parenthesis--;
            }
        }
        j++;
    }

    return (open_double_quote || open_single_quote || open_parenthesis);
}

int is_separator(char *s, int i) {
    if ((s[i] == '|' && s[i + 1] == '|') || (s[i] == '&' && s[i + 1] == '&'))
        return 1;
    else if (s[i] == '<' || s[i] == '>' || s[i] == '|' | s[i] == '&')
        return 1;
    else if (s[i] == '>' && s[i + 1] == '>')
        return 1;
    else
        return 0;
}

int len_separator(char *s, int i)
{
    if ((s[i] == '|' && s[i + 1] == '|') || (s[i] == '&' && s[i + 1] == '&'))
        return 2;
    else if (s[i] == '>' && s[i + 1] == '>')
        return 2;
    else if (s[i] == '<' || s[i] == '>' || s[i] == '|')
        return 1;
    else
        return 0;
}

int check_separator(char *s, int i)
{
    if (!(in_special_zone(s, i)))
    {
        if (is_separator(s, i))
            return (1);
    }
    return (0);
}

int count_words(char *str)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (str[i])
    {
        if (!check_separator(str, i) && (check_separator(str, i + 1) || str[i + 1] == '\0'))
            j++;
        if (check_separator(str, i))
        {
            i += len_separator(str, i) - 1;
            j++;
        }
        i++;
    }
    return j;
}

int	ft_strlenc(char *str)
{
	int	i;

	i = 0;
    if (is_separator(str, 0))
    {
        return (len_separator(str, 0));
    }
	while (str[i] != '\0' && !check_separator(str, i))
		i++;
	return (i);
}
char	*get_word(char *str)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (ft_strlenc(str) + 1));
	while (i < ft_strlenc(str))
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char *get_separator(char *s)
{
    char *sep;
    int i;

    i = 0;
    sep = malloc(sizeof(char *) * (len_separator(s, 0) + 1));
    while (i < len_separator(s, 0))
    {
        sep[i] = s[i];
        i++;
    }
    sep[i] = '\0';
    return (sep);
}

char	**split_prompt(char *s)
{
	char	**tab;
	int		i;
    int     j;

	i = 0;
    j = 0;
	tab = malloc(sizeof(char *) * (count_words(s) + 1));
	if (!tab)
		return (NULL);
	while (s[j] != '\0')
	{
		while (s[j] != '\0' && check_separator(s, j))
        		j += len_separator(s + j, 0);
		if (s[j] != '\0')
		{
			tab[i] = get_word(s + j);
			i++;
            j += ft_strlenc(s + j);
            if (s[j] != '\0')
                tab[i++] = get_separator(s + j);
		}
	}
	tab[i] = 0;
	return (tab);
}

char   **aff_table(char **tab)
{
    int j;

    j = 0;
    printf("-=-=PROMPT APRES SPLIT=-=-\n\n");
    while (tab[j])
    {
        printf("%s\n", tab[j]);
        j++;
    }
    return (tab);
}

void chained_split_prompt(Token **list, char **tab)
{
    int j;

    j = 0;
    while (tab[j])
    {
        printf("%s : %d, %d\n", tab[j], tab[j][0], tab[j][1]);
        if (tab[j][0] == '(')
            appendToken(list, TOKEN_PAREN, tab[j]);
        else if (tab[j][0] == '|' && tab[j][1] == '\0')
            appendToken(list, TOKEN_PIPE, NULL);
        else if (tab[j][0] == '<' && tab[j][1] == '\0')
            appendToken(list, TOKEN_REDIRECTION_IN, tab[++j]);
        else if (tab[j][0] == '>' && tab[j][1] == '\0')
            appendToken(list, TOKEN_REDIRECTION_OUT, tab[++j]);
        else if (tab[j][0] == '>' && tab[j][1] == '>')
            appendToken(list, TOKEN_REDIRECTION_APPEND, tab[++j]);
        else if (tab[j][0] == '&' && tab[j][1] == '&')
            appendToken(list, TOKEN_LOGICAL_AND, NULL);
        else if (tab[j][0] == '|' && tab[j][1] == '|')
            appendToken(list, TOKEN_LOGICAL_OR, NULL);
        else
            appendToken(list, TOKEN_COMMAND, tab[j]);
        j++;
    }
}

int len_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
    {
        i++;
    }
    return (i);
}

char **tab_clean(char **tab)
{
    char **new_tab;
    int     j;

    j = 0;
    new_tab = malloc(sizeof(char *) * len_tab(tab));
    while (tab[j])
    {
        new_tab[j] = clean_white_space(tab[j]);
        j++;
    }
    free(tab);
    new_tab[j] = 0;
    aff_table(new_tab);
    return (new_tab);
}

void lexer(char *input)
{
    char **tab_input;
    Token *list = NULL;

    tab_input = split_prompt(input);
    aff_table(tab_input);
    chained_split_prompt(&list, tab_input);
    tab_input = tab_clean(tab_input);
    printf("\n-=-=Liste Chainée=-=-\n");
    printTokens(list);
    parser(list);
    freeTokens(list);
}
