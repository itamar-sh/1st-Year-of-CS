#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LINE 63
#define STUDENT_LINE 60
#define LENGTH_OF_ID 10
#define PLACE_OF_FIRST_COMMA 10
#define MIN_AGE 18
#define MAX_AGE 120
#define MAX_GRADE 100


int check_student_info(char student_info[]);
typedef struct Student{
    int age;
    int grade;
    long long int id;
} Student; //new type of struct
void best_student(Student *start, Student *end);
Student* manage_input(int num_of_students);
int contoller(int argc, char *argv[]);
int get_num_of_students();
void bubble_sort(Student *start, Student *end);
void quick_sort(Student *start, Student *end);
void print_arr(Student *start, Student *end);
Student *partition(Student *start, Student *end);


int main (int argc, char *argv[]){
    return contoller(argc, argv);
}

int contoller(int argc, char *argv[]){
    if (argc == 2 && strcmp(argv[1], "best") == 0){
            int num_of_students = get_num_of_students();
            if (num_of_students == 0){
                fprintf(stdout, "ERROR: cant read num of students\n");
                return EXIT_FAILURE;
            }
            Student *p_students = manage_input(num_of_students);
            if (p_students == NULL){
                fprintf(stdout, "ERROR: not enough space\n");
                return EXIT_FAILURE;
            }
            best_student(p_students, (p_students+num_of_students));
            free(p_students);
    }
    else if (argc == 2 && strcmp(argv[1], "bubble") == 0){
            int num_of_students = get_num_of_students();
            if (num_of_students == 0){
                fprintf(stdout, "ERROR: cant read num of students\n");
                return EXIT_FAILURE;
            }
            Student *p_students = manage_input(num_of_students);
            if (p_students == NULL){
                fprintf(stdout, "ERROR: not enough space\n");
                return EXIT_FAILURE;
            }
            bubble_sort(p_students, (p_students+num_of_students));
            free(p_students);
    }
    else if (argc == 2 && strcmp(argv[1], "quick") == 0){
            int num_of_students = get_num_of_students();
            if (num_of_students == 0){
                fprintf(stdout, "ERROR: cant read num of students\n");
                return EXIT_FAILURE;
            }
            Student *p_students = manage_input(num_of_students);
            if (p_students == NULL){
                fprintf(stdout, "ERROR: not enough space\n");
                return EXIT_FAILURE;
            }
            quick_sort(p_students, (p_students+num_of_students-1));
            print_arr(p_students, (p_students+num_of_students));
            free(p_students);
    }
    else {
        fprintf(stdout, "USAGE: there was a problem with the arguments\n");
        return EXIT_FAILURE;
    }
    return 0;
}

int get_num_of_students(){
    int num_of_students = 0;
    while(num_of_students ==0) {
        char input_line[INPUT_LINE];
        fprintf(stdout, "Enter number of students. Then enter\n");
        if(fgets(input_line, INPUT_LINE, stdin) == NULL){
            return 0;
        }
        if (sscanf(input_line, "%d", &num_of_students) == EOF){
            return 0;
        }
        if (num_of_students <= 0){
            fprintf(stdout, "ERROR: cant read num of students\n");
            num_of_students = 0;
        }
    }
    return num_of_students;
}
Student* manage_input(int num_of_students){
    char student_line[STUDENT_LINE];
    int counter_students = 0;
    int age, grade;
    long long int id;

    Student *p_student = malloc(sizeof(Student)*num_of_students);
    if(p_student == NULL){
        return NULL;
    }

    while(counter_students<num_of_students){
        fprintf(stdout, "Enter student info. Then enter\n");
        if(fgets(student_line, STUDENT_LINE, stdin) == NULL){
            free(p_student);
            return NULL;
        }
        if (check_student_info(student_line) == 1){
            if(sscanf(student_line, "%lld,%d,%d", &id, &grade, &age) == EOF)
            {
                free(p_student);
                return NULL;
            }
            // add check for right input
            p_student[counter_students].id = id;
            p_student[counter_students].age = age;
            p_student[counter_students].grade = grade;
            counter_students++;
        }
    }
    return p_student;
}

int check_student_info(char student_info[]){
    unsigned int length_of_string = strlen(student_info);
    // check id format
    if(student_info[0]=='0'){
            fprintf(stdout, "ERROR: Id must have 10 digits and "
                            "can not start with 0\n");
            return 0;
        }
    for (int i=0; i< LENGTH_OF_ID;i++){
            if(student_info[i]<'0' || student_info[i]>'9'){
                    fprintf(stdout, "ERROR: Id must have 10 digits and"
                                    " can not start with 0\n");
                    return 0;
                }
        }
    if(student_info[PLACE_OF_FIRST_COMMA]!=','){
        fprintf(stdout, "ERROR: Id must have 10 digits and can "
                        "not start with 0\n");
        return 0;
    }
    // check grade format
    for(unsigned int i=LENGTH_OF_ID+1;i<length_of_string-1;i++){
        if((student_info[i]<'0' || student_info[i]>'9') &&
        student_info[i] != ','){
            fprintf(stdout, "ERROR: Grade or age should be an integer "
                            "between 0 and 100.\n");
            return 0;
        }
    }
    // check age and grade values
    int age = 0;
    int grade = 0;
    long long int id = 0;
    if(sscanf(student_info, "%lld,%d,%d", &id, &grade, &age) == EOF){
        return 0;
    }
    if(age<MIN_AGE || age >MAX_AGE){
            fprintf(stdout, "ERROR: Age should be an integer "
                            "between 18 and 120\n");
            return 0;
        }
    if(grade<0 || grade >MAX_GRADE){
            fprintf(stdout, "ERROR: Grade should be an integer "
                            "between 0 and 100\n");
            return 0;
        }
    // everything is good: return 1
    return 1;
}

void best_student(Student *start, Student *end){
    Student *max = start;
    float max_score = start->grade;
    max_score = max_score / start->age;
    while (start < end){
            float cur_score = start->grade;
            cur_score = cur_score / start->age;
            if(max_score<cur_score){
                    max_score = cur_score;
                    max = start;
            }
            start++;
    }
    fprintf(stdout, "best student info is: %lld,%d,%d\n", max->id,
            max->grade, max->age);
}

void bubble_sort(Student *start, Student *end){
    int length = end-start;
    int swapped;
    for (int i = 0; i<length;i++){
            swapped = 0;
            for(int j = 0; j<length-i-1;j++){
                    if ((*(start+j)).grade > (*(start+j+1)).grade){
                        Student temp_student = *(start+j);
                        *(start+j) = *(start+j+1);
                        *(start+j+1) = temp_student;
                        swapped = 1;
                        }
            }
            if(swapped == 0){
                break;
            }
    }
    print_arr(start,end);
}

void quick_sort(Student *start, Student *end){
    if(start<end){
            Student *pivot = partition(start, end);
            quick_sort(start, pivot-1);
            quick_sort(pivot+1, end);
        }
}

Student *partition(Student *start, Student *end){
    int pivot = (*(end)).age;
    Student *j = start-1;
    for (Student *i = start;i<=end-1;i++){
        if ((*i).age <= pivot){
            j++;
            Student temp_student = *(j);
            *(j) = *(i);
            *(i) = temp_student;
        }
    }
    Student temp_student = *(j+1);
    *(j+1) = *(end);
    *(end) = temp_student;
    return j+1;
}

void print_arr(Student *start, Student *end){
    int length = end - start;
    for(int i = 0;i < length;i++){
            fprintf(stdout, "%lld,%d,%d\n", (*(start+i)).id,
                    (*(start+i)).grade, (*(start+i)).age);

    }
}
