
using namespace std;  

//#include <bits/stdc++.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
//#include <cppy/cppy.h>


#define NUMBERED_CARDS  19
#define SK_PS_RV_CARDS  6
#define BLACK_CARDS  8
#define COLORS  4
#define BLUE 0
#define GREEN 1
#define YELLOW 2
#define RED 3
#define BLACK 4
#define REVERSE 10
#define SKIP 11
#define DRAW 12
#define DRAWF 13
#define WILD 14
//black wild is BLACK ,14 Once a color is selected the cards color is changed
//black draw4 is black,13 Once a color is selected the cards number (num) is changed to that color



#define NUM_CARDS  (4 *(NUMBERED_CARDS + SK_PS_RV_CARDS))  + BLACK_CARDS

struct card{
    int color;
    int num;
};
struct player{
    int NumCards;
    card  Hand[NUM_CARDS -1];
};

// player p1;
// player p2;
// card Deck[NUM_CARDS];


extern struct card CARDS[NUM_CARDS];

// int shuffleCards(card * CARDS, int shufN, dkpos){
//     for (int i=0; i< shufN ;i++) 
//     { 
//         // Random for remaining positions. 
//         int r = i + (rand() % (shufN -i)); 
//         c = CARDS[i];
//         CARDS[i] = CARDS[r];
//         CARDS[r] = c;
//        // swap(card[i], card[r]); 
//     }
//     return 1;
    
    
// };
//encode deck of uno cards and then randomly shuffle
int initializeDeck(card * CARDS){
    card c;
    //card CARDS[NUM_CARDS];
    c.color = 0;
    c.num = 0;
    for(int i =0; i<COLORS; i++){
        CARDS[25*i].color = i;
        CARDS[25*i].num = 0;
        for(int j = 1; j<=12; j++){
            CARDS[(25*i) + (j*2 -1)].color = i;
            CARDS[(25*i) + (j*2 -1)].num = j;
            CARDS[(25*i) + (j*2)].color = i;
            CARDS[(25*i) + (j*2)].num = j;
        }    
    }
    
    //to initialize black cards into the deck uncomment below section and update definition of NUM_CARDS
    
    //// BEGIN BLACK CARDS CODE
    for(int i = 0; i<4; i++){//wild draw four = BLACK,13
        CARDS[100+i].color = BLACK;
        CARDS[100+i].num = DRAWF;
    }
    for(int i = 0; i<4; i++){//wild cards BLACK , 14
        CARDS[104+i].color = BLACK;
        CARDS[104+i].num = WILD;
    }
    /////END BLACK CARDS CODE
        
    ////////shuffle deck//////
        // Initialize seed randomly 
//        srand(time(NULL)); 
    
   // shuffleCards(CARDS, NUM_CARDS);
    for (int i=0; i< NUM_CARDS ;i++) 
    { 
        // Random for remaining positions. 
        int r = i + (rand() % (NUM_CARDS -i)); 
        c = CARDS[i];
        CARDS[i] = CARDS[r];
        CARDS[r] = c;
       // swap(card[i], card[r]); 
    }
    /*
    printf("-------------------------shuffled------------------------------------------- \n");
    // PRINT SHUFFLED DECK
    for( int i = 0; i<NUM_CARDS; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, CARDS[i].color, CARDS[i].num);
    }
    */
    return 1;
};

int initializePlayersHand(card * Deck, int * deckPos, player * P1, player * P2){
    P1->NumCards = 0;
    P2->NumCards = 0;
    for(int i = 0; i<(NUM_CARDS-1); i++){
        P1->Hand[i].color = 99;
        P1->Hand[i].num = 99;
        P2->Hand[i].color = 99;
        P2->Hand[i].num = 99;   
    }
    
    //confirm hand initialized
    /*
     printf("-------------------------initialized player hand------------------------------------------ \n");
    for( int i = 0; i<NUM_CARDS-1; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
    }
    */
    int k =0;
    for(; (*deckPos) <14; (*deckPos)++){
       if(k){
           k--;
           P2->Hand[P2->NumCards] = Deck[*deckPos];
           P2->NumCards++;
       } else{
           k++;
           P1->Hand[P1->NumCards] = Deck[*deckPos];
           P1->NumCards++;
       }
    } 
    return 1;
};
int draw(player * A, card * dk, int *dkPos, int n){ //may need to reshuffle played cards
    if((*dkPos + n) >= NUM_CARDS) //reshuffle new deck
        return  0;
    
    for(int i = 0; i < n; i++){
        A->Hand[i+A->NumCards] = dk[*dkPos];
        (*dkPos)++;
    }
    A->NumCards +=n;
    return 1;
}; //THIS FUNCTION HAS BEEN TESTED

