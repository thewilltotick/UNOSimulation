
using namespace std;  

//#include <bits/stdc++.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>

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

extern struct card CARDS[NUM_CARDS];

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
        CARDS[100+i].num = 13;
    }
    for(int i = 0; i<4; i++){//wild cards BLACK , 14
        CARDS[104+i].color = BLACK;
        CARDS[104+i].num = 14;
    }
    /////END BLACK CARDS CODE
    
    
    //print deck to check correctness
    for( int i = 0; i<NUM_CARDS; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, CARDS[i].color, CARDS[i].num);
    }
        
    //shuffle deck
    // Initialize seed randomly 
    srand(time(0)); 
  
    for (int i=0; i< NUM_CARDS ;i++) 
    { 
        // Random for remaining positions. 
        int r = i + (rand() % (NUM_CARDS -i)); 
        c = CARDS[i];
        CARDS[i] = CARDS[r];
        CARDS[r] = c;
       // swap(card[i], card[r]); 
    }
    printf("-------------------------shuffled------------------------------------------- \n");
    // PRINT SHUFFLED DECK
    for( int i = 0; i<NUM_CARDS; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, CARDS[i].color, CARDS[i].num);
    }
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
card draw(player * A, card * dk, int *dkPos, int n){
    for(int i = 0; i < n; i++){
        A->Hand[i+A->NumCards] = dk[*dkPos];
        (*dkPos)++;
    }
    A->NumCards +=n;
    return dk[(*dkPos -1)];//returns the card last added to the players hand
};
//finds and returns playable cards from players hand by looking at prviously played card
player playablecards(player * A, card *crcd){
    
   // int k =0;
    player playable;//should be large enough
    playable.NumCards = 0;
    //finds  playable cards with either matching number or color
    for(int i = (A->NumCards - 1); i >= 0; i--){
        if(A->Hand[i].color == crcd->color || A->Hand[i].num == crcd->num){
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
    
};




void play(player * A, card * deck, int * dkPos, card * CurCrd, bool * turn, bool * skpl ){
    card b;
    if((*skpl) == 1){
        if((CurCrd->color == BLACK) && (CurCrd->num == 13)){
            draw(A, deck, dkPos, 4);
            *turn = !(*turn);
            (*skpl) =0;
            return;
        } else if(CurCrd->num == DRAW){
            draw(A, deck, dkPos, 2);
            *turn =!(*turn);
            (*skpl) = 0;
            return;
        }
        printf("ERROR in function PLAY or with value of SKPL");
    }else{
        
       // int k =0;
        player playableCrd = playablecards(A, CurCrd);//should be large enough
       
        
        /* playable.NumCards = 0;
        //finds  playable cards with either matching number or color
        for(int i = (A->NumCards - 1); i >= 0; i--){
            if(A->Hand[i].color == CurCrd->color || A->Hand[i].num == CurCrd->num){
                playable.Hand[k]=A->Hand[i];
                k++;
            } else if(A->Hand[i].color == BLACK){
                //could improve this section to more closely follow game rules but instead will play by house rules 
                //black can be played whenever; possibly change later
                playable.Hand[k] = A->Hand[i];
                k++;
            }
        }
        */
    //if no card playable in hand then draw 1 if is playable then play, if card is not playable then draw
        if(playableCrd.NumCards == 0){
            do{b = draw(A, deck, dkPos, 1);}
            while( (b.num != CurCrd->num) && (b.color != CurCrd->color) && (b.color != BLACK));
            // efficiency increases possible since only 1 card should be playable
            playableCrd.Hand[playableCrd.NumCards] = b;
            playableCrd.NumCards++;
        }
         //Select a card to play from the playable cards list
        //Remove card from players hand
        //decrement NUMCARDS in players hand
        //Update CURRENT CARD
        //Update TURN, if playing SKIP or REVERSE cards maintain TURN as current value, else !TURN is applied
        //Update SKPL : if playing a draw card set to 1
        //
    }
};




int main(){
    card Deck[NUM_CARDS];
    initializeDeck(Deck);
    card curCrd;
    //confirmed correct return of shuffled deck
    /*
    printf("-------------------------shuffled-2------------------------------------------ \n");
    for( int i = 0; i<NUM_CARDS; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, DECK[i].color, DECK[i].num);
    }
    */
    
    //begin round dealing 7 cards to each player.
    player p1;//maximum number of cards possible in a players hand
    player p2;
    int DkPos=0;
    
    
    initializePlayersHand(Deck, &DkPos, &p1, &p2);    
    //confirm hand initialized  
    //initialization CONFIRMED
    /*
    printf("----------------DkPos = %d ---------------- \n", DkPos);
    printf("----------------P1 hand ---------------- \n");
    
    for(int i = 0; i < p1.NumCards +1; i++){
         printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
    }
    printf("----------------P2 hand ---------------- \n");
    for(int i = 0; i < p2.NumCards +1; i++){
         printf("CARDS[%d]: C: %d, N: %d \n", i, p2.Hand[i].color, p2.Hand[i].num);
    }
    */
    
    //flip over first card and start game
    curCrd = Deck[DkPos];
    bool turn = 0;
    while(p1.NumCards != 0 && p2.NumCards != 0){
        if(turn){//player 2 turn 
            turn = 0;
            
        }else{//player 1 turn
            turn = 1;
            //execute turn function given input( p1, prevturntype[ie. draw], turn[to implement skip and draw pass turns], dkPos, curCrd)
        }
    }
    
    
    return 0;
}