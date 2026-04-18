#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<ctype.h>

//===========================================Project Features=============================================
// 1. Basic (random word selection from word bank)
// 2. Difficulty level according to the word length
// 3. Display of guessed letters & remaining attempts
// 4. Displays the letters already entered by user regardless of correct or incorrect 
// 5. Displays hangman
// 6. Play again functionality without restarting the program
// 7. Hint system — type '?' to reveal a letter at the cost of 1 attempt
// 8. Score tracking — wins and losses tracked across rounds  ← NEW
//=================================================================================================================

int main(){

    char playagain;

    srand(time(NULL));

    // ===================== NEW: Score Tracking =====================
    int totalwins = 0;
    int totallosses = 0;
    float win_percentage = 0;
    // ===============================================================

    do {

        //word list
        char words[][20]={"fortuner","excellent","doctor","jaguar","showroom",
                     "mango","french","holiday","festival","beast","hangman"};

        //number of words
        int totalwords=sizeof(words)/sizeof(words[0]);

        //picking random word
        int randomIndex= rand() % totalwords;
        char *selectedword = words[randomIndex];

        printf("The word is :%s\n",selectedword); //Temp to check program

        int length=0;
        while(selectedword[length]!='\0'){
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

        // ===================== NEW: Show current score at start of each round =====================
        printf("Score -> Wins: %d | Losses: %d\n", totalwins, totallosses);
        // ==========================================================================================

        //To avoid the user to enter same alphabet again
        char used[26];
        int usedcount=0;

        int wrong=0;
        int hints=3;

        int roundresult = 0; // 1 = win, -1 = loss

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

            printf("\nAttempts left: %d | Hints left: %d (type '?' for a hint)\n", attempts, hints);
            printf("Enter a letter: ");
            scanf(" %c", &guess);
            guess = tolower(guess);

            // Hint System
            if(guess == '?'){
                if(hints <= 0){
                    printf("No hints left!\n");
                    continue;
                }
                if(attempts <= 1){
                    printf("Not enough attempts to use a hint!\n");
                    continue;
                }

                int unguessed[20];
                int count = 0;
                for(int i = 0; i < length; i++){
                    if(guessed[i] == '_'){
                        unguessed[count] = i;
                        count++;
                    }
                }

                if(count == 0){
                    printf("No letters left to reveal!\n");
                    continue;
                }

                int pick = unguessed[rand() % count];
                char revealed = selectedword[pick];
                guessed[pick] = revealed;

                used[usedcount] = revealed;
                usedcount++;

                attempts--;
                wrong++;
                hints--;
                printf("Hint used! The letter '%c' has been revealed. (-1 attempt)\n", revealed);
            }
            else {

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

                for(int i=0;i<length;i++){
                    if(selectedword[i]==guess && guessed[i]=='_'){
                        guessed[i]=guess;
                        correct = 1;
                    }
                }

                if(!correct){
                    attempts--;
                    wrong++;
                    printf("Wrong guess!\n");
                }

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

            //checks if all letters are guessed
            int win = 1;
            for (int i = 0; i < length; i++) {
                if (guessed[i] != selectedword[i]) {
                    win = 0;
                    break;
                }
            }

            if (win) {
                printf("\nYOU WIN! The word was: %s\n", selectedword);
                roundresult = 1;
                attempts = 0;
                break;
            }

        }

        if(wrong==6){
            printf("\nYOU LOSE! The word was: %s\n", selectedword);
            roundresult = -1;
        }

        // ===================== NEW: Update and display score =====================
        if(roundresult == 1){
            totalwins++;
        } else if(roundresult == -1){
            totallosses++;
        }
        printf("\n----- Score -----\n");
        printf("Wins: %d | Losses: %d\n", totalwins, totallosses);
        printf("-----------------\n");
        // =========================================================================

        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playagain);
        playagain = tolower(playagain);

    } while(playagain == 'y');
    win_percentage = (float)totalwins/(float)(totalwins + totallosses) * 100;
    // ===================== NEW: Final summary on quit =====================
    printf("\n===== Final Score =====\n");
    printf("Wins:   %d\n", totalwins);
    printf("Losses: %d\n", totallosses);
    printf("Games:  %d\n", totalwins + totallosses);
    printf("Win Percentage: %.1f", win_percentage);
    printf("=======================\n");
    // =====================================================================

    printf("\nThanks for playing! Goodbye.\n");

    return 0;
}
