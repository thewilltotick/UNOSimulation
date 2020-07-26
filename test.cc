
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

extern struct card CARDS[NUM_CARDS];

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
card draw(player * A, card * dk, int *dkPos, int n){ //may need to reshuffle played cards
    for(int i = 0; i < n; i++){
        A->Hand[i+A->NumCards] = dk[*dkPos];
        (*dkPos)++;
    }
    A->NumCards +=n;
    return dk[(*dkPos -1)];//returns the card last added to the players hand
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





////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//execute turn function given input( p1, prevturntype[ie. draw], turn[to implement skip and draw pass turns], dkPos, curCrd)

void play(player * A, card * deck, int * dkPos, card * CurCrd, bool * turn, bool * skpl , card (choice(player *, player *)) ){
    
    card b;
    int Cmodes[5];
    int CmodesNodes[4];
    CmodesNodes[0] = 0;CmodesNodes[1]=1;CmodesNodes[2]=2; CmodesNodes[3]=3;
    Cmodes[0]=0;Cmodes[1]=0; Cmodes[2]=0;Cmodes[3]=0;
    
    if((*skpl) == 1){
        if(/*(CurCrd->color == BLACK) && */ (CurCrd->num == DRAWF)){
            draw(A, deck, dkPos, 4);
            *turn = !(*turn);
            (*skpl) =0;
            return;
        } else if(CurCrd->num == DRAW){
            draw(A, deck, dkPos, 2);
            *turn = !(*turn);
            (*skpl) = 0;
            return;
        }
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
            b = draw(A, deck, dkPos, 1);
            if((b.num != CurCrd->num) && (b.color != CurCrd->color) && (b.color != BLACK)){
            *turn = !(*turn);
//             printf("Draw card not playable \n");
            return;
            }
            // efficiency increases possible since only 1 card should be playable
            playableCrd.Hand[playableCrd.NumCards] = b;
            playableCrd.NumCards++;
        }
        if(playableCrd.NumCards == 1){ //A Black Card is only played if it is the only option
            *CurCrd = playableCrd.Hand[0];
            //find card in player hand and remove
            
            //taken care of below
            /*
            if(CurCrd->color == BLACK){
                CurCrd->color = CmodesNodes[0]; //A black cards color is chosen by whichever color is most common in the players hand
               
            }
        */
        }else{ // options to select a card vary
            //Select a card to play from the playable cards list
            //set cur card
//             printf("multiple play cards \n");
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
        return;
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
    bool turn;
    bool skipPlay;
  //  int coll[NUM_CARDS][2];
   

    int count = 0;
    
    printf("1");
    DkPos =0;
    //initializeDeck(Deck);
    int  coll[NUM_CARDS][2] = {{1 , 11 }, {3 , 2 }, {2 , 1 }, {1 , 9 }, {3 , 5 }, {0 , 1 }, {0 , 0 }, {1 , 4 }, {2 , 5 }, {1 , 11 }, {3 , 0 }, {4 , 13 }, {4 , 14 }, {1 , 5 }, {3 , 12 }, {3 , 12 }, {0 , 11 }, {2 , 8 }, {0 , 8 }, {0 , 3 }, {1 , 12 }, {3 , 3 }, {0 , 12 }, {1 , 6 }, {1 , 3 }, {0 , 5 }, {3 , 11 }, {1 , 5 }, {2 , 5 }, {1 , 1 }, {4 , 13 }, {4 , 14 }, {1 , 7 }, {0 , 5 }, {3 , 4 }, {2 , 11 }, {4 , 14 }, {0 , 3 }, {2 , 2 }, {0 , 11 }, {1 , 2 }, {2 , 10 }, {2 , 2 }, {3 , 9 }, {3 , 3 }, {1 , 1 }, {2 , 12 }, {2 , 3 }, {2 , 8 }, {2 , 7 }, {1 , 7 }, {3 , 6 }, {3 , 2 }, {1 , 4 }, {4 , 13 }, {2 , 9 }, {3 , 7 }, {0 , 12 }, {1 , 8 }, {2 , 6 }, {4 , 14 }, {3 , 11 }, {2 , 12 }, {0 , 7 }, {2 , 4 }, {2 , 3 }, {2 , 4 }, {0 , 9 }, {3 , 10 }, {1 , 9 }, {1 , 12 }, {0 , 10 }, {3 , 6 }, {3 , 8 }, {3 , 9 }, {2 , 9 }, {3 , 10 }, {1 , 10 }, {3 , 5 }, {4 , 13 }, {2 , 6 }, {0 , 9 }, {3 , 4 }, {1 , 10 }, {1 , 3 }, {3 , 8 }, {0 , 7 }, {1 , 6 }, {0 , 1 }, {3 , 7 }, {0 , 2 }, {2 , 11 }, {0 , 4 }, {1 , 0 }, {0 , 6 }, {2 , 1 }, {3 , 1 }, {0 , 4 }, {3 , 1 }, {0 , 6 }, {2 , 7 }, {1 , 2 }, {0 , 2 }, {2 , 0 }, {0 , 10 }, {1 , 8 }, {0 , 8 }, {2 , 10 }};
    for(int i=0; i < NUM_CARDS; i++){
        Deck[i].color= coll[i][0];
        Deck[i].num= coll[i][1];
    }
    //confirmed correct return of shuffled deck
    
    printf("-------------------------shuffled-2------------------------------------------ \n");
    for( int i = 0; i<NUM_CARDS; i++){
        printf(" {%d , %d },", Deck[i].color, Deck[i].num);
    }
    printf("loop %d\n", count++);

    
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
    
    
    //flip over first card and start game
    curCrd = Deck[DkPos];
    while(curCrd.color == BLACK){
        DkPos = 0;
        initializeDeck(Deck);
        initializePlayersHand(Deck, &DkPos, &p1, &p2);  
        curCrd = Deck[DkPos];
    }
//    printf("-----------------THE CURRENT CARD IS: C: %d, N: %d \n", curCrd.color, curCrd.num);
    DkPos++;
    turn = 0; //turn=0; player 1 turn ::::::turn = 1; player 2 turn
    skipPlay = 0;
   
    /*
    printf("----------------P1 hand ----before play------------ \n");
    for(int i = 0; i < p1.NumCards +1; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
    }
    play( &p1, Deck, &DkPos, &curCrd, &turn, &skipPlay, HighestValue );
            //execute turn function given input( p1, prevturntype[ie. draw], turn[to implement skip and draw pass turns], dkPos, curCrd)
    printf("----------------P1 hand ------after play---------- \n");
    for(int i = 0; i < p1.NumCards +1; i++){
        printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
    }
    printf("-----------------THE played CARD IS: C: %d, N: %d \n", curCrd.color, curCrd.num);
    */
    
    while(p1.NumCards != 0 && p2.NumCards != 0){ //while neither player has won
        if(turn){//player 2 turn 
//             printf("----------------P2 hand -----BEFORE----------- \n");
//             for(int i = 0; i < p2.NumCards +1; i++){
//                  printf("CARDS[%d]: C: %d, N: %d \n", i, p2.Hand[i].color, p2.Hand[i].num);
//             }
            
            play( &p2, Deck, &DkPos, &curCrd, &turn, &skipPlay, HighestValue );
            
//             printf("----------------P2 hand -----AFTER----------- \n");
//             for(int i = 0; i < p2.NumCards +1; i++){
//                  printf("CARDS[%d]: C: %d, N: %d \n", i, p2.Hand[i].color, p2.Hand[i].num);
//             }
        }else{//player 1 turn
//             printf("----------------P1 hand --BEFORE-------------- \n");
//             for(int i = 0; i < p1.NumCards +1; i++){
//                 printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
//             }
            
            play( &p1, Deck, &DkPos, &curCrd, &turn, &skipPlay, HighestValue );
            
//             printf("----------------P1 hand --------AFTER-------- \n");
//             for(int i = 0; i < p1.NumCards +1; i++){
//                 printf("CARDS[%d]: C: %d, N: %d \n", i, p1.Hand[i].color, p1.Hand[i].num);
//             }
        } 
//         printf("-----------------THE played CARD IS: C: %d, N: %d \n", curCrd.color, curCrd.num);
    }
    if(p1.NumCards == 0 ){
        
        p1s += score(&p2);
        printf("Player 1 Wins: Score: %d \n hello \n", p1s);
        printf("HI");
        
    }else{
        p2s += score(&p1);
        printf("Player 2 Wins: Score: %d \n hello \n", p2s);
        printf("HI");
    }
    printf("9");
    
    
    
    return 0;
}