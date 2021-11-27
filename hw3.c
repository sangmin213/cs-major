#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_LEN 100

typedef enum {false,true,red,black} bool;

/*
기존 rbt에서 Node의 key값을 Student ID로 지정해야한다.
Node가 앞서 specification에서 정의한 Student 구조체의
역할을 다해야한다.
*/


//student database
typedef struct _info{
    struct _info* next;
    char Semester;
    int Year;
    char Course[MAX_LEN];
    char Grade[MAX_LEN];
    int Credit;
    float GradeScore; //삭제할 때 필요
}Information;

//node
typedef struct _node{
    struct _node* parent;
    struct _node* left;
    struct _node* right;
    int key; // student ID
    int Credit_hour; //whole credit of this student
    float GPA;
    Information* info; //each course
    char color;
    char nil;
}Node; //Student

//red black tree
typedef struct{
    Node* root;
    Node* nil;
}RBT;

//The database of students data by rbt
typedef struct{
    int Student_num; //# of students
    float GPA; // GPA of all students
    RBT* head; // student rbt
}Database; //


//Use Queue for print rbt function 
typedef struct{
    Node* items[MAX_LEN];
    int front, rear;
}Queue;

void InitQueue (Queue *pqueue)
{
    pqueue->front=pqueue->rear=0;
}

bool QueueIsFull(Queue *pqueue)
{
    return pqueue->front==(pqueue->rear+1) % MAX_LEN;
}

bool QueueIsEmpty(Queue *pqueue)
{
    return pqueue->front==pqueue->rear;
}

Node* QueuePeek(Queue *pqueue)
{
    if(!QueueIsEmpty(pqueue))
        return pqueue->items[pqueue->front];
    else
        exit(1);
}

void QueuePush(Queue *pqueue,Node* item)
{
    if(!QueueIsFull(pqueue)){        
        pqueue->items[pqueue->rear]=item;
        pqueue->rear=(pqueue->rear+1)%MAX_LEN;
    }
    else
        exit(1);
}

void QueuePop(Queue *pqueue)
{
    if(!QueueIsEmpty(pqueue))
        pqueue->front=(pqueue->front+1)%MAX_LEN;
    else
        exit(1);
}


//<red black tree implementing functions>
//init student node of rbt
void InitNode(Node* node){
    node->parent=NULL;
    node->left=NULL;
    node->right=NULL;
    node->key=-1;
    node->Credit_hour=0;
    node->GPA=0;
    node->info=NULL;
    node->color=black;
    node->nil=0; // 1이면 nil
}


Node* Minimum(Node* x)
{
    while(x->left->nil==0)
        x=x->left;
    return x;
}


Node* Successor(Node* x)
{
    if(x->right->nil==0)
        return Minimum(x->right);
    else{
        Node* y=x->parent;
        while(y->nil==0 && x==y->right){
            x=y;
            y=y->parent;
        }
        return y;
    }
}


void LeftRotate(RBT* T, Node* x)
{
    Node* y=x->right;
    x->right=y->left;
    if(y->left!=T->nil)
        y->left->parent=x;
    y->parent=x->parent;
    if(x->parent==T->nil)
        T->root=y;
    else if(x==x->parent->left)
        x->parent->left=y;
    else
        x->parent->right=y;
    y->left=x;
    x->parent=y;
    return;
}


void RightRotate(RBT* T, Node* x)
{
    Node* y=x->left;//
    x->left=y->right;
    if(y->right!=T->nil)
        y->right->parent=x;
    y->parent=x->parent;
    if(x->parent==T->nil)
        T->root=y;
    else if(x==x->parent->right)
        x->parent->right=y;
    else
        x->parent->left=y;//
    y->right=x;
    x->parent=y;
    return;
}


