# The Forbidden Sketchbook
Elaborato anno accademico 2022-2023

## Concept:
  - ### GamePlay
      Gioco "single-player" strategico a turni top-down 2D. Il giocatore ha l'obbiettivo di sbloccare e quindi raggiungere i tre piedistalli identificati dai diversi colori. Per completare questa missione dovrà sconfiggere nemici di diverso genere che cercheranno di ostacolarlo. Il programma è simulativo di un gioco da tavolo, quindi lo svolgimento del turno del giocatore sarà interattivo vale a dire determinabile dai diversi input dati dal giocatore fisico, i nemici sono come pedine il cui svolgimento del turno è determinato completamente all'inizio della loro fase di gioco. 
      Il gioco ha anche la possibilità di essere salvato ed è presente una modalità Editor nella quale il giocatore ha la possibilità di disegnare la propria mappa.

## Meccaniche di gioco:
  - ### Mappa
      La mappa è composta di tile che costituiscono la scacchiera di gioco. Lungo il percorso il giocatore si imbatterà in ostacoli fisici e "psicologici": 
      + __muri__
      + __fosse__
      
         > a differenza dei muri possono essere occupati da nemici volanti.
      
      + __piedistalli__ 

         > conservano gli _strumenti_ e _colori_ non acora sbloccati.
     
  - ### Giocatore
      Il giocatore controlla la propria pedina nel mondo di gioco cliccando sui tile con i quali vuole interagire, limitato, però, dall' __energia__ (stamina).
      L'energia viene reimpostata ad un valore di default all'inzio del turno del giocatore, senza, quindi, accumulare i punti non impiegati nel turno           precedente. Il protagonista ha a disposizione strumenti ed armi per affrontare i vari ostacoli. Lo stato dei __punti vita__, strumenti e punti azione sono         sempre visibili al giocatore. Il turno del giocatore termina con l'esaurimento dell'energia.

  - ### Combattimento e movimento
      La fase di combattimento si può svolgere in due modi: a distanza o corpo a corpo. Il giocatore è dotato di un' arma a distanza mentre i diversi nemici hanno diverse armi che possono essere di entrambi i tipi.
      In caso il mob si trovi fuori dal range ma il giocatore possedesse abbastanza energia ( vedi [Giocatore](#giocatore) ) il giocatore procederà ad           avvicinarsi al mob fino a entrare nel range necessario per poi attaccarlo. Il giocatore può alternare movimento e attacco i nemici invece possono muoversi solo prima di sferrare un attacco. 
             
  - ### Nemici
      La mappa è disseminata di vari nemici, che rinascono dopo un certo numero di turni (dalla morte), nel tile in cui sono nati. Così come per il player, le entità nemiche si conclude con l'esaurimento dell'energia o con l'impossibilità di svolgere altre azioni (è possibile che anche se il nemico ha abbastanza energia per muoversi non ne abbia abbastanza per attacare, quindi se si trova in range passerà semplicemente il suo turno).
      + __nemici volanti__ 

         > ricercano la posizione del giocatore avvicinandosi per colpirlo ingnorando le fosse.

      + __nemici "corpo a corpo"__
      
         > ricercano la posizione del giocatore avvicinandosi per colpirlo.
         
      + __nemici "da distanza"__
      
         > Se il giocatore si trova in un certo range dalla sua posizione, si muovono ignorando ogni tipo di ostacolo in un tile casuale ma distante dal                 giocatore. Se il giocatore è sufficientemente distante lo attaccherà dalla distanza.
___
## Autori:
Bernacchioni Lapo </br>
Tescaro Rocco
