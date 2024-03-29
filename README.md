# ESP-32 misurazioni distanza con BLE


## Descrizione
Questo progetto utilizza un ESP-32 per misurare la distanza utilizzando il sensore HC-SR04 e trasmettere i dati tramite BLE a un client.

## Funzionalità
- Inizializzazione e configurazione del dispositivo BLE.
- Definizione dei pin e impostazioni del sensore HC-SR04.
- Gestione degli eventi di connessione e disconnessione BLE.
- Creazione e avvio del servizio BLE per la notifica della distanza.
- Misurazione e calcolo della distanza tramite il sensore HC-SR04.
- Trasmissione dei dati di distanza al client BLE.

## Requisiti
- ESP-32
- Sensore HC-SR04
- Arduino IDE o PlatformIO

## Installazione
Caricare il codice sull'ESP-32 utilizzando l'Arduino IDE o PlatformIO.

## Uso
Accendere l'ESP-32 e connettersi al server BLE utilizzando un client compatibile per ricevere le notifiche della distanza misurata.
