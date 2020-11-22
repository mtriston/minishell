# minishell
My shell implementation.

![gif with a demonstration](https://github.com/sudo-spivak/minishell/blob/master/minishell.gif)

## Features
* Promt with username and current directory.
* Search and launch the right executable (based on the PATH variable or by using
relative or absolute path) like in bash
* The builtins like in bash:
  * echo with option ’-n’
  * cd with only a relative or absolute path
  * pwd without any options
  * export without any options
  * unset without any options
  * env without any options and any arguments
  * exit without any options
* ; in the command separate commands like in bash
* ’ and " work like in bash include multiline commands
* Redirections < > “>>” work like in bash except for file descriptor aggregation
* Pipes | work like in bash
* Environment variables ($ followed by characters) work like in bash
* $? work like in bash
* ctrl-C, ctrl-D and ctrl-\ have the same result as in bash

## How to install
```bash
git clone https://github.com/sudo-spivak/minishell.git
cd minishell
make
./minishell
```
