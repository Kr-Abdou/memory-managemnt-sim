#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct partition
{
    int start_adr;
    int size;
    bool free;
} part;

typedef struct program {
    char name[20];
    int size;
    int time;
} prg;

typedef struct NODE
{
    prg data;
    struct NODE *next;
}node;

typedef struct liste {
    node *head;
    node *fin;
}Queue;

part * create_part (int num_part)
{
    part *partitions = (part*) malloc (num_part *sizeof(part));// table of structures
    int start = 0,i;
    for (i=0;i<num_part;i++)
    {
        printf("Enter the size of partition %d: ",i+1);
        scanf("%d",&(partitions [i].size));
        partitions[i].start_adr = start;
        partitions[i].free = true;
        start = start + partitions[i].size;
    }
    return partitions;
}

void print_part (part *partitions,int num_part)
{
    int i;
    for (i=0;i<num_part;i++)
    {
        if (partitions[i].free)
        {
            printf("partition %d: starting @: %d kb, size: %d kb, status:free\n",i+1,partitions[i].start_adr,partitions[i].size);
        }else{
            printf("partition %d: starting @: %d kb, size: %d kb, status:allocated\n",i+1,partitions[i].start_adr,partitions[i].size);
        }
    }
}
void addtoQ (Queue *q, prg p)
{
    node *n = (node*)malloc(sizeof(node));
    n->data = p;
    n->next = NULL;
    if (q->head==NULL)
    {
        q->head = n;
    }else{
        q->fin->next=n;
    }
    q->fin = n;
}

prg deleteQ (Queue *q)
{
    node *head = q->head;
    prg p = head->data;

    q->head = head->next;
    if (q->head ==NULL)
    {
        q->fin =NULL;
    }
    free (head);
    return p;
}
void printlist (Queue *wait)
{
    node *current = wait->head;
    while (current != NULL)
    {
        printf("[%s]|[%d]-->",current->data.name,current->data.size);
        current = current->next;
    }
    printf("NULL");
}

void first_fit (part *partitons,int num_part,Queue *wait)
{
    node *current = wait->head;
    while (current !=NULL)
    {
        prg curr_prg = current->data;

        bool found_part = false ;
        for (int i=0;i<num_part;i++)
        {
            if(partitons[i].free && partitons[i].size >= curr_prg.size)
            {
                partitons[i].free = false ;
                partitons[i].size = partitons[i].size - curr_prg.size;
                found_part = true;
                printf("your program is loaded into partition: [%d] free remaining size is %d kb",i,partitons[i].size);
                break;
            }
        }
         if (!found_part)
        {
           printf("\n error program %s couldnt be loaded in memory\n",curr_prg.name);
        }
        printf("\n program %s removed from list going to the next program\n",curr_prg.name);
        deleteQ(wait);
        current = current->next;

    }
        printf("\nthere is no next program you reached the last program\n");

}

int main()
{
    int num_part, choice, num_prg;
    part *partitions = NULL;
    Queue waiting;
    waiting.head = NULL;
    waiting.fin = NULL;

    while (1) {
        printf("\n1. Create Partitions\n");
        printf("2. Add Programs\n");
        printf("3. Display Program Queue\n");
        printf("4. Allocate Memory (First Fit)\n");
        printf("5. Display Partition Table\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (partitions != NULL) {
                    printf("\nPartitions already created!\n");
                } else {
                    printf("\nEnter the number of partitions: ");
                    scanf("%d", &num_part);
                    partitions = create_part(num_part);
                    printf("\nPartition table created successfully!\n");
                }
                break;
            case 2:
                if (partitions == NULL) {
                    printf("\nPlease create partitions first!\n");
                } else {
                    printf("\nEnter the number of programs: ");
                    scanf("%d", &num_prg);
                    for (int i = 0; i < num_prg; i++) {
                        prg p;
                        printf("\nEnter the name and size of program %d: ", i + 1);
                        scanf("%s %d", p.name, &p.size);
                        addtoQ(&waiting, p);
                    }
                    printf("\nPrograms added to the queue successfully!\n");
                }
                break;
            case 3:
                if (waiting.head == NULL) {
                    printf("\nProgram queue is empty!\n");
                } else {
                    printf("\n--------Program Queue--------\n");
                    printlist(&waiting);
                    printf("\n-----------------------------\n");
                }
                break;
            case 4:
                if (partitions == NULL) {
                    printf("\nPlease create partitions first!\n");
                } else if (waiting.head == NULL) {
                    printf("\nProgram queue is empty!\n");
                } else {
                    printf("\nAllocating memory using First Fit algorithm...\n");
                    first_fit(partitions, num_part, &waiting);
                    printf("\nMemory allocation completed!\n");
                }
                break;
            case 5:
                if (partitions == NULL) {
                    printf("\nPlease create partitions first!\n");
                } else {
                    printf("\n--------Partition Table--------\n");
                    printf("\n| Partition Number | Starting Address | Remaining Size (in KB) | Status |\n");
                    printf("\n|------------------|------------------|------------------------|--------|\n");
                    for (int i = 0; i < num_part; i++) {
                        printf("| %-16d | %-16d | %-24d | %-6s |\n", i + 1, partitions[i].start_adr, partitions[i].size, partitions[i].free ? "Free" : "Allocated");
                    }
                    printf("\n------------------------------\n");
                }
                break;
            case 6:
                printf("\nExiting...\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
                break;
        }
    }
    return 0;
}
