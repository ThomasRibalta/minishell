# Minishell

Minishell is a project from the 42 school that involves creating a small Unix command interpreter in C. The goal is to recreate a simplified version of bash, with basic functionalities to execute commands, handle redirections, pipes, environment variables, and built-in commands.

## Features

- Display a prompt to enter commands.
- Parse and execute commands.
- Input (`<`), output (`>`), and append (`>>`) redirections.
- Pipe (`|`) handling.
- Environment variable management.
- Advanced signal handling (`Ctrl-C`, `Ctrl-D`, etc.).
- Command history, allowing navigation through previous commands.
- Support for advanced features like subshells and command substitution.
- Implementation of built-in commands: `cd`, `echo`, `env`, `exit`, `export`, `unset`.

## Bonus

- Wildcard (`*`) handling for filename pattern matching.
- Support for logical operators `||` and `&&` for conditional command execution.

## Usage

1. Clone the repository:
    ```sh
    git clone https://github.com/ThomasRibalta/minishell.git
    cd minishell
    ```

2. Compile the project:
    ```sh
    make
    ```

3. Run the shell:
    ```sh
    ./minishell
    ```

## Built-in Commands

- `cd [path]` : Change the current directory.
- `echo [args]` : Display arguments.
- `env` : Display environment variables.
- `exit [status]` : Exit the shell with the given status.
- `export [name]=[value]` : Add or modify an environment variable.
- `unset [name]` : Remove an environment variable.

## Examples

```sh
minishell$ echo Hello, World!
Hello, World!

minishell$ export VAR=value
minishell$ echo $VAR
value

minishell$ ls -l | grep minishell
-rwxr-xr-x  1 user  staff  123456 Jan 1 12:34 minishell

minishell$ cat file.txt > output.txt
minishell$ cat < input.txt
```

## Contributor
[jeduran](https://github.com/Beorlor)