void RB_Insert_Fixup(RBT* T, Node* z)
{
    Node* y;
    while(z->parent->color==red){
        if(z->parent==z->parent->parent->left){
            y=z->parent->parent->right;
            if(y->color==red){
                z->parent->color=black;
                y->color=black;
                z->parent->parent->color=red;
                z=z->parent->parent;
            }
            else{
                if(z==z->parent->right){
                    z=z->parent;
                    LeftRotate(T,z);
                }
                z->parent->color=black;
                z->parent->parent->color=red;
                RightRotate(T,z->parent->parent);
            }
        }
        else{
            y=z->parent->parent->left;
            if(y->color==red){
                z->parent->color=black;
                y->color=black;
                z->parent->parent->color=red;
                z=z->parent->parent;
            }
            else{
                if(z==z->parent->left){
                    z=z->parent;
                    RightRotate(T,z);
                }
                z->parent->color=black;
                z->parent->parent->color=red;
                LeftRotate(T,z->parent->parent);
            }
        }

    }
    T->root->color=black;
}


void RB_Insert(RBT* T, Node* z)
{
    Node* y=T->nil;
    Node* x=T->root;
    while(x!=T->nil){
        y=x;
        if(z->key<x->key)
            x=x->left;
        else
            x=x->right;
    }
    z->parent=y;
    if(y==T->nil)
        T->root=z;
    else if(z->key<y->key)
        y->left=z;
    else
        y->right=z;
    z->left=T->nil;
    z->right=T->nil;
    z->color=red;
    RB_Insert_Fixup(T,z);
}


//Head를 통한 input 입력
//잘못 입력 시 에러 출력하고 return 0
Node* search(RBT* T,int k); //밑에 있는 함수 미리 땡겨옴. 코딩 순서 고려
int Input(Database* DB)
{
    int studentID;
    char semester;
    int year;
    char course[MAX_LEN];
    char grade[MAX_LEN];
    int credit;

    float grade_score;

    Node* student=(Node*)malloc(sizeof(Node));
    Node* node; // student가 tree 안에 있는지 확인용
    Information* information=(Information*)malloc(sizeof(Information));
    Information* tmp; // student의 마지막 information 찾는 용도

    //입력 과정
    printf("Student id(2022123456): ");
    scanf("%d",&studentID);
    getchar();
    
    printf("Semester(S or F): ");
    scanf("%c",&semester);
    getchar();

    printf("Year(2021): ");
    scanf("%d",&year);
    getchar();

    printf("Course(SWE2021): ");
    fgets(course,MAX_LEN,stdin);
    course[strlen(course)-1]='\0';

    printf("Grade(A+,A,B+,...,D,F): ");
    fgets(grade,MAX_LEN,stdin);
    grade[strlen(grade)-1]='\0';
    
    printf("Credit(3/2/1): ");
    scanf("%d",&credit);
    getchar();
    
    //학점 계산
    if(strlen(grade)==1){
        switch(grade[0]){
            case 'A':
                grade_score=4;
                break;
            case 'B':
                grade_score=3;
                break;
            case 'C':
                grade_score=2;
                break;
            case 'D':
                grade_score=1;
                break;
            case 'F':
                grade_score=0;            
        }
    }
    else{
        switch(grade[0]){
            case 'A':
                grade_score=4.5;
                break;
            case 'B':
                grade_score=3.5;
                break;
            case 'C':
                grade_score=2.5;
                break;
            case 'D':
                grade_score=1.5;
        }
    }

    //Node 생성
    InitNode(student);
    student->key=studentID;
    //information 생성
    information->next=NULL;
    information->Semester=semester;
    information->Year=year;
    strcpy(information->Course,course);
    strcpy(information->Grade,grade);
    information->Credit=credit;
    information->GradeScore=grade_score;


    //student 있는지 search
    if(DB->head->nil==(node=search(DB->head,studentID))){ //없다면
        student->info=information; //info 의 첫 원소로 이번 information 넣음
        student->Credit_hour+=credit;
        student->GPA=grade_score;
        RB_Insert(DB->head,student);
        // 전체 학생 정보 update
        DB->GPA=(DB->GPA*DB->Student_num+grade_score)/(++DB->Student_num); //gpa, student num update
    }
    else{ //있다면
        tmp=node->info;
        while(tmp->next!=NULL)
            tmp=tmp->next;
        tmp->next=information; // student의 info에 이번 information 추가
        //student 및 전체 학생 정보 update
        float gpa;
        gpa = DB->GPA * DB->Student_num - node->GPA; // DB gpa update 용
        node->GPA = (node->GPA * node->Credit_hour + grade_score * credit) / (node->Credit_hour + credit); //this student gpa update
        node->Credit_hour+=credit; //this student credit hour update
        DB->GPA = (gpa + node->GPA)/DB->Student_num; //whole gpa update 
    }
    
    Print(DB,student,NULL); //input result 출력
    return 1; //정상적으로 입력했을 시.
}



