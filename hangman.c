#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<ctype.h> // To convert all guesses entered by the user to lowercase (avoids error)

//===========================================Project Features uptil now=============================================

// 1. Basic (random word selection from word bank)
// 2. Difficulty level according to the word length
// 3. Display of guessed letters and remaining attempts
// 4. Displays the letters alrady entered by user regardless of correct or incorrect 
// 5. Displays hangman

//===================================================================================================================//
int main(){

//word list
char words[][20]={"fortuner","excellent","doctor","jaguar","showroom",
             "mango","french","holiday","festival","beast","hangman"};

//number of words;
int totalwords=sizeof(words)/sizeof(words[0]);

//picking randm word
srand(time(NULL));
int randomIndex= rand() % totalwords;
char *selectedword = words[randomIndex];

printf("The word is :%s\n",selectedword); //Temp to check program

int length=0;
while(selectedword[length]!='\0'){ //length of the word
    length++;
}

char guessed[20];
for(int i=0;i<length;i++){
    guessed[i]='_';
}
guessed[length]='\0';

char guess;
int correct;

//Difficulty level based on Word length
int attempts=6;
if(length<=5){
    printf("Difficulty Level: Easy\n");
}else if(length<=7){
    printf("Difficulty Level: Medium\n");
}else {
    printf("Difficulty Level: Hard\n");
}

//To avoid the user to enter same alphabet again
char used[26];
int usedcount=0;

int wrong=0;

while(attempts>0){
    //printf _ _ acc to word
    printf("\nWord: ");
     for(int i=0;i<length;i++){
        printf("%c ",guessed[i]);
     }
    
    //Printf the already used letters
    printf("\nUsed letters: ");
    if(usedcount==0){
      printf("None");
    } else {
      for(int i=0;i<usedcount;i++){
      printf("%c ",used[i]);
      }
    }
printf("\n");

    //Takes input of guess
    printf("\nAttempts left: %d\n", attempts);
    printf("Enter a letter: ");
    scanf(" %c", &guess);
    guess = tolower(guess); 

    //To avoid the user to enter same alphabet again
    int alreadyused=0;
    for(int i=0;i<usedcount;i++){
        if(guess==used[i]){
            alreadyused=1;
            break;
        }
    }
    if (alreadyused) {
      printf("You already entered '%c'. Try another letter.\n", guess);
      continue;
    }else{
        used[usedcount]=guess;
        usedcount++;
    }
    

    correct = 0;

    //checks for the alphabet (guess) in the word
    for(int i=0;i<length;i++){
        if(selectedword[i]==guess && guessed[i]=='_'){
            guessed[i]=guess;
            correct = 1;
        }
    }
    
    if(!correct){
        attempts--;
        wrong++; //for display
        printf("Wrong guess!\n");
    }
//Displays hangman
printf("\n===============Hangman=================\n");
    if(wrong==0){
        printf(" +---+\n");
        printf(" |   |\n");
        printf("     |\n");
        printf("     |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (wrong==1){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("     |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (wrong == 2){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf(" |   |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (wrong == 3) {
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("/|   |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (wrong == 4){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("/|\\  |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (wrong==5){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("/|\\  |\n");
        printf("/    |\n");
        printf("     |\n");
    }else{
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("/|\\  |\n");
        printf("/ \\  |\n");
        printf("     |\n");
    }

    //checks if all words are guessed
    int win = 1;
        for (int i = 0; i < length; i++) {
            if (guessed[i] != selectedword[i]) {
                win = 0;
                break;
            }
        }

        if (win) {
            printf("\nYOU WIN! The word was: %s\n", selectedword);
            return 0;
        }

}
   printf("\nYOU LOSE! The word was: %s\n", selectedword);


    return 0;
}