#include <cstdio>
#include <cstdlib>
#include <ctime>

const unsigned int SOLVE_NUM = 1000;
const unsigned int MAX_DEPTH_SEARCH = 1000;
const int CUTOFF = -1;

int InputProcess( int initialPancake[], const char path[] );
void SwapPancake( int pancake[], int position );
int IterativeDeepeningSearch( int swapOrder[], int pancake[], int pancakeLength );
int DepthLimitedSearch( int swapOrder[], int pancake[], int pancakeLength, int depth, int lastSwap );
bool IsFinalState( const int pancake[], int pancakeLength );
void OutputPancake( FILE *fp, int pancake[], int pancakeLength );

int main(){
  clock_t startTime = clock();
  FILE *fp = fopen( "output.txt", "w" );
  int pancake[SOLVE_NUM];
  int swapOrder[MAX_DEPTH_SEARCH];

  int pancakeLength = InputProcess(pancake, "input.txt");
  int move = IterativeDeepeningSearch( swapOrder, pancake, pancakeLength );
  if( move != CUTOFF ){
    fprintf( fp, "An optimal solution has %d moves.\n", move );
    OutputPancake( fp, pancake, pancakeLength );
    for( int i = move-1 ; i >= 0 ; --i ){
      SwapPancake(pancake, swapOrder[i]);
      OutputPancake( fp, pancake, pancakeLength );
    }
    fprintf( fp, "Total run time = %.2f seconds.\n", (float)(clock()-startTime) / CLOCKS_PER_SEC );
  }
  else{
    fprintf( fp, "Fail...\n" );
  }
  fclose(fp);
  return 0;
}

int InputProcess( int initialPancake[], const char path[] ){
  FILE *fp = fopen( path, "r" );
  int index = 0;

  while( fscanf( fp, "%d", &initialPancake[index] ) != EOF ){
    ++index;
  }

  fclose( fp );
  return index;
}

void SwapPancake( int pancake[], int position ){
  for( int i = 0 ; i <= position/2 ; ++i ){
    int temp = pancake[i];
    pancake[i] = pancake[position-i];
    pancake[position-i] = temp;
  }
}


int IterativeDeepeningSearch( int swapOrder[], int pancake[], int pancakeLength ){
  for( int i = 0 ; i < MAX_DEPTH_SEARCH ; ++i ){
    swapOrder[i] = 0;
  }

  for( int i = 0 ; i < MAX_DEPTH_SEARCH ; ++i ){
    int result = DepthLimitedSearch(swapOrder, pancake, pancakeLength, i, 0);
    if( result != CUTOFF ) return result;
  }
  return CUTOFF;
}

int DepthLimitedSearch( int swapOrder[], int pancake[], int pancakeLength, int depth, int lastSwap ){
  if( IsFinalState(pancake, pancakeLength) ) return depth;
  if( depth == 0 ) return CUTOFF;

  for( int i = 1 ; i < pancakeLength ; i++ ){
    if( i == lastSwap ) continue;
    SwapPancake( pancake, i );
    swapOrder[depth-1] = i;

    int result = DepthLimitedSearch( swapOrder, pancake, pancakeLength, depth-1, i);
    SwapPancake( pancake, i );
    if( result != CUTOFF ) return depth;
    swapOrder[depth-1] = 0;
  }
  return CUTOFF;
}

bool IsFinalState( const int pancake[], int pancakeLength ){
  for( int i = 0 ; i < pancakeLength ; i++ )
    if( pancake[i] != i+1 ) return false;
  return true;
}

void OutputPancake( FILE* fp, int pancake[], int pancakeLength ){
  for( int i = 0 ; i < pancakeLength ; i++ ){
    if( i != 0 ) fprintf( fp, " " );
    fprintf( fp, "%d", pancake[i] );
  }
  fprintf( fp, "\n" );
}