void RB_Delete_Fixup(RBT* T,Node* x)
{
    while(x!=T->root && x->color==black){
        Node* w;
        if(x==x->parent->left){
            w=x->parent->right;
            if(w->color==red){
                w->color=black;
                x->parent->color=red;
                LeftRotate(T,x->parent);
                w=x->parent->right;
            }
            if(w->left->color==black && w->right->color==black){
                w->color=red;
                x=x->parent;
            }
            else{
                if(w->right->color==black){
                    w->left->color=black;
                    w->color=red;
                    RightRotate(T,w);
                    w=x->parent->right;
                }
                w->color=x->parent->color;
                x->parent->color=black;
                w->right->color=black;
                LeftRotate(T,x->parent);
                x=T->root;
            }
        }
        else{
            w=x->parent->left;
            if(w->color==red){
                w->color=black;
                x->parent->color=red;
                RightRotate(T,x->parent);
                w=x->parent->left;
            }
            if(w->left->color==black && w->right->color==black){
                w->color=red;
                x=x->parent;
            }
            else{
                if(w->left->color==black){
                    w->right->color=black;
                    w->color=red;
                    LeftRotate(T,w);
                    w=x->parent->left;
                }
                w->color=x->parent->color;
                x->parent->color=black;
                w->left->color=black;
                RightRotate(T,x->parent);
                x=T->root;
            }
        }
    }
    x->color=black;
}


Node* RB_Delete(RBT* T,Node* z)
{
    Node* y;
    Node* x;
    if(z->left==T->nil || z->right==T->nil)
        y=z;
    else
        y=Successor(z);

    if(y->left!=T->nil)
        x=y->left;
    else
        x=y->right;

    x->parent=y->parent;
    if(y->parent==T->nil)
        T->root=x;
    else if(y==y->parent->left)
        y->parent->left=x;
    else
        y->parent->right=x;

    if(y!=z)
        z->key=y->key;

    if(y->color==black)
        RB_Delete_Fixup(T,x);
    return y;
}


void Remove(Database* DB)
{
    int studentID;
    char course[MAX_LEN];
    int credit;

    float gpa_score; // The student's gpa
    float grade_score; // The student's course's grade

    Node* node; // 해당 student
    Information* tmp1,*tmp2; // student의 마지막 information 찾는 용도

    //삭제할 대상 입력 과정
    printf("Student id(2022123456): ");
    scanf("%d",&studentID);
    getchar();

    printf("Course(SWE2021 or if you want to delete all, enter 'ALL'): ");
    fgets(course,MAX_LEN,stdin);
    course[strlen(course)-1]='\0';
    
    node=search(DB->head,studentID);
    //해당 학생 없는 경우
    if(node==DB->head->nil){
        printf("There is no student whose id is %d\n",studentID);
        return;
    }
    
    //case 1 : 학생 id만 입력
    if(strcmp(course,"ALL")==0){
        grade_score=node->GPA;
        DB->GPA=(DB->GPA*DB->Student_num-grade_score)/(--DB->Student_num); //출력을 위해 먼저 전체 학생 정보 계산하여 update
        Print(DB,node,NULL); //삭제할 학생 정보 및 전체 정보 출력
        //삭제 진행
        tmp1=node->info;
        while(tmp1!=NULL){
            tmp2=tmp1;
            tmp1=tmp1->next;
            free(tmp2);
        }
        RB_Delete(DB->head,node);
    }
    else{
        gpa_score=node->GPA;
        tmp1=node->info;
        while(strcmp(tmp1->Course,course)!=0){
            tmp2=tmp1;
            tmp1=tmp1->next;
        }
        if(tmp1==NULL){ //해당 학생이 입력한 과목을 수강하지 않은 경우
            printf("This student does not learn %s\n",course);
            return;
        }
        else{
            tmp2->next=tmp1->next->next;
            grade_score=tmp1->GradeScore;
            credit=tmp1->Credit;
            //student gpa & credit hour update
            node->GPA=(gpa_score * node->Credit_hour - grade_score * credit) / (node->Credit_hour - credit);
            node->Credit_hour-=credit;
            //databse gpa & # of students update
            DB->GPA=(DB->GPA * DB->Student_num - gpa_score + node->GPA) / DB->Student_num;
            //print deleted course and total student information
            Print(DB,node,tmp1);
            free(tmp1); // delete the course
        }
    }

    return;
}


