// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//

#include <unistd.h>
#include "copyright.h"
#include "system.h"
#include "dinningph.h"
#include "synch.h" //semaforos

//DinningPh * dp;
Semaphore * sH;
Semaphore * sO;

/*void Philo( void * p ) {

    int eats, thinks;
    long who = (long) p;

    currentThread->Yield();

    for ( int i = 0; i < 10; i++ ) {

        printf(" Philosopher %ld will try to pickup sticks\n", who + 1);

        dp->pickup( who );
        dp->print();
        eats = Random() % 6;

        currentThread->Yield();
        sleep( eats );

        dp->putdown( who );

        thinks = Random() % 6;
        currentThread->Yield();
        sleep( thinks );
    }

}*/
int cO = 0;
int cH = 0;
void H(void*){
	if(cH > 0 && cO > 0){
		printf("Iniciando H. Haciendo agua.... cO %d y cH %d'\n'", cO,cH);
		cH--;
		cO--;
		sH->V();
		sO->V();
	} else{
		cH++;
		sH->P();
	}
}


void O(void*){
	if(cH > 1){
		printf("Iniciando O. Haciendo agua.... cO %d y cH %d'\n'", cO,cH); //%d para numeros y %s para strings	
		cH -= 2;
		sH->V();
		sH->V();	
	} else{
		cO++;
		sO->P();
	}
}


//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------

void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;
    
    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("*** thread %s looped %d times\n", threadName, num);
	//interrupt->SetLevel(oldLevel);
        //currentThread->Yield();
    }
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
    //interrupt->SetLevel(oldLevel);
}



//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling 
//	SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    sH = new Semaphore("sH",0);
    sO = new Semaphore("sO",0);
    //Thread * Ph;
    DEBUG('t', "Entering SimpleTest");
	
	for(int i =0; i< 100; i++){
		char* threadname = new char[100];
		sprintf(threadname, "Hilo %d", i);
      	Thread* newThread = new Thread (threadname);
		int part = Random() % 3;
		if(part % 2 == 0){
			newThread->Fork (H, (void*)threadname);
		} else{
			newThread->Fork (O, (void*)threadname);
		}
	}
	

/*
    dp = new DinningPh();

    for ( long k = 0; k < 5; k++ ) {
        Ph = new Thread( "dp" );
        Ph->Fork( Philo, (void *) k );
    }

    return;
*/
    /*for ( int k=1; k<=5; k++) {
      char* threadname = new char[100];
      sprintf(threadname, "Hilo %d", k);
      Thread* newThread = new Thread (threadname);
      newThread->Fork (SimpleThread, (void*)threadname);
    }
    
    SimpleThread( (void*)"Hilo 0");*/
}

