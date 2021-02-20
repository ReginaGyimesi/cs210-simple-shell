# Finalisation

## Tasks
Testing, commenting and clean up

1. Everyone needs to go through the test pdf, trying our shell (including different environments)
2. Comment code, that you wrote
3. Clean up code or refactor to improve performance

# Stage 9

## Tasks
Enhanced aliases

In this stage, you should add to your implementation of the alias command the ability
to alias aliases and to alias history invocations.

The main challenge here is that your input handling should keep on replacing the
command token with the aliased command or command from history until you end up
with a built-in command or an external program or rubbish (i.e. no more substitutions
can be performed). In doing so, you should be careful about circular dependencies, i.e.
scenario like `alias a b`, `alias b a`, or `alias a !x`, where command number `x` in history is a,
etc. An easy way to deal with this is to set a limit on the number of substitutions carried
out (e.g. 3) and give an error when that limit is exceeding. A more sophisticated way to
do so would be to remember the substitutions you carried out (using something an array
or preferably something dynamic like a linked list) and before carrying out a
substitution check whether the same label was encountered before in which case you
give an error.

Remember that aliases and history invocation may involve parameters, not just simple
commands, so these should be properly concatenated to build the final command string
to be parsed for execution.

1. Either recursively or incrementally check for tokens to apply commands. 


# Stage 8

## Tasks
Persistent aliases

Keeping persistent aliases means that the aliases are saved on file and loaded between activations of the shell. 
For this purpose,a `.aliases` file can be used.

**Save aliases**

You  just  write  the  current  contents  of  the  aliases  data  structure  into  the  `.aliases`  file, overwriting  its  
previous  contents,  just  before  the  shell  exits  (Note:  it doesn’t matter whether the history or the aliases are saved 
first). It is probably a good idea to follow a fairly simple format for this file, e.g. ‘alias aliased command’ per line.

**Load aliases**

When your shell starts it will try to open the file `.aliases` (in the user’s home directory),
read its contents and initialise the aliases data structure with them. In  your shell, you  can simplify  matters  by  considering  that  each  command  line  is  at most 512 characters long. 
In the case of the aliases file,this would mean that you can use the `fgets` function to read each line. 
Then you know that the first token is the alias itself and the rest of the tokens the aliased command.

1. Create `int save_aliases(Alias* head)`. Save format: `alias <key> <command>`. Return TRUE or ERROR accordingly. Call 
   before exiting the shell.
2. Create `int load_aliases()`. Call before starting the shell.


# Stage 7

## Tasks
Setting  up  aliases, removing  aliases, invoking  aliases,  and  printing  all aliases

In  general, aliasing means that the shell can use a different label (usually simpler) to invoke a command. 
For example, users more familiar with a windows environment will set an alias for `dir` to be `ls –lF` in a Linux/Unix environment. 
In this case when the user types in `dir` then the shell will execute `ls –lF`. 
This will be the case until the user removes the alias by using the command `unalias dir`. As was the case in the example, 
quite often the aliased command will also include parameters. Note also that the alias could also work with additional parameters. 
For example, in the case of the `dir` above, the user could also type in `dir <a_directory>` and will execute `ls –lF <a directory>`, etc.

Note that the same command can be aliased with a number of different aliases, but of course, 
each  alias  should  map  to  only  one  command. If  the  same  alias  is  used  again your shell 
should override the previous one, providing an appropriate message. Note also that there is nothing to 
prevent a user from using a built-in command or an external program name as an alias. Your shell should allow this. 
In this case,the shell will  invoke  the  aliased  command  until  the  alias  is  removed  
(i.e.  when  processing  a command  you  first  check  for  aliases  and  then  for  built-in  commands  and  external programs).

Note also that at this stage it is not possible to alias an alias, 
only built-in commands and external programs. This basically means that you can check for an alias 
only once, and  after  that point,you  treat  the  aliased  command  as  a  normal  
one  (not  an  alias) provided by the user.At this stage,you can also alias a history 
invocation. This means that you don’t have to check whether it is a history invocation after you checked for an alias.