void Print(Database* DB, Node* student, Information* deleted)
{
    //error case
    if(student==NULL || student==DB->head->nil){
        printf("There is no such student\n");
        return;
    }

    Information* cur; //현재 info 노드
    Information* prev; //이전 info 노드(학기 혹은 년도 바뀐 것 확인 용)
    int remain_credit;
    
    /* 
    <deleted=null 인 경우>
    1. 그냥 학생의 정보를 원하는 경우
    2. insert인 경우
    3. deletion all 인 경우
    */
    if(deleted!=NULL){
        cur=student->info;
        prev=cur;
        while(cur!=NULL){
            if(strcmp(prev->Semester,cur->Semester)!=0 || prev->Year!=cur->Year) // 학기/년도 별로 구분
                printf("\n");
            printf("%d %c %s %s %d\n",cur->Year, cur->Semester, cur->Course, cur->Grade, cur->Credit);
            prev=cur;
            cur=cur->next;
        }
        if((remain_credit = 140- student->Credit_hour) > 0);
        else
            remain_credit = 0;
        printf("[GPA: %.1f, Credit hour: %d]\n",student->GPA, remain_credit);
        printf("[Total Students: %d, GPA: %.1f]\n",DB->Student_num, DB->GPA);
    }

    //deletion 특정인 경우: condition==2 and deleted=something;
    else{
        cur=deleted;
        printf("%d %c %s %s %d\n",cur->Year, cur->Semester, cur->Course, cur->Grade, cur->Credit);
        
        //졸업 잔여 학점 계산
        if((remain_credit = 140- student->Credit_hour) > 0);
        else
            remain_credit = 0;
        printf("[GPA: %.1f, Credit hour: %d]\n",student->GPA, remain_credit);
        printf("[Total Students: %d, GPA: %.1f]\n",DB->Student_num, DB->GPA);
    }
}


