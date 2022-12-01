typedef struct ListNode
{
    double* data_value;
    int* data_timestamp;
    struct ListNode* next;
    struct ListNode* prev;
} ListNode;
typedef struct DoubleList
{
    ListNode* head;
    ListNode* tail;
    int length;
} DoubleList;

//initialize node
ListNode* init_node(int* new_timestamp, double* new_value);

//initialize list
DoubleList* init_list();

//free node
void free_node(ListNode* node);

//destroy list
void destroy_list(DoubleList* list);

//print list
void print_list(DoubleList* list, int print_timestamp, int print_value);

//add node to list
int insert_node(DoubleList* list, int timestamp, double value, int position);

//remove node
int remove_node(DoubleList* list, int position);

//approximates values from the list with 2 decimals
void aproximate(DoubleList* list, DoubleList* new_list);

//eliminate the nodes that are do not belong to [x-σ,x+σ]
void eliminate_exceptions(DoubleList* list, int k, DoubleList* result_list);

//executes what is requested at task 2.1
void task_e1(DoubleList* list, DoubleList* new_list);

//executes the task that it has been given
DoubleList* execute_task(DoubleList* list, char* task);

//inserts a node on the right position so that the list is sorted by timestamp
int insert_node_sorted(DoubleList* list, int timestamp, double value);

//creates a list with the middle nodes of an auxiliar sorted list formed from k nodes of the initial list
void build_median_list(DoubleList* list, DoubleList* median_list, int k);

//executes what is requested at task 2.2,1
void task_e2(DoubleList* list, DoubleList* new_list);

//executes what is requested at task 2.2.2
void task_e3(DoubleList* list, DoubleList* new_list);

//executes what is requested at task 2.3
void task_u(DoubleList* list, DoubleList* new_list);

//modifies timestamp and value according to the task's request if the timestamp belongs to the interval [100,1000]
void time_frequence(DoubleList* list, DoubleList* new_list);

//copies the list
void copy_list(DoubleList* list, DoubleList* dstList);

//finds the minimum value from the list
double find_min_list(DoubleList* list);

//prints what is requested at task 2.5
void task_st(DoubleList* list,int length);

//executes what is requested at task 2.4
void task_c(DoubleList* list, DoubleList* new_list);

//creates a list formed by all arithmetic means of k values from the list; the timestamp will be the integer from middle node of the k nodes in the aux_list   
void arithmetic_mean(DoubleList* list, int k, DoubleList* new_list);
