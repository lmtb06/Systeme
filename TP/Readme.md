# TP1
TP sur la gestion des signaux avec quelques rappels sur les forks et execl.

- **Exo1**: Attend une durée défini et intercepte le signal SIGINT (CTRL+Z) sur une plage de temps donnée, affiche un message si il reçoit le signal 1 fois et affiche un message puis quitte s'il reçoit le signal 2 fois, quitte le programme à la fin du temps.
- **Exo2**: Mise en place d'un timer qui si on lui envoie le signal SIGINT (CTRL+Z) avant la fin du timer nous dit le temps qu'il restait avant la fin du timer, et nous dit qu'on a perdu le cas échéant.
- **Exo3**: Exo de rappel pour les forks et execl le pere prend en argument deux nombre et un texte et le fils retourne "J'ai __nbr1__*__nbr2__ __texte__".
- **Exo4**: Timer qui crée 2 fils et qui affiche comme gagnant le premier fils qui envoie son signal au père les affiche comme ex aequo s'ils ont le même temps.