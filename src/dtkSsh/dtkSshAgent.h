#ifndef __AGENT_H
#define __AGENT_H

#include "dtkSshLibssh.h"

/* Messages for the authentication agent connection. */
#define SSH_AGENTC_REQUEST_RSA_IDENTITIES        1
#define SSH_AGENT_RSA_IDENTITIES_ANSWER          2
#define SSH_AGENTC_RSA_CHALLENGE                 3
#define SSH_AGENT_RSA_RESPONSE                   4
#define SSH_AGENT_FAILURE                        5
#define SSH_AGENT_SUCCESS                        6
#define SSH_AGENTC_ADD_RSA_IDENTITY              7
#define SSH_AGENTC_REMOVE_RSA_IDENTITY           8
#define SSH_AGENTC_REMOVE_ALL_RSA_IDENTITIES     9

/* private OpenSSH extensions for SSH2 */
#define SSH2_AGENTC_REQUEST_IDENTITIES           11
#define SSH2_AGENT_IDENTITIES_ANSWER             12
#define SSH2_AGENTC_SIGN_REQUEST                 13
#define SSH2_AGENT_SIGN_RESPONSE                 14
#define SSH2_AGENTC_ADD_IDENTITY                 17
#define SSH2_AGENTC_REMOVE_IDENTITY              18
#define SSH2_AGENTC_REMOVE_ALL_IDENTITIES        19

/* smartcard */
#define SSH_AGENTC_ADD_SMARTCARD_KEY             20
#define SSH_AGENTC_REMOVE_SMARTCARD_KEY          21

/* lock/unlock the agent */
#define SSH_AGENTC_LOCK                          22
#define SSH_AGENTC_UNLOCK                        23

/* add key with constraints */
#define SSH_AGENTC_ADD_RSA_ID_CONSTRAINED        24
#define SSH2_AGENTC_ADD_ID_CONSTRAINED           25
#define SSH_AGENTC_ADD_SMARTCARD_KEY_CONSTRAINED 26

#define SSH_AGENT_CONSTRAIN_LIFETIME             1
#define SSH_AGENT_CONSTRAIN_CONFIRM              2

/* extended failure messages */
#define SSH2_AGENT_FAILURE                       30

/* additional error code for ssh.com's ssh-agent2 */
#define SSH_COM_AGENT2_FAILURE                   102

#define SSH_AGENT_OLD_SIGNATURE                  0x01

struct ssh_agent_struct {
  struct socket *sock;
  ssh_buffer ident;
  unsigned int count;
};

#ifndef _WIN32
/* agent.c */
/**
 * @brief Create a new ssh agent structure.
 *
 * @return An allocated ssh agent structure or NULL on error.
 */
struct ssh_agent_struct *agent_new(struct ssh_session_struct *session);

void agent_close(struct ssh_agent_struct *agent);

/**
 * @brief Free an allocated ssh agent structure.
 *
 * @param agent The ssh agent structure to free.
 */
void agent_free(struct ssh_agent_struct *agent);

/**
 * @brief Check if the ssh agent is running.
 *
 * @param session The ssh session to check for the agent.
 *
 * @return 1 if it is running, 0 if not.
 */
int agent_is_running(struct ssh_session_struct *session);

int agent_get_ident_count(struct ssh_session_struct *session);

struct ssh_public_key_struct *agent_get_next_ident(struct ssh_session_struct *session,
    char **comment);

struct ssh_public_key_struct *agent_get_first_ident(struct ssh_session_struct *session,
    char **comment);

ssh_string agent_sign_data(struct ssh_session_struct *session,
    struct ssh_buffer_struct *data,
    struct ssh_public_key_struct *pubkey);
#endif

#endif /* __AGENT_H */
/* vim: set ts=2 sw=2 et cindent: */
