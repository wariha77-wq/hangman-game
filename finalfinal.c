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
// 11. Custom word bank — player adds their own words as a 6th category
//=================================================================================================================

// ===================== STRUCTURE: Player =====================
// Holds all player-related data in one place
typedef struct {
    char name[35];
    int  totalwins;
    int  totallosses;
    float win_percentage;
} Player;

// struct

typedef struct {
    char *selectedword;
    char *guessed;
    char *used;
    int   length;
    int   attempts;
    int   wrong;
    int   hints;
    int   usedcount;
    int   roundresult; 
} GameRound;

// ===================== FUNCTION: displayHangman =====================
// Takes a pointer to GameRound and prints the hangman figure
void displayHangman(GameRound *round) {
    printf("\n===============Hangman=================\n");
    if(round->wrong==0){
        printf(" +---+\n");
        printf(" |   |\n");
        printf("     |\n");
        printf("     |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (round->wrong==1){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("     |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (round->wrong == 2){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf(" |   |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (round->wrong == 3) {
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("/|   |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (round->wrong == 4){
        printf(" +---+\n");
        printf(" |   |\n");
        printf(" O   |\n");
        printf("/|\\  |\n");
        printf("     |\n");
        printf("     |\n");
    }else if (round->wrong==5){
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
}

void saveScore(Player *p) {
    FILE *savefp = fopen("hangmanscores.txt", "w");
    if(savefp != NULL){
        fprintf(savefp, "Player: %s\n", p->name);
        fprintf(savefp, "Wins: %d\n", p->totalwins);
        fprintf(savefp, "Losses: %d\n", p->totallosses);
        fclose(savefp);
    }
}

void printStats(Player *p) {
    printf("\n----- Player Statistics -----\n");
    printf("Games Played: %d\n", p->totalwins + p->totallosses);
    printf("Wins:         %d\n", p->totalwins);
    printf("Losses:       %d\n", p->totallosses);
    printf("Win%%:         %.1f%%\n", p->win_percentage);
    printf("-----------------------------\n");
}

int main(){

    char playagain;

    srand(time(NULL));

    Player player;
    player.totalwins    = 0;
    player.totallosses  = 0;
    player.win_percentage = 0;

    // Word Categories
    char animals[][20]    = {"elephant","tiger","giraffe","dolphin","penguin","cheetah","kangaroo","cobra"};
    char sports[][20]     = {"cricket","football","basketball","swimming","badminton","volleyball","hockey","tennis"};
    char programming[][20]= {"array","pointer","function","variable","compiler","recursion","loop","syntax"};
    char countries[][20]  = {"pakistan","germany","australia","brazil","canada","japan","france","egypt"};
    char movies[][20]     = {"inception","avatar","interstellar","gladiator","titanic","joker","parasite","dune"};

    int categorysizes[6]  = {8, 8, 8, 8, 8, 0};
    char categorynames[6][20] = {"Animals","Sports","Programming","Countries","Movies","Custom"};

    // ======== NEW: Custom Word Bank ================
    char customwords[10][20];
    int customcount = 0;

    char addcustom;
    printf("=====Hangman Game =====\n");

    Player *playerptr = &player;

    printf("Enter your Name:\n");
    scanf("%34s", playerptr->name);   // Access struct member via pointer
    while(getchar() != '\n');

    FILE *fp = fopen("hangmanscores.txt", "r");
    if(fp != NULL){
        char savedname[35];
        fscanf(fp, "Player: %s\n", savedname);
        fscanf(fp, "Wins: %d\n", &playerptr->totalwins);
        fscanf(fp, "Losses: %d\n", &playerptr->totallosses);
        fclose(fp);
        if(strcmp(savedname, playerptr->name) == 0){
            printf("Welcome back again %s. Wins: %d | Losses: %d\n", playerptr->name, playerptr->totalwins, playerptr->totallosses);
        }
        else{
            printf("Welcome %s.\n", playerptr->name);
        }
    }
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

    do {

        GameRound round;
        round.attempts    = 6;
        round.wrong       = 0;
        round.hints       = 3;
        round.usedcount   = 0;
        round.roundresult = 0;
        round.guessed     = NULL;
        round.used        = NULL;

        // Category Selection Menu
        int catchoice = 0;
        printf("\n===== Choose a Category =====\n");
        printf("1. Animals\n");
        printf("2. Sports\n");
        printf("3. Programming\n");
        printf("4. Countries\n");
        printf("5. Movies\n");

        // ===================== Only show Custom if words were added=============
        if(customcount > 0){
            printf("6. Custom (%d words)\n", customcount);
            printf("============\n");
            printf("Enter choice (1-6): ");
        } else {
            printf("==============\n");
            printf("Enter choice (1-5): ");
        }

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
            }
        }

        if(filteredcount > 0){
            round.selectedword = filteredwords[rand() % filteredcount];
        }
        else{
            printf("No words found. Using random word\n");
            round.selectedword = catptr[rand() % categorysizes[catchoice - 1]];
        }

        round.length = strlen(round.selectedword);

        if(round.length <= 5){
            printf("Difficulty: Easy\n");
        }
        else if(round.length <= 7){
            printf("Difficulty: Medium\n");
        }
        else{
            printf("Difficulty: High\n");
        }

        printf("\nCategory: %s\n", categorynames[catchoice - 1]);
        printf("The word is: %s\n", round.selectedword); // Temp to check program

        round.guessed = (char *)malloc((round.length + 1) * sizeof(char));
        if(round.guessed == NULL){
            printf("Memory allocation failed!\n");
            return 1;
        }
        for(int i = 0; i < round.length; i++){
            round.guessed[i] = '_';
        }
        round.guessed[round.length] = '\0';

        round.used = (char *)malloc(26 * sizeof(char));
        if(round.used == NULL){
            printf("Memory allocation failed!\n");
            free(round.guessed);
            return 1;
        }

        char guess;
        int correct;

        // Show current score + win percentage at start of each round
        if(playerptr->totalwins + playerptr->totallosses == 0){
            printf("Score -> Wins: %d | Losses: %d | Win%%: N/A\n", playerptr->totalwins, playerptr->totallosses);
        } else {
            playerptr->win_percentage = (float)playerptr->totalwins / (float)(playerptr->totalwins + playerptr->totallosses) * 100;
            printf("Score -> Wins: %d | Losses: %d | Win%%: %.1f%%\n", playerptr->totalwins, playerptr->totallosses, playerptr->win_percentage);
        }

        GameRound *roundptr = &round;


        while(roundptr->attempts > 0){
            printf("\nCategory: %s | Word: ", categorynames[catchoice - 1]);
            for(int i = 0; i < roundptr->length; i++){
                printf("%c ", roundptr->guessed[i]);
            }

            printf("\nUsed letters: ");
            if(roundptr->usedcount == 0){
                printf("None");
            } else {
                for(int i = 0; i < roundptr->usedcount; i++){
                    printf("%c ", roundptr->used[i]);
                }
            }
            printf("\n");

            printf("\nAttempts left: %d | Hints left: %d (type '?' for a hint)\n", roundptr->attempts, roundptr->hints);
            printf("Enter a letter: ");
            scanf(" %c", &guess);
            while(getchar() != '\n');
            guess = tolower(guess);


            if(guess == '?'){
                if(roundptr->hints <= 0){
                    printf("No hints left!\n");
                    continue;
                }
                if(roundptr->attempts <= 1){
                    printf("Not enough attempts to use a hint!\n");
                    continue;
                }
                
                int *unguessed = (int *)malloc(roundptr->length * sizeof(int));
                if(unguessed == NULL){
                    printf("Memory allocation failed!\n");
                    continue;
                }
                int count = 0;
                for(int i = 0; i < roundptr->length; i++){
                    if(roundptr->guessed[i] == '_'){
                        unguessed[count] = i;
                        count++;
                    }
                }

                if(count == 0){
                    printf("No letters left to reveal!\n");
                    free(unguessed);
                    continue;
                }

                int pick = unguessed[rand() % count];
                char revealed = roundptr->selectedword[pick];
                roundptr->guessed[pick] = revealed;

                roundptr->used[roundptr->usedcount] = revealed;
                roundptr->usedcount++;

                roundptr->attempts--;
                roundptr->wrong++;
                roundptr->hints--;
                printf("Hint used! The letter '%c' has been revealed. (-1 attempt)\n", revealed);

                free(unguessed);
            }
            else {

                int alreadyused = 0;
                for(int i = 0; i < roundptr->usedcount; i++){
                    if(guess == roundptr->used[i]){
                        alreadyused = 1;
                        break;
                    }
                }
                if (alreadyused) {
                    printf("You already entered '%c'. Try another letter.\n", guess);
                    continue;
                } else {
                    roundptr->used[roundptr->usedcount] = guess;
                    roundptr->usedcount++;
                }

                correct = 0;

                for(int i = 0; i < roundptr->length; i++){
                    if(roundptr->selectedword[i] == guess && roundptr->guessed[i] == '_'){
                        roundptr->guessed[i] = guess;
                        correct = 1;
                    }
                }

                if(!correct){
                    roundptr->attempts--;
                    roundptr->wrong++;
                    printf("Wrong guess!\n");
                }
            }

            displayHangman(roundptr);

            int win = 1;
            for (int i = 0; i < roundptr->length; i++) {
                if (roundptr->guessed[i] != roundptr->selectedword[i]) {
                    win = 0;
                    break;
                }
            }

            if (win) {
                printf("\nYOU WIN! The word was: %s\n", roundptr->selectedword);
                roundptr->roundresult = 1;
                roundptr->attempts = 0;
                break;
            }
        }

        if(roundptr->wrong == 6){
            printf("\nYOU LOSE! The word was: %s\n", roundptr->selectedword);
            roundptr->roundresult = -1;
        }

        if(roundptr->roundresult == 1){
            playerptr->totalwins++;
        } else if(roundptr->roundresult == -1){
            playerptr->totallosses++;
        }

        saveScore(playerptr);

        playerptr->win_percentage = (float)playerptr->totalwins / (float)(playerptr->totalwins + playerptr->totallosses) * 100;
        printStats(playerptr);

        free(roundptr->guessed);
        roundptr->guessed = NULL;
        free(roundptr->used);
        roundptr->used = NULL;
        // =========
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playagain);
        playagain = tolower(playagain);

    } while(playagain == 'y');

    playerptr->win_percentage = (float)playerptr->totalwins / (float)(playerptr->totalwins + playerptr->totallosses) * 100;
    printf("\n===== Final Score ===\n");
    printf("Wins:           %d\n", playerptr->totalwins);
    printf("Losses:         %d\n", playerptr->totallosses);
    printf("Games:          %d\n", playerptr->totalwins + playerptr->totallosses);
    printf("Win Percentage: %.1f%%\n", playerptr->win_percentage);
    printf("==============\n");

    printf("\nThanks for playing! Goodbye.\n");

    return 0;
}
