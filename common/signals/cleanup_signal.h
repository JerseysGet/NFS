#ifndef __CLEANUP_SIGNAL_H
#define __CLEANUP_SIGNAL_H

void initEscapeHatch(void (*cleanup)());
void destroyEscapeHatch();

#endif