# 2A Projet Compilation

## Usage

Use `make` to generate the executable `bin/scalpa`

Use `make clean` to delete temporary files

Use `make doc` to generate the doxygen html pages (in _doc/_)

Use `make test` to run the tests.

## Requirements

Some packages are needed to compile and run the tests

-   _bison_, for syntactic analysis
-   _flex_, for lexical analysis
-   _spim_, to create MIPS vm
-   _doxygen_, _graphvis_, for doc generation

## Capacités (fr)

|       Intitulé        |       Description        |  Etat   |
| :-------------------: | :----------------------: | :-----: |
| Opération sur entiers |        +,-,/,\*,^        | Terminé |
| Opérateur comparaison |       <,<=,>,>=,==       | Terminé |
| Opérateur sur booléen |      and,or,xor,not      | Terminé |
| Stucture de contrôle  |     while,if,ifelse      | Terminé |
|       Fonctions       | implémenter une fonction | Terminé |
|       Tableaux        |    générer un tableau    |   Non   |

## Tests (fr)

Un jeu de tests est fourni afin de vérifier le bon fonctionnement
de chaque partie.
Les programmes sources en scalpa se situent dans le dossier test puis
dans les différents dossier associés à la partie testée sous le nom "src.scalpa".
Quand il y en as une, le résultat fourni par l'exécution du ficher source se trouve
dans le même dossier sous le nom "out".
Les retours des tests, erreurs ou bon fonctionnement sont inscrits dans le fhichier test.log dans le dossier test.

## Conception

Le projet est séparé en plusieurs dossiers :

-   _bin/_ contient l'éxécutable généré
-   _build/_ contient les fichiers temporaire pour la compilation de l'éxécutable
-   _doc/_ contient la documentation doxygen, qui peut être générée avec la commande `make doc`
-   _include/_ contient tous les en-têtes c
-   _src/_ contient les fichiers sources c
-   _test/_ contient tous les tests

Le point d'entrée se situe dans les fichiers "grammar.y" et "grammar.l" dans le dossier "grammar".
Il s'agit des fichiers Yacc et Lex qui permettent d'analyser la syntaxe d'un fichier et d'ainsi vérifier sa bonne
implémentation et d'en reconnaitre les différentes composantes.

Lors de leur exécution ceux-ci vont appeller les différents fichiers .c contenu dans les dossier voisins.
Le dossier "génération" contient l'ensemble des fichiers .c nécessaire à la génération du code
intermédiaire vers des quads. Les quads sont stockés dans un tableau global et représentés par la structure quad_t.

Le dossier "mips" contient les fichiers nécessaires à la traduction des quads vers le langage mips.

Nous nous sommes forcé à utiliser que l'API POSIX de Yacc, car certains d'entres nous codent sur mac et leur version de yacc ne disposait que de l'api POSIX.
Cela a notamment posé problème pour la gestion de la mémoire (le paramètre `%destructor` n'étant pas POSIX).
Nous avons cherché des alternatives mais le travail n'est pas totalement terminé. 
Ainsi il est probable qu'en cas d'erreur la mémoire ne soit pas totalement libérée.
Par contre en cas de fonctionnement normal tout est correct (voir les tests valgrind).

## Limitations

Ce compilateur est assez limité.

Par exemple, un programe ne peut générer qu'au plus 10000 quads. 
Cela pourrait être corrigé en remplaçant le tableau de taille fixe tabQuad (cf *quad.h*) par une liste chainée ou plus simplement en augmentent la taille du tableau.

Les tableaux ne sont pas implémentés par manque de temps, l'implémentation des fonctions ayant été plus compliquée que prévue.

Les symboles doivent avoir un nom de moins de 64 caractères (peut être modifié avec la constante SYM_NAME_MAX_LEN dans *symbol.h*)
Les fonctions ne peuvent pas avoir plus de 4 arguments car MIPS ne propose que 4 registres pour passer des arguments aux fonctions.

On pourrait les passer via la pile, mais l'implémentation auraient été encore une fois plus complexe, nous avons préféré rester plus simple.
Par contre l'appel de fonction et la sauvegarde de contexte sont fonctionnelles (exemple : les fonctions récursives marchent)

Le passage de paramètre par références n'est pas disponible non plus.

Le programme généré n'est pas optimisé du tout, encore une fois par manque de temps.

## Authors

-   Téo DUCAROUGE
-   Thomas LEPINE
-   Alexandre MELKONIAN
-   Clément ROSSETTI