//finds and returns playable cards from players hand by looking at prviously played card
player playablecards(player * A, card *crcd){
    
   // int k =0;
    player playable;//should be large enough
    playable.NumCards = 0;
    //finds  playable cards with either matching number or color
    for(int i = (A->NumCards - 1); i >= 0; i--){
        if((A->Hand[i].color == crcd->color || A->Hand[i].num == crcd->num) && (A->Hand[i].color != BLACK)){
            playable.Hand[playable.NumCards]=A->Hand[i];
            playable.NumCards++;
        } else if(A->Hand[i].color == BLACK){
        //could improve this section to more closely follow game rules but instead will play by house rules
        //black can be played whenever; possibly change later
            playable.Hand[playable.NumCards] = A->Hand[i];
            playable.NumCards++;
        }
    }
    
    return playable; 
    
};//This function has been confirmed to work properly

///////////////////////////////////////////////////CHOICE FUNCTIONS///////////////////////////////////////////

card  HighestValue(player * P, player * playable){ //if given multiple options never choose black unless black is the only option
    int MaxCardIndex =0;
    for(int i =0; i < playable->NumCards; i++){
        if(playable->Hand[MaxCardIndex].color == BLACK){
            if(playable->Hand[i].color != BLACK){ MaxCardIndex = i;
            }else if( playable->Hand[i].num < playable->Hand[MaxCardIndex].num )
                MaxCardIndex =i;
        }else if(playable->Hand[i].color != BLACK){
            if((playable->Hand[i].num) > (playable->Hand[MaxCardIndex].num)){
                MaxCardIndex = i;
            }
        }
    }
    return playable->Hand[MaxCardIndex];
  
    
    
};
//This staregy builds off the assumption that whatever card is played the opponents card is most likley to match either the number or color of your card. so this selection method sums up all cards which share either the same color or number in your hand as the card you are about to play and bases its decision off of the card with the most links hence  the name link modes
card linkModes(player * P, player * playable){ //selects card which it can play the most cards off of, 
//     int Cmodes[5];
//     int Nmodes[15];
    int dupl = 0;
    int choiceCardI=0;
    int Cweights[NUM_CARDS];
    int choiceweights[NUM_CARDS];
    for(int k =0; k <NUM_CARDS; k++){
        Cweights[k]=0;
        choiceweights[k]=0;
    }
    for(int i = 0; i< P->NumCards; i++){
        for(int j = 0; j < P->NumCards; j++){
            if(j != i){
                if((P->Hand[i].num == P->Hand[j].num) || (P->Hand[i].color == P->Hand[j].color))
                    Cweights[i]++;
            }
        }
    }
    for(int i = 0; i< playable->NumCards; i++){
        dupl = 0;
        for(int k = 0; k< P->NumCards; k++){
            if(((playable->Hand[i].color == P->Hand[k].color) && (playable->Hand[i].num == P->Hand[k].num)) && (dupl ==0 )){
                dupl++;
            }else if((playable->Hand[i].color == P->Hand[k].color) || (playable->Hand[i].num == P->Hand[k].num)){
                choiceweights[i] += Cweights[k];
            }
        }
    }
    //find maximum choice weight index
    for(int i= 0 ; i< playable->NumCards ; i++){
        if(playable->Hand[choiceCardI].color == BLACK){
            if(playable->Hand[i].color != BLACK)
                choiceCardI = i;
        }
        if(choiceweights[i] > choiceweights[choiceCardI]){
            choiceCardI = i;
        }else if((choiceweights[i]==choiceweights[choiceCardI]) && (playable->Hand[i].num > playable->Hand[choiceCardI].num)){
            choiceCardI = i;
        }
    }
    return playable->Hand[choiceCardI];
                

    
};


