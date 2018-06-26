/* Run this to create a test file called "temp.in" */

#include <stdio.h>

int main(void)
{
    long x;

    for (x = 0; x < 200000L; x++)
    {
        printf("123456789012345678901234567890\n");
    }
    return (0);
}
