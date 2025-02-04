#pragma once


void AcquireSpinLock(volatile LONG* plCount);
void AcquireSpinLockNoYield(volatile LONG* plCount);
BOOL TryAcquireSpinLock(volatile LONG* plCount);
void AcquireSpinLockWithSleep(volatile LONG* plCount,DWORD dwSleepTime);
void ReleaseSpinLock(volatile LONG* plCount);

