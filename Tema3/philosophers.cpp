#include <windows.h> 
#include <iostream> 
#include <stdio.h> 
#include <stdlib.h> 
#include "conio.h"
#include "list"
#include <tchar.h>

#define MIN_TIME 30
#define MAX_TIME 1000
#define Number_OF_Philosophers 5
#define STARVING 5
 
using namespace std;

list<int> Threads_sequence;

// подія для збору в нитки чисел філософів
HANDLE iteratorControl = CreateEvent(NULL, TRUE, TRUE, NULL);// ручне скидання, ініціюється сигнал
int philosopher_number = 0;

//cs for unbreakable part of philosopher activity
CRITICAL_SECTION philosopherControl;

// події для виделок та ниток для філософів
HANDLE EVENt[Number_OF_Philosophers];
HANDLE Philosophers[Number_OF_Philosophers];
bool  STATUS[Number_OF_Philosophers];
 
DWORD WINAPI Thread(LPVOID arg)
{

	 int left_fork = philosopher_number, right_fork = philosopher_number + 1;
     if (left_fork == Number_OF_Philosophers - 1) right_fork = 0;
     int count = 0;
	SetEvent(iteratorControl); // ця подія повертає керування іншими потоками після отримання власного номера 
    printf("The Philosopher[%i] sat down at the table\n", left_fork);
    Sleep(1000); // деяка затримка
    while (1)
	{
        printf("The Philosopher[%i] thinks.\n", left_fork);
        Sleep(MIN_TIME + rand() % (MAX_TIME - MIN_TIME)); // деяка затримка

          Threads_sequence.push_back(left_fork);
        // цикл для правильної послідовності введення критичного розділу потоків
          while (true)
        {
            if (Threads_sequence.front() == left_fork) { Threads_sequence.pop_back(); break; }
            else continue;
        } 

         EnterCriticalSection(&philosopherControl);   // непорушна діяльність (початок) ///Критичні секції! 
         if (WaitForSingleObject(EVENt[left_fork], 1) == WAIT_TIMEOUT && WaitForSingleObject(EVENt[right_fork], 1) == WAIT_TIMEOUT)///Взаємовиключення !
        {
              
                printf("The Philosopher[%i] wants to eat. L[%i] R[%i] fork - success.\n", left_fork, left_fork, right_fork);
                // зайняті виделки    
 
                 SetEvent(EVENt[left_fork]);
                 SetEvent(EVENt[right_fork]);
                LeaveCriticalSection(&philosopherControl);// непорушна діяльність (зупинка) Критичні секції !
                printf("The Philosopher[%i] began to eat\n", left_fork);
                Sleep(MIN_TIME + rand() % (MAX_TIME - MIN_TIME));// деяка затримка
                 // незайняті виделки
                ResetEvent(EVENt[left_fork]);
                ResetEvent(EVENt[right_fork]);
                STATUS[left_fork] = true;
                STATUS[right_fork] = true;
                printf("The Philosopher[%i] finished eating\n", left_fork);
                count = 0;
        }    
        else 
        {
                printf("The philosopher [%i] wants to eat. Fork - not available\n", left_fork ); 
                count++;
                if(count >= STARVING) printf("\n!!!!!The philosopher [%i] - голодує!!!!.\n\n", left_fork);
                LeaveCriticalSection(&philosopherControl);// непорушна діяльність (зупинка) критичні секції
        }  
       
        
        
	}
}
int main(void)
{
	setlocale(LC_ALL, "Russian"); //Чтобы функция isalpha() работала с русским текстом 
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    InitializeCriticalSection(&philosopherControl);

    for (int i = 0; i < Number_OF_Philosophers; i++)
    {
        EVENt[i] = CreateEvent(NULL, TRUE, FALSE, NULL);// FALSE - автоматичне скидання, TRUE - сигналізується початковий стан
        ResetEvent(EVENt[i]);
    }

    for (int i = 0; i < Number_OF_Philosophers; i++)
    {
        WaitForSingleObject(iteratorControl, INFINITE);
        ResetEvent(iteratorControl); // зафіксуйте інші потоки, щоб зловити правильний номер
        philosopher_number = i;
        Philosophers[i] = CreateThread(NULL, 0, Thread, NULL, 0, NULL);
        STATUS[i] = true;
    }
    _getch();
}