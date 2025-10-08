#include <stdio.h>
int main()
{
    double num1,num2,result;
    char operation;

    printf("Enter an Expression like '3+5' \n ");
      scanf("%lf %c %lf",&num1,&operation,&num2); 

    switch(operation)
    {
        case '+':
        result = num1+num2;
        printf("Result = %lf\n",result);
        break;

        case '-':
        result = num1-num2;
        printf("Result = %lf\n",result);
        break;

        case '*':
        result = num1*num2;
        printf("Result = %lf\n",result);
        break;

        case '/':
        if(num2==0)
        {
            printf("0\n");
        }
        else
        {
            result = num1/num2;
            printf("Result =%lf\n",result);
        }
        break;
        default:
        printf("Error:Invalid Operator\n");

    }
    return 0;
}
