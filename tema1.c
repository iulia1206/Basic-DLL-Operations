#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "tema1.h"

//initialize node
ListNode* init_node(int* new_timestamp, double* new_value)
{
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    
    new_node->data_value = (double*)malloc(sizeof(double));
    new_node->data_timestamp = (int*)malloc(sizeof(int));
    
    *(new_node->data_timestamp) = *new_timestamp;
    *(new_node->data_value) = *new_value;
    
    new_node->next = NULL;
    new_node->prev = NULL;
    
    return new_node;
}

//initialize list
DoubleList* init_list()
{
    DoubleList* list = (DoubleList*)malloc(sizeof(DoubleList));
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    
    return list;
}

//free node
void free_node(ListNode* node)
{
    node->next = NULL;
    node->prev = NULL;
    free(node->data_value);
    free(node->data_timestamp);
    free(node);
}

//destroy list
void destroy_list(DoubleList* list)
{
    ListNode* node;

    while (list->length > 0)
    {
        node = list->tail;
        list->tail = list->tail->prev;
        free_node(node);
        list->length--;
    }
    free(list);
}

//print list
void print_list(DoubleList* list, int print_timestamp, int print_value)
{
    ListNode* node = list->head;

    while (node != NULL)
    {
        //print_timestamp and print_value will be printed if they equal 1
        if (print_timestamp == 1 && print_value == 1)
        {
            printf("%d %.2lf\n", *(node->data_timestamp), round(*(node->data_value) * 100) / 100);
            node = node->next;
        }
        else
            if (print_timestamp == 1)
            {
                printf("%d\n", *(node->data_timestamp));
                node = node->next;
            }
            else
            {
                printf("%.2lf\n", round(*(node->data_value) * 100) / 100);
                node = node->next;
            }
    }
}

//add node to list
int insert_node(DoubleList* list, int timestamp, double value, int position)
{
    //check if the position is out of range
    if (position > list->length || position < 0)
        return -1;

    ListNode* new_node = init_node(&timestamp, &value);

    //check if the memory is dinamicaly allocated properly
    if (new_node == NULL)
        return -1;

    //insert at the end or insert the first element
    if (position == list->length)
    {
        if (list->tail != NULL)
        {
            new_node->prev = list->tail;
            list->tail->next = new_node;
        }
        else
        {
            list->head = new_node;
        }
        list->tail = new_node;
        list->length++;
     
        return 0;
    }

    // Insert at the beginning
    if (position == 0)
    {
        new_node->next = list->head;
        new_node->prev = NULL;
        list->head->prev = new_node;
        list->head = new_node;
        list->length++;
        return 0;
    }

    //insert between nodes
    if (position <= list->length / 2)
    {
        int index = 0;
        ListNode* current_node = list->head;

        while (index < position)
        {
            current_node = current_node->next;
            index++;
        }

        new_node->next = current_node;
        new_node->prev = current_node->prev;
        current_node->prev->next = new_node;
        current_node->prev = new_node;
        list->length++;
    }
    else
    {
        int index = list->length - 1;
        ListNode* current_node = list->tail;

        while (index > position)
        {
            current_node = current_node->prev;
            index--;
        }

        new_node->next = current_node;
        new_node->prev = current_node->prev;
        current_node->prev->next = new_node;
        current_node->prev = new_node;
        list->length++;
    }
    return 0;
}

//remove node
int remove_node(DoubleList* list, int position)
{
    //check if the position is out of range or if the list is null
    if (position<0 || position>list->length - 1 || list->length == 0)
        return -1;
    
    ListNode* node;

    //remove first node
    if (position == 0)
    {
        node = list->head;
       
        if (list->length == 1)
            list->tail = NULL;
        else
            node->next->prev = NULL;
        list->head = node->next;
        free_node(node);
        list->length--;
        return 0;
    }

    //remove last node
    if (position == list->length - 1)
    {
        node = list->tail;
        node->prev->next = NULL;
        list->tail = node->prev;
        list->length--;
        free_node(node);
        return 0;
    }

    //remove intermediary node
    if (position <= list->length / 2)
    {
        int index = 0;
        node = list->head;

        while (index < position)
        {
            node = node->next;
            index++;
        }

        node->prev->next = node->next;
        node->next->prev = node->prev;
        free_node(node);
        list->length--;
    }
    else
    {
        int index = list->length - 1;
        node = list->tail;

        while (index > position)
        {
            node = node->prev;
            index--;
        }

        node->next->prev = node->prev;
        node->prev->next = node->next;
        free_node(node);
        list->length--;
    }
    return 0;
}

