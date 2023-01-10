#include <iostream>
#include <cstdlib>
#include <cctype>
using namespace std;

const int MAX_LIST_SIZE = 10;

class Money{
public:
    friend Money operator +(const Money& amount1, const Money& amount2);
    friend bool operator ==(const Money& amount1, const Money& amount2);
    Money(long dollars, int cents);
    Money(long dollars);
    Money();
    double get_value() const;
    void input(istream& ins);
    void output(ostream& outs) const;
private:
    long all_cents;
};
int digit_to_int(char c);

class Check : public Money{
public:
    Check(int checkNum, long checkAmount, bool cashedBool); 
    Check();

    void input(istream& ins); // file input
    void output(ostream& outs) const; // file output
    void update();
// getters
    int get_check_num();
    long get_check_amount();
    bool get_cashed_bool();
// 
    void compute_bank_balance();
    void check_list();

    void initialize_account(long n);
private:
    void confirm_values();

    int check_number;
    long check_amount;
    bool cashed_bool;

    long old_balance;
    long bank_balance;
    long check_amount_list[MAX_LIST_SIZE];
    bool bool_list[MAX_LIST_SIZE];
};
long account_balance;
long cashed_amount;
int exit_prompt;
int size;

int main(){
    Check check;
    check.initialize_account(0);
    while(exit_prompt != -1){
        cout<<"New Check"<<endl;

        check.input(cin);
        check.update();
        check.output(cout);
        check.compute_bank_balance();
        cout<<"Enter -1 to exit, enter any other number to continue."<<endl;
        cin>>exit_prompt;
    }
    check.check_list();
}
Check::Check(int checkNum, long checkAmount, bool cashedBool){
    check_number = checkNum;
    check_amount = checkAmount;
    cashed_bool = cashedBool;
    confirm_values();
}
Check::Check(){
}
void Check::initialize_account(long n){
    account_balance = n;
    old_balance = n;
    cashed_amount = 0;
    size = 1;
}
int Check::get_check_num(){
    return check_number;
}
long Check::get_check_amount(){
    return check_amount;
}
bool Check::get_cashed_bool(){
    return cashed_bool;
}
void Check::update(){
    old_balance = account_balance;
    account_balance = account_balance + check_amount;
}
void Check::compute_bank_balance(){
    cashed_amount = 0;
    for(int i=1; i<size; i++){
        if(bool_list[i] == 1){cashed_amount += check_amount_list[i];}
    }
    bank_balance = account_balance - cashed_amount;
    cout<<"Bank balance is "<<bank_balance<<endl;
}
void Check::check_list(){
    cout<<"Cashed checks: "<<endl;
    for(int i=1; i<size; i++){
        if(bool_list[i] == 1){
            cout<<i<<" "<<check_amount_list[i]<<" "<<endl;
        }
    }
    cout<<"Uncashed checks: "<<endl;
    for(int i=1; i<size; i++){
        if(bool_list[i] == 0){
            cout<<i<<" "<<check_amount_list[i]<<" "<<endl;;
        }
    }
}
void Check::input(istream& ins){
    cout<<"Enter check number (in order): ";
    ins>>check_number;
    cout<<"Enter check amount: ";
    ins>>check_amount;
    cout<<"Was the check cashed? Enter '1' if yes and '0' if no: ";
    ins>>cashed_bool;
    confirm_values();
    check_amount_list[size] = check_amount;
    bool_list[size] = cashed_bool;
    size += 1;
    cout<<endl;
}
void Check::output(ostream& outs) const{
    outs.setf(ios::fixed);
    outs.setf(ios::showpoint);
    outs.precision(2);
    outs<<"Check Number: "<<check_number<<endl;
    outs<<"Check Amount: "<<check_amount<<endl;
    if(cashed_bool == 1){
        outs<<"Cashed"<<endl;
    } else outs<<"Not cashed."<<endl;
    outs<<"Account balance is "<<account_balance<<endl;
}
void Check::confirm_values(){
    if(check_number < 0 || check_amount < 1){
        cout<<"Illegal values for checkbook.\n";
        exit(1);
    }
}
Money operator +(const Money& amount1, const Money& amount2){
    Money temp;
    temp.all_cents = amount1.all_cents + amount2.all_cents;
    return temp;
}
bool operator ==(const Money& amount1, const Money& amount2){
    return (amount1.all_cents == amount2.all_cents);
}
Money::Money(long dollars, int cents){
    if(dollars*cents < 0){
        cout<<"Illegal values for dollas and cents.\n";
        exit(1);
    }
    all_cents = dollars*100 + cents;
}
Money::Money(long dollars) : all_cents(dollars*100){
}
Money::Money() : all_cents(0){
}
double Money::get_value() const{
    return (all_cents * 0.01);
}
void Money::input(istream&ins){
    char one_char, decimal_point, digit1, digit2;
    long dollars;
    int cents;
    bool negative;

    ins>>one_char;
    if (one_char == '-'){
        negative = true;
        ins>>one_char;
    }
    else
        negative = false;
    ins>>dollars>>decimal_point>>digit1>>digit2;
    if(one_char !='$' || decimal_point != '.'
        || !isdigit(digit1) || !isdigit(digit2)){
        cout<<"Error illegal form for money input\n";
        exit(1);
    }
    cents = digit_to_int(digit1)*10 + digit_to_int(digit2);

    all_cents = dollars*100 + cents;
    if(negative)
        all_cents = -all_cents;
}
void Money::output(ostream& outs) const{
    long positive_cents, dollars, cents;
    positive_cents = labs(all_cents);
    dollars = positive_cents%100;
    cents = positive_cents%100;

    if(all_cents < 0)
        outs<<"-$"<<dollars<<'.';
    else
        outs<<"$"<<dollars<<'.';
    
    if(cents<10)
        outs<<'0';
    outs<<cents;
}
int digit_to_int(char c){
    return (static_cast<int>(c) - static_cast<int>('0'));
}
