#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#define MAX_LINE 80 /* The maximum length command */

int main(void) {
     char *args[MAX_LINE/2 + 1]; /* command line arguments */ 
   // char args[10][10];
    int should_run = 1; /* flag to determine when to exit program */
    
    while (should_run){ 
        printf("mysh:~$ "); 
        fflush(stdout);

        /** After reading user input, the steps are:  
         * (1) fork a child process using fork()  
         * (2) the child process will invoke execvp()  
         * (3) parent will invoke wait() unless command included & 
         */



/***********************************************************************************/
        char input[80];
        fgets(input, 80, stdin);
        int length = strlen(input);
        input[length - 1] = '\0';
        length--;
        printf("Original length of input without tokenizing is %d\n", length);
        if(strcmp(input, "exit") == 0){
            should_run = 0;
		break;
        }

        else{

                    int out = 0; // output redirection
                    int in = 0; // input redirection
                    int pipeCmd = 0;
                    int fdp[2];
                    pipe(fdp);




            /************************************************************************************/

            char previous[80];
                    if(strcmp(input, "!!") != 0){
                        // copying user input to another variable
                        
                        int j;
                        for(j = 0; j < length; j++){
                            previous[j] = input[j];
                        }
                        previous[j] = '\0';

                    }
                    else{

                        if(strlen(previous) == 0){
                            printf("No commands in history.\n");
                        }
                        else{
                            //copying previous command to input
                            int i;
                            for(i = 0; i < strlen(previous); i++){
                                input[i] = previous[i];
                            }
                            input[i] = '\0';
			    length = strlen(previous);
                        }   
			printf("%s\n", previous);  

                    }

                    printf("\nThe previous command was %s\n", previous);        
            /***********************************************************************************/
                    // look for input/output symbol and change the flag accordingly. Also, get the specified file name
                    int k, temp;
                    int  m, l;
                    m = 0; 
                
                    char input2[40]; // storing anything at/after '<' or '>'

                    for(k = 0; k < length; k++){
                        if(input[k] == '>'){
                            out = 1;
                            input[k - 1] = '\0';
                            temp = k;

                                    // copying other half of original input to input2
                                    for(l = temp; l < length; l++){
                                        input2[m] = input[l];
                                        m++;
                                    }
                                    input2[m] = '\0';

                        }
                        if(input[k] == '<'){
                            in = 1;
                            input[k - 1] = '\0';
                            temp = k;

                            // copying other half of original input to input2
                                for(l = temp; l < length; l++){
                                    input2[m] = input[l];
                                    m++;
                                }
                                input2[m] = '\0';                 
                        }

                        if(input[k] == '|'){
                            pipeCmd = 1;
                            input[k - 1] = '\0';
                            temp = k + 2;

                            // copying other half of original input to input2
                                for(l = temp; l < length; l++){
                                    input2[m] = input[l];
                                    m++;
                                }
                                input2[m] = '\0';

                        }
                    }

            /***********************************************************************************/
                    //tokenizing input2
                    char file[40];
                    char *args2[40];

                    if(strlen(input2) != 0){
                        printf("Input2 is %s\n", input2);

                        
                        int ip2 = 0;
                        char *ptr = strtok (input2, " ");
                        while (ptr != NULL){ 
                            args2[ip2] = ptr;
                            ip2++;
                            ptr = strtok (NULL, " ");
                        }
                        args2[ip2] = NULL;

                        int temp = 0;

                        // printing tokenized input2
                        for(temp = 0; temp < 2; temp++){
                            printf("The element at args2[%d] is %s\n", temp, args2[temp]);
                        }

                    }
                    
                    
            /***********************************************************************************/
                    
                    // check to see the last character is "&"

            /***********************************************************************************/        

                    int isAmpersand = 0;
                    if(input[length - 2] == '&'){
                        isAmpersand = 1;
                        input[length - 2] = '\0';
                    } 
            /***********************************************************************************/
                    
                // tokenizing the string
                    char delim[] = " ";
                    int i = 0;
                    char *p = strtok (input, delim);
                    while (p != NULL){ 
                        args[i] = p;
                        i++;
                        //printf("%s\n", p);
                        p = strtok (NULL, delim);
                    }
                    args[i] = NULL;

            /***********************************************************************************/ 



                    
            /***********************************************************************************/        
                    int x;

                    //printing args value to see if the user input is actually tokenized
                    for(x = 0; x <= i; x++)
                    {
                        printf("The element at args[%d] is %s\n",x, args[x]);
                    }
            /***********************************************************************************/

                    pid_t pid;
                    //int fd, fd2;
                    int fd;
                    /* fork a child process */
                    pid = fork();
                    if (pid < 0) { /* error occurred */
                        fprintf(stderr, "Fork Failed");
                        return 1;
                    }
                    else if (pid == 0) { /* child process */
                
                        if (strcmp(args[0], "cd\0") == 0){
                            chdir(args[1]);  

                        }
                        else if(out == 1){
                            strcpy(file, args2[1]);
                            fd = open(file, O_WRONLY | O_CREAT, 0777);

                            if(fd == -1){printf("Error\n");}
                            dup2(fd, 1); // duplicating file descriptor to 1 i.e stdout
                            out = 0;
                            close(fd);
                            execvp(args[0], args);
                        }    
                        else if (in == 1){
                            strcpy(file, args2[1]);
                            fd = open(file, O_RDONLY | O_CREAT, 0777);

                            if(fd == -1){printf("Error\n");}
                            dup2(fd, 0); // duplicating file descriptor from 0 i.e stdin
                            in = 0;
                            close(fd);
                            execvp(args[0], args);

                        }   

                        else if(pipeCmd == 1){
                                
                                pid_t pid2;
                                pid2 = fork();

                                if(pid2 != 0){
                                    // parent process. It will close fdp[1] and read from fdp[0]
                                    wait(NULL);
                                    dup2(fdp[0], 0);
                                    close(fdp[1]);
                                    execvp(args2[0], args2); // irst command before | eg. wc in ls | wc
                                    printf("execpvp failed\n");


                                    
                                }
                                else{
                                        //child process. it will close fdp[0] and write to fdp[1] which parent will read
                                        // so parent has to wait till child finishes

                                        dup2(fdp[1], 1);
                                        close(fdp[0]);
                                        execvp(args[0], args);  // first command before | eg. ls in ls | wc
                                        printf("execvp failed\n");


                                }

                        } 
                            
                        else{
                            execvp(args[0], args);
                            printf("%s: command not found\n", input);

                        }
                    
                    }
                    else { /* parent process */
                    /* parent will wait for the child to complete */
                        close(fdp[0]);
                        close(fdp[1]);
                        if (isAmpersand == 0){
                            
                            wait(NULL);
                            printf("All Child Completed\n");
                    
                        }
                        else{
                        
                            printf("No wait used\n");
                            printf("%d\n", pid);
                            isAmpersand = 0;
                                 
                        }
                        
                    }

                    } 
                                
                }

return 0;
}


