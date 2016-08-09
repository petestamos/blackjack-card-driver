////////////////////////////////////////////////////////////////////////
////////////////// CS449 PROJECT 4 - PETER STAMOS //////////////////////
////////////////////////////////////////////////////////////////////////

#include <stdio.h>

FILE *deck;

////////////////////////////////////////////////////////////////////////

int draw(int *x) {

  char card[1];
  int value;

  fread(card, 1, 1, deck);

  card[0] = (card[0] % 13);

  switch(card[0]) {

    case 0:

      *x = *x + 1;
      value = 11;
      break;
 
    case 10:

      value = 10;
      break;

    case 11:

      value = 10;
      break;

    case 12:

      value = 10;
      break;

    default:

      value = card[0] + 1;
      break;
  }

  return value;
}

////////////////////////////////////////////////////////////////////////

int ace(int *score, int *ace) {

  int total = *score + 11;

  if (total > 21) {
    *ace = *ace - 1;
    return 1;
  }

  else {
    return 11;
  }
}

////////////////////////////////////////////////////////////////////////

int check_score(int *score, int *ace) {

  if (*score > 21) {

    if (*ace > 0) {
      *(score) -= 10;
      *ace = *ace - 1;
    }

    else {
      return -1;
    }
  }

  return 1;
}

////////////////////////////////////////////////////////////////////////

int computer_score(int *score, int *ace) {

  if (*score > 21) {

    if (*ace > 0) {
      *(score) -= 10;
      *ace = *ace - 1;
    }

    else {
      return -1;
    }
  }

  else if (*score >= 17) {
    return 0;
  }

  return 1;
}

////////////////////////////////////////////////////////////////////////

void play() {

  deck = fopen("/dev/cards", "r");

  char input[10];

  int flag = 1;

  int player_total = 0;
  int player_aces = 0;
  int current_card;

  int computer_total = 0;
  int computer_aces = 0;
  int computer_card;

  computer_total = draw(&computer_aces);
  computer_card = draw(&computer_aces);

  player_total = draw(&player_aces);

  while (flag == 1) {

    printf("The dealer: \n ? + %d \n\n", computer_card);

    current_card = draw(&player_aces);

    if (current_card == 11) {
      current_card = ace(&player_total, &player_aces);
    }

    check_score(&player_total, &player_aces);

    printf("You: \n%d + %d = %d \n", player_total, current_card, player_total + current_card, player_aces);

    player_total += current_card;

    flag = check_score(&player_total, &player_aces);

    if (flag == -1) {
      printf("Busted! \n");
    }

    if (flag == 1) {

      printf("Would you like to \"hit\" or \"stand\"?");
      scanf("%s", input);

      if (strcmp(input, "hit")) {
        flag = 0;
      }
    }
  }

  if (flag == -1) {
    printf("You busted. Dealer wins.\n");
  }

  else {

    printf("The dealer: \n%d + %d = %d \n \n", computer_total, computer_card, computer_total + computer_card);
    computer_total += computer_card;

    do {

      computer_card = draw(&computer_aces);

      if (computer_card == 11) {
        computer_card = ace(&computer_total, &computer_aces);
      }

      printf("The dealer: \n%d + %d = %d \n \n", computer_total, computer_card, computer_total + computer_card);

      computer_total += computer_card;

      flag = computer_score(&computer_total, &computer_aces);
    }

    while (flag == 1);

    if (flag == 0) {

      if (player_total > computer_total) {
        printf("You win!");
      }

      else {
        printf("Dealer wins!");
      }
    }

    else {
      check_score(&player_total, &player_aces);
      printf("Dealer busted. You win!");
    }
  }
}

int main() {

  play();
  return 0;
}

////////////////////////////////////////////////////////////////////////
////////////////// CS449 PROJECT 4 - PETER STAMOS //////////////////////
////////////////////////////////////////////////////////////////////////
