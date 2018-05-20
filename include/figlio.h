#ifndef FIGLIO_H
#define FIGLIO_H

/// @file

/// @defgroup figlio

/// @{

/**
 *	@brief	Wrapper del processo figlio
 *	@param s1Size Dimensione del buffer s1
 */
void figlio(int s1Size);

/**
 *	@brief	Signal handler, stampa quale stringa sta venendo esaminata da quale nipote accedendo alla struttura Status presente in memoria condivisa
 *	@param	currentSignal Il segnale da gestire
 */
void status_updated(int currentSignal);

/**
 *	@brief	Deposita il messaggio di terminazione (mtype=1) nella coda di messaggi del processo logger
 */
void send_terminate();

/// @}

#endif
