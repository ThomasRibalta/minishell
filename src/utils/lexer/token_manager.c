#include "../../header/minishell.h"

Token* createToken(TokenType type, const char* value)
{
    Token* token;
    
    token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->value = value != NULL ? strdup(value) : NULL;
    token->next = NULL;
    return token;
}

void appendToken(Token** head, TokenType type, const char* value)
{
    Token* newToken;
    Token* current;
    
    newToken = createToken(type, value);
    if (*head == NULL) 
    {
        *head = newToken;
    }
    else
    {
        current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newToken;
    }
}

void freeTokens(Token* head)
{
    Token* tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        if (tmp->value != NULL) free(tmp->value);
        free(tmp);
    }
}