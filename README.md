# 2A Projet Compilation

## Usage

Use `make` to generate the executable `bin/scalpa`
Use `make clean` to delete temporary files
Use `make doc` to generate the doxygen html pages (in _doc/_)
Use `make test` to run the tests.

## Requirements

Some packages are needed to compile and run the tests
* _bison_, for syntactic analysis
* _flex_, for lexical analysis
* _spim_, to create MIPS vm
* _doxygen_, _graphvis_, for doc generation

## Authors

- Téo DUCAROUGE
- Thomas LEPINE
- Alexandre MELKONIAN
- Clément ROSSETTI

## Capacités

| Intitulé      		| Description     	  		|
|:---------------------:|:-------------------------:|
| Opération sur entiers	| +,-,/,*,^		  			|
| Opérateur comparaison | <,<=,>,>=,==    			|
| Opérateur sur booléen	| and,or,xor,not			|
| Stucture de contrôle	| while,if,ifelse 			|
| Fonctions				| implémenter une fonction  |
| Tableaux				| générer un tableau 		|

## Tests

Un jeu de tests est fourni afin de vérifier le bon fonctionnement
de chaque partie.
Les programmes sources en scalpa se situent dans le dossier test puis
dans les différents dossier associés à la partie testée sous le nom "src.scalpa".
Quand il y en as une, le résultat fourni par l'exécution du ficher source se trouve
dans le même dossier sous le nom "out".
Les retours des tests, erreurs ou bon fonctionnement sont inscrits dans le test.log
dans le dossier test.

## Conception

Tout commence avec les fichiers "grammar.y" et "grammar.l" dans le dossier "grammar",
ils permettent d'analyser la syntaxe d'un programme et d'ainsi vérifier sa bonne
implémentation et d'en reconnaitre les différentes composantes.
Lors de leur exécution ceux-ci vont appeller les différents fichiers .c contenu dans les dossier voisins.
Le dossier "génération" contient l'ensemble des fichiers .c nécessaire à la génération du code
intermédiaire vers des quads.
Le dossier "mips" contient les fichiers nécessaires à la traduction des quads vers
le langage mips.