[Linked List](https://www.learn-c.org/en/Linked_lists)

1. Create `Alias` struct in `main.h`. Fields: `char* key`, `char* command`, `struct Alias* next`. Create an `Alias` variable in `main.c`
2. Create `alias_handler.h`:
    1. Create `int add_alias(Alias* head, char* key, char* command)`. Pushing an alias element to the header. Check is key already exists, 
       if yes, override it with the new command. Return TRUE or ERROR. Check that NULL values are not saved. If more then 20 is in the list, return error (full)
    2. Create `int remove_alias(Alias* head, char* key)`. Removing a specific alias from the list. Check for NULL values, if the key exists, remove
    it accordingly, keeping the lists integrity.
    3. Create `char* invoke_alias(Alias* head, char* key)`. Gets the command, from the list and returns it. Return NULL, if no command exists. 
    4. Create `int print_alias(Alias* head)`, printing out the current aliases, with the key-value pairs. If the list empty, print error.
    8. Include functions in `apply_command.h`, if command `alias <key> <value>` -> `add_alias()`, if `unalias` -> `remove_alias()`, if `alias` -> `print_alias()`

# Stage 6

## Tasks
Keeping a persistent history means that the history is saved on file and loaded between 
activations  of  the  shell.  For  this purpose,a `.hist_list`  file can  be used.
(Note:  that  in Unix-type systems files starting with `.` are considered hidden in that they are not shown in directory listings unless the user explicit asks for them).

**Save history**

You just write the current contents of the history data structure into the `.hist_list` file, overwriting its previous contents, 
just before the shell exits. It is probably a good idea to follow a fairly simple format for this file, e.g. ‘numbercommand’ per line.

**Load history**

When your shell starts it will try to open the file `.hist_list` (in the user’s home directory), 
read its contents and initialise the history with them. In your shell, you  can  simplify  matters  by  
considering  that  each  command  line  is  at most 512 characters long. In the case of the history file,
this would mean that you can use  the `fgets` function  to  read  each  line.  Then  you know  that  the  first  
token  is  the command number.

1. Create `int save_history(char* history)` in `history_handler.h`
2. Create `int load_history()` in `history_handler.h`

# Stage 5

## Tasks
In general, what a history feature means is that the shell remembers the instructions the
user types in and it can invoke again previous commands without the user having to retype them.

This typically works as follows:
- If the user types in ‘!<no.>’, then the shell re-executes command with number
<no.>, provided that the user has already typed that many commands and they
are still in history. The shell counts commands from 1 onwards and for
simplicity, you could assume that the shell only remembers the last 20
commands. Note that it doesn’t matter whether the command was a built-in or
external one, or even whether it was a valid command at all (i.e. if the previous
command was just garbage then the shell tries to execute the garbage again). It
is important that the command is executed exactly as it was typed, i.e. if the
command had a number of parameters these will also have to be used when it is
invoked again. For these reasons, it is a good idea to just store the full command
line for each command entered to the history.
- If the user types in ‘!-<no.>’, then the shell re-executes command with the
number the current command number minus <no.> provided that such a number
exists (i.e. it is positive) and the command is still in history.
- If the user types in ‘!!’, then the last command from the history is executed

Note that a command invoked from the history is not entered again into the history (i.e.
only new commands are entered into the history).
Moreover, the command should be checking for appropriateness of inputs and provide
the user with informative messages when the inputs are not appropriate (i.e. when not
enough commands have been saved or the number is invalid).
To maintain the history inside your shell, you have two options. Either you can define
a struct with an integer (the command number, starting from 1 and keep on going
forever) and a string (the command line itself). As the shell only keeps the last 20
commands, you can store the history in an array of 20 struct elements. Alternatively,
you can just keep an array of strings ordered from least to most recent again of size 20.

1. Create `History` struct. Fields: `int number, char* command`. Create array of `History` with 20 elements. Add in `main.h`
2. Create `history_handler.h`. Implement the following methods:
    1. `int check_history_type(char* user_input)`: check whether the user entered an input starting with:
       1. '!': call `int add_to_history(char* user_input)`
       2. '!<no.>': call `int exec_number_history(int number)`
       3. '!-<no.>': call `int exec_minus_number_history(int number)`
       4. '!!': call `exec_recent_history()`
    2. `add_to_history(char* user_input)`: if array has not reached its limit (20) than add it into the next available, else start from 0 index again following the
       circular array implementation.
    3. `int exec_number_history(int number)`: execute the command indicated by the parameter, return TRUE if can be done, else FALSE
    4. `int exec_minus_number_history(int number)`: re-executes command with the
       number the current command number minus <no.> provided that such a number
       exists (i.e. it is positive) and the command is still in history, return TRUE if can be done, else FALSE
    5. `int exec_recent_history()`: execute the last command from the history, return TRUE if can be done, else FALSE
3. Create `print_history.h`. On command `history`, the array of `History` should be printed out.
 
# Stage 4

## Tasks
Changing directories

This command has two forms. In the first, it has no parameters and changes the working
directory to the user’s home directory. 

In the second, it has one parameter which is a directory (either in full path or relative path form) and changes the working directory
to the directory provided as a parameter. If the directory provided does not exist, then
it should say that there is no such directory. In fact, you must go further and utilise the
error value of the system calls using the `perror()` function. 

The shell should check appropriate parameters are used by the user and provide informative messages if this is
not the case. 

Note, though, that any string could be considered a path as far as the
command is concerned.

Remember that directory changes are effected with the `chdir()` function.
Remember that `.` and `..` refer to the current and the parent directory and they can be
used as part of the path.

1. Create `change_directory.h` header, with regard the above mentioned requirements.

# Stage 3

## Tasks
For this stage, we are only interested in two things from the environment the HOME –
the user’s home directory, and the PATH – the list of directories within which the shell
looks for executable programs.

1. Create String variable which stores the original PATH on startup, using `getenv()`. On exit set `PATH` back to the original value. Include it into `main.c`.
2. Change current C directory to `HOME` directory on startup. Call `getenv("HOME")` and `chdir()`. Check if this happened correctly by calling `getcwd()`, if not
   print out error message. Include it into `main.c`. 
3. Create 2 built-in commands: `getpath` & `setpath` methods. *Create a new `path.h` in `functions` folder.* The first - `getpath` - just gets and prints
   the `PATH` value. The second - `setpath` - takes a path (string of a colon-separated list of directories) as a parameter and makes it the value of `PATH`.
   Use `getenv()` & `setenv()` to achieve this. Your shell should also check appropriate parameters are provided by the user and print  
   informative error messages when this is not the case. Note that there are no constraints
   on the format of the path placed by the environment.
4. Add these built-in commands into `applycommand.h`. (How? is up for discussion)
5. Testing

# Stage 2

## Tasks
The key point for this stage is to choose an appropriate variant of `exec()` that will take
into account the `PATH` environment parameter. Read also the manual very carefully
about how to handle external program parameters. Remember that we should be able to
support any number of command parameters.

In order to simplify the parsing so that built-in commands can be easily added, it is a
good idea to generate a single string array that includes all the tokens of the user input
line. This way you can parse the user input line, in the same way, all the time
irrespective of whether it contains built-in or external commands or the number of
tokens it contains!

To make your life even easier you can use a fixed size array, thus putting a limit on the
number of tokens accepted. Fifty tokens will be a reasonable number here.

1. Modify `tokenize.h`. Limit number of tokens to 50.
2. Create `execute_shell_command.h` header. If the functions is not an in-built function (such as exit ^D etc...), then call the fork function from `execute_shell_command.h`
3. Testing

# Stage 1

## Tasks

1. do-while loop, which is the driving method for our whole project
	do{
		// shell itself
	while(exit_condition);
2. print prompt and read user input
3. parse user input
4. execute command
5. testing

**Function 1** 
function read_user_input -> returns string pointer
2.1 print prompt ("> ") something like this
2.2 return user input string 

**Function 2** 
function tokenize_input(string) -> return array of strings
3.1 check length
3.2 tokenize by delimiter
3.3 return array of strings

**Function 3** 
function apply_command(string[]) -> return int (0 if exit, everything else >0)
4.1 check if first element of array is an existing command (right now our only one is 'exit')
4.2 execute command

**Testing**
By the PDF, and you can come up with other stuff
