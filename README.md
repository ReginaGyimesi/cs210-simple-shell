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
