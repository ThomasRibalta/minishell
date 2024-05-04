#include "../../header/minishell.h"

void echo(char **tab){
  int j;
  
  j = 1;
  if (ft_strncmp(tab[j], "-n", 2))
    j++;
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