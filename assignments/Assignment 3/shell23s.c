/*
    Name: Rohit Kumar
    Student Id: 110088741
    Section: 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <regex.h>
#include <sys/wait.h>

#define MAX_ALLOWED_INPUT 1024
#define MAX_ALLOWED_COMMAND 8
#define MAX_ALLOWED_ARGS 5
#define APPEND_OUTPUT 2
#define TRUNCATE_OUTPUT 1

/*
    below function checks if a character at a given index in a string is escaped or not
*/
int escapedSequence(const char* userInputString, int rohitIndex) 
{
    //checks double backslash escaped sequence
    if (rohitIndex > 0 && userInputString[rohitIndex-1] == '\\') 
    {
        int rohitCount = 0;
        while (rohitIndex > 0 && userInputString[rohitIndex-1] == '\\')
        {
            rohitCount++;
            rohitIndex--;
        }
        return (rohitCount % 2) != 0;
    }
    return 0;
}

/*
    below function trims leading and trailing whitespace characters from the input inputTrimString 
    using pointer and standard library function (isspace).
*/
char* trimWhiteSpace(char *inputTrimString) {

    int j;

    //checks input string is whitespace or not
    while (isspace(*inputTrimString)) 
    {     
        inputTrimString++;  
    }

    for (j = strlen (inputTrimString) - 1; (isspace (inputTrimString[j])); j--);

    //adding null terminator at the end
    inputTrimString[j + 1] = '\0';
    
    //return value
    return inputTrimString;
}

/*
    Format command strings by deleting any escape characters and adding spaces around special characters
    and double special characters using dynamic memory allocation (malloc)

    Explanation: The code enters a while loop and checks if current character is a single special character 
    then it adds spaces around it and if current character is not a special character then it modifies the string 
    as it is. Once loop finishes then add a null character and returned the modified string.
*/
char* formatInputCommand(const char* inputUserCommand) 
{
    //Single-Special-Character
    const char* rohitSingalSpecialCharacter = "<>|&;";

    //Double-Special-Character
    const char* rohitDoubleSpecialCharacter = ">|&";

    //Input User Command Length
    size_t inputCommandLength = strlen(inputUserCommand);

    
    //Dynamic memory allocation using malloc
    char* updateInputCommand = malloc(inputCommandLength*2 + 1); 
    size_t i = 0, j = 0;

    /*
        if current character is a single special character then it adds spaces around it and 
        if current character is not a special character then it modifies the string as it is
    */
    while (i < inputCommandLength) 
    {
        if (strchr(rohitSingalSpecialCharacter, inputUserCommand[i]) != NULL && !escapedSequence(inputUserCommand, i)) 
        {
            
            if (strchr(rohitDoubleSpecialCharacter, inputUserCommand[i]) != NULL &&
                i + 1 < inputCommandLength &&
                inputUserCommand[i] == inputUserCommand[i+1]) {
                updateInputCommand[j++] = ' ';
                updateInputCommand[j++] = inputUserCommand[i++];
                updateInputCommand[j++] = inputUserCommand[i];
                updateInputCommand[j++] = ' ';
                updateInputCommand[j++] = ' ';
            } 
            else 
            {
                updateInputCommand[j++] = ' ';
                updateInputCommand[j++] = inputUserCommand[i];
                updateInputCommand[j++] = ' ';
            }
        } 
        else 
        {
            
            updateInputCommand[j++] = inputUserCommand[i];
        }
        i++;
    }

    //adding null chararacter at end
    updateInputCommand[j] = '\0';

    // reutrn final result
    return updateInputCommand;
}

/*
    below function checks if a command provided by user has a valid number of argument or not.
*/
int inputAllowedCommand (char* inputAllowedCommand) {

    //make copy of input user command
    char* inputUserCommand = strdup(inputAllowedCommand);

    //remove whitespace
    inputUserCommand = trimWhiteSpace(inputUserCommand);
    
    //tokenized input user command separated by " "
    char* tokenizedString = strtok(inputUserCommand," ");


    int argumentCount = 0;

    // chekcing for NULL
    while (tokenizedString != NULL) {
        
        tokenizedString = strtok (NULL, " ");
        argumentCount++;
    }
    
    // return argumentCount
    //return argumentCount <= MAX_ALLOWED_ARGS && argumentCount > 0;
    return argumentCount > 0 && argumentCount <= MAX_ALLOWED_ARGS;
}

