#include <stdio.h>
#include <stdlib.h>
#include "tema1.h"
#include <string.h>

int main(int argc, char *argv[])
{
    DoubleList* list, *new_list;
    int i,nr_pairs,timestamp;
    double value;

    list = init_list();

    scanf("%d",&nr_pairs);
    
    for(i=0;i<nr_pairs;i++)
        {
            scanf("%d%lf",&timestamp,&value);
            insert_node(list,timestamp,value,i);
        }

    for(i=1;i<argc;i++)
    {
    
        //if the task is one of the first 4 it will return the new list that has been requested
        if(strcmp(argv[1], "--e1") == 0 || strcmp(argv[1], "--e2") == 0 || strcmp(argv[1], "--e3") == 0 || strcmp(argv[1], "--u") == 0 || strcmp(argv[1],"--c")==0)       
 	{
	   //the initial list will be destroyed and replaced with the new one
	   new_list=execute_task(list,argv[i]);
           destroy_list(list);        
           list = new_list;
	}
	//otherwise task 5 will be executed and the result will be printed 
	else
	{
	   char char_length[10];

	   //calculate the length of an interval
	   strcpy(char_length,argv[1]);
	   strcpy(char_length, char_length + 4);
           int length = atoi(char_length);
	   task_st(list, length);
	}
    }

   //if we executed one of the first 4 tasks then we will print the new list
   if(strcmp(argv[1], "--e1") == 0 || strcmp(argv[1], "--e2") == 0 || strcmp(argv[1], "--e3") == 0 || strcmp(argv[1], "--u") == 0 || strcmp(argv[1], "--c") == 0)
   { 
      new_list = NULL;
      printf("%d\n",list->length);
      print_list(list,1,1);
   }

    destroy_list(list);

    return 0;
}