//copies the list
void copy_list(DoubleList* list, DoubleList* dstList)
{
    ListNode* node = list->head;
    int index = 0;

    while (node != NULL)
    {
        insert_node(dstList, *(node->data_timestamp), *(node->data_value), index);
        index++;
        node = node->next;
    }
}

//eliminate the nodes that are do not belong to [x-σ,x+σ]
void eliminate_exceptions(DoubleList* list, int k, DoubleList* aux_list)
{
    ListNode* current_node, * node;
    ListNode* middle_node = list->head;

    copy_list(list, aux_list);

    int i, index, nr_elim_nodes = 0;
    double average, deviation;
    node = list->head;

    //we are going through the list
    for (i = 0; i < list->length - k + 1; i++)
    {
        //calculating arithmetic mean
        current_node = node;
        index = 0;
        average = 0;

        while (index < k)
        {
            average = average + *(current_node->data_value);
            if (index == k / 2)
                middle_node = current_node;
            index++;
            current_node = current_node->next;
        }

        average = (double)average / k;

        //calculating deviation
        current_node = node;
        index = 0;
        deviation = 0;
        double square;

        while (index < k)
        {
            square = (*(current_node->data_value) - average) * (*(current_node->data_value) - average);
            deviation = deviation + square;
            index++;
            current_node = current_node->next;
        }

        deviation = (double)deviation / k;
        deviation = sqrt(deviation);

        //eliminate from the auxiliary list the nodes that need to be removed
        if (*(middle_node->data_value) >= deviation + average || *(middle_node->data_value) <= average - deviation)
        {
            remove_node(aux_list, k / 2 + i - nr_elim_nodes);
            nr_elim_nodes = nr_elim_nodes + 1;
        }
        node = node->next;
    }
}

//executes what is requested at task 2.1
void task_e1(DoubleList* list, DoubleList* new_list)
{
    eliminate_exceptions(list, 5, new_list);
}

//executes what is requested at task 2.2.1
void task_e2(DoubleList* list, DoubleList* new_list)
{
    build_median_list(list, new_list, 5);
}

//executes the task that it has been given and returns the new created list
DoubleList* execute_task(DoubleList* list, char* task)
{
    DoubleList* new_list = init_list();

    if (strcmp(task, "--e1") == 0)
        task_e1(list, new_list);
    else
        if (strcmp(task, "--e2") == 0)
            task_e2(list, new_list);
        else
            if (strcmp(task, "--e3") == 0)
                task_e3(list, new_list);
            else
                if (strcmp(task, "--u") == 0)
                    task_u(list, new_list);
                else
                    task_c(list,new_list);
    
    return new_list;
}

//inserts a node on the right position so that the list is sorted by timestamp
int insert_node_sorted(DoubleList* list, int timestamp, double value)
{
    if (list->length == 0)
    {
        insert_node(list, timestamp, value, 0);
    }
    else
    {
        ListNode* current_node = list->head;
        int index = 0;
        
        while (current_node != NULL)
        {
            //check where to insert the new node
            if (*(current_node->data_value) >= value)
            {
                insert_node(list, timestamp, value, index);
                return 0;
            }
        
            index++;
            current_node = current_node->next;
        }

        insert_node(list, timestamp, value, index);
    }
    return 0;
}

//creates a list with the middle nodes of an auxiliar sorted list formed from k nodes of the initial list
void build_median_list(DoubleList* list, DoubleList* median_list, int k)
{
    ListNode* current_node;
    int nr_nodes = 0, i;
    ListNode* node = list->head;

    //we are going through the list
    for (i = 0; i < list->length - k + 1; i++)
    {
        int index = 0;
        //we are taking an auxiliary list to sort the first k nodes and identificate the one in the middle 
        DoubleList* aux_list = init_list();
        current_node = node;

        while (index < k)
        {
            insert_node_sorted(aux_list, *(current_node->data_timestamp), *(current_node->data_value));
            index++;
            current_node = current_node->next;
        }

        ListNode* aux_node = aux_list->head;
        int nr_aux_list = 0;

        while (nr_aux_list < k / 2)
        {
            aux_node = aux_node->next;
            nr_aux_list++;
        }

        insert_node(median_list, *(aux_node->data_timestamp), *(aux_node->data_value), nr_nodes);
        destroy_list(aux_list);
        nr_nodes++;
        node = node->next;
    }

    int index = 0;
    ListNode* median_node = median_list->head;
    int index_list = 0;
    ListNode* list_node = list->head;
    
    while (index_list < k / 2)
    {
        index_list++;
        list_node = list_node->next;
    }

    //data_timestemp must remain the same so we are replacing it with the right data_stemp from the initial list
    while (index < median_list->length - k / 2 + 2)
    {
        *(median_node->data_timestamp) = *(list_node->data_timestamp);
        index++;
        median_node = median_node->next;
        list_node = list_node->next;
    }
}

