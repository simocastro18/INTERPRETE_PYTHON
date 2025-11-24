# Python Subset Interpreter

Un interprete scritto in C++ per un sottoinsieme del linguaggio Python, sviluppato come progetto per il corso di Informatica e Computazione.

## Descrizione

Questo progetto implementa un interprete per un sottoinsieme del linguaggio Python, sviluppato interamente in C++ secondo le specifiche del corso di Informatica e Computazione. L'interprete supporta costrutti fondamentali della programmazione come variabili, strutture di controllo, funzioni e operazioni di I/O di base.

## Caratteristiche Supportate

### Tipi di Dato
- **Numeri interi** (`int`) - interi a 64 bit con segno
- **Booleani** (`bool`) - `True` e `False`
- **Liste** (`list`) - vettori dinamici di valori interi o booleani

### Strutture di Controllo
- **Condizionali**: `if`, `elif`, `else`
- **Cicli**: `while`
- **Break e Continue**: per il controllo dei cicli

### Operazioni su Liste
- Creazione: `id = list()`
- Aggiunta elementi: `id.append(expr)`
- Accesso in lettura/scrittura: `id[expr]`

### Operatori
- **Aritmetici**: `+`, `-`, `*`, `//` (divisione intera)
- **Relazionali**: `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Booleani**: `and`, `or`, `not`
- **Unari**: `-` (negazione), `not`

### Input/Output
- Funzione `print(expr)` per l'output

## Specifiche Tecniche

### Sistema di Tokenizzazione
L'interprete implementa un sistema di tokenizzazione che include:
- Gestione dell'indentazione con token `indent` e `dedent`
- Riconoscimento di numeri, identificatori e parole chiave
- Supporto per newline e endmarker

### Grammatica Supportata
La grammatica implementata segue le produzioni specificate nel progetto:
- Programmazione strutturata con blocchi indentati
- Espressioni con precedenza e associatività corrette
- Gestione completa degli scope delle variabili

### Semantica
- Variabili globali uniche (scope unico)
- Tipizzazione dinamica
- Liste eterogenee (possono contenere interi e booleani)
- Short-circuit per operatori booleani
- Gestione errori di tipo e runtime

