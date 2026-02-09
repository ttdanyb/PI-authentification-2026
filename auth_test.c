/**
 * @file auth_test.c
 * @brief Tests simples pour la bibliothèque d'authentification.
 */

#include <stdio.h>
#include <string.h>

#include "auth.h"

static int test_inscription_et_auth(const char *csv)
{
    printf("[TEST] Inscription et authentification basique...\n");

    AuthStatus st;

    st = auth_init(csv);
    if (st != AUTH_OK)
    {
        printf("  Echec auth_init (code=%d)\n", st);
        return 0;
    }

    st = auth_register_user(csv, "alice", "password", "user");
    if (st != AUTH_OK)
    {
        printf("  Echec auth_register_user (code=%d)\n", st);
        return 0;
    }

    AuthUser u;
    st = auth_authenticate(csv, "alice", "password", &u);
    if (st != AUTH_OK)
    {
        printf("  Echec auth_authenticate (code=%d)\n", st);
        return 0;
    }

    if (strcmp(u.username, "alice") != 0 || strcmp(u.role, "user") != 0)
    {
        printf("  Données utilisateur incorrectes.\n");
        return 0;
    }

    printf("  OK\n");
    return 1;
}

static int test_mauvais_mdp(const char *csv)
{
    printf("[TEST] Authentification avec mauvais mot de passe...\n");

    AuthStatus st;
    st = auth_init(csv);
    if (st != AUTH_OK)
    {
        printf("  Echec auth_init (code=%d)\n", st);
        return 0;
    }

    st = auth_register_user(csv, "bob", "secret", "user");
    if (st != AUTH_OK)
    {
        printf("  Echec auth_register_user (code=%d)\n", st);
        return 0;
    }

    st = auth_authenticate(csv, "bob", "mauvais", NULL);
    if (st != AUTH_ERR_INVALID)
    {
        printf("  Attendu AUTH_ERR_INVALID, obtenu %d\n", st);
        return 0;
    }

    printf("  OK\n");
    return 1;
}

static int test_desactivation(const char *csv)
{
    printf("[TEST] Désactivation de compte...\n");

    AuthStatus st;
    st = auth_init(csv);
    if (st != AUTH_OK)
    {
        printf("  Echec auth_init (code=%d)\n", st);
        return 0;
    }

    st = auth_register_user(csv, "carol", "pwd", "user");
    if (st != AUTH_OK)
    {
        printf("  Echec auth_register_user (code=%d)\n", st);
        return 0;
    }

    st = auth_set_active(csv, "carol", 0);
    if (st != AUTH_OK)
    {
        printf("  Echec auth_set_active (code=%d)\n", st);
        return 0;
    }

    st = auth_authenticate(csv, "carol", "pwd", NULL);
    if (st != AUTH_ERR_INVALID)
    {
        printf("  Attendu AUTH_ERR_INVALID (compte inactif), obtenu %d\n", st);
        return 0;
    }

    printf("  OK\n");
    return 1;
}

int main(void)
{
    const char *csv = "test_users.csv";

    int ok = 1;
    ok &= test_inscription_et_auth(csv);
    ok &= test_mauvais_mdp(csv);
    ok &= test_desactivation(csv);

    if (ok)
    {
        printf("\nTous les tests ont réussi.\n");
        return 0;
    }
    else
    {
        printf("\nCertains tests ont échoué.\n");
        return 1;
    }
}

