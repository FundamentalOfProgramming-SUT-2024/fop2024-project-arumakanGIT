#include <string.h>
#include <stdio.h>

int checkMail(char *email)
{
    if (strchr(email, ' '))
        return 0;
    if (!strrchr(email, '@'))
        return 0;
    if (strchr(email, '@') != strrchr(email, '@'))
        return 0;
    if (!strrchr(email, '.'))
        return 0;
    if (strchr(email, '.') != strrchr(email, '.'))
        return 0;
    int a = strchr(email, '@') - email;
    int dot = strchr(email, '.') - email;
    if (!a)
        return 0;
    if (a > dot)
        return 0;
    if (!(dot - a - 1))
        return 0;
    if (!(strlen(email) - dot - 1))
        return 0;
    return 1;
}

int main()
{
    char *email = "arumakan@gmail.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arumakangmailcom";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arumakangmail.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arumakan@gmailcom";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "a@g.c";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arumakan@.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arumakan@gmail.";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = " @gmail.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "r@sharif.edu";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "@gmail.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arum@akan@gmail.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    email = "arum.akan@gmail.com";
    puts(email);
    if (checkMail(email))
        printf("Correct\n");
    else
        printf("InCorrect!\n");
    return 0;
}
