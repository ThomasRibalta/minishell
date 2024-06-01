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

#define NUM_SYMBOLS 7

const char *symbols[NUM_SYMBOLS] = {"||", "&&", ">>", ">", "<<", "<", "|"};

int is_symbol(const char *str, int *symbol_length) {
    for (int i = 0; i < NUM_SYMBOLS; i++) {
        int len = strlen(symbols[i]);
        if (strncmp(str, symbols[i], len) == 0) {
            *symbol_length = len;
            return 1;
        }
    }
    return 0;
}

int count_tokens(const char *input) {
    int count = 0;
    const char *start = input;
    const char *current = input;
    int i;

    i = 0;
    while (current[i]) {
        int symbol_length = 0;
        if (is_symbol(current + i, &symbol_length) && !in_special_zone((char *)current, i)){
            if (current + i > start) {
                count++;
            }
            count++;
            i += symbol_length;
            start = current + i;
        } else {
            i++;
        }
    }
    if (current + i > start)
        count++;
    return count;
}

char **split_with_symbols(const char *input) {
    int token_count = count_tokens(input);
    char **result = (char **)malloc(sizeof(char *) * (token_count + 1));
    int count = 0;
    const char *start = input;
    const char *current = input;
    int i;

    i = 0;
    while (current[i]) {
        int symbol_length = 0;
        if (is_symbol(current + i, &symbol_length) && !in_special_zone((char *)current, i)){
            if (current + i > start) {
                int len = (current + i) - start;
                char *token = (char *)malloc(len + 1);
                strncpy(token, start, len);
                token[len] = '\0';
                result[count++] = token;
            }

            char *symbol = (char *)malloc(symbol_length + 1);
            strncpy(symbol, current + i, symbol_length);
            symbol[symbol_length] = '\0';
            result[count++] = symbol;

            i += symbol_length;
            start = current + i;
        } else {
            i++;
        }
    }

    if (current + i > start) {
        int len = (current + i) - start;
        char *token = (char *)malloc(len + 1);
        strncpy(token, start, len);
        token[len] = '\0';
        result[count++] = token;
    }

    result[count] = NULL;

    return result;
}

char *here_doc(char *end)
{
    char *value;
    char	*tmp;
	int		i;
    char *tmp2;

	i = 0;
    value = malloc(sizeof(char *));
    value[0] = '\0';
    tmp2 = clean_quote(ft_strdup(end));
	while (i != 1)
	{
		write(1, "Here_doc >> ", 13);
		tmp = get_next_line(0);
		if (ft_strncmp(tmp, tmp2, ft_strlen(end)) == 0)
			break ;
        value = ft_strjoin(value, tmp);
        free(tmp);
	}
    free(tmp2);
    return (value);
}

