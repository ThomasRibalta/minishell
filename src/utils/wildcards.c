#include "../header/minishell.h"

char** fusionner_tableaux(char **tab1, char **tab2, int sizetab1, int sizetab2, int i) {
    int taille_totale = sizetab1 + sizetab2;
    
    char **tableau_fusionne = (char **)malloc(taille_totale * sizeof(char *));
    if (tableau_fusionne == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    for (int j = 0; j < i; j++) {
        tableau_fusionne[j] = ft_strdup(tab1[j]);
    }
    for (int j = 0; j < sizetab2; j++) {
        tableau_fusionne[i + j] = ft_strdup(tab2[j]);
    }
    for (int j = i + 1; tab1[j]; j++) {
        tableau_fusionne[sizetab2 + j - 1] = ft_strdup(tab1[j]);
    }
    tableau_fusionne[taille_totale - 1] = NULL;
    free_tab(tab1);
    free_tab(tab2);
    return (tableau_fusionne);
}

int get_number_of_files(char *after, char *before)
{
    int n;
    DIR *dir;
    struct dirent *entry;

    n = 0;
    dir = opendir(".");
    if (!after && !before)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
                continue;
            n++;
        }
    }
    else if (after && !before)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
                continue;
            if (strncmp(after, entry->d_name + strlen(entry->d_name) - strlen(after), strlen(after)) == 0)
                n++;
        }
    } else if (after && before)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
                continue;
            if (strncmp(before, entry->d_name, ft_strlen(before)) == 0 && strncmp(after, entry->d_name + ft_strlen(entry->d_name) - ft_strlen(after), ft_strlen(after)) == 0)
                n++;
        }
    } 
    else if (!after && before)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
                continue;
            if (strncmp(before, entry->d_name, strlen(before)) == 0)
                n++;
        }
    }
    closedir(dir);
    return n;
}

char **get_current_file()
{
    char **tab;
    int n;
    int i;
    DIR *dir;
    struct dirent *entry;

    n = get_number_of_files(NULL, NULL);
    if (n == 0)
        return (NULL);
    i = 0;
    tab = malloc(sizeof(char *) * (n + 1));
    if (!tab)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    dir = opendir(".");
    if (!dir)
    {
        perror("diropen");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) || entry->d_name[0] == '.')
            continue;
        tab[i] = strdup(entry->d_name);
        i++;
    }
    closedir(dir);
    tab[n] = NULL;
    return (tab);
}

char **get_current_file_after(char *str)
{
    char **tab;
    int n;
    int i;
    DIR *dir;
    struct dirent *entry;

    n = get_number_of_files(str, NULL);
    if (n == 0)
        return (NULL);
    i = 0;
    tab = malloc(sizeof(char *) * (n + 1));
    if (!tab)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    dir = opendir(".");
    if (!dir)
    {
        perror("diropen");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) || entry->d_name[0] == '.')
            continue;
        if (strncmp(str, entry->d_name + ft_strlen(entry->d_name) - ft_strlen(str), ft_strlen(str)) == 0)
        {
            tab[i] = strdup(entry->d_name);
            i++;
        }
    }
    closedir(dir);
    tab[i] = NULL;
    return (tab);
}

char **get_current_file_after_before(char *str, char *str2)
{
    char **tab;
    int n;
    int i;
    DIR *dir;
    struct dirent *entry;

    n = get_number_of_files(str, str2);
    if (n == 0)
        return (NULL);
    i = 0;
    tab = malloc(sizeof(char *) * (n + 1));
    if (!tab)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    dir = opendir(".");
    if (!dir)
    {
        perror("diropen");
        exit(EXIT_FAILURE);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if ((entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) || entry->d_name[0] == '.')
            continue;
        if (strncmp(str2, entry->d_name, ft_strlen(str2)) == 0 && strncmp(str, entry->d_name + ft_strlen(entry->d_name) - ft_strlen(str), ft_strlen(str)) == 0)
        {
            tab[i] = strdup(entry->d_name);
            i++;
        }
    }
    closedir(dir);
    tab[i] = NULL;
    return (tab);
}

void aff_tab(char **tab)
{
    int i = 0;
    while (tab[i])
    {
        printf("tab[%d] = %s\n", i, tab[i]);
        i++;
    }
}

int only_char(char *str, char c)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] != c)
            return (i + 1);
        i++;
    }
    return (i);
}

char **check_wildcard(char **split_nodeValue)
{
    int i = 0;
    int j = 0;
    char **tab_current_file = split_nodeValue;
    char **tab_current_file2 = NULL;

    while (tab_current_file[i])
    {
        while(tab_current_file[i][j])
        {
            if (only_char(tab_current_file[i], '*') == ft_strlen(tab_current_file[i]) && tab_current_file[i][0] == '*')
            {
                tab_current_file2 = get_current_file();
                if (!tab_current_file2)
                    break;
                tab_current_file = fusionner_tableaux(tab_current_file, tab_current_file2, len_tab(tab_current_file), len_tab(tab_current_file2), i);
                break;
            }
            else if (tab_current_file[i][j] == '*' && j == 0)
            {
                tab_current_file2 = get_current_file_after(tab_current_file[i] + only_char(tab_current_file[i] + 1, '*'));
                if (!tab_current_file2)
                    break;
                tab_current_file = fusionner_tableaux(tab_current_file, tab_current_file2, len_tab(tab_current_file), len_tab(tab_current_file2), i);
                break;
            }
            else if (tab_current_file[i][j] == '*')
            {
                tab_current_file2 = get_current_file_after_before(tab_current_file[i] + j + only_char(tab_current_file[i] + 1, '*'), ft_substr(tab_current_file[i], 0, j));
                if (!tab_current_file2)
                    break;
                tab_current_file = fusionner_tableaux(tab_current_file, tab_current_file2, len_tab(tab_current_file), len_tab(tab_current_file2), i);
                break;
            }
            j++;
        }
        j = 0;
        i++;
    }
    return (tab_current_file);
}
