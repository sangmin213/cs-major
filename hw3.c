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
typedef struct _trans{
    struct _trans* next;
    int year; //년도
    int credit; // 수강 학점
    char semester; //학기
}Transcript;

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
    Transcript* transcript;
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


void InitNode(Node* node);
Node* Minimum(Node* x);
Node* Successor(Node* x);
void LeftRotate(RBT* T, Node* x);
void RightRotate(RBT* T, Node* x);
void RB_Insert_Fixup(RBT* T, Node* z);
void RB_Insert(RBT* T, Node* z);
int Input(Database* DB);
void RB_Delete_Fixup(RBT* T,Node* x);
Node* RB_Delete(RBT* T,Node* z);
void Remove(Database* DB);
void PrintModified(Database* DB, Node* student, Information* deleted); //그때그때 변경 결과 Print 함수
void Prefix(RBT* T, Node* node);
int max(int a, int b);
int RB_Height(RBT* T, Node* node);
int RB_nodes(RBT* T, Node* node);
void RB_Print(RBT* T);
void Print_BST(RBT* T);
Node* search(RBT* T,int k);
void CheckStudent(Database* DB);


//<red black tree implementing functions>
//init student node of rbt
void InitNode(Node* node)
{
    node->parent=NULL;
    node->left=NULL;
    node->right=NULL;
    node->key=-1;
    node->Credit_hour=0;
    node->GPA=0;
    node->info=NULL;
    node->transcript=NULL;
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
    Information* tmp2=NULL; // student의 마지막 information 찾는 용도
    Transcript* trans=(Transcript*)malloc(sizeof(Transcript));
    Transcript* temp; // student의 알맞은 transcript 찾는 용도
    Transcript* prev; // student의 알맞은 transcript 찾는 용도

    //입력 과정
    printf("Student id(2022123456): ");
    scanf("%d",&studentID);
    getchar();
    
    printf("Semester(S or F): ");
    scanf("%c",&semester);
    getchar();
    //error case
    if(semester!=70 && semester!=83){
        printf("\nSemester should be S or F !!! \n");
        return 0;
    }

    printf("Year(2021): ");
    scanf("%d",&year);
    getchar();

    printf("Course(SWE2021): ");
    fgets(course,MAX_LEN,stdin);
    course[strlen(course)-1]='\0';

    printf("Grade(A+,A,B+,...,D,F): ");
    fgets(grade,MAX_LEN,stdin);
    grade[strlen(grade)-1]='\0';
    //error case
    if(grade[0]<65 || grade[0]>70 || grade[10]=='E'){
        printf("\nGrade should be A+,A,B+,B,C+,C,D+,D,F !!! \n");
        return 0;
    }
    else if(grade[1]!='\0' && grade[1]!='+'){
        printf("\nGrade should be A+,A,B+,B,C+,C,D+,D,F !!! \n");
        return 0;
    }
    else if(strlen(grade)>2){
        printf("\nGrade should be A+,A,B+,B,C+,C,D+,D,F !!! \n");
        return 0;
    }

    printf("Credit(3/2/1): ");
    scanf("%d",&credit);
    getchar();
    //error case
    if(credit<1 || credit>3){
        printf("\nCredit should be 1~3 !!!\n");
        return 0;
    }
    
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
    //transcript 생성
    trans->next=NULL;
    trans->year=year;
    trans->credit=credit;
    trans->semester=semester;

    //student 있는지 search
    if(DB->head->nil==(node=search(DB->head,studentID))){ //없다면
        student->info=information; //info 의 첫 원소로 이번 information 넣음
        student->transcript=trans; //첫 transcript 추가
        student->Credit_hour+=credit;
        student->GPA=grade_score;
        RB_Insert(DB->head,student);
        // 전체 학생 정보 update
        DB->GPA=(DB->GPA*DB->Student_num+grade_score)/(++DB->Student_num); //gpa, student num update
    }
    else{ //있다면
        //transcript를 확인하여 21학점 안 넘는지 확인
        temp=node->transcript;
        while(temp!=NULL){
            if(temp->year==year && temp->semester==semester){
                if(temp->credit + credit > 21){
                    printf("Over 21 credits. You cannot add more.\n"); //넘으면 에러 메세지 & 종료
                    //free(information); free(trans); free(student);
                    return 0;
                }
                //안 넘으면 더하기(추가)
                temp->credit+=credit;
                break;
            }
            prev=temp;
            temp=temp->next;
        }
        //해당 년도 해당 학기 없었다면 추가
        if(temp==NULL)
            prev->next=trans;
        
        // student의 info에 이번 information 년도/학기 순서에 맞추어 추가
        tmp=node->info;
        while(tmp!=NULL) {
            //현재 확인하고 있는 노드의 수강 년도가 이번에 추가할 수업의 년도 보다 더 크다면 멈추고 그 자리에 추가
            if(tmp->Year > year){
                break;
            }
            //현재 확인하고 있는 노드의 수강 년도가 이번에 추가할 수업의 년도 보다 더 작다면 멈추고 그냥 바로 다음으로 넘어가기
            else if(tmp->Year<year){
                tmp2=tmp;
                tmp=tmp->next;
            }
            else{
                //동일한 년도 일 때, 학기가 더 앞선 학기라 S > F 라면, 바로 다음으로 넘어가기
                if(tmp->Semester > semester){
                    tmp2=tmp;
                    tmp=tmp->next;
                }
                //그렇지 않다면 그 자리에 추가
                else
                    break;
            }
        }
        if(tmp2==NULL){ //맨 앞에 추가하는 경우
            information->next=tmp;
            node->info=information;
        }
        else{
            information->next=tmp2->next;
            tmp2->next=information;
        }
        
        //student 및 전체 학생 정보 update
        float gpa;
        gpa = DB->GPA * DB->Student_num - node->GPA; // DB gpa update 용
        node->GPA = (node->GPA * node->Credit_hour + grade_score * credit) / (node->Credit_hour + credit); //this student gpa update
        node->Credit_hour+=credit; //this student credit hour update
        DB->GPA = (gpa + node->GPA)/DB->Student_num; //whole gpa update 
    }
    //free(student); free(trans); // 사용 안한 메모리 해제
    printf("\n[New Insertion]\n");
    PrintModified(DB,student,NULL); //input result 출력
    RB_Print(DB->head);
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

    if(y!=z){
        z->key=y->key;
        z->Credit_hour=y->Credit_hour;
        z->GPA=y->GPA;
        z->info=y->info;
        z->transcript=y->transcript;
    }
        
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
    Information* tmp1,*tmp2=NULL; // student의 마지막 information 찾는 용도
    Transcript* temp1, *temp2;

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
        PrintModified(DB,node,NULL); //삭제할 학생 정보 및 전체 정보 출력
        //삭제 진행
        tmp1=node->info;
        while(tmp1!=NULL){
            tmp2=tmp1;
            tmp1=tmp1->next;
            free(tmp2);
        }
        temp1=node->transcript;
        while(temp1!=NULL){
            temp2=temp1;
            temp1=temp1->next;
            free(temp2);
        }
        RB_Delete(DB->head,node);
        RB_Print(DB->head);
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
            if(tmp2==NULL) // info의 첫 번째 노드를 삭제한 경우.
                node->info=tmp1->next;
            else
                tmp2->next=tmp1->next->next;
            grade_score=tmp1->GradeScore;
            credit=tmp1->Credit;
            //student transcript update
            temp1=node->transcript;
            while(temp1!=NULL){
                if(temp1->year==tmp1->Year && temp1->semester==tmp1->Semester){
                    temp1->credit-= tmp1->Credit;
                    if(temp1->credit==0){
                        //credit==0이라서 삭제해야 할 temp1이 trans의 첫번째 노드 인 경우
                        if(temp2==NULL)
                            node->transcript->next=temp1->next;
                        else //아니라면 연결 고리 update
                            temp2->next=temp1->next;
                        free(temp1);
                    }
                    break;
                }
                temp2=temp1;
                temp1=temp1->next;
            }
            //student gpa & credit hour update
            node->GPA=(gpa_score * node->Credit_hour - grade_score * credit) / (node->Credit_hour - credit);
            node->Credit_hour-=credit;
            //databse gpa & # of students update
            DB->GPA=(DB->GPA * DB->Student_num - gpa_score + node->GPA) / DB->Student_num;
            //print deleted course and total student information
            PrintModified(DB,node,tmp1);
            RB_Print(DB->head);
            free(tmp1); // delete the course
        }
    }

    return;
}