void chained_split_prompt(Token **list, char **tab)
{
    int j;

    j = 0;
    while (tab[j])
    {
        if (tab[j][0] == '(')
            appendToken(list, TOKEN_PAREN, tab[j]);
        else if (tab[j][0] == '|' && tab[j][1] == '\0')
            appendToken(list, TOKEN_PIPE, tab[j]);
        else if (tab[j][0] == '<' && tab[j][1] == '\0')
            appendToken(list, TOKEN_REDIRECTION_IN, tab[++j]);
        else if (tab[j][0] == '>' && tab[j][1] == '\0')
            appendToken(list, TOKEN_REDIRECTION_OUT, tab[++j]);
        else if (tab[j][0] == '>' && tab[j][1] == '>')
            appendToken(list, TOKEN_REDIRECTION_APPEND, tab[++j]);
        else if (tab[j][0] == '<' && tab[j][1] == '<')
            appendToken(list, TOKEN_HEREDOC, here_doc(tab[++j]));
        else if (tab[j][0] == '&' && tab[j][1] == '&')
            appendToken(list, TOKEN_LOGICAL_AND, tab[j]);
        else if (tab[j][0] == '|' && tab[j][1] == '|')
            appendToken(list, TOKEN_LOGICAL_OR, tab[j]);
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
        i++;
    return (i);
}

char **tab_clean(char **tab)
{
    char **new_tab;
    int     j;

    j = 0;
    new_tab = malloc(sizeof(char *) * (len_tab(tab) + 1));
    while (tab[j])
    {
        new_tab[j] = clean_white_space(tab[j]);
        j++;
    }
    free(tab);
    new_tab[j] = 0;
    return (new_tab);
}

bool contains_invalid_sequences(char **tab, int size) 
{
    const char *symbols[] = {"||", "&&", ">>", ">", "<<", "<", "|"};
    int num_symbols = sizeof(symbols) / sizeof(symbols[0]);
    int i = 0;

    while (i < size)
    {
        int j = 0;
        while (j < num_symbols)
        {
            if (strcmp(tab[i], symbols[j]) == 0)
            {
                if (i == size - 1)
                {
                    ft_putstr_fd("syntax error near unexpected token `", 2);
                    ft_putstr_fd(tab[i], 2);
                    ft_putstr_fd("'\n", 2);
                    return true;
                }
                int k = 0;
                while (k < num_symbols)
                {
                    if (strcmp(tab[i + 1], symbols[k]) == 0)
                    {
                        ft_putstr_fd("syntax error near unexpected token `", 2);
                        ft_putstr_fd(tab[i + 1], 2);
                        ft_putstr_fd("'\n", 2);
                        return true;
                    }
                    k++;
                }
            }
            j++;
        }
        i++;
    }
    return false;
}

int check_input_error(char *input)
{
    int i;

    i = 0;
    if ((input[0] == ':' || input[0] == '#') && input[1] == '\0')
        return (0);
    else if (input[0] == '!' && input[1] == '\0')
        return (0);
    else if (input[0] == '\0')
        return (0);
    while (input[i] == ' ' || input[i] == '\t')
            i++;
    if (input[i] == '\n')
        return (0);
    return (1);
}

char *fuse_split_result(char **split_input, int start)
{
    char *result;
    int i;

    i = start;
    result = ft_strdup(split_input[start]);
    while (split_input[i])
    {
        result = ft_strjoin(result, ft_strdup(" "));
        if (split_input[i + 1])
            result = ft_strjoin(result, ft_strdup(split_input[i + 1]));
        i++;
    }
    return (result);
}

char **re_build_with_redir(char **tab_input)
{
    char **split_input;
    int i;

    i = 0;
    while (tab_input[i])
    {
        if (((tab_input[i][0] == '<' && (tab_input[i][1] == '\0' || tab_input[i][1] == '<')) 
        || (tab_input[i][0] == '>' && (tab_input[i][1] == '\0' || tab_input[i][1] == '>'))) && tab_input[i + 1])
        {
            split_input = ft_split(tab_input[i + 1], ' ');
            if (len_tab(split_input) > 1)
            {
                char **tmptab = malloc(sizeof(char *) * 3);
                tmptab[0] = ft_strdup(tab_input[i]);
                tmptab[1] = ft_strdup(split_input[0]);
                tmptab[2] = NULL;
                tab_input[i + 1] = fuse_split_result(split_input, 1);
                tab_input = fusionner_tableaux(tab_input, tmptab, len_tab(tab_input), len_tab(tmptab), i);
            }
            free_tab(split_input);
        }
        i++;
    }
    return (tab_input);
}

void lexer(char *input, char ***env, char ***export, int *exit_status)
{
    char **tab_input;
    Token *list = NULL;

    if (!input || !check_input_error(input))
        return ;
    tab_input = split_with_symbols(input);
    tab_input = re_build_with_redir(tab_input);
    tab_input = tab_clean(tab_input);
    if(contains_invalid_sequences(tab_input, len_tab(tab_input)))
    {
        *exit_status = 2;
        free_tab(tab_input);
        return ;
    }
    chained_split_prompt(&list, tab_input);
    free_tab(tab_input);
    parser(list, env, export, exit_status);
    freeTokens(list);
}