/*
    Split a string into tokens based on the delimiters "&&", "||", '|', and ';'

    Explanation: The code enters a for loop and compare different delimiters. If condition matches then create a 
    dynamic memory using malloc for the token array and increase the counter and assign to variables accordingly
    after the loop function returns the numbers of newToekn created
*/
int createformattedCommand(char *inputFormattedString, char **inputFormattedTokens)
{

    //initialize input variables
    int inputFormattedTokenNew = 0;
    int inputStringLength = strlen(inputFormattedString);
    int i;
    int formattedIndex = 0;

    for (i = 0; i < inputStringLength; i++)
    {
        if (strncmp(inputFormattedString + i, "&&", 2) == 0) //valiadte '&&'
        {
            //dynamic memory allocation using malloc
            inputFormattedTokens[inputFormattedTokenNew] = malloc((i - formattedIndex + 1) * sizeof(char)); 

            //string copy
            strncpy(inputFormattedTokens[inputFormattedTokenNew++], inputFormattedString + formattedIndex, i - formattedIndex);
            inputFormattedTokens[inputFormattedTokenNew++] = "&&";
            i++;
            formattedIndex = i + 2;
        }
        else if (strncmp(inputFormattedString + i, "||", 2) == 0)  //validate '||'
        {
             //dynamic memory allocation using malloc
            inputFormattedTokens[inputFormattedTokenNew] = malloc((i - formattedIndex + 1) * sizeof(char));

            //string copy
            strncpy(inputFormattedTokens[inputFormattedTokenNew++], inputFormattedString + formattedIndex, i - formattedIndex);
            inputFormattedTokens[inputFormattedTokenNew++] = "||";
            i++;
            formattedIndex = i + 2;
        }
        else if (inputFormattedString[i] == '|') //validate '|'
        {
            //dynamic memory allocation using malloc
            inputFormattedTokens[inputFormattedTokenNew] = malloc((i - formattedIndex + 1) * sizeof(char));

            //string copy
            strncpy(inputFormattedTokens[inputFormattedTokenNew++], inputFormattedString + formattedIndex, i - formattedIndex);
            inputFormattedTokens[inputFormattedTokenNew++] = "|";
            formattedIndex = i + 1;
        }
        else if (inputFormattedString[i] == ';')   //validate ';'
        {
             //dynamic memory allocation using malloc
            inputFormattedTokens[inputFormattedTokenNew] = malloc((i - formattedIndex + 1) * sizeof(char));

            //string copy
            strncpy(inputFormattedTokens[inputFormattedTokenNew++], inputFormattedString + formattedIndex, i - formattedIndex);
            inputFormattedTokens[inputFormattedTokenNew++] = ";";
            formattedIndex = i + 1;
        }
    }

    //copied last string toeknizer
    inputFormattedTokens[inputFormattedTokenNew] = malloc((inputStringLength - formattedIndex + 1) * sizeof(char));
    strncpy(inputFormattedTokens[inputFormattedTokenNew++], inputFormattedString + formattedIndex, inputStringLength - formattedIndex);

    //return final result
    return inputFormattedTokenNew;
}

