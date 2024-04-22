#include "../../header/minishell.h"

void echo(char *str){
  char **tab;
  char *new_str;
  int j;
  
  j = 0;
  new_str = clean_quote(str);
  tab = ft_split(new_str, ' ');
  while (tab[j])
  {
    printf("%s ",tab[j]);
    j++;
  }
  if (ft_strncmp(tab[1], "-n", 2))
  {
    printf("\n");
  }
}