//play random playable card
card Random( player * P , player * playable){
    int j = rand() % playable->NumCards;
    return playable->Hand[j];
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//execute turn function given input( p1, prevturntype[ie. draw], turn[to implement skip and draw pass turns], dkPos, curCrd)

int play(player * A, card * deck, int * dkPos, card * CurCrd, bool * turn, bool * skpl , card (choice(player *, player *)) ){

    int Cmodes[5];
    int CmodesNodes[4];
    CmodesNodes[0] = 0;CmodesNodes[1]=1;CmodesNodes[2]=2; CmodesNodes[3]=3;
    Cmodes[0]=0;Cmodes[1]=0; Cmodes[2]=0;Cmodes[3]=0;
    
    if((*skpl) == 1){
        if(/*(CurCrd->color == BLACK) && */ (CurCrd->num == DRAWF)){
            if(draw(A, deck, dkPos, 4)== 0)
                return 0;
            *turn = !(*turn);
            (*skpl) =0;
            return 1;
        } else if(CurCrd->num == DRAW){
            if(draw(A, deck, dkPos, 2)== 0)
                return 0;
            *turn = !(*turn);
            (*skpl) = 0;
            return 1;
        }
        return 0;
        printf("ERROR in function PLAY or with value of SKPL");
    }else{ //take turn and play card
        
       
        player playableCrd = playablecards(A, CurCrd);
        
        for(int k = 0; k< A->NumCards ; k++){ //find color modes 
            Cmodes[A->Hand[k].color]++;            
        }
        for(int i =0; i <3; i++){
            for(int k=i+1; k<4; k++){
                if(Cmodes[i] < Cmodes[k]){
                    swap(CmodesNodes[i], CmodesNodes[k]);//might not be built in
                    swap(Cmodes[i],Cmodes[k]);
                }
            }
        }
        
        //if no card playable in hand then draw 1 if is playable then play, if card is not playable then pass turn
        if(playableCrd.NumCards == 0) {
            if(draw(A, deck, dkPos, 1) == 0) return 0;
            
            if((A->Hand[(A->NumCards - 1)].num != CurCrd->num) && (A->Hand[(A->NumCards -1)].color != CurCrd->color) && (A->Hand[(A->NumCards -1)].color != BLACK)){
            *turn = !(*turn);
//             printf("Draw card not playable \n");
            return 1;
            }
            // efficiency increases possible since only 1 card should be playable
            playableCrd.Hand[playableCrd.NumCards] = A->Hand[(A->NumCards - 1)];
            playableCrd.NumCards++;
        }
        if(playableCrd.NumCards == 1){ //A Black Card is only played if it is the only option
            *CurCrd = playableCrd.Hand[0];
            //find card in player hand and remove
            
        }else{ // options to select a card vary
            //Select a card to play from the playable cards list
            //set cur card
            *CurCrd = choice(A, &playableCrd); 
           
        }
        //Update CURRENT CARD above
        
        //////remove current card from hand and set signals of turn and skpl
       
        //find selected card in player hand
        int playcardIndex =0;
        while( (playcardIndex < (A->NumCards)) && ((A->Hand[playcardIndex].num != CurCrd->num) || (A->Hand[playcardIndex].color != CurCrd->color)))
            playcardIndex++;  
        
        //decrement NUMCARDS in players hand
        (A->NumCards)--;
        //Remove card from players hand
        A->Hand[playcardIndex] = A->Hand[(A->NumCards)];
        A->Hand[A->NumCards].color=99;
        A->Hand[A->NumCards].num=99;
       
        if(CurCrd->color == BLACK)
            CurCrd->color = CmodesNodes[0];
        
        //Update TURN, if playing SKIP or REVERSE cards maintain TURN as current value, else !TURN is applied
        if(CurCrd->num != REVERSE && CurCrd->num != SKIP )
            *turn = !(*turn);
        
        
        //Update SKPL : if playing a draw card set to 1
        if(CurCrd->num == DRAW || CurCrd->num == DRAWF)
            *skpl = 1;
//         if(*skpl ==1) printf("skip next turn \n");
        return 1;
        //
    }
    
};

int score(player * Loser){
    int scr =0;
    for(int i = 0; i < Loser->NumCards; i++){
        if(Loser->Hand[i].num <= 9) scr+= Loser->Hand[i].num;
        if(Loser->Hand[i].num == SKIP || Loser->Hand[i].num == REVERSE|| Loser->Hand[i].num == DRAW) scr +=20;
        if(Loser->Hand[i].num == WILD || Loser->Hand[i].num == DRAWF) scr+=50;
    }
    return scr;
};


int main(){
    srand(time(NULL)); 
    int p1s=0;
    int p2s =0;
    card Deck[NUM_CARDS]; //this contains all cards in the deck
    card curCrd; //contains data of the last card played
    player p1;
    player p2;
    int DkPos; //points to the current position in the deck
    bool turn =0;
    bool skipPlay;
    bool ti = 0;
    int p1w =0;
    int p2w =0;
    
    DkPos =0;
    initializeDeck(Deck);
    
    
    //confirmed correct return of shuffled deck


    
    //begin round dealing 7 cards to each player.
   
    initializePlayersHand(Deck, &DkPos, &p1, &p2);  
    //confirm hand initialized  
    //initialization CONFIRMED
    
    //printf("----------------DkPos = %d ---------------- \n", DkPos);
    
    /* //PRINT EACH PLAYERS CURRENT HAND
     printf("----------------P1 hand ---------------- \n");
   
    for(int i = 0; i < p1.NumCards +1; i++){
         printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
    }
    
    printf("----------------P2 hand ---------------- \n");
    for(int i = 0; i < p2.NumCards +1; i++){
         printf("CARDS[%d]: C: %d, N: %d \n", i, p2.Hand[i].color, p2.Hand[i].num);
    }
    */
    //    printf("-----------------THE CURRENT CARD IS: C: %d, N: %d \n", curCrd.color, curCrd.num);
    
    //flip over first card and start game
    curCrd = Deck[DkPos];
    while(curCrd.color == BLACK){
        DkPos = 0;
        initializeDeck(Deck);
        initializePlayersHand(Deck, &DkPos, &p1, &p2);  
        curCrd = Deck[DkPos];
    }

    DkPos++;
  //p1 always starts  ti = !ti;// ti switches each round giving p1 and p2 equal number of starts 
    turn = ti;//turn=0; player 1 turn ::::::turn = 1; player 2 turn
    skipPlay = 0;

    while(p1.NumCards != 0 && p2.NumCards != 0){ //while neither player has won
        if(turn){//player 2 turn  
            
            printf("----------------P2 hand -----BEFORE----------- \n");
            for(int i = 0; i < p2.NumCards +1; i++){
                printf("CARDS[%d]: C: %d, N: %d \n", i, p2.Hand[i].color, p2.Hand[i].num);
            }
            
            if(!play( &p2, Deck, &DkPos, &curCrd, &turn, &skipPlay, Random ))//EDIT Random , HighestValue, linkModes to change bot function
                break; //deck runs out
            
            
            printf("----------------P2 hand -----AFTER----------- \n");
            for(int i = 0; i < p2.NumCards +1; i++){
                printf("CARDS[%d]: C: %d, N: %d \n", i, p2.Hand[i].color, p2.Hand[i].num);
            }
            
            
        }else{//player 1 turn
             
            printf("----------------P1 hand--BEFORE--------------- \n");
             for(int i = 0; i < p1.NumCards +1; i++){
                 printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
             }    
            
            if(!play( &p1, Deck, &DkPos, &curCrd, &turn, &skipPlay, Random ))//EDIT Random , HighestValue, linkModes to change bot function
                break; //deck runs out
            
            printf("----------------P1 hand--AFTER--------------- \n");
             for(int i = 0; i < p1.NumCards +1; i++){
                 printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
             }  
        
        } 
       printf("-----------------THE CURRENT CARD IS: C: %d, N: %d \n", curCrd.color, curCrd.num);  
    }
    if(p1.NumCards == 0 ){
//         turn = 1; //loser plays first
        p1s += score(&p2);
        printf("Player 1 Wins: Score: %d \n", p1s);
        p1w++;
    }else{
//         turn = 0; //loser plays first
        p2s += score(&p1);
        printf("Player 2 Wins: Score: %d \n", p2s);
        p2w++;
    }
    
    
    
        printf("Player 1 Score: %d \n", p1s);
        printf("Player 1 win #: %d \n", p1w);
    //printf("Player 1 avg PPG: %d \n", p1s/p1w);
        printf("Player 2 Score: %d \n", p2s);
        printf("Player 2 win #: %d \n", p2w);
     //printf("Player 2 avg PPG: %d \n", p2s/p2w);
   
    
    return 0;
}