/*
    Execute single command based on deliminator using fork(), dup2() and exec()

    Explanation: below function execute the user command and redirect the input and output and background process 
    execution. It basically forks a child process to run the command and redirects. 
    It also waits for the child process to finish unless it is a background process.
*/
int executeCommand(char* inputUserCommand, int inputProcess) 
{
    
    char *spaceDeliminator = " "; //separaterd string by space
    
    char *commandFileOutput = NULL; //create command file output
    
    int backgroundProcess = 0; //initialized background process
    
    char *commandFileInput = NULL; //create command file input

    char *commandToken; //create command Token
    
    char *inputArgument[10];  //initialized input Argument

    char* executeInputcommand = strdup(inputUserCommand);

    executeInputcommand = trimWhiteSpace(executeInputcommand);
    
    int commandOutputMode = TRUNCATE_OUTPUT;

    //validating '&'
    if (executeInputcommand[strlen(executeInputcommand) - 1] == '&') 
    {
        backgroundProcess = 1;
        executeInputcommand[strlen(executeInputcommand) - 1] = '\0';
        executeInputcommand = trimWhiteSpace(executeInputcommand);
    }

    //validating if 'ext'
    if (strcmp(executeInputcommand, "exit") == 0) 
    {
        exit(0);
    }

    //validating if 'cd'
    if (strcmp(executeInputcommand, "cd") == 0) 
    {
        chdir(getenv("HOME")); //change dir to HOME
        return 0;
    }
    else if (strstr(executeInputcommand, "cd ") == executeInputcommand) 
    {
        char* changeDIR = executeInputcommand + 3;   // skip cd at start
        
        changeDIR = trimWhiteSpace(changeDIR); //whitespace extraspace

        chdir(changeDIR); //change directory
        return 0;
    }

    int i = 0;
    char* commandNewToken;
    
    //generating string tokenizer
    commandToken = strtok_r(executeInputcommand, spaceDeliminator, &commandNewToken);

    //execute till all string token
    while (commandToken != NULL) 
    {
        if (strcmp(commandToken, "<") == 0) //validating for '<'
        {
            commandFileInput = strtok_r(NULL, spaceDeliminator, &commandNewToken); //redirection to input

        } 
        else if (strcmp(commandToken, ">") == 0)  //validating for '>' 
        {
            commandFileOutput = strtok_r(NULL, spaceDeliminator, &commandNewToken); //redirection to output

            commandOutputMode = TRUNCATE_OUTPUT; //commandOutputMode

        } 
        else if (strcmp(commandToken, ">>") == 0) //validating for '>>'
        {
            commandFileOutput = strtok_r(NULL, spaceDeliminator, &commandNewToken); //appending output

            commandOutputMode = APPEND_OUTPUT;
        } 
        else 
        {
            inputArgument[i++] = commandToken;
        }
        commandToken = strtok_r(NULL, spaceDeliminator, &commandNewToken);
    }

    inputArgument[i] = NULL;


    //forking starts from here
    int forkProcess = 0;
    if (inputProcess) 
    {
        forkProcess = fork();
    }

    if (forkProcess < 0) 
    {  
        fprintf(stderr, "!!Error: while forking\n");
        exit(1);

    } 
    else if (forkProcess == 0)  //child process
    { 
        int fdInput, fdOut; // file I/P and O/P file descriptors

        if (commandFileInput) 
        {
            fdInput = open(commandFileInput, O_RDONLY); //Open Input file

            if (fdInput < 0)  //File error check
            {
                fprintf(stderr, "!!Error: while opening the file\n");
                exit(1);
            }

            dup2(fdInput, STDIN_FILENO); //redirection

            close(fdInput); //Closing Input file
        }
        if (commandFileOutput) 
        {
            if (commandOutputMode == TRUNCATE_OUTPUT) 
            {  
                fdOut = open(commandFileOutput, O_WRONLY | O_CREAT | O_TRUNC, 0777); //Open Output file
            } 
            else if (commandOutputMode == APPEND_OUTPUT) 
            {
                fdOut = open(commandFileOutput, O_WRONLY | O_CREAT | O_APPEND, 0777); //Open Output file
            }
            if (fdOut < 0) 
            {
                fprintf(stderr, "Error: While opening the file\n");
                exit(1);
            }

            dup2(fdOut, STDOUT_FILENO); //redirection

            close(fdOut); //Closing Output file
        }
        if (execvp(inputArgument[0], inputArgument) < 0) 
        {
            fprintf(stderr, "Error: In exec()\n");
            exit(1);
        }
    } 
    else  //Parent process
    {    
        if (!backgroundProcess) //Wait until child finish
        {     
            int status;
            wait(&status);
            return status;
        } 
        else 
        {
            printf("Background process triggred- PID =  %d\n", forkProcess);
            return 0;
        }
    }

    if (inputProcess == 0) 
    {
        if (!backgroundProcess) //Wait until child finish
        {
            int status;
            wait(&status);
            return status;
        } 
        else 
        {
            printf("Background process triggred- PID = %d\n", forkProcess);
            return 0;
        }
    }
    return 0;
    }