//creates a list formed by all arithmetic means of k values from the list; the timestamp will be the integer from middle node of the k nodes in the aux_list   
void arithmetic_mean(DoubleList* list, int k, DoubleList* new_list)
{
    ListNode* current_node;
    int nr_nodes = 0, i;
    ListNode* node = list->head;

    //we are going through the list
    for (i = 0; i < list->length - k + 1; i++)
    {
        int index = 0;
        current_node = node;
        double mean = 0;

        //we are taking an auxiliary list with the first k nodes
        DoubleList* aux_list = init_list();
        ListNode* middle_node;

        while (index < k)
        {
            insert_node(aux_list, *(current_node->data_timestamp), *(current_node->data_value), index);
            index++;
            current_node = current_node->next;
        }

        ListNode* aux_node = aux_list->head;
        int nr_aux_list = 0;

        //we are calculating the arithmetc mean 
        while (nr_aux_list < k)
        {
            mean = mean + *(aux_node->data_value);
            nr_aux_list++;

            if (nr_aux_list == k / 2)
                middle_node = aux_node;

            aux_node = aux_node->next;
        }

        mean = (double)mean / k;

        //we insert the middle node with the new data_value
        middle_node = aux_list->head->next->next;
        insert_node(new_list, *(middle_node->data_timestamp), mean, i);

        destroy_list(aux_list);
        node = node->next;
    }
    int index = 0;
    ListNode* new_list_node = new_list->head;
    int index_list = 0;
    ListNode* list_node = list->head;
    while (index_list < k / 2)
    {
        index_list++;
        list_node = list_node->next;
    }

    //we adjust data_timestemp according to the task's request
    while (index < new_list->length - k / 2 + 2)
    {
        *(new_list_node->data_timestamp) = *(list_node->data_timestamp);
        index++;
        new_list_node = new_list_node->next;
        list_node = list_node->next;
    }
}

//executes what is requested at task 2.2.2
void task_e3(DoubleList* list, DoubleList* new_list)
{
    arithmetic_mean(list, 5, new_list);
}

//executes what is requested at task 2.3
void task_u(DoubleList* list, DoubleList* new_list)
{
    time_frequence(list, new_list);
}

//modifies timestamp and value according to the task's request if the timestamp belongs to the interval [100,1000]
void time_frequence(DoubleList* list, DoubleList* new_list)
{
    ListNode* node;
    double timestamp,value, dif;
    node = list->head;
    
    insert_node(new_list, *(node->data_timestamp), *(node->data_value), 0);
    int index = 1;

    //we are going through the list
    for (node = list->head->next; node != NULL; node = node->next)
    {
        dif = *(node->data_timestamp) - *(node->prev->data_timestamp);
        
        if (dif >= 100 && dif <= 1000)
        {
            timestamp = (int)round((*(node->data_timestamp) + *(node->prev->data_timestamp)) / 2);
            value= (double)(*(node->data_value) + *(node->prev->data_value)) / 2;
            
            *(node->data_timestamp) = ( (double)*(node->data_timestamp) + (double)*(node->prev->data_timestamp) ) / 2;
            *(node->data_value) = (double)(*(node->data_value) + *(node->prev->data_value)) / 2;
            insert_node(new_list,timestamp, value, index);
        }
        else
        {
            insert_node(new_list, *(node->data_timestamp), *(node->data_value), index);
        }
        index++;
    }
}

//finds the minimum value from the list
double find_min_list(DoubleList* list)
{
    ListNode* node=list->head;
    double min_value=*(node->data_value);
    
    for (node = list->head->next; node != NULL; node = node->next)
        if (min_value > *(node->data_value))
            min_value = *(node->data_value);
    
    return min_value;
}