void Print_BST(RBT* T)
{ 
	Queue queue;
    Node* node=T->root;
    int jmp[6]={1,3,7,15,31,63};
    int gap[63]={64,32,60,16,28,28,28,8,12,12,12,12,12,12,12,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    int val[63];
    int col[63];
    int idx=0, level=1, jmpidx=0;
	if(node==T->nil) 
		return;
	InitQueue(&queue);
	QueuePush(&queue, node);
    //printf("hi\n");
	while(!QueueIsEmpty(&queue)){
        node = QueuePeek(&queue);
		QueuePop(&queue);
		val[idx]=node->key;
        col[idx++]=node->color;
		if(node->left==NULL || node->left==T->nil)
			QueuePush(&queue, T->nil);
        else
            QueuePush(&queue, node->left);
		if(node->right==NULL || node->right==T->nil)
			QueuePush(&queue, T->nil);
        else
            QueuePush(&queue, node->right);
        if(idx==63)
            break;
	}
    for(int i=0;i<63;i++){
        if(jmp[jmpidx]==i){
            printf("\n");
            jmpidx++;
        }
        if(i==0){
            for(int j=0;j<gap[i]-1;j++)
                printf(" ");
            printf("%2d",val[i]);
            if(col[i]==red) 
                printf("r");
            else
                printf("b");
        }
        else if(i%2==1){
            for(int j=0;j<gap[i]-1;j++)
                printf(" ");    
            if(val[i]==-1)
                printf("(   ");
            else{
                printf("(%2d",val[i]);
                if(col[i]==red)
                    printf("r");
                else
                    printf("b");
            }
        }
        else if(i%2==0){
            for(int j=0;j<gap[i]-1;j++)
                printf(" ");
            if(val[i]==-1)
                printf("   )");
            else{
                printf("%2d",val[i]);
                if(col[i]==red) 
                    printf("r)");
                else
                    printf("b)");
            }
        }
    }
}


Node* search(RBT* T,int k)
{
    Node* x=T->root;
    while(x!=T->nil){
        if(x->key==k)
            return x;
        else if(x->key<k)
            x=x->right;
        else
            x=x->left;
    }
    return T->nil;
}


bool checkExist(int duplicate[], int key, Node* node, RBT* rbt)
{
    if(duplicate[key]==0){
        node->key=key;
        RB_Insert(rbt,node);
        return true;
    }
    return false;
}


int main()
{
    srand(time(NULL));
    Database DB;
    RBT rbt;
    Node nilnode; InitNode(&nilnode); nilnode.nil=1;
    rbt.nil=&nilnode; rbt.root=rbt.nil;
    //init DB
    DB.GPA=0; DB.Student_num=0; DB.head=&rbt;

    Input(&DB); printf("\n"); Input(&DB);
    printf("%d %.2f %d %s\n",rbt.root->key, rbt.root->GPA, rbt.root->info->Year, rbt.root->info->Course);
    printf("%.2f %d\n",DB.GPA,DB.Student_num);

    /*
    Node a; Node b; Node c; Node d; Node e; Node f; Node g; Node h; Node i; Node j; 
    Node k; Node l; Node m; Node n; Node o; Node p; Node q; Node r; Node s; Node t; 
    Node nodes[20]; 
    nodes[0]=a; nodes[1]=b; nodes[2]=c; nodes[3]=d; nodes[4]=e; nodes[5]=f; nodes[6]=g; nodes[7]=h; nodes[8]=i; nodes[9]=j;
    nodes[10]=k; nodes[11]=l; nodes[12]=m; nodes[13]=n; nodes[14]=o; nodes[15]=p; nodes[16]=q; nodes[17]=r; nodes[18]=s; nodes[19]=t;
    
    int duplicate[60]={0,};
    for(int i=0;i<20;i++){
        InitNode(&nodes[i]);
        while(1){
            int tmp=rand()%60;
            if(duplicate[tmp]==0){ //check duplicate
                duplicate[tmp]=1;
                nodes[i].key=tmp;
                break;
            }
        }
    }
    for(int i=0;i<20;i++){
        RB_Insert(&rbt,&nodes[i]);
    }

    Print_BST(&rbt);
    printf("\n\n");

    Node one, two, three, four, five;
    InitNode(&one); InitNode(&two); InitNode(&three); InitNode(&four); InitNode(&five);
    if(checkExist(duplicate,33,&one,&rbt)){
        Print_BST(&rbt);
        printf("\n\n");
    }
    if(checkExist(duplicate,12,&two,&rbt)){
        Print_BST(&rbt);
        printf("\n\n");
    }
    if(checkExist(duplicate,27,&three,&rbt)){
        Print_BST(&rbt);
        printf("\n\n");
    }
    if(checkExist(duplicate,41,&four,&rbt)){
        Print_BST(&rbt);
        printf("\n\n");
    }
    if(checkExist(duplicate,25,&five,&rbt)){
        Print_BST(&rbt);
        printf("\n\n");
    }

    Node* del;
    int arr[5]={41,27,25,12,33};
    for(int i=0;i<5;i++){
        del=search(&rbt,arr[i]);
        printf("Delete %d\n",del->key);
        RB_Delete(&rbt,del);
        Print_BST(&rbt);
        printf("\n\n");
    }
    */
    
}