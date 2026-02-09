/**
 * @file main.c
 * @brief Interface console pour le système d'authentification.
 */

#include <stdio.h>
#include <string.h>

#include "auth.h"

#define CSV_PATH "users.csv"

static void lire_ligne(const char *invite, char *buffer, size_t taille)
{
    printf("%s", invite);
    if (fgets(buffer, (int)taille, stdin))
    {
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
        {
            buffer[len - 1] = '\0';
        }
    }
}

static void afficher_utilisateur(const AuthUser *u)
{
    printf(" - %s (role=%s, actif=%s)\n",
           u->username,
           u->role,
           u->active ? "oui" : "non");
}

static void menu_inscription(void)
{
    char username[AUTH_MAX_USERNAME + 1];
    char password[AUTH_MAX_PASSWORD + 1];
    char role[AUTH_MAX_ROLE + 1];

    lire_ligne("Identifiant : ", username, sizeof(username));
    lire_ligne("Mot de passe : ", password, sizeof(password));
    lire_ligne("Rôle (user/admin, etc.) : ", role, sizeof(role));

    AuthStatus st = auth_register_user(CSV_PATH, username, password, role);
    if (st == AUTH_OK)
    {
        printf("Utilisateur créé avec succès.\n");
    }
    else if (st == AUTH_ERR_EXISTS)
    {
        printf("Erreur : un utilisateur avec cet identifiant existe déjà.\n");
    }
    else
    {
        printf("Erreur lors de la création de l'utilisateur (code=%d).\n", st);
    }
}

static void menu_connexion(void)
{
    char username[AUTH_MAX_USERNAME + 1];
    char password[AUTH_MAX_PASSWORD + 1];

    lire_ligne("Identifiant : ", username, sizeof(username));
    lire_ligne("Mot de passe : ", password, sizeof(password));

    AuthUser u;
    AuthStatus st = auth_authenticate(CSV_PATH, username, password, &u);
    if (st == AUTH_OK)
    {
        printf("Authentification réussie. Bonjour %s (role=%s) !\n",
               u.username, u.role);
    }
    else if (st == AUTH_ERR_NOTFOUND)
    {
        printf("Utilisateur inconnu.\n");
    }
    else if (st == AUTH_ERR_INVALID)
    {
        printf("Mot de passe incorrect ou compte inactif.\n");
    }
    else
    {
        printf("Erreur d'authentification (code=%d).\n", st);
    }
}

static void menu_changer_mdp(void)
{
    char username[AUTH_MAX_USERNAME + 1];
    char old_password[AUTH_MAX_PASSWORD + 1];
    char new_password[AUTH_MAX_PASSWORD + 1];

    lire_ligne("Identifiant : ", username, sizeof(username));
    lire_ligne("Ancien mot de passe : ", old_password, sizeof(old_password));
    lire_ligne("Nouveau mot de passe : ", new_password, sizeof(new_password));

    AuthStatus st = auth_change_password(CSV_PATH, username,
                                         old_password, new_password);
    if (st == AUTH_OK)
    {
        printf("Mot de passe mis à jour.\n");
    }
    else if (st == AUTH_ERR_NOTFOUND)
    {
        printf("Utilisateur inconnu.\n");
    }
    else if (st == AUTH_ERR_INVALID)
    {
        printf("Ancien mot de passe incorrect.\n");
    }
    else
    {
        printf("Erreur lors du changement de mot de passe (code=%d).\n", st);
    }
}

static void menu_activation(int activer)
{
    char username[AUTH_MAX_USERNAME + 1];
    lire_ligne("Identifiant : ", username, sizeof(username));

    AuthStatus st = auth_set_active(CSV_PATH, username, activer);
    if (st == AUTH_OK)
    {
        printf("Compte %s %s.\n",
               username,
               activer ? "activé" : "désactivé");
    }
    else if (st == AUTH_ERR_NOTFOUND)
    {
        printf("Utilisateur inconnu.\n");
    }
    else
    {
        printf("Erreur lors de la mise à jour du compte (code=%d).\n", st);
    }
}

static void menu_lister(void)
{
    AuthUser *users = NULL;
    size_t    count = 0;

    AuthStatus st = auth_list_users(CSV_PATH, &users, &count);
    if (st != AUTH_OK)
    {
        printf("Impossible de lire la liste des utilisateurs (code=%d).\n", st);
        return;
    }

    printf("Utilisateurs (%zu) :\n", count);
    for (size_t i = 0; i < count; ++i)
    {
        afficher_utilisateur(&users[i]);
    }

    auth_free_user_list(users);
}

static void afficher_menu(void)
{
    printf("\n=== Menu Authentification ===\n");
    printf("1. Inscription (créer un compte)\n");
    printf("2. Connexion\n");
    printf("3. Changer de mot de passe\n");
    printf("4. Activer un compte\n");
    printf("5. Désactiver un compte\n");
    printf("6. Lister les utilisateurs\n");
    printf("0. Quitter\n");
}

int main(void)
{
    AuthStatus st = auth_init(CSV_PATH);
    if (st != AUTH_OK)
    {
        printf("Erreur d'initialisation du fichier utilisateurs (code=%d).\n", st);
        return 1;
    }

    int choix = -1;
    char buffer[32];

    do
    {
        afficher_menu();
        lire_ligne("Votre choix : ", buffer, sizeof(buffer));
        if (sscanf(buffer, "%d", &choix) != 1)
        {
            choix = -1;
        }

        switch (choix)
        {
            case 1:
                menu_inscription();
                break;
            case 2:
                menu_connexion();
                break;
            case 3:
                menu_changer_mdp();
                break;
            case 4:
                menu_activation(1);
                break;
            case 5:
                menu_activation(0);
                break;
            case 6:
                menu_lister();
                break;
            case 0:
                printf("Au revoir.\n");
                break;
            default:
                printf("Choix invalide.\n");
                break;
        }

    } while (choix != 0);

    return 0;
}

