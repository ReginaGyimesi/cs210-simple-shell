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

## How to add a new function to the code
1. Add a new `my_function.h` to the "functions" folder
2. Write your code
3. In `main.h` add line `#include "functions/my_function.h"`
4. Use it in `main.c` as you want

## How to add tests to the code
1. Add a new `my_functions_test.h`to "tests/test_functions"
2. Use the `test_template.h` to copy the methods to your test code.
3. Only edit between the indicated comments
4. Set a test title in `char test_title`
5. While testing you must set a value for `test_list[temp].testPassed`either `TRUE` or `FALSE`
6. After you are done with writing, in `tests.h`you have to add line `#include "test_functions/my_functions_test.h"`
7. In the `runt_tests()`in `tests.h`you have to add the driver function for the `my_functions_test.h`with parameters `(test_list, &i)`so something like: `run_my_function_tests(test_list, &i);`
8. In `main.c`set testing to TRUE if you want to see the tests run

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
