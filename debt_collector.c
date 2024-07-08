#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define INF 99999


//Debtor details of the array format are stored in this structure.
typedef struct Debtor
{
    char D_name[30];
    int amount;
    float total_amount;
     int day;
    int month;
    int year;
}D;


//Agent details are stored in this array
typedef struct Agent
{
    char A_name[30];
    char User_ID[20];
    int password;
    float total_cash;

    struct Agent *next;
}A;


//Agent details are hashed, for this hashing we need a linked list data structure.
//The below is a linked list data structure for chain hashing.
typedef struct Agent1
{
    char A_name[30];
    char User_ID[20];
    int password;
    float total_cash;
}A1;



//Debtors who have paid the money are stored in this structure.
//This is a tree structure.
typedef struct Debtortree{
    struct Debtortree *lchild;
    char D_name[30];
    int amount;
    float total_amount;
    struct Debtortree *rchild;
} Debtor;


float c=0;
Debtor *root=NULL;


// Function to create a new paid debtor node
Debtor* createDebtorNode(char name[], int amount, float total_amount) {
    Debtor *newDebtor;
    newDebtor= (Debtor *)malloc(sizeof(Debtor));
    strcpy(newDebtor->D_name, name);
    newDebtor->amount = amount;
    newDebtor->total_amount = total_amount;
    newDebtor->lchild = NULL;
    newDebtor->rchild = NULL;
    return newDebtor;
}



// Function to insert a paid debtor node into the BST
Debtor* insertDebtorNode(Debtor *root, char name[], int amount, float total_amount) {
    if (root == NULL) {
        return createDebtorNode(name, amount, total_amount);
    }
    if (root->total_amount > total_amount) {
        root->lchild = insertDebtorNode(root->lchild, name, amount, total_amount);
    } else if (root->total_amount  < total_amount) {
        root->rchild = insertDebtorNode(root->rchild, name, amount, total_amount);
    }
    return root;
}



// Function to perform inorder traversal of the BST
void inorder(Debtor *root) {
    if (root == NULL) {
        return;
    }
    inorder(root->lchild);
    printf("\n%s %d %.2f\n", root->D_name, root->amount, root->total_amount);
    inorder(root->rchild);
}



// Function to search for a debtor node in the BST
Debtor* search(Debtor *root, char name[]) {
    if (root == NULL || strcmp(root->D_name, name) == 0) {
        return root;
    }
    if (strcmp(name, root->D_name) < 0) {
        return search(root->lchild, name);
    } //return (search(root->rchild, name));


return(search(root->rchild,name));
}



//Calculates interest of all debtors according to todays date.
 void Intrest_calc(D *C,int d,int m,int y)
 {

    int D,M,Y;


               float temp;
                for(int i=0;i<15;i++)
                {
                    if(y==(C+i)->year)
                    {
                        M=m-(C+i)->month;
                    }
                    else
                    {
                        M = (12*(y-(C+i)->year))-(C+i)->month + m;
                    }



                    temp= ((C+i)->amount);
                   // printf("\n  %d-%d-%d  \n",D,M,Y);
                    for(int j=0;j<M;j++)
                    {
                         temp=temp+(temp*0.05)/12;
                    }
                    (C+i)->total_amount=temp;
                   // printf("\n%f",temp);
                }

 }



 //Prints the adjacency matrix.
void print(int n,float CO[][15])
{
    printf("\n\n\n");
    int i,j;
    for(i = 0; i <n; i++)
    {
        for(j = 0; j< n; j++)
        {
            if(CO[i][j]==INF)
            {
                printf("INF\t");
            }
            else{
                    printf("%0.1f\t" , CO[i][j]);
                }
        }
        printf("\n");}
}



//Converts the distance in km weights to money/km weights
//This is done to collect more money in less time
void changeWeights(int n,float CO[][15],D *C)
{
    int x,y;
    for(x=0;x<n;x++)
    {
        for(y=0;y<n;y++)
        {
            if(x!=y){
                CO[x][y]=(C+y)->total_amount/CO[x][y];
            }
        }
    }
}



