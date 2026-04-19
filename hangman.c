#include<stdio.h>
#include<stdlib.h>
#include<time.h> 
#include<ctype.h>
#include<string.h>

//===========================================Project Features=============================================
// 1. Basic (random word selection from word bank)
// 2. Difficulty level according to the word length
// 3. Display of guessed letters & remaining attempts
// 4. Displays the letters already entered by user regardless of correct or incorrect 
// 5. Displays hangman
// 6. Play again functionality without restarting the program
// 7. Hint system — type '?' to reveal a letter at the cost of 1 attempt
// 8. Score tracking — wins and losses tracked across rounds
// 9. Player statistics — win percentage shown mid-round and in final summary
// 10. Word categories — player picks a category before each round
// 11. Custom word bank — player adds their own words as a 6th category  ← NEW
//=================================================================================================================

int main(){

    char playagain;

    srand(time(NULL));

    int totalwins = 0;
    int totallosses = 0;
    float win_percentage = 0;

    // Word Categories
    char animals[][20]    = {"elephant","tiger","giraffe","dolphin","penguin","cheetah","kangaroo","cobra"};
    char sports[][20]     = {"cricket","football","basketball","swimming","badminton","volleyball","hockey","tennis"};
    char programming[][20]= {"array","pointer","function","variable","compiler","recursion","loop","syntax"};
    char countries[][20]  = {"pakistan","germany","australia","brazil","canada","japan","france","egypt"};
    char movies[][20]     = {"inception","avatar","interstellar","gladiator","titanic","joker","parasite","dune"};

    int categorysizes[6]  = {8, 8, 8, 8, 8, 0};
    char categorynames[6][20] = {"Animals","Sports","Programming","Countries","Movies","Custom"};

    // ===================== NEW: Custom Word Bank =====================
    char customwords[10][20];
    int customcount = 0;

    char addcustom;
    printf("===== Hangman Game =====\n");
    printf("Do you want to add custom words? (y/n): ");
    scanf(" %c", &addcustom);
    while(getchar() != '\n');
    addcustom = tolower(addcustom);

    if(addcustom == 'y'){
        int numwords = 0;
        printf("How many words do you want to add? (max 10): ");
        scanf("%d", &numwords);
        while(getchar() != '\n');

        if(numwords < 1) numwords = 1;
        if(numwords > 10) numwords = 10;

        for(int i = 0; i < numwords; i++){
            printf("Enter word %d: ", i + 1);
            scanf("%19s", customwords[i]);
            while(getchar() != '\n');

            // convert to lowercase
            for(int j = 0; customwords[i][j] != '\0'; j++){
                customwords[i][j] = tolower(customwords[i][j]);
            }
            customcount++;
        }
        categorysizes[5] = customcount;
        printf("%d custom word(s) added successfully!\n", customcount);
    }
    // =================================================================

    do {

        // Category Selection Menu
        int catchoice = 0;
        printf("\n===== Choose a Category =====\n");
        printf("1. Animals\n");
        printf("2. Sports\n");
        printf("3. Programming\n");
        printf("4. Countries\n");
        printf("5. Movies\n");

        // ===================== NEW: Only show Custom if words were added =====================
        if(customcount > 0){
            printf("6. Custom (%d words)\n", customcount);
            printf("=============================\n");
            printf("Enter choice (1-6): ");
        } else {
            printf("=============================\n");
            printf("Enter choice (1-5): ");
        }
        // =====================================================================================

        scanf("%d", &catchoice);
        while(getchar() != '\n');

        // validate input
        if(customcount > 0){
            if(catchoice < 1 || catchoice > 6){
                printf("Invalid choice! Defaulting to Animals.\n");
                catchoice = 1;
            }
        } else {
            if(catchoice < 1 || catchoice > 5){
                printf("Invalid choice! Defaulting to Animals.\n");
                catchoice = 1;
            }
            // block custom if no words added
            if(catchoice == 6){
                printf("No custom words added. Defaulting to Animals.\n");
                catchoice = 1;
            }
        }
        //difficulty choice
        printf("Select difficulty level (E = Easy, M = Medium, H = Hard)\n");
        char difficultychoice;
        scanf(" %c", &difficultychoice);
        while(getchar() != '\n');
        difficultychoice = toupper(difficultychoice);

        char (*catptr)[20];
        switch(catchoice){
            case 1:
                catptr = animals;
                break;
            case 2:
                catptr = sports;
                break;
            case 3:
                catptr = programming;
                break;
            case 4:
                catptr = countries;
                break;
            case 5:
                catptr = movies;
                break;
            case 6:
                catptr = customwords;
                break;
            default:
                printf("No category selected.\n");
            break;
        }
            char *filteredwords[20];
            int filteredcount = 0;

            for(int i = 0; i < categorysizes[catchoice - 1]; i++){
                int lengthofword = strlen(catptr[i]);
            if(difficultychoice == 'E' && lengthofword <= 5){
                filteredwords[filteredcount++] = catptr[i];
            }
            else if(difficultychoice == 'M' && lengthofword > 5 && lengthofword <= 7){
                filteredwords[filteredcount++] = catptr[i];
            }
            else if(difficultychoice == 'H' && lengthofword > 7){
                filteredwords[filteredcount++] = catptr[i];
            }}

            char *selectedword;
            if(filteredcount > 0){
                selectedword = filteredwords[rand() % filteredcount];
            }
            else{
                printf("No words found. Using random word\n");
                selectedword = catptr[rand() % categorysizes[catchoice - 1]];
            }

        int actuallength = strlen(selectedword);
        if(actuallength <= 5){
            printf("Difficulty: Easy\n");
        }
        else if(actuallength <= 7){
            printf("Difficulty: Medium\n");
        }
        else{
            printf("Difficulty: High\n");
        }
        
        printf("\nCategory: %s\n", categorynames[catchoice - 1]);
        printf("The word is: %s\n", selectedword); // Temp to check program

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
        int attempts = 6;

        // Show current score + win percentage at start of each round
        if(totalwins + totallosses == 0){
            printf("Score -> Wins: %d | Losses: %d | Win%%: N/A\n", totalwins, totallosses);
        } else {
            win_percentage = (float)totalwins / (float)(totalwins + totallosses) * 100;
            printf("Score -> Wins: %d | Losses: %d | Win%%: %.1f%%\n", totalwins, totallosses, win_percentage);
        }

        //To avoid the user to enter same alphabet again
        char used[26];
        int usedcount=0;

        int wrong=0;
        int hints=3;

        int roundresult = 0; // 1 = win, -1 = loss

        while(attempts>0){
            printf("\nCategory: %s | Word: ", categorynames[catchoice - 1]);
            for(int i=0;i<length;i++){
                printf("%c ",guessed[i]);
            }

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
            while(getchar() != '\n');
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

        // Update score
        if(roundresult == 1){
            totalwins++;
        } else if(roundresult == -1){
            totallosses++;
        }

        // Mid-round stats
        win_percentage = (float)totalwins / (float)(totalwins + totallosses) * 100;
        printf("\n----- Player Statistics -----\n");
        printf("Games Played: %d\n", totalwins + totallosses);
        printf("Wins:         %d\n", totalwins);
        printf("Losses:       %d\n", totallosses);
        printf("Win%%:         %.1f%%\n", win_percentage);
        printf("-----------------------------\n");

        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playagain);
        playagain = tolower(playagain);

    } while(playagain == 'y');

    // Final summary on quit
    win_percentage = (float)totalwins / (float)(totalwins + totallosses) * 100;
    printf("\n===== Final Score =====\n");
    printf("Wins:           %d\n", totalwins);
    printf("Losses:         %d\n", totallosses);
    printf("Games:          %d\n", totalwins + totallosses);
    printf("Win Percentage: %.1f%%\n", win_percentage);
    printf("=======================\n");

    printf("\nThanks for playing! Goodbye.\n");

    return 0;
}