/*
    below function executed commands which are join by Pipe and creates child process for each command and redirect
    the input and output. It also runs background process by validating the "&" command at last.
*/
int executePipeCommand (char** inputUserCommands, int inputPipeCommand) 
{
    int forkProcess = fork(); //forking
    int i=0;
    if (!forkProcess) 
    {
        for( i=0; i<inputPipeCommand-1; i+=2)
        {
            
            int pipeArr[2]; //Declare Pipes

            pipe(pipeArr); //Create Pipes

            if (!fork()) 
            {
                dup2(pipeArr[1], 1); //redirection
                executeCommand(inputUserCommands[i], 0);
                perror("!!Error in exec!!\n");
            }

            dup2(pipeArr[0], 0); //Redirection from output to Input

            close(pipeArr[1]); //Close file
        }

        executeCommand(inputUserCommands[i], 0); //Invoke executeCommand

    } 
    else if (forkProcess > 0) 
    {
        //fetching updated command
        char* updatedCommand = strdup(inputUserCommands[inputPipeCommand - 1]);
        
        updatedCommand = trimWhiteSpace(updatedCommand); //Invoke trimWhiteSpace

        if (updatedCommand[strlen(updatedCommand) - 1] != '&')   //Wait for forkprocess
        {
            waitpid(forkProcess, NULL, 0);
        } 
        else 
        {
            printf("Background process = %d\n", forkProcess);
        }
    }
}

/*
    Execute commands based on different separators (||, &&, |, and ;) using fork(), dup2() and exec()

    Explanation: code enters a while loop where it checks the current command at commands[i] against various separators 
    if current command is other than pipe then it simply increase the count and skip and invoke executeCommand menthod
    where those command will be excuted separately and in case of pipe it invokes pipe execution method where it uses
    fork() and dup2() system call to run the command.
*/
void invokeInputCommand(char **inputUsercommands, int commandLength) 
{
    int storeLastResult = 1; //store last result

    int i = 0; //initialize 

    int storeResult = 0; //store result

    //int fd[2]; //variable for pipes

    //i=0;

    while (i<commandLength) 
    {
        //validating for || if found then invoke next command
        if (strcmp(inputUsercommands[i], "||") == 0) 
        {
            if (storeLastResult == 0) 
            {      
                i++;
                while (i<commandLength && strcmp(inputUsercommands[i], "&&") != 0 && 
                strcmp(inputUsercommands[i], "|") != 0 &&
                strcmp(inputUsercommands[i], ";") != 0) 
                {
                    i++;
                }
                if (i>0 && i<commandLength && strcmp(inputUsercommands[i], "|") == 0)
                    i+=2;
            } else 
            {
                i++;
            }
        } else if (strcmp(inputUsercommands[i], "&&") == 0) 
        {
            if (storeLastResult != 0)
             {
                //validating for && if found the invoke next command
                i++;
                while (i<commandLength && strcmp(inputUsercommands[i], "||") != 0 && 
                strcmp(inputUsercommands[i], "|") != 0 &&
                strcmp(inputUsercommands[i], ";") != 0)
                 {
                    i++;
                }
                if (i>0 && i<commandLength && strcmp(inputUsercommands[i], "|") == 0)
                   i+=2;
            } 
            else
            {
                i++;
            }
        } 
        else if (strcmp(inputUsercommands[i], "|") == 0)
        {
            i++;
        }
        else if (strcmp(inputUsercommands[i], ";") == 0) 
        {
            i++;
        } 
        else if (i<commandLength-1 && strcmp(inputUsercommands[i+1], "|") == 0)
        {
            
            //Add all inputUsercommands with pipe
            int first = i;
            while(i<commandLength-1 && strcmp(inputUsercommands[i+1], "|") == 0) i+=2;
            executePipeCommand(inputUsercommands+first, i-first+1); //invoke executePipeCommand
            i++;
        }
        else 
        { 
            storeResult = executeCommand(inputUsercommands[i], 1);
            storeLastResult = storeResult;
            i++;
        }
    }
}

