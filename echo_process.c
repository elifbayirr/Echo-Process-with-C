#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <wait.h>


void strrev(char *x, int begin, int end) {
   char c;
 
   if (begin >= end)
      return;
 
   c          = *(x+begin);
   *(x+begin) = *(x+end);
   *(x+end)   = c;
 
   strrev(x, ++begin, --end);
}

int main(void){
    pid_t   fork_id_c;

        char    readbuffer[200];
        char    readbuffer_to_send[200];

        int     first_p[2], p1_read;
        int     second_p[2], p2_read;


        pipe(first_p);
        pipe(second_p);
        
	/* Cocuk prosess yaratiliyor */ 
        if((fork_id_c = fork()) == -1)
        {
	    perror("fork err");
    	    exit(1);
        }

	
        if(fork_id_c == 0)   /* Cocuk proses */
	{
	    
	    close (first_p[1]);
	    close (second_p[0]);
 	    while (true)
	    {
        	    p1_read = read(first_p[0], readbuffer, sizeof(readbuffer));
	            if (!strcmp(readbuffer,"-1"))
	                exit(0);
	            else
		    {
	                strrev(readbuffer, 0, strlen(readbuffer)-1);
	                write(second_p[1],readbuffer,(strlen(readbuffer)+1));
	            }
	    }
        } 
	else /* Ebeveyn prosess */
	{
	   
	    close (first_p[0]);
	    close (second_p[1]);

            char inp[20];
	    while (true)
 	    {
            	printf("Write Something: ");
	        scanf("%s", inp);
        	write(first_p[1],inp,(strlen(inp)+1));
	        if (!strcmp(inp, "-1"))
                {
        	        wait (NULL);
			break;
        	}
 		else
		{
                	p2_read = read(second_p[0], readbuffer_to_send, sizeof(readbuffer_to_send));
	                printf("Here is the reversed version: %s\n",readbuffer_to_send);
        	}
	    }
        }
	
	return(0);
}


