# Pour installer, utiliser et compiler avec SDL
Dans un terminal :  
`sudo apt-get install libsdl2-dev `  
En haut de vos sources :  
`#include "SDL.h"`  
pour compiler :  
``gcc -c -Wall -Wextra main.c `sdl2-config --cflags` ``  
pour linker :  
``gcc -o exe main.o `sdl2-config --libs` ``  

# Pour commenter:
```
/**
* \fn 
* \brief
*/
```

# instructions pour commencer a participer :  

`sudo apt-get install git`

déplacez vous dans le répertoire de votre choix 
`git init`  
cette commande crée un dossier .git qui est utilisé par git pour stoquer les informations de versionning sur le projet  

A tout moment, vous pouvez faire  
`git status`  
pour savoir ou en est git  

les deux commandes qui suivent servent a signer vos contributions  
`git config --global user.name "votre_nom"`  
`git config --global user.email "votre_adresse_mail"`  

allez sur https://git.ares-ensiie.eu/ connectez vous, cherchez le projet que j'ai créé qui s'appelle "MCAC snake". En dessous du logo, il y a un lien en ssh:// Copiez le lien  
et faites :  
`git remote add origin le_lien`  
explication de la commande : "git" car c'est le programme qu'on utilise, "remote" car on veut gérer les répertoires distants (remote repository, c'est un répertoire sur internet qui contient aussi notre code, ici, c'est gitlab qui joue ce rôle), "add" car on veut déclarer un répertoire existant a git, "origin" pour dire a git qu'on y fera référence par le nom "origin" désormais, "lien" ben ca c'est pour lui donner le lien  

puis faites  
`git pull origin master`  
cette commande aura pour effet de mettre a jour les fichiers locaux si des modifications ont été apporté sur le répertoire distant, en l'occurence, comme vous n'avez encore rien, git va vous télécharger tous les fichiers que les autres ont mis sur gitlab  
"git" car c'est le programme qu'on utilise, "pull" pour "tirer" -> récupérer depuis un répertoire distant, "origin" pour désigner le répertoire dont on souhaite télécharger les modifications, "master" pour désigner quelle *branche* locale mettre a jour.  

faites vos modification en local  
ajoutez les a la liste des fichiers a suivre avec  
`git add le_fichier`  

avec git status, assurez vous que toutes vos modifications sont prises en compte  
faites  
`git commit -m "votre message"`  
cette commande aura pour effet d'enregistrer les modifications que vous avez apportées au projet sous forme de "commit"  
note : il est TRES important de mettre un message adéquat, qui décrive les modifications apportées.  

pour envoyer vos commits sur gitlab  
`git push origin master`  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  
.  

zenbu zenbu zenbu

