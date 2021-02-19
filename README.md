# Stage 6

## Tasks

Keeping a persistent history means that the history is saved on file and loaded between
activations of the shell. For this purpose, a .hist_list file can be used. (Note: that in
Unix-type systems files starting with . are considered hidden in that they are not shown
in directory listings unless the user explicit asks for them).

Save history
You just write the current contents of the history data structure into the .hist_list file,
overwriting its previous contents, just before the shell exits.
It is probably a good idea to follow a fairly simple format for this file, e.g.
‘number command’ per line.
To read or write a file in a C program you need to first open it using the fopen() function.
Files are opened in a particular mode, i.e. read, write, read/write, append. Files are then
processed using similar functions as those you have for processing standard input and
output, typically with an f in front of the function name and a file pointer as one of the
parameters. For example, scanf becomes fscanf, gets becomes fgets, printf becomes
fprintf, etc.

Load history
When your shell starts it will try to open the file .hist_list (in the user’s home directory),
read its contents and initialise the history with them.
In your shell, you can simplify matters by considering that each command line is at
most 512 characters long. In the case of the history file, this would mean that you can
use the fgets function to read each line. Then you know that the first token is the
command number.

In history_handler create:
1. int save_history(char* history[]);
2. int load_history(char* history[]);

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
