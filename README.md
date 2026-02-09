# Système d'authentification simple en C (CSV)

Ce projet fournit une petite bibliothèque d'authentification en C basée sur un
fichier CSV, ainsi qu'une interface console de démonstration et quelques tests.

> **Attention sécurité :** les mots de passe sont stockés en clair dans le CSV.
> Ce code est destiné à l'apprentissage et à la démonstration, pas à la
> production. Pour un vrai projet, il faut utiliser un hachage sécurisé
> (bcrypt, Argon2, etc.) et des politiques de mots de passe robustes.

## Fichiers principaux

- `auth.h` : définition de l'API (types, fonctions) utilisable depuis la
  console ou une future interface graphique en C.
- `auth.c` : implémentation de la bibliothèque (lecture/écriture CSV,
  vérification des identifiants, changement de mot de passe, activation,
  désactivation, listing).
- `main.c` : interface console interactive utilisant la bibliothèque.
- `auth_test.c` : tests simples de la bibliothèque.
- `Makefile` : règles de compilation.

## Format du fichier CSV

Le fichier utilisateurs est un fichier texte où chaque ligne représente un
utilisateur, au format :

```text
identifiant;mot_de_passe;role;actif
```

Exemple :

```text
alice;password;user;1
bob;secret;admin;1
carol;pwd;user;0
```

- `identifiant` : nom d'utilisateur (unique).
- `mot_de_passe` : mot de passe en clair (démonstration !).
- `role` : texte libre (par exemple `user` ou `admin`).
- `actif` : `1` si le compte est actif, `0` sinon.

Le chemin du fichier CSV utilisé par défaut par la console est `users.csv`.

## Compilation

Sous Windows avec `gcc` (MinGW) ou sous Linux avec `gcc` classique, vous pouvez
simplement lancer :

```bash
make
```

Cela génère :

- l'exécutable `main` (ou `main.exe` sous Windows),
- l'exécutable de tests `auth_test` (ou `auth_test.exe`).

Pour nettoyer les fichiers générés :

```bash
make clean
```

Pour lancer les tests :

```bash
make test
```

Pour lancer l'interface console :

```bash
make run
```

ou directement :

```bash
./main
```

## Utilisation de l'API dans une interface graphique

L'API dans `auth.h` est indépendante de l'interface utilisateur. Vous pouvez
l'inclure dans un projet C avec une interface graphique (GTK, Win32, etc.) et
appeler les fonctions suivantes :

- `auth_init(const char *csv_path)` : s'assure que le fichier d'utilisateurs
  existe.
- `auth_register_user(const char *csv_path, const char *username, const char *password, const char *role)` :
  crée un nouvel utilisateur.
- `auth_authenticate(const char *csv_path, const char *username, const char *password, AuthUser *out_user)` :
  vérifie un couple identifiant/mot de passe.
- `auth_change_password(const char *csv_path, const char *username, const char *old_password, const char *new_password)` :
  change le mot de passe d'un utilisateur.
- `auth_set_active(const char *csv_path, const char *username, int active)` :
  active/désactive un compte.
- `auth_list_users(const char *csv_path, AuthUser **out_users, size_t *out_count)` :
  récupère tous les utilisateurs en mémoire (à libérer ensuite avec
  `auth_free_user_list`).
- `auth_free_user_list(AuthUser *users)` : libère la liste allouée.

Les commentaires dans `auth.h` et `auth.c` suivent un style proche de la
Javadoc (compatible avec Doxygen), ce qui permet de générer une documentation
HTML en utilisant Doxygen si vous le souhaitez.

## Workflow Git et GitHub pour les étudiants

Cette section explique comment :

- tester le projet en local avec Git,
- publier ce dépôt sur GitHub,
- collaborer entre étudiants,
- intégrer cette bibliothèque dans un autre projet C.

### 1. Initialiser un dépôt Git local

Depuis le dossier du projet (par exemple `PI auth 2026`) :

```bash
git init
git add .
git commit -m "Initialisation du projet d'authentification CSV"
```

Pour vérifier l’état du dépôt :

```bash
git status
```

### 2. Créer un dépôt GitHub et pousser le code

1. Créez un nouveau dépôt vide sur GitHub (sans README, sans `.gitignore`).
2. Copiez l’URL fournie par GitHub, par exemple :
   - `https://github.com/mon-compte/pi-auth-2026.git`
   - ou `git@github.com:mon-compte/pi-auth-2026.git`
3. Liez votre dépôt local au dépôt GitHub et poussez :

```bash
git remote add origin https://github.com/mon-compte/pi-auth-2026.git
git branch -M main
git push -u origin main
```

Ensuite, pour envoyer des modifications :

```bash
git add .
git commit -m "Description courte et claire de la modification"
git push
```

### 3. Récupérer et tester le projet en local (pour les étudiants)