void PrintModified(Database* DB, Node* student, Information* deleted)
{
    //error case
    if(student==NULL){
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
    if(deleted==NULL){
        cur=student->info;
        prev=cur;
        while(cur!=NULL){
            if(prev->Semester!=cur->Semester || prev->Year!=cur->Year) // 학기/년도 별로 구분
                printf("\n");
            printf("%d %c %s %s %d\n",cur->Year, cur->Semester, cur->Course, cur->Grade, cur->Credit);
            prev=cur;
            cur=cur->next;
        }
        if((remain_credit = 140- student->Credit_hour) > 0);
        else
            remain_credit = 0;
        printf("[GPA: %.1f, Credit hour: %d]\n",student->GPA, remain_credit);
        printf("[Total Students: %d, GPA: %.3f]\n",DB->Student_num, DB->GPA);
    }

    //deletion 특정인 경우: deleted=something;
    else{
        cur=deleted;
        printf("%d %c %s %s %d\n",cur->Year, cur->Semester, cur->Course, cur->Grade, cur->Credit);
        
        //졸업 잔여 학점 계산
        if((remain_credit = 140- student->Credit_hour) > 0);
        else
            remain_credit = 0;
        printf("[GPA: %.1f, Credit hour: %d]\n",student->GPA, remain_credit);
        printf("[Total Students: %d, GPA: %.3f]\n",DB->Student_num, DB->GPA);
    }

    //RB_Print(DB->head);
}


void Prefix(RBT* T, Node* node)
{
    if(node==T->nil)
        return;
    Prefix(T,node->left);
    printf("%d ",node->key);
    Prefix(T,node->right);
}


int max(int a, int b)
{
    if(a>b)
        return a;
    return b;
}


int RB_Height(RBT* T, Node* node)
{
    int r=0,l=0;
    if(node->left!=T->nil)
        l=RB_Height(T,node->left);
    if(node->right!=T->nil)
        r=RB_Height(T,node->right);

    return 1+max(r,l);
}


int RB_nodes(RBT* T, Node* node)
{
    int r=0,l=0;
    if(node->left!=T->nil)
        l=RB_nodes(T,node->left);
    if(node->right!=T->nil)
        r=RB_nodes(T,node->right);

    return 1+r+l;
}


void RB_Print(RBT* T)
{
    int height;
    int nodes;
    if(T->root==T->nil){
        printf("There is no student yet.\n");
        return;
    }
    height=RB_Height(T,T->root);
    nodes=RB_nodes(T,T->root);
    printf("[DATABASE with red black tree]\n");
    printf("Total nodes: %d, Height: %d\n",nodes,height);
    Prefix(T,T->root);
    printf("\n");
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


void CheckStudent(Database* DB)
{
    int key;
    Node* node;
    printf("Student ID: ");
    scanf("%d",&key);
    getchar();

    node=search(DB->head,key);
    PrintModified(DB,node,NULL);    
}


void Interaction(Database* DB)
{
    int what;
    while(1){
        printf("\nWhat do you want to do?\n");
        printf("1: Insert\n2: Delete\n3: Check All\n4: Check a student\n0: Quit\nChoice: ");
        scanf("%d",&what);
        getchar();

        if(what==0)
            exit(1);
        else if(what==1)
            Input(DB);
        else if(what==2)
            Remove(DB);
        else if(what==3)
            RB_Print(DB->head);
        else
            CheckStudent(DB);
    }
}


int TestcaseInput(Database* DB,int studentID, char semester, int year, char* course, char* grade, int credit)
{
    float grade_score;

    Node* student=(Node*)malloc(sizeof(Node));
    Node* node; // student가 tree 안에 있는지 확인용
    Information* information=(Information*)malloc(sizeof(Information));
    Information* tmp; // student의 마지막 information 찾는 용도
    Information* tmp2=NULL; // student의 마지막 information 찾는 용도
    Transcript* trans=(Transcript*)malloc(sizeof(Transcript));
    Transcript* temp; // student의 알맞은 transcript 찾는 용도
    Transcript* prev; // student의 알맞은 transcript 찾는 용도

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
    //transcript 생성
    trans->next=NULL;
    trans->year=year;
    trans->credit=credit;
    trans->semester=semester;

    node=search(DB->head,studentID);

    //student 있는지 search
    if(DB->head->nil==node){ //없다면
        student->info=information; //info 의 첫 원소로 이번 information 넣음
        student->transcript=trans; //첫 transcript 추가
        student->Credit_hour+=credit;
        student->GPA=grade_score;
        RB_Insert(DB->head,student);
        // 전체 학생 정보 update
        DB->GPA=(DB->GPA*DB->Student_num+grade_score)/(++DB->Student_num); //gpa, student num update
    }
    else{ //있다면
        //transcript를 확인하여 21학점 안 넘는지 확인
        temp=node->transcript;
        while(temp!=NULL){
            if(temp->year==year && temp->semester==semester){
                if(temp->credit + credit > 21){
                    //printf("Over 21 credits. You cannot add more.\n"); //넘으면 에러 메세지 & 종료
                    //free(information); free(trans); free(student);
                    return 0;
                }
                //안 넘으면 더하기(추가)
                temp->credit+=credit;
                break;
            }
            prev=temp;
            temp=temp->next;
        }

        //해당 년도 해당 학기 없었다면 추가
        if(temp==NULL)
            prev->next=trans;

        // student의 info에 이번 information 년도/학기 순서에 맞추어 추가
        tmp=node->info;
        while(tmp!=NULL) {
            //현재 확인하고 있는 노드의 수강 년도가 이번에 추가할 수업의 년도 보다 더 크다면 멈추고 그 자리에 추가
            if(tmp->Year > year){
                break;
            }
            //현재 확인하고 있는 노드의 수강 년도가 이번에 추가할 수업의 년도 보다 더 작다면 멈추고 그냥 바로 다음으로 넘어가기
            else if(tmp->Year<year){
                tmp2=tmp;
                tmp=tmp->next;
            }
            else{
                //동일한 년도 일 때, 학기가 더 앞선 학기라 S > F 라면, 바로 다음으로 넘어가기
                if(tmp->Semester > semester){
                    tmp2=tmp;
                    tmp=tmp->next;
                }
                //그렇지 않다면 그 자리에 추가
                else
                    break;
            }
        }
        if(tmp2==NULL){ //맨 앞에 추가하는 경우
            information->next=tmp;
            node->info=information;
        }
        else{
            information->next=tmp2->next;
            tmp2->next=information;
        }
        
        //student 및 전체 학생 정보 update
        float gpa;
        gpa = DB->GPA * DB->Student_num - node->GPA; // DB gpa update 용
        node->GPA = (node->GPA * node->Credit_hour + grade_score * credit) / (node->Credit_hour + credit); //this student gpa update
        node->Credit_hour+=credit; //this student credit hour update
        DB->GPA = (gpa + node->GPA)/DB->Student_num; //whole gpa update 
    }
    //free(student); free(trans); // 사용 안한 메모리 해제
    //printf("\n[New Insertion]\n");
    //PrintModified(DB,student,NULL); //input result 출력
    return 1; //정상적으로 입력했을 시.
}


void TestcaseRemove(Database* DB, int studentID, char* course)
{
    int credit;

    float gpa_score; // The student's gpa
    float grade_score; // The student's course's grade

    Node* node; // 해당 student
    Information* tmp1,*tmp2=NULL; // student의 마지막 information 찾는 용도
    Transcript* temp1, *temp2;


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
        PrintModified(DB,node,NULL); //삭제할 학생 정보 및 전체 정보 출력
        //삭제 진행
        tmp1=node->info;
        while(tmp1!=NULL){
            tmp2=tmp1;
            tmp1=tmp1->next;
            free(tmp2);
        }
        temp1=node->transcript;
        while(temp1!=NULL){
            temp2=temp1;
            temp1=temp1->next;
            free(temp2);
        }
        RB_Delete(DB->head,node);
        RB_Print(DB->head);
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
            if(tmp2==NULL) // info의 첫 번째 노드를 삭제한 경우.
                node->info=tmp1->next;
            else
                tmp2->next=tmp1->next->next;
            grade_score=tmp1->GradeScore;
            credit=tmp1->Credit;
            //student transcript update
            temp1=node->transcript;
            while(temp1!=NULL){
                if(temp1->year==tmp1->Year && temp1->semester==tmp1->Semester){
                    temp1->credit-= tmp1->Credit;
                    if(temp1->credit==0){
                        //credit==0이라서 삭제해야 할 temp1이 trans의 첫번째 노드 인 경우
                        if(temp2==NULL)
                            node->transcript->next=temp1->next;
                        else //아니라면 연결 고리 update
                            temp2->next=temp1->next;
                        free(temp1);
                    }
                    break;
                }
                temp2=temp1;
                temp1=temp1->next;
            }
            //student gpa & credit hour update
            node->GPA=(gpa_score * node->Credit_hour - grade_score * credit) / (node->Credit_hour - credit);
            node->Credit_hour-=credit;
            //databse gpa & # of students update
            DB->GPA=(DB->GPA * DB->Student_num - gpa_score + node->GPA) / DB->Student_num;
            //print deleted course and total student information
            PrintModified(DB,node,tmp1);
            RB_Print(DB->head);
            free(tmp1); // delete the course
        }
    }

    return;
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

    /*
    Input(&DB); printf("\n"); Input(&DB);
    printf("%d %.2f %d %s\n",rbt.root->key, rbt.root->GPA, rbt.root->info->Year, rbt.root->info->Course);
    printf("%.2f %d\n",DB.GPA,DB.Student_num);
    */

    //Test-case
    //학번, 년도, semester, credit, 과목명, 성적
    int ID[100];
    int Year[100];
    char Semester[100];
    int Credit[100];
    char Grade[10][10]={"A+","A","B+","B","C+","C","D+","D","F"};
    char Course[100][10];
    char tmp1[10];
    char swe[10]="SWE";
    char tmp2[10];
    char zero[10]={0,};
    for(int i=0;i<100;i++){
        ID[i]=2021310121+i;
        Year[i]=2015+i%5;
        if(i%5==0 || i%6==0)
            Semester[i]='S';
        else
            Semester[i]='F';
        Credit[i]=i%3+1;
        strcpy(tmp1,zero);
        strcpy(tmp2,zero); //초기화
        strcpy(tmp1,swe);  //초기화
        sprintf(tmp2,"%d",2003+i); //SWE + 0000 숫자 담기
        strcat(tmp1,tmp2); // 과목명 생성
        strcpy(Course[i],tmp1); //저장
    }

    for(int i=0;i<100;i++){
        for(int j=0;j<i+1;j++)
            TestcaseInput(&DB,ID[i],Semester[j],Year[j],Course[j],Grade[j%9],Credit[j]);
    }
    RB_Print(&rbt);
    /*
    for(int i=0;i<10;i++){
        printf("%s ",Course[i]);
    }
    printf("\n");
    printf("%s\n",tmp2);
    printf("%s\n",tmp1);
    */
    
    Interaction(&DB);
}