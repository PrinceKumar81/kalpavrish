#include <stdio.h>
#include <string.h>

struct Student
{
    int rollNo;
    char name[50];
    int marks[3];
    int total;
    float average;
    char grade;
};
 
int studentCount = 0;

int calculateTotal(int marks[]);
float calculateAverage(int total);
char assignGrade(float average);
void displayStars(char grade);
void printRollNumberRecursive(int rollNo, int digit);
void displayStudentInfo(struct Student s);
void processStudents();

int main()
{
    printf("   STUDENT PERFORMANCE ANALYZER\n");

    processStudents();

    printf("Total students processed: %d\n", studentCount);

    return 0;
}

void processStudents()
{
    int n, i;

    printf("Enter the number of students (1-100): ");
    scanf("%d", &n);

    if (n < 1 || n > 100)
    {
        printf("Invalid input! Number of students must be between 1 and 100.\n");
        return;
    }

    struct Student students[n];

    for (i = 0; i < n; i++)
    {
        printf("\n--- Student %d ---\n", i + 1);
        int rollduplicate;
        do
        {
            rollduplicate=0;
            printf("Enter roll number  ");
            scanf("%d", &students[i].rollNo);
            if (students[i].rollNo <= 0)
            {
                printf("  Invalid Roll Number! Please enter a positive number.\n");
                rollduplicate=1;
                continue;
            }
            for(int k=0;k<i;k++)
            {
                if (students[i].rollNo == students[k].rollNo)
                {
                    printf("  Roll Number already exists\n");
                    rollduplicate = 1;
                    break;
                }
            }
        } while (rollduplicate);

        printf("Enter Name: ");
        scanf(" %[^\n]s", students[i].name);

        printf("Enter marks for 3 subjects (0-100):\n");
        for (int j = 0; j < 3; j++)
        {
            do
            {
                printf("  Subject %d: ", j + 1);
                scanf("%d", &students[i].marks[j]);

                if (students[i].marks[j] < 0 || students[i].marks[j] > 100)
                {
                    printf("  Invalid marks! Please enter between 0-100.\n");
                }
            } while (students[i].marks[j] < 0 || students[i].marks[j] > 100);
        }

        students[i].total = calculateTotal(students[i].marks);
        students[i].average = calculateAverage(students[i].total);
        students[i].grade = assignGrade(students[i].average);

        studentCount++;
    }

    printf("       PERFORMANCE REPORT     \n");

    for (i = 0; i < n; i++)
    {

        if (students[i].average < 35)
        {
            printf("\n--- Student %d ---\n", i + 1);
            printf("Roll Number: ");
            //printRollNumberRecursive(students[i].rollNo, 1);
            printf("\nName: %s\n", students[i].name);
            printf("Total Marks: %d/300\n", students[i].total);
            printf("Average: %.2f%%\n", students[i].average);
            printf("Grade: %c\n", students[i].grade);
            printf("Status: FAIL\n");
        }

        displayStudentInfo(students[i]);
    }
    // printf("list of rollNo via Recursion");
    // for (i = 0; i < n; i++)
    // {
    //     printRollRecursively(students[i].rollNo, 1);
    //     printf(" ");
    // }
}

int calculateTotal(int marks[])
{

    int sum = 0;

    sum = marks[0] + marks[1] + marks[2];

    return sum;
}

float calculateAverage(int total)
{

    return (float)total / 3;
}

char assignGrade(float average)
{
    char grade;

    if (average >= 85)
    {
        grade = 'A';
    }
    else if (average >= 70)
    {
        grade = 'B';
    }
    else if (average >= 50)
    {
        grade = 'C';
    }
    else if (average >= 35)
    {
        grade = 'D';
    }
    else
    {
        grade = 'F';
    }

    return grade;
}

char assignGradeSwitch(float average)
{
    int avg = (int)average / 10;
    char grade;

    switch (avg)
    {
    case 10:
    case 9:
    case 8:
        grade = 'A';
        break;
    case 7:
        grade = 'B';
        break;
    case 6:
    case 5:
        grade = 'C';
        break;
    case 4:
    case 3:
        grade = 'D';
        break;
    default:
        grade = 'F';
        break;
    }

    return grade;
}

void displayStars(char grade)
{
    int starCount;

    switch (grade)
    {
    case 'A':
        starCount = 5;
        break;
    case 'B':
        starCount = 4;
        break;
    case 'C':
        starCount = 3;
        break;
    case 'D':
        starCount = 2;
        break;
    default:
        starCount = 0;
        break;
    }

    for (int i = 0; i < starCount; i++)
    {
        printf("* ");
    }
    printf("\n");
}

void printRollRecursively(int rollNo, int digit)
{
    if(rollNo<0)
    {
        printf("Negative rollNot is not allowed");
        return;
    }
    if(rollNo==0)
    {
        printf("You have entered zero,it is not allowed");
        return;
    }
    printRollRecursively(rollNo/10,digit+1);
    printf("%d",rollNo%10);
}

void displayStudentInfo(struct Student s)
{
    printf("\n--- Performance Report ---\n");

    printf("Roll Number: ");
    //printRollNumberRecursive(s.rollNo, 1);
    printf("\n");

    printf("Name: %s\n", s.name);
    printf("Marks: Subject 1: %d | Subject 2: %d | Subject 3: %d\n",
           s.marks[0], s.marks[1], s.marks[2]);
    printf("Total Marks: %d/300\n", s.total);
    printf("Average: %.2f%%\n", s.average);
    printf("Grade: %c\n", s.grade);

    if (s.grade != 'F')
    {
        displayStars(s.grade);
    }
}