/*
    Validate a set of user commands and only allow when it matches with certain conditions.

    Explanation: It validates if user input commands is valid or not and then validates Pipes, Conditional Execution
    and Sequential Execution are upto limit 7 or not and finally validates each command is >=1 and <=5 as per Rule
    in case of any error, it stops the execution and noitify the user with proper error message.
*/
int validateInputCommand(char** inputUserCommand, int inputCommandCount) 
{
   //if no command passed by user
    if (inputCommandCount%2 == 0) 
    {
        printf("!! Error Incorrect Input Provided by User!!\n");
        return 0;
    }

    //if more than 7 commands passed by user
    if (inputCommandCount > 2*MAX_ALLOWED_COMMAND - 1) 
    {
        printf("ONLY UPTO 7 PIPES OR CONDITIONAL EXECUTION OR SEQUENCIAL EXECUTION ARE ALLOWED\n");
        return 0;
    }
       
    //if more than 5 commands passed by user within each command as per rule
    for (int i=0; i<inputCommandCount; i++)
    {
        //ignore
        if (i%2) 
            continue;

        //validate input allowed command 
        int successfulValidated = inputAllowedCommand(inputUserCommand[i]);

        if (!successfulValidated) 
        {
            printf("ONLY UPTO 5 COMMAND ALLOWED WITHIN EACH COMMAND\n");
            return 0;
        }
    }
    return 1;
}

/*
    This is main method and execution starts from here. This method mainly perform below operations
    1. Format command strings by deleting any escape characters and placing spaces around special characters.
    2. Split a string into tokens based on the delimiters "&&", "||", '|', and ';'
    3. Validate a set of user commands and only allow when it matches with certain conditions.
    4. Executie commands based on different separators (||, &&, |, and ;) using fork(), dup2() and exec()
*/
int main()
{
    while (1) {

        /*
            creating variable for input
        */
        char* rohitInput = malloc(4096);

        /*
            flushing standard output buffer and making spaces
        */
        fflush(stdout);

        /*
            printing mshell$ in terminal
        */
        printf("mshell$ ");

        /*
            below function reads input from the standard input (stdin) and
            store it in the character array (rohitInput)
        */
        fgets(rohitInput, MAX_ALLOWED_INPUT, stdin);

        /*
            below function removes whitespace characters from the input 
        */
        rohitInput = trimWhiteSpace(rohitInput);
        
        /*
            In case of no input mshell$ will continue
        */
        if (strlen(rohitInput) == 0) 
        {
            continue;
        }

        /*
            Format command strings by deleting any escape characters and placing spaces around special characters.
        */
        char* formattedInput = formatInputCommand(rohitInput);

        /*
          create a char string
        */
        char *formattedCommand[150];

        /*
            Split a string into formattedCommand based on the delimiters "&&", "||", '|', and ';'
        */
        int formattednewCommand = createformattedCommand(formattedInput, formattedCommand);
        
        /*
            Validate a set of user commands and only allow when it matches with certain conditions.
        */        
        int successfulValidated = validateInputCommand(formattedCommand, formattednewCommand);

        /*
            Executie commands based on different separators (||, &&, |, and ;) using fork(), dup2() and exec()
        */
        if (successfulValidated)
        {
            invokeInputCommand(formattedCommand, formattednewCommand);
        }     
    }
    return 0;
}