//finds the maximum value from the list
double find_max_list(DoubleList* list)
{
    ListNode* node = list->head;
    double max_value = *(node->data_value);
    
    for (node = list->head->next; node != NULL; node = node->next)
        if (max_value < *(node->data_value))
            max_value = *(node->data_value);
    
    return max_value;
}

//prints what is requested at task 2.5
void task_st(DoubleList* list,int length)
{
    ListNode* node;
    double min_value = find_min_list(list);
    int interval_down = (int)floor(min_value);
    int interval_up = interval_down + length;
    double max_value = find_max_list(list);

    //while there are still numbers left without an interval selected
    while (interval_down<max_value)
    {
        int index = 0;

        for (node = list->head; node != NULL; node = node->next)
        {
            
            if (interval_down <= *(node->data_value) && *(node->data_value) <= interval_up)
                index++;

        }

        if (index != 0)
            printf("[%d, %d] %d\n", interval_down, interval_up, index);

        interval_down = interval_up;
        interval_up = interval_up + length;
    }
}

//calculating C as requested 
double calculate_C(int timestamp, int left_timestamp, int right_timestamp)
{
    double C;
    C = (double)(timestamp - left_timestamp) / (right_timestamp - left_timestamp);
    return C;
}

//calculating w as requested 
double calculate_w(int i, int k)
{
    double w;
    w = ((double)i / (k - 1)) * ((double)i / (k - 1)) * 0.9 + 0.1;
    double s=0;
    int j;
    for (j = 0; j < k; j++)
    {
        s=s+ ((double)j / (k - 1)) * ((double)j / (k - 1)) * 0.9 + 0.1;
    }
    w = (double)w / s;
    return w;
}

//calculating data_value as requested 
double calculate_value(double C, DoubleList* left_list, DoubleList* right_list)
{
    ListNode* left_node = left_list->tail;
    ListNode* right_node = right_list->tail;
    int i;
    double s_left = 0;
    double s_right = 0;
    double value;

    for (i = 0; i < 3; i++)
    {
        double w = calculate_w(i, 3);
        s_left = s_left + *(left_node->data_value) * w;
        left_node = left_node->prev;
        s_right = s_right + *(right_node->data_value) * w;
        right_node = right_node->prev;
    }

    value = (1 - C) * s_left + C * s_right;
    return value;
}

//executes what is requested at task 2.4
void task_c(DoubleList* list, DoubleList* new_list)
{
    ListNode* node;
    double timestamp, value, dif;

    ListNode *new_list_node = new_list->head;
    int index = 3;

    //we are going through the list starting with the third node
    for (node = list->head->next->next; node != list->tail->prev->prev; node = node->next)
    {
        //we create a list with the left 'neighbors'
        DoubleList* left_list = init_list();
        insert_node(left_list, *(node->data_timestamp), *(node->data_value), 0);
        insert_node(left_list, *(node->prev->data_timestamp), *(node->prev->data_value), 1);
        insert_node(left_list, *(node->prev->prev->data_timestamp), *(node->prev->prev->data_value), 2);

        //we create a list with the right 'neighbors'
        DoubleList* right_list = init_list();
        insert_node(right_list, *(node->next->data_timestamp), *(node->next->data_value), 0);
        insert_node(right_list, *(node->next->next->data_timestamp), *(node->next->next->data_value), 1);
        insert_node(right_list, *(node->next->next->next->data_timestamp), *(node->next->next->next->data_value), 2);

        dif = *(node->next->data_timestamp) - *(node->data_timestamp);

        if (dif >= 1000)
        {
            timestamp = *(node->data_timestamp) + 200;
            int aux = *(node->next->data_timestamp);

            //we insert a new node with the previous data_timestamp + 200 and with the new value 
            while (timestamp < aux)
            {
                ListNode* left_node = left_list->head;
                ListNode* right_node = right_list->head;
                double C = calculate_C(timestamp, *(left_node->data_timestamp), *(right_node->data_timestamp));

                insert_node(list, timestamp, calculate_value(C, left_list, right_list), index);
                node = node->next;
                index++;
                timestamp = timestamp + 200;
            }
            index = index + 1;
        }
        else
            index++;
        destroy_list(left_list);
        destroy_list(right_list);
    }
    copy_list(list, new_list);
}