//This gives the highest index
//This is highest index is nothing but the next house to visit
int Find_Highest(float CO[][15],int s,D *C,float a)
{
    int z,i;
    float temp;
    float H,p;
    H=CO[s][0];
    for(i=1;i<15;i++)
    {
        if(CO[s-1][i]>H)
        {
             H=CO[s-1][i];
             //printf("\n%f",*c);

             z=i;
        }
    }
    temp = c + (C+z)->total_amount;
    //printf("\n%f\n",temp);
    //printf("\n%s paid -> %f ",(C+(z))->D_name,temp);
    if(temp > a+5000)
    {
        CO[s-1][z]=0;
        z=Find_Highest(CO,s,C,a);
        //printf("\n%d\n",z);
    }
    return (z);
}



//This function does the amount collection
//The agent is the user of this function
//The debtor is the person the function is executed on
void collection(float a,float CO[][15],int s,D *C)
{
    int j;
    for(j=0;j<15;j++)
    {
        CO[j][s-1]=0;
    }
    root=insertDebtorNode(root,(C+(s-1))->D_name,(C+(s-1))->amount,(C+(s-1))->total_amount);

    c = c + (C+(s-1))->total_amount;

    //printf("\n%s",(C+(s-1))->D_name);
    printf("\n%s paid -> %f ",(C+(s-1))->D_name,(C+(s-1))->total_amount);
    //printf("\n%d ",s);
    int i;
    //printf("\n%f",c);
    if(a<=c)
    {
        printf("\nTotal collection: %f",c);
        c=0;
        return;
    }
    else
    {
        i=Find_Highest(CO,s,C,a);
        collection(a,CO,i+1,C);
    }
}



//This function gives the shortest possible path from one house to all the other houses
//Shortest path algorithm
void floyed(int n,float CO[][15])
{
    int i,j,k;
    //printf("The supplied input is\n");
/*  for(i = 0; i <n; i++)
    {
        for(j = 0; j< n; j++)
        {
            if(CO[i][j]==INF)
            {
                printf("INF\t");
            }
            else
            {
                printf("%0.1f\t" , CO[i][j]);
            }

        }
        printf("\n");
    }*/


    for(k = 0; k <n; k++) {
        for(i = 0; i < n; i++) {
            for(j = 0; j < n; j++) {
                if((CO[i][k] + CO[k][j]) < CO[i][j])
                    CO[i][j] = CO[i][k] + CO[k][j];
            }
        }
    }

  /*  printf("\n\nAll pair shortest paths:\n\n");
    for(i = 0; i <n; i++)
    {
        for(j = 0; j< n; j++)
        {
            if(CO[i][j]==INF)
            {
                printf("INF\t");
            }
            else
            {
                    printf("%0.1f\t" , CO[i][j]);
            }
        }
        printf("\n");
    }*/
}



//Used to store the agent data using a Chain Hash table
void Hash_data(A **H[10],A1 Z[15])
{
    int k,i;
    for(k=0;k<15;k++)
    {
        i=Z[k].password%10;
        insert_at_the_end(&H[i],Z[k].A_name,Z[k].User_ID,Z[k].password);
    }
}



//Create node for Hash function
A* getnode()
{
    A *l;
    l=(A*)malloc(sizeof(A));
    l->User_ID[0]='\0';
    l->password=0;
    l->A_name[0]='\0';
    l->total_cash=0;
    l->next=NULL;
    return l;
}



//Inserts the data at the end in an Hash table
void insert_at_the_end (A **H,char Tname[30],char Tuserid[30],int Tpassword)
{
    A *temp,*newnode,*p=*H;
    newnode=getnode();
    newnode->password=Tpassword;
    strcpy(newnode->A_name,Tname);
    strcpy(newnode->User_ID,Tuserid);
    if(*H==NULL)
    {
        *H=newnode;

    }
    else
    {
        temp=*H;
        while(temp->next!=NULL)
        {
            temp=temp->next;

        }
        temp->next=newnode;
    }

}