Pour cloner le projet sur une autre machine ou pour un autre étudiant :

```bash
git clone https://github.com/mon-compte/pi-auth-2026.git
cd pi-auth-2026
```

Puis compiler et tester :

```bash
make        # compile les binaires
make test   # lance les tests automatiques
make run    # lance l’interface console
```

Si vous modifiez le code :

```bash
git status           # voir les fichiers modifiés
git add fichier.c    # ou git add .
git commit -m "Explication de la modification"
git push             # envoyer sur GitHub
```

### 4. Collaborer sur le projet (branches, pull requests)

#### Travailler sur une branche

Pour développer une nouvelle fonctionnalité sans casser la version principale :

```bash
git checkout -b feature/nouvelle-fonction
```

Faites vos modifications, puis :

```bash
git add .
git commit -m "Ajout de la fonctionnalité X"
git push -u origin feature/nouvelle-fonction
```

Sur GitHub, créez ensuite une **Pull Request** (PR) depuis la branche
`feature/nouvelle-fonction` vers `main`. Les autres étudiants peuvent :

- commenter le code,
- demander des changements,
- approuver la PR,
- puis la fusionner.

#### Récupérer les changements des autres

Pour mettre à jour votre dépôt local avec les dernières modifications de `main` :

```bash
git checkout main
git pull
```

Pour mettre à jour votre branche avec la dernière version de `main` :

```bash
git checkout feature/nouvelle-fonction
git merge main
```

### 5. Travailler à partir d’un fork (cas d’un chef de groupe)

Ce scénario est courant en projet de groupe :

- le **chef de groupe** crée le dépôt GitHub principal,
- les **autres étudiants** créent un **fork** sur leur propre compte GitHub,
- chacun travaille sur son fork, puis envoie ses changements au dépôt du chef
  via des Pull Requests.

#### Rôle du chef de groupe

1. Créer le dépôt sur GitHub (par exemple `https://github.com/chef/pi-auth-2026.git`).
2. Pousser le code initial (voir section 2 ci-dessus).
3. Donner l’URL du dépôt aux autres membres du groupe.

#### Rôle d’un étudiant (membre du groupe)

1. Aller sur la page GitHub du dépôt du chef (ex. `https://github.com/chef/pi-auth-2026`).
2. Cliquer sur le bouton **Fork** pour créer une copie sur son propre compte
   (par ex. `https://github.com/etudiant/pi-auth-2026`).
3. Cloner **son fork** en local :

```bash
git clone https://github.com/etudiant/pi-auth-2026.git
cd pi-auth-2026
```

4. Configurer le dépôt du chef comme *remote* supplémentaire (`upstream`) pour
   pouvoir récupérer les mises à jour du projet principal :

```bash
git remote add upstream https://github.com/chef/pi-auth-2026.git
git remote -v   # pour vérifier origin (fork) et upstream (dépôt du chef)
```

5. Créer une branche de travail sur son fork :

```bash
git checkout -b feature/ma-tache
```

6. Modifier le code, tester localement (`make test`, `make run`), puis valider :

```bash
git add .
git commit -m "Implémentation de ma tâche"
git push -u origin feature/ma-tache
```

7. Sur GitHub (sur le fork de l’étudiant), créer une **Pull Request** depuis
   `feature/ma-tache` vers la branche `main` du dépôt du chef (`chef/pi-auth-2026`).

#### Mettre à jour son fork avec les changements du chef

Régulièrement, un étudiant doit synchroniser son fork avec le dépôt principal :

```bash
# 1. Aller sur la branche main locale
git checkout main

# 2. Récupérer les dernières modifications du dépôt du chef
git pull upstream main

# 3. Envoyer ces modifications sur son propre fork
git push origin main
```

Pour mettre à jour une branche de travail à partir de la dernière `main` :

```bash
git checkout feature/ma-tache
git merge main
```

### 6. Intégrer cette bibliothèque dans un autre projet C

Pour utiliser cette bibliothèque dans un autre projet C :

1. Ajouter les fichiers `auth.c` et `auth.h` dans le nouveau projet
   (par exemple dans un sous-dossier `auth/`).
2. Inclure `auth.h` dans vos fichiers C :

```c
#include "auth.h"
```

3. Appeler les fonctions de l’API dans votre code (par exemple, dans un code
   GTK, Win32, etc.) :

```c
AuthStatus st = auth_authenticate("users.csv", username, password, &user);
```

4. Adapter votre système de build (Makefile, CMake, etc.) pour compiler aussi
   `auth.c` et lier le tout dans votre exécutable.

Exemple simple dans un autre `Makefile` :

```make
OBJS = main.o auth.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)
```

Ainsi, le même cœur d’authentification peut être utilisé à la fois dans ce
projet de démonstration en console et dans vos projets C avec interface
graphique.

