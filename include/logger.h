#ifndef LOGGER_H
#define LOGGER_H

/// @file

/// @defgroup logger

/// @{

/**
 *	@brief Wrapper del processo logger
 */
void logger();

/**
 *	@brief	Stampa la coda di messaggi su stdout
 *	@return 1 se ha ricevuto un mtype=1 (quit signal), 0 altrimenti
 */
int polling_receive();
 
/// @} 

#endif