//Finding the agent details in the Hash table
bool Find_hash(A **H,char user[20],int pass)
{
    A *temp;
    int i;
    bool b;
    b=false;
    i=pass%10;
    temp=*(H+i);
    while(temp!=NULL)
    {
        if(temp->password==pass)
        {
            b=true;
            break;
        }
        temp=temp->next;

    }
    if(b)
    {
        if(strcmp(temp->User_ID,user)==0)
        {
            b=true;
        }
        else
        {
            b=false;
        }
    }
    return b;
}



int main() {
    int i,j,k,l;
    int d,m,y;


    //insert agent details
    A1 Z[15] = {
    {"Rahul", "rahul1", 1234, 0},
    {"Priya", "priya2", 5678, 0},
    {"Anjali", "anjali_3", 9012, 0},
    {"Karan", "karan4", 3456, 0},
    {"Sneha", "sneha5", 7890, 0},
    {"Rajesh", "rajesh_6", 2345, 0},
    {"Amit", "amit7", 6789, 0},
    {"Manisha", "manisha8", 1534, 0},
    {"Pranav", "pranav_9", 5178, 0},
    {"Radhika", "radhika10", 9312, 0},
    {"Vivek", "vivek11", 3756, 0},
    {"Smita", "smita12", 7790, 0},
    {"Arjun", "arjun13", 2645, 0},
    {"Neha", "neha14", 6709, 0},
    {"Amitabh", "amitabh15", 1230, 0}};


    /*for(int i=0;i<15;i++)
    {
        printf("%s %s %d\n",Z[i].A_name,Z[i].User_ID,Z[i].password);
    }*/

    //create hash table
    A *H[10]={NULL};
    Hash_data(&H,Z);


    //insert debtor detials
    // Initialize C1 array
    D C1[15] = {
        {"Ravi", 10000,0,10,01,2023},
        {"Priya", 5000,0, 12,01,2023},
        {"Amit", 7500, 0,29,12,2022},
        {"Sneha", 15000, 0,30,12,2022},
        {"Vikram", 20000, 0 , 31,12,2022},
        {"Kavita", 3000, 0,23,01,2023},
        {"Anjali", 12000, 0 , 15,02,2023},
        {"Raj", 8000, 0,17,02,2023},
        {"Neha", 25000, 0,06,02,2023},
        {"Arun", 7000, 0,19,02,2023},
        {"Aisha", 18000, 0,25,01,2023},
        {"Sanjay", 9000, 0,29,01,2023},
        {"Mayank", 6000, 0,30,02,2023},
        {"Pooja", 11000, 0,12,01,2023},
        {"Siddharth", 4000, 0,26,02,2023},
    };

    // Initialize C2 array
    D C2[15] = {
        {"Gaurav", 12000, 0,10,01,2022},
        {"Nisha", 8000, 0,19,11,2021},
        {"Rahul", 15000, 0,10,06,2022},
        {"Smita", 10000, 0,18,03,2020},
        {"Aakash", 9000, 0,02,11,2021},
        {"Anita", 4000, 0,12,12,2022},
        {"Deepak", 6000, 0,21,10,2022},
        {"Shikha", 20000, 0,02,01,2023},
        {"Hitesh", 13000, 0,19,07,2022},
        {"Nehal", 3000, 0,18,11,2022},
        {"Mukesh", 18000, 0,25,02,2023},
        {"Tanvi", 7000, 0,11,11,2022},
        {"Divya", 5000, 0,12,03,2022},
        {"Shashi", 14000, 0,12,05,2021},
        {"Ritu", 11000, 0,22,12,2021},
    };

    // Initialize C3 array


        D C3[15]= {
                   {"Rohan",8000,0,12,03,2022},
                   {"Rohini",5000,0,11,11,2021},
                   {"Ramu",4000,0,5,12,2020},
                   {"varun",9000,0,17,11,2022},
                   {"kumar",5000,0,01,02,2023},
                   {"Amith",9700,0,18,12,2020},
                   {"Singham",6000,0,20,11,2021},
                   {"Kavya",2000,0,17,10,2020},
                   {"Omshree",6050,0,19,11,2022},
                   {"Raone",9800,0,29,12,2020},
                   {"Sidharnth",4800,0,17,11,2019},
                   {"Raghani",6900,0,22,12,2022},
                   {"Raghav",1000,0,31,10,2021},
                   {"Raksha",1000,0,28,02,2022},
                   {"Vineet", 8000, 0,1,9,2022},

                   };



        //Colony 1 Adjacency matrix
        float CO1[15][15]={{0,1,0,10,4,0,0,0,0,2,0,0,0,0,0},
                    {1,0,7,9,0,0,0,0,0,0,3,1,0,0,0},
                    {0,7,0,3,0,0,0,5,0,0,0,0,4,4,0},
                    {10,9,3,0,0,0,0,0,0,0,0,0,0,0,0},
                    {4,0,0,0,0,3,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,3,0,5,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,5,0,0,3,0,0,0,0,0,0},
                    {0,0,5,0,0,0,0,0,0,0,0,6,0,0,0},
                    {0,0,0,0,0,0,3,0,0,0,0,0,0,0,0},
                    {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,3,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,1,0,0,0,0,0,6,0,0,0,0,0,0,0},
                    {0,0,4,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,4,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}};

        for(i=0;i<15;i++)
        {
            for(j=0;j<15;j++)
            {
                if(i!=j)
                {
                    if(CO1[i][j]==0)
                    {
                        CO1[i][j]=INF;
                    }
                }
            }
        }




    //Colony 2 Adjacency matrix
   float CO2[15][15] = {
    {0, 2, 6, INF, 4, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, 0, INF, 1, 5, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, 3, 0, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {2, INF, 3, 0, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, 2, 0, 6, INF, 1, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, 0, 1, INF, 2, INF, 4, 5, 3, INF, INF},
    {INF, INF, INF, 3, INF, INF, 0, 2, 1, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, 0, INF, 1, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, 0, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, 3, 0, 2, INF, INF, 4, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 0, INF, INF, INF, 2},
    {INF, INF, INF, INF, INF, 4, INF, INF, INF, INF, INF, 0, 1, INF, INF},
    {INF, INF, INF, INF, INF, 2, INF, INF, INF, INF, INF, INF, 0, 5, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 2, INF, INF, INF, 0, 3},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, INF, INF, INF, 0}
};



//Colony 3 Adjacency matrix
float CO3[15][15]=  {
    {0, 2, INF, 1, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {2, 0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, 2, 0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {1, INF, 2, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, 1, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, 1, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, 1, 0, 1, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, 1, 0, 1, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, 1, 0, 1, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, 1, 0, 1, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 1, 0, 2, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 2, 0, 2, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 2, 0, 2, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 2, 0, 1},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 1, 0}
};


    while(true)
    {

        printf("\nPRESS ANY BUTTON EXCEPT 0 TO LOGIN:\n");
        int g;
        scanf("%d",&g);
        if(g==0)
        {
            break;
        }
        else
        {
                char user[20];
                int pass;
                bool a;
                printf("Enter your UserID:\n");
                scanf("%s",user);
                printf("Enter password:\n");
                scanf("%d",&pass);
                a=Find_hash(&H,user,pass);
                if(a)
                {
                    printf("\nLOGIN SUCCESSFUL!\n");
                    while(true)
                    {
                        int c,n=15,s,d,m,y;
                        float amount;


                        printf("\nENTER TODAYS DATE");
                        printf("\nDate : ");
                        scanf("%d",&d);
                        printf("\nMonth : ");
                        scanf("%d",&m);
                        printf("\nYear : ");
                        scanf("%d",&y);
                        Intrest_calc(C1,d,m,y);
                        Intrest_calc(C2,d,m,y);
                        Intrest_calc(C3,d,m,y);



                        printf("\nENTER AMOUNT TO BE COLLECTED:\n");
                        scanf("%f",&amount);

                        printf("\n WHICH COLONY WOULD YOU LIKE TO SELECT:\n");
                        printf("1->Colony 1\n2->Colony 2\n3->Colony 3\n4->Exit\n");
                        scanf("%d",&c);


                        switch(c)
                        {
                            case 1:
                                printf("\nENTER AGENTS STARTING LOCATION:\n");
                                scanf("%d",&s);
                                floyed(n,CO1);
                                changeWeights(n,CO1,C1);
                                //print(n,CO1);
                                collection(amount,CO1,s,C1);
                                break;

                            case 2:
                                printf("\nENTER AGENTS STARTING LOCATION:\n");
                                scanf("%d",&s);
                                floyed(n,CO2);
                                changeWeights(n,CO2,C2);
                                //print(n,CO2);
                                collection(amount,CO2,s,C2);
                                break;
                            case 3:
                                printf("\nENTER AGENTS STARTING LOCATION:\n");
                                scanf("%d",&s);
                                floyed(n,CO3);
                                changeWeights(n,CO3,C3);
                                //print(n,CO3);
                                collection(amount,CO3,s,C3);
                                break;
                            default: printf("\nNo colony , TRY Again!\n");

                        }
                        printf("\n\nDo you want to continue for the next trip or logout?\nIf continue press 0 else press 1.\n");
                        int ans;
                        scanf("%d",&ans);
                        if(ans==1)
                        {
                            break;
                        }
                    }
                    printf("\n\nDo you want to view PAID debtor details?\nIf yes press 1 else 0:\n");
                    int choice;
                    scanf("%d",&choice);
                    if(choice==1)
                    {
                        printf("\n\nPAID DEBTOR DETAILS:\n");
                        inorder(root);
                    }
                }
                else
                {
                    printf("\nINVALID LOGIN.\nTRY AGAIN!\n");
                }
                printf("\n\nDo you want to login again with another user or exit?\n1->Login again.0->Exit\n");
                int h;
                scanf("%d",&h);
                if(h==0)
                {
                    break;
                }
        }
    }
    return 0;
}




/*
CO3

(0)---1---(1)---2---(2)
 |         |         |
 5         2         3
 |         |         |
(5)       (6)       (3)
 | \       | \       | \
  |  6      |  7      |  4
 |   \     |   \    |   \
(6)---7---(2)---8---(7)---9---(8)
          |         |
          7         8
          |         |
         (9)       (4)
*/
/*
CO4
  0 --2-- 1 --1-- 3 --2-- 4
  |       |         |         |
  6       5         |         6
  |       |         |       z  |
  7 --1-- 8 --2-- 9 --4-- 10 --5--11
                      |             |
                      1             2
                      |             |
                    12 --1-- 13    14 --3-- 15
*/


/* colony 6
0 --2-- 1 --1-- 3 --1-- 4
|     |     |     |
2     2     2     1
|     |     |     |
INF   |     |     |
|     |     |     |
6     |     |     |
|     |     |     |
|     |     |     |
|     1     |     |
|     |     |     |
INF   5     1     |
|     |     |     |
|     |     |     |
|     3 --1-- 6 --1-- 7
|           |     |
|           1     1
|           |     |
|           8 --1-- 9
|           |     |
|           1     |
|           |     |
|           10    |
|           |     |
|           2 --2-- 11
|                 |
|                 2
|                 |
|                 12 --2-- 13
|                       |
|                       2
|                       |
|                       14 --1-- 15
|                             |
|                             1
|                             |
|                             INF
*/