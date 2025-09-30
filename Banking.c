#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int acc_no;
    char name[50];
    char address[100];
    float balance;
} Account;

typedef struct {
    int acc_no;
    char trans;
    float amount;
    float balance;
    float interest;
    int dd, mm, yy;
} Transaction;

void main_menu(); 
void new_account();
void transaction();
void give_balance(int acc_no);
int last_accno();
void add_to_file(Account acc);
void add_to_banking(Transaction t);
int display_list();
void call_balance();
int display_account();
void update_balance(int acc_no, float new_balance);
void modify_account();
void delete_account(int acc_no);
void close_account();
void edit_menu();

int found_account(int acc_no)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    Account acc;
    while (fread(&acc, sizeof(Account), 1, fp))
	{
        if (acc.acc_no == acc_no)
		{
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void modify_account()
{
    int acc_no;
    printf("Enter Account Number to Modify: ");
    scanf("%d", &acc_no);

    FILE *fp = fopen("INITIAL.dat", "rb+");
    Account acc;
    int found = 0;

    while (fread(&acc, sizeof(Account), 1, fp))
	 {
        if (acc.acc_no == acc_no)
		 {
            found = 1;
            getchar();
            printf("Enter New Name: ");
            fgets(acc.name, sizeof(acc.name), stdin);

            printf("Enter New Address: ");
            fgets(acc.address, sizeof(acc.address), stdin);

            fseek(fp, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);
            printf("Account modified successfully.\n");
            break;
    }

   }
    if (!found) printf("Account not found.\n");
    fclose(fp);
}

void delete_account(int acc_no)
{


    FILE *fp = fopen("INITIAL.dat", "rb");
    FILE *temp = fopen("TEMP.dat", "wb");
    Account acc;
    int found = 0;

    while (fread(&acc, sizeof(Account), 1, fp))
	 {
        if (acc.acc_no == acc_no)
		{
            found = 1;
        }
		else
		{
            fwrite(&acc, sizeof(Account), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("INITIAL.dat");
    rename("TEMP.dat", "INITIAL.dat");

    if (found)
		printf("Account deleted successfully.\n");
    else
		printf("Account not found.\n");
}

void close_account()
{
    int acc_no;
    printf("Enter Account Number to Close: ");
    scanf("%d", &acc_no);

    FILE *fp = fopen("BANKING.dat", "rb");
    FILE *temp = fopen("TEMP.dat", "wb");
    Transaction t;
    int found = 0;

    while (fread(&t, sizeof(Transaction), 1, fp))
	{
        if (t.acc_no == acc_no)
		{
            found = 1;
        } else
		{
            fwrite(&t, sizeof(Transaction), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("BANKING.dat");
    rename("TEMP.dat", "BANKING.dat");

    if (found)
		 printf("All transaction records deleted successfully.\n");
    else
		printf("Account not found in BANKING.dat.\n");

    delete_account(acc_no);
}

void edit_menu() {
    int choice;
    while (1) {
        printf("\n--- EDIT ACCOUNT MENU ---\n");
        printf("1. Modify Account\n");
        printf("2. Close Account\n");
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: modify_account(); break;
            case 2: close_account(); break;
            case 3: return;
            default: printf("Invalid choice.\n");
        }
    }
}

int last_accno()
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    Account acc;
    int last = 1000;  // Start account numbers from 1001

    if (fp != NULL) {
        while (fread(&acc, sizeof(Account), 1, fp)) {
            last = acc.acc_no;
        }
        fclose(fp);
    }

    return last + 1;
}

void add_to_file(Account acc)
 {
    FILE *fp = fopen("INITIAL.dat", "ab");
    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);
}

void add_to_banking(Transaction t)
{
    FILE *fp = fopen("BANKING.dat", "ab");
    fwrite(&t, sizeof(Transaction), 1, fp);
    fclose(fp);
}

void give_balance(int acc_no)
{
    FILE *fp = fopen("INITIAL.dat", "rb");
    Account acc;

    if (fp == NULL) {
        printf("File not found.\n");
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("Balance: %.2f\n", acc.balance);
            fclose(fp);
            return;
        }
    }

    printf("Account not found.\n");
    fclose(fp);
}

void call_balance()
{
    int acc_no;
    printf("Enter the account number: ");
    scanf("%d", &acc_no);
    give_balance(acc_no);
}

int display_list(){
     FILE *fp;
    Account acc;


    fp = fopen("INITIAL.dat", "rb");
    if(fp==NULL){
        printf("FILE NOT FOUND.\n");
        return -1;
    }

    printf("Details of account:\n");
    while(fread(&acc,sizeof(acc),1,fp)){

        printf("Account Number:%d\nName:%s\nAddress:%s\nBalance:%.2f\n\n",acc.acc_no,acc.name,acc.address,acc.balance);
        printf("---------------------------------------------------------------\n");
    }
    fclose(fp);
}

int display_account(){
        FILE *fp;
    Account acc;
    Transaction t;
    int account_no;
    int found=1;


    fp = fopen("INITIAL.dat", "rb");
    if(fp==NULL){
        printf("FILE NOT FOUND.\n");
        return -1;
    }

    printf("Enter the account no.:\n");
    scanf("%d",&account_no);
    while(fread(&acc,sizeof(acc),1,fp)){
        if(acc.acc_no==account_no){
             found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("ACCOUNT NOT FOUND.\n");
        exit (1);
    }


    printf("\nAccount Number: %d\t\t\tName: %s", acc.acc_no, acc.name);

    printf(" Global Report of Account\n\n");


    printf("%-12s  %-10s %-10s %-10s\n", "DATE", "DEPOSIT", "WITHDRAW", "BALANCE");
    printf("---------------------------------------------------------------\n");


    FILE *tran_fp = fopen("BANKING.dat", "rb");

    float total_deposit = 0, total_withdraw = 0;
    int has_tran = 0;

 while (fread(&t, sizeof(Transaction), 1, tran_fp)) {

    if (t.acc_no == account_no) {
        has_tran = 1;

        char date[20];
        sprintf(date, "%02d-%02d-%04d", t.dd, t.mm, t.yy);

       //deposit
        if (t.trans == 'D'||t.trans == 'd') {
            printf("%-12s %-10.2f %-10s %-10.2f\n", date, t.amount, "", t.balance);
            total_deposit += t.amount;
        }

        //withdrawl
        else if (t.trans == 'W'||t.trans == 'w') {
            printf("%-12s %-10s %-10.2f %-10.2f\n", date, "", t.amount, t.balance);
            total_withdraw += t.amount;
        }
    }


    }
    fclose(tran_fp);

    if (!has_tran) {
        printf("NO TRANSACTION FOUND FOR THIS ACCOUNT.\n");
        exit(1);
    }


    printf("---------------------------------------------------------------\n");
    printf("TOTAL ->            %-10.2f %-10.2f %-10.2f\n", total_deposit, total_withdraw, acc.balance);
}

void update_balance(int acc_no, float new_balance)
 {
    FILE *fp = fopen("INITIAL.dat", "r+b");
    Account acc;

    if (fp == NULL) {
        printf("FILE NOT FOUND.\n");
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp))
	 {
        if (acc.acc_no == acc_no)
		{
            acc.balance = new_balance;
            fseek(fp, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);
}

void transaction() {
    FILE *fp = fopen("BANKING.dat", "ab");
    Transaction t;
    Account acc;
    int acc_no;
    char trans_type;

    printf("Enter account number for transaction: ");
    scanf("%d", &acc_no);

    if (!found_account(acc_no)) {
        printf("Account not found.\n");
        return;
    }

    FILE *acc_fp = fopen("INITIAL.dat", "rb");
    while (fread(&acc, sizeof(Account), 1, acc_fp)) {
        if (acc.acc_no == acc_no) break;
    }
    fclose(acc_fp);

    printf("Enter transaction type (D for Deposit, W for Withdrawal): ");
    scanf(" %c", &trans_type);
    t.acc_no = acc_no;
    t.trans = trans_type;
    t.balance = acc.balance;

    if (trans_type == 'D'||trans_type == 'd') {
        printf("Enter amount to deposit: ");
        scanf("%f", &t.amount);
        t.balance += t.amount;
        t.interest = 0;
        printf("New balance: %.2f\n", t.balance);
    } else if (trans_type == 'W'||trans_type == 'w') {
        printf("Enter amount to withdraw: ");
        scanf("%f", &t.amount);
        if (t.amount > t.balance) {
            printf("INSUFFICIENT FUNDS.\n");
            return;
        }
        t.balance -= t.amount;
        t.interest = 0;
        printf("New balance: %.2f\n", t.balance);
    } else {
        printf("INVALID TRANSCATION TYPE.\n");
        return;
    }

    printf("Enter Date of transaction(DD MM YYYY): ");
    scanf("%d %d %d", &t.dd, &t.mm, &t.yy);
    fwrite(&t, sizeof(Transaction), 1, fp);
    fclose(fp);

    update_balance(acc_no, t.balance);
}

void new_account()
 {
    Account a;
    a.acc_no = last_accno();

    printf("Enter Name: ");
    getchar();
    fgets(a.name, sizeof(a.name), stdin);
    printf("Enter Address: ");
    fgets(a.address, sizeof(a.address), stdin);

    do {
        printf("Enter initial deposit (Min Rs.500): ");
        scanf("%f", &a.balance);
    } while (a.balance < 500);

    add_to_file(a);

    Transaction t;
    t.acc_no = a.acc_no;
    t.trans = 'D';
    t.amount = a.balance;
    t.balance = a.balance;
    t.interest = 0;

    printf("Enter Date of account created (DD MM YYYY): ");
    scanf("%d %d %d", &t.dd, &t.mm, &t.yy);

    add_to_banking(t);

    printf("ACCOUNT CREATED SUCCESSFULLY. ACCOUNT NO: %d\n", a.acc_no);
}

void main_menu()
{
    int choice;
    do {
        printf("\n---------- MAIN MENU ----------\n");
        printf("1. Open New Account\n");
        printf("2. List of Accounts\n");
        printf("3. See Individual Account\n");
        printf("4. Daily Transaction\n");
        printf("5. Show Balance\n");
        printf("6. Edit Account\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: new_account(); break;
            case 2: display_list(); break;
            case 3: display_account(); break;
            case 4: transaction(); break;
            case 5: call_balance(); break;
            case 6: edit_menu(); break;
            case 0: printf("EXITING.....\n"); break;
            default: printf("INVALID CHOICE!\n");
        }
    } while (choice != 0);
}

int main() {
    main_menu();
    return 0;
}