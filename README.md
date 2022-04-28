# Lecture d'un fichier MIDI

Ce code permet d'extraire la suite d'événements MIDI d'un fichier.

Une fois exécuté, il affiche la suite d'événements MIDI dans la console.

Le fichier `test.mid` contient un accord Do majeur en noir.  

La sortie sur la console de ce fichier est :
```console
Event time : 0 - Note : 60 ON
Event time : 25 - Note : 64 ON
Event time : 25 - Note : 67 ON
Event time : 25 - Note : 72 ON
```

La première valeur affichée est le nombre d'incréments qu'il faut attendre avant
d'exécuter l'action.

Dans ce cas, la note `60` donc `DO4` sera jouée immédiatement et ensuite il faut passer à la ligne suivante.  
Maintenant, il faut attendre 25 incréments avant de jouer la note `64` et ainsi de suite.


Le fichier `sol_maj.mid` contient un accord de Do et Sol majeur simultané.
```console
1  Event time : 0 - Note : 60 ON
2  Event time : 0 - Note : 67 ON
3  Event time : 25 - Note : 64 ON
4  Event time : 0 - Note : 71 ON
5  Event time : 25 - Note : 67 ON
6  Event time : 0 - Note : 74 ON
7  Event time : 25 - Note : 72 ON
8  Event time : 0 - Note : 79 ON
```

Cet exemple s'exécute ainsi :
- ligne 1, on joue directement la note `60`.
- ligne 2, pas d'attente, on joue directement la note `67`, elle commence donc en même temps.
- ligne 3, il faut attendre `25` incréments avant de jouer la note `64`.
- ligne 4, il faut jouer immédiatement la note `71`
- idem pour la suite.
