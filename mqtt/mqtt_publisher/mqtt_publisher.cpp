/*
 Copyright (c) 2009-2014 Roger Light <roger@atchoo.org>

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Eclipse Public License v1.0
 and Eclipse Distribution License v1.0 which accompany this distribution.

 The Eclipse Public License is available at
 http://www.eclipse.org/legal/epl-v10.html
 and the Eclipse Distribution License is available at
 http://www.eclipse.org/org/documents/edl-v10.php.

 Contributors:
 Roger Light - initial implementation and documentation.
 */
#include <iostream>
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#include <winsock2.h>
#define snprintf sprintf_s
#endif

#include <mosquitto.h>

#define STR(x) #x << '=' << x


/* pub_client.c modes */
enum msgmode
{
  MSGMODE_NONE,
  MSGMODE_CMD,
  MSGMODE_STDIN_LINE,
  MSGMODE_STDIN_FILE,
  MSGMODE_FILE,
  MSGMODE_NULL
};

#define CASE_STREAM_LITERAL_BREAK(x,y) case y: x << #y; break;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

std::string
toString (const msgmode& mm)
{
  std::ostringstream oss;
  switch (mm)
  {
    CASE_STREAM_LITERAL_BREAK(oss, MSGMODE_NONE);
    CASE_STREAM_LITERAL_BREAK(oss, MSGMODE_CMD);
    CASE_STREAM_LITERAL_BREAK(oss, MSGMODE_STDIN_LINE);
    CASE_STREAM_LITERAL_BREAK(oss, MSGMODE_STDIN_FILE);
    CASE_STREAM_LITERAL_BREAK(oss, MSGMODE_FILE);
    CASE_STREAM_LITERAL_BREAK(oss, MSGMODE_NULL);
  }
  return oss.str();
}

#pragma GCC diagnostic pop

std::ostream&
operator<< (std::ostream& os, const msgmode& mm)
{
  os << toString(mm);
  return os;
}
//#define MSGMODE_NONE 0
//#define MSGMODE_CMD 1
//#define MSGMODE_STDIN_LINE 2
//#define MSGMODE_STDIN_FILE 3
//#define MSGMODE_FILE 4
//#define MSGMODE_NULL 5

#define CLIENT_PUB 1
#define CLIENT_SUB 2

struct mosq_config
{
  char *id;
  char *id_prefix;
  int protocol_version;
  int keepalive;
  char *host;
  int port;
  int qos;
  bool retain;
  msgmode pub_mode; /* pub */
  char *file_input; /* pub */
  char *message; /* pub */
  long msglen; /* pub */
  char *topic; /* pub */
  char *bind_address;
#ifdef WITH_SRV
  bool use_srv;
#endif
  bool debug;
  bool quiet;
  unsigned int max_inflight;
  char *username;
  char *password;
  char *will_topic;
  char *will_payload;
  long will_payloadlen;
  int will_qos;
  bool will_retain;
#ifdef WITH_TLS
  char *cafile;
  char *capath;
  char *certfile;
  char *keyfile;
  char *ciphers;
  bool insecure;
  char *tls_version;
#  ifdef WITH_TLS_PSK
  char *psk;
  char *psk_identity;
#  endif
#endif
  bool clean_session; /* sub */
  char **topics; /* sub */
  int topic_count; /* sub */
  bool no_retain; /* sub */
  char **filter_outs; /* sub */
  int filter_out_count; /* sub */
  bool verbose; /* sub */
  bool eol; /* sub */
  int msg_count; /* sub */
#ifdef WITH_SOCKS
  char *socks5_host;
  int socks5_port;
  char *socks5_username;
  char *socks5_password;
#endif
};

std::string toString(const mosq_config& cfg)
{
  std::ostringstream oss;
  oss
    << STR(cfg.topic) << ','
    << STR(cfg.message) << ','
    << STR(cfg.qos) << ','
    << STR(cfg.retain) << ','
    << STR(cfg.pub_mode) << ','
    << STR(cfg.username) << ','
    << STR(cfg.password) << ','
    << STR(cfg.quiet);
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const mosq_config& cfg)
{
  os << toString(cfg);
  return os;
}

int
client_opts_set (struct mosquitto *mosq, struct mosq_config *cfg)
{
  //int rc;

  if (cfg->will_topic
      && mosquitto_will_set(mosq, cfg->will_topic, cfg->will_payloadlen, cfg->will_payload,
                            cfg->will_qos, cfg->will_retain))
  {

    if (!cfg->quiet)
      fprintf(stderr, "Error: Problem setting will.\n");
    mosquitto_lib_cleanup();
    return 1;
  }
  if (cfg->username && mosquitto_username_pw_set(mosq, cfg->username, cfg->password))
  {
    if (!cfg->quiet)
      fprintf(stderr, "Error: Problem setting username and password.\n");
    mosquitto_lib_cleanup();
    return 1;
  }
#ifdef WITH_TLS
  if((cfg->cafile || cfg->capath)
      && mosquitto_tls_set(mosq, cfg->cafile, cfg->capath, cfg->certfile, cfg->keyfile, NULL))
  {

    if(!cfg->quiet) fprintf(stderr, "Error: Problem setting TLS options.\n");
    mosquitto_lib_cleanup();
    return 1;
  }
  if(cfg->insecure && mosquitto_tls_insecure_set(mosq, true))
  {
    if(!cfg->quiet) fprintf(stderr, "Error: Problem setting TLS insecure option.\n");
    mosquitto_lib_cleanup();
    return 1;
  }
#  ifdef WITH_TLS_PSK
  if(cfg->psk && mosquitto_tls_psk_set(mosq, cfg->psk, cfg->psk_identity, NULL))
  {
    if(!cfg->quiet) fprintf(stderr, "Error: Problem setting TLS-PSK options.\n");
    mosquitto_lib_cleanup();
    return 1;
  }
#  endif
  if(cfg->tls_version && mosquitto_tls_opts_set(mosq, 1, cfg->tls_version, cfg->ciphers))
  {
    if(!cfg->quiet) fprintf(stderr, "Error: Problem setting TLS options.\n");
    mosquitto_lib_cleanup();
    return 1;
  }
#endif
  mosquitto_max_inflight_messages_set(mosq, cfg->max_inflight);
#ifdef WITH_SOCKS
  if(cfg->socks5_host)
  {
    rc = mosquitto_socks5_set(mosq, cfg->socks5_host, cfg->socks5_port, cfg->socks5_username, cfg->socks5_password);
    if(rc)
    {
      mosquitto_lib_cleanup();
      return rc;
    }
  }
#endif
  mosquitto_opts_set(mosq, MOSQ_OPT_PROTOCOL_VERSION, &(cfg->protocol_version));
  return MOSQ_ERR_SUCCESS;
}

int
client_connect (struct mosquitto *mosq, struct mosq_config *cfg)
{
  char err[1024];
  int rc;

#ifdef WITH_SRV
  if(cfg->use_srv)
  {
    rc = mosquitto_connect_srv(mosq, cfg->host, cfg->keepalive, cfg->bind_address);
  }
  else
  {
    rc = mosquitto_connect_bind(mosq, cfg->host, cfg->port, cfg->keepalive, cfg->bind_address);
  }
#else
  rc = mosquitto_connect_bind(mosq, cfg->host, cfg->port, cfg->keepalive, cfg->bind_address);
#endif
  if (rc > 0)
  {
    if (!cfg->quiet)
    {
      if (rc == MOSQ_ERR_ERRNO)
      {
#ifndef WIN32
        strerror_r(errno, err, 1024);
#else
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errno, 0, (LPTSTR)&err, 1024, NULL);
#endif
        fprintf(stderr, "Error: %s\n", err);
      }
      else
      {
        fprintf(stderr, "Unable to connect (%s).\n", mosquitto_strerror(rc));
      }
    }
    mosquitto_lib_cleanup();
    return rc;
  }
  return MOSQ_ERR_SUCCESS;
}

//#include "client_shared.h"

#define STATUS_CONNECTING 0
#define STATUS_CONNACK_RECVD 1
#define STATUS_WAITING 2

/* Process a tokenised single line from a file or set of real argc/argv */
static int
client_config_line_proc (struct mosq_config *cfg, int pub_or_sub, int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port"))
    {
      if (i == argc - 1)
      {
        fprintf(stderr, "Error: -p argument given but no port specified.\n\n");
        return 1;
      }
      else
      {
        cfg->port = atoi(argv[i + 1]);
        if (cfg->port < 1 || cfg->port > 65535)
        {
          fprintf(stderr, "Error: Invalid port given: %d\n", cfg->port);
          return 1;
        }
      }
      i++;
    }
    else if (!strcmp(argv[i], "-A"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -A argument given but no address specified.\n\n");
        return 1;
      }
      else
      {
        cfg->bind_address = strdup(argv[i + 1]);
      }
      i++;
#ifdef WITH_TLS
    }
    else if(!strcmp(argv[i], "--cafile"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --cafile argument given but no file specified.\n\n");
        return 1;
      }
      else
      {
        cfg->cafile = strdup(argv[i+1]);
      }
      i++;
    }
    else if(!strcmp(argv[i], "--capath"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --capath argument given but no directory specified.\n\n");
        return 1;
      }
      else
      {
        cfg->capath = strdup(argv[i+1]);
      }
      i++;
    }
    else if(!strcmp(argv[i], "--cert"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --cert argument given but no file specified.\n\n");
        return 1;
      }
      else
      {
        cfg->certfile = strdup(argv[i+1]);
      }
      i++;
    }
    else if(!strcmp(argv[i], "--ciphers"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --ciphers argument given but no ciphers specified.\n\n");
        return 1;
      }
      else
      {
        cfg->ciphers = strdup(argv[i+1]);
      }
      i++;
#endif
    }
    else if (!strcmp(argv[i], "-C"))
    {
      if (pub_or_sub == CLIENT_PUB)
      {
        goto unknown_option;
      }
      else
      {
        if (i == argc - 1)
        {
          fprintf(
          stderr,
                  "Error: -C argument given but no count specified.\n\n");
          return 1;
        }
        else
        {
          cfg->msg_count = atoi(argv[i + 1]);
          if (cfg->msg_count < 1)
          {
            fprintf(stderr, "Error: Invalid message count \"%d\".\n\n", cfg->msg_count);
            return 1;
          }
        }
        i++;
      }
    }
    else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug"))
    {
      cfg->debug = true;
    }
    else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file"))
    {
      if (pub_or_sub == CLIENT_SUB)
      {
        goto unknown_option;
      }
      if (cfg->pub_mode != MSGMODE_NONE)
      {
        fprintf(
        stderr,
                "Error: Only one type of message can be sent at once.\n\n");
        return 1;
      }
      else if (i == argc - 1)
      {
        fprintf(stderr, "Error: -f argument given but no file specified.\n\n");
        return 1;
      }
      else
      {
        cfg->pub_mode = MSGMODE_FILE;
        cfg->file_input = strdup(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "--help"))
    {
      return 2;
    }
    else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--host"))
    {
      if (i == argc - 1)
      {
        fprintf(stderr, "Error: -h argument given but no host specified.\n\n");
        return 1;
      }
      else
      {
        cfg->host = strdup(argv[i + 1]);
      }
      i++;
#ifdef WITH_TLS
    }
    else if(!strcmp(argv[i], "--insecure"))
    {
      cfg->insecure = true;
#endif
    }
    else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--id"))
    {
      if (cfg->id_prefix)
      {
        fprintf(
        stderr,
                "Error: -i and -I argument cannot be used together.\n\n");
        return 1;
      }
      if (i == argc - 1)
      {
        fprintf(stderr, "Error: -i argument given but no id specified.\n\n");
        return 1;
      }
      else
      {
        cfg->id = strdup(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "-I") || !strcmp(argv[i], "--id-prefix"))
    {
      if (cfg->id)
      {
        fprintf(
        stderr,
                "Error: -i and -I argument cannot be used together.\n\n");
        return 1;
      }
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -I argument given but no id prefix specified.\n\n");
        return 1;
      }
      else
      {
        cfg->id_prefix = strdup(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--keepalive"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -k argument given but no keepalive specified.\n\n");
        return 1;
      }
      else
      {
        cfg->keepalive = atoi(argv[i + 1]);
        if (cfg->keepalive > 65535)
        {
          fprintf(stderr, "Error: Invalid keepalive given: %d\n", cfg->keepalive);
          return 1;
        }
      }
      i++;
#ifdef WITH_TLS
    }
    else if(!strcmp(argv[i], "--key"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --key argument given but no file specified.\n\n");
        return 1;
      }
      else
      {
        cfg->keyfile = strdup(argv[i+1]);
      }
      i++;
#endif
    }
    else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--stdin-line"))
    {
      if (pub_or_sub == CLIENT_SUB)
      {
        goto unknown_option;
      }
      if (cfg->pub_mode != MSGMODE_NONE)
      {
        fprintf(
        stderr,
                "Error: Only one type of message can be sent at once.\n\n");
        return 1;
      }
      else
      {
        cfg->pub_mode = MSGMODE_STDIN_LINE;
      }
    }
    else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--message"))
    {
      if (pub_or_sub == CLIENT_SUB)
      {
        goto unknown_option;
      }
      if (cfg->pub_mode != MSGMODE_NONE)
      {
        fprintf(
        stderr,
                "Error: Only one type of message can be sent at once.\n\n");
        return 1;
      }
      else if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -m argument given but no message specified.\n\n");
        return 1;
      }
      else
      {
        cfg->message = strdup(argv[i + 1]);
        cfg->msglen = strlen(cfg->message);
        cfg->pub_mode = MSGMODE_CMD;
      }
      i++;
    }
    else if (!strcmp(argv[i], "-M"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -M argument given but max_inflight not specified.\n\n");
        return 1;
      }
      else
      {
        cfg->max_inflight = atoi(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--null-message"))
    {
      if (pub_or_sub == CLIENT_SUB)
      {
        goto unknown_option;
      }
      if (cfg->pub_mode != MSGMODE_NONE)
      {
        fprintf(
        stderr,
                "Error: Only one type of message can be sent at once.\n\n");
        return 1;
      }
      else
      {
        cfg->pub_mode = MSGMODE_NULL;
      }
    }
    else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--protocol-version"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: --protocol-version argument given but no version specified.\n\n");
        return 1;
      }
      else
      {
        if (!strcmp(argv[i + 1], "mqttv31"))
        {
          cfg->protocol_version = MQTT_PROTOCOL_V31;
        }
        else if (!strcmp(argv[i + 1], "mqttv311"))
        {
          cfg->protocol_version = MQTT_PROTOCOL_V311;
        }
        else
        {
          fprintf(
          stderr,
                  "Error: Invalid protocol version argument given.\n\n");
          return 1;
        }
        i++;
      }
#ifdef WITH_SOCKS
    }
    else if(!strcmp(argv[i], "--proxy"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --proxy argument given but no proxy url specified.\n\n");
        return 1;
      }
      else
      {
        if(mosquitto__parse_socks_url(cfg, argv[i+1]))
        {
          return 1;
        }
        i++;
      }
#endif
#ifdef WITH_TLS_PSK
    }
    else if(!strcmp(argv[i], "--psk"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --psk argument given but no key specified.\n\n");
        return 1;
      }
      else
      {
        cfg->psk = strdup(argv[i+1]);
      }
      i++;
    }
    else if(!strcmp(argv[i], "--psk-identity"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --psk-identity argument given but no identity specified.\n\n");
        return 1;
      }
      else
      {
        cfg->psk_identity = strdup(argv[i+1]);
      }
      i++;
#endif
    }
    else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--qos"))
    {
      if (i == argc - 1)
      {
        fprintf(stderr, "Error: -q argument given but no QoS specified.\n\n");
        return 1;
      }
      else
      {
        cfg->qos = atoi(argv[i + 1]);
        if (cfg->qos < 0 || cfg->qos > 2)
        {
          fprintf(stderr, "Error: Invalid QoS given: %d\n", cfg->qos);
          return 1;
        }
      }
      i++;
    }
    else if (!strcmp(argv[i], "--quiet"))
    {
      cfg->quiet = true;
    }
    else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--retain"))
    {
      if (pub_or_sub == CLIENT_SUB)
      {
        goto unknown_option;
      }
      cfg->retain = 1;
    }
    else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--stdin-file"))
    {
      if (pub_or_sub == CLIENT_SUB)
      {
        goto unknown_option;
      }
      if (cfg->pub_mode != MSGMODE_NONE)
      {
        fprintf(
        stderr,
                "Error: Only one type of message can be sent at once.\n\n");
        return 1;
      }
      else
      {
        cfg->pub_mode = MSGMODE_STDIN_FILE;
      }
#ifdef WITH_SRV
    }
    else if(!strcmp(argv[i], "-S"))
    {
      cfg->use_srv = true;
#endif
    }
    else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--topic"))
    {
      if (i == argc - 1)
      {
        fprintf(stderr, "Error: -t argument given but no topic specified.\n\n");
        return 1;
      }
      else
      {
        if (pub_or_sub == CLIENT_PUB)
        {
          if (mosquitto_pub_topic_check(argv[i + 1]) == MOSQ_ERR_INVAL)
          {
            fprintf(
            stderr,
                    "Error: Invalid publish topic '%s', does it contain '+' or '#'?\n",
                    argv[i + 1]);
            return 1;
          }
          cfg->topic = strdup(argv[i + 1]);
        }
        else
        {
          if (mosquitto_sub_topic_check(argv[i + 1]) == MOSQ_ERR_INVAL)
          {
            fprintf(
                stderr,
                "Error: Invalid subscription topic '%s', are all '+' and '#' wildcards correct?\n",
                argv[i + 1]);
            return 1;
          }
          cfg->topic_count++;
          cfg->topics = (char **) realloc(cfg->topics, cfg->topic_count * sizeof(char *));
          cfg->topics[cfg->topic_count - 1] = strdup(argv[i + 1]);
        }
        i++;
      }
    }
    else if (!strcmp(argv[i], "-T") || !strcmp(argv[i], "--filter-out"))
    {
      if (pub_or_sub == CLIENT_PUB)
      {
        goto unknown_option;
      }
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -T argument given but no topic filter specified.\n\n");
        return 1;
      }
      else
      {
        if (mosquitto_sub_topic_check(argv[i + 1]) == MOSQ_ERR_INVAL)
        {
          fprintf(
          stderr,
                  "Error: Invalid filter topic '%s', are all '+' and '#' wildcards correct?\n",
                  argv[i + 1]);
          return 1;
        }
        cfg->filter_out_count++;
        cfg->filter_outs = (char **) realloc(cfg->filter_outs,
                                             cfg->filter_out_count * sizeof(char *));
        cfg->filter_outs[cfg->filter_out_count - 1] = strdup(argv[i + 1]);
      }
      i++;
#ifdef WITH_TLS
    }
    else if(!strcmp(argv[i], "--tls-version"))
    {
      if(i==argc-1)
      {
        fprintf(stderr, "Error: --tls-version argument given but no version specified.\n\n");
        return 1;
      }
      else
      {
        cfg->tls_version = strdup(argv[i+1]);
      }
      i++;
#endif
    }
    else if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--username"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -u argument given but no username specified.\n\n");
        return 1;
      }
      else
      {
        cfg->username = strdup(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "-P") || !strcmp(argv[i], "--pw"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: -P argument given but no password specified.\n\n");
        return 1;
      }
      else
      {
        cfg->password = strdup(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "--will-payload"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: --will-payload argument given but no will payload specified.\n\n");
        return 1;
      }
      else
      {
        cfg->will_payload = strdup(argv[i + 1]);
        cfg->will_payloadlen = strlen(cfg->will_payload);
      }
      i++;
    }
    else if (!strcmp(argv[i], "--will-qos"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: --will-qos argument given but no will QoS specified.\n\n");
        return 1;
      }
      else
      {
        cfg->will_qos = atoi(argv[i + 1]);
        if (cfg->will_qos < 0 || cfg->will_qos > 2)
        {
          fprintf(stderr, "Error: Invalid will QoS %d.\n\n", cfg->will_qos);
          return 1;
        }
      }
      i++;
    }
    else if (!strcmp(argv[i], "--will-retain"))
    {
      cfg->will_retain = true;
    }
    else if (!strcmp(argv[i], "--will-topic"))
    {
      if (i == argc - 1)
      {
        fprintf(
        stderr,
                "Error: --will-topic argument given but no will topic specified.\n\n");
        return 1;
      }
      else
      {
        if (mosquitto_pub_topic_check(argv[i + 1]) == MOSQ_ERR_INVAL)
        {
          fprintf(
          stderr,
                  "Error: Invalid will topic '%s', does it contain '+' or '#'?\n", argv[i + 1]);
          return 1;
        }
        cfg->will_topic = strdup(argv[i + 1]);
      }
      i++;
    }
    else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--disable-clean-session"))
    {
      if (pub_or_sub == CLIENT_PUB)
      {
        goto unknown_option;
      }
      cfg->clean_session = false;
    }
    else if (!strcmp(argv[i], "-N"))
    {
      if (pub_or_sub == CLIENT_PUB)
      {
        goto unknown_option;
      }
      cfg->eol = false;
    }
    else if (!strcmp(argv[i], "-R"))
    {
      if (pub_or_sub == CLIENT_PUB)
      {
        goto unknown_option;
      }
      cfg->no_retain = true;
    }
    else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
    {
      if (pub_or_sub == CLIENT_PUB)
      {
        goto unknown_option;
      }
      cfg->verbose = 1;
    }
    else
    {
      goto unknown_option;
    }
  }

  return MOSQ_ERR_SUCCESS;

  unknown_option: fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
  return 1;
}

static int
client_id_generate (struct mosq_config *cfg, const char *id_base)
{
  int len;
  char hostname[256];

  if (cfg->id_prefix)
  {
    cfg->id = (char *) malloc(strlen(cfg->id_prefix) + 10);
    if (!cfg->id)
    {
      if (!cfg->quiet)
        fprintf(stderr, "Error: Out of memory.\n");
      mosquitto_lib_cleanup();
      return 1;
    }
    snprintf(cfg->id, strlen(cfg->id_prefix) + 10, "%s%d", cfg->id_prefix, getpid());
  }
  else if (!cfg->id)
  {
    hostname[0] = '\0';
    gethostname(hostname, 256);
    hostname[255] = '\0';
    len = strlen(id_base) + strlen("/-") + 6 + strlen(hostname);
    cfg->id = (char *) malloc(len);
    if (!cfg->id)
    {
      if (!cfg->quiet)
        fprintf(stderr, "Error: Out of memory.\n");
      mosquitto_lib_cleanup();
      return 1;
    }
    snprintf(cfg->id, len, "%s/%d-%s", id_base, getpid(), hostname);
    if (strlen(cfg->id) > MOSQ_MQTT_ID_MAX_LENGTH)
    {
      /* Enforce maximum client id length of 23 characters */
      cfg->id[MOSQ_MQTT_ID_MAX_LENGTH] = '\0';
    }
  }
  return MOSQ_ERR_SUCCESS;
}

static void
client_config_cleanup (struct mosq_config *cfg)
{
  int i;
  free(cfg->id);
  free(cfg->id_prefix);
  free(cfg->host);
  free(cfg->file_input);
  free(cfg->message);
  free(cfg->topic);
  free(cfg->bind_address);
  free(cfg->username);
  free(cfg->password);
  free(cfg->will_topic);
  free(cfg->will_payload);
#ifdef WITH_TLS
  free(cfg->cafile);
  free(cfg->capath);
  free(cfg->certfile);
  free(cfg->keyfile);
  free(cfg->ciphers);
  free(cfg->tls_version);
#  ifdef WITH_TLS_PSK
  free(cfg->psk);
  free(cfg->psk_identity);
#  endif
#endif
  if (cfg->topics)
  {
    for (i = 0; i < cfg->topic_count; i++)
    {
      free(cfg->topics[i]);
    }
    free(cfg->topics);
  }
  if (cfg->filter_outs)
  {
    for (i = 0; i < cfg->filter_out_count; i++)
    {
      free(cfg->filter_outs[i]);
    }
    free(cfg->filter_outs);
  }
#ifdef WITH_SOCKS
  free(cfg->socks5_host);
  free(cfg->socks5_username);
  free(cfg->socks5_password);
#endif
}

/* Global variables for use in callbacks. See sub_client.c for an example of
 * using a struct to hold variables for use in callbacks. */
static char *topic = NULL;
static char *message = NULL;
static long msglen = 0;
static int qos = 0;
static int retain = 0;
static msgmode mode = MSGMODE_NONE;
static int status = STATUS_CONNECTING;
static int mid_sent = 0;
static int last_mid = -1;
static int last_mid_sent = -1;
static bool connected = true;
static char *username = NULL;
static char *password = NULL;
static bool disconnect_sent = false;
static bool quiet = false;

static void
my_connect_callback (struct mosquitto *mosq, void *obj, int result)
{
  int rc = MOSQ_ERR_SUCCESS;

  if (!result)
  {
    std::cout << __FUNCTION__ << ' ' << STR(mode) << '\n';
    switch (mode)
    {
      case MSGMODE_CMD:
      case MSGMODE_FILE:
      case MSGMODE_STDIN_FILE:
        rc = mosquitto_publish(mosq, &mid_sent, topic, msglen, message, qos, retain);
        break;
      case MSGMODE_NULL:
        rc = mosquitto_publish(mosq, &mid_sent, topic, 0, NULL, qos, retain);
        break;
      case MSGMODE_STDIN_LINE:



        status = STATUS_CONNACK_RECVD;
        break;
      case MSGMODE_NONE:
        break;
    }
    if (rc)
    {
      if (!quiet)
      {
        switch (rc)
        {
          case MOSQ_ERR_INVAL:
            fprintf(stderr, "Error: Invalid input. Does your topic contain '+' or '#'?\n");
            break;
          case MOSQ_ERR_NOMEM:
            fprintf(stderr, "Error: Out of memory when trying to publish message.\n");
            break;
          case MOSQ_ERR_NO_CONN:
            fprintf(stderr, "Error: Client not connected when trying to publish.\n");
            break;
          case MOSQ_ERR_PROTOCOL:
            fprintf(stderr, "Error: Protocol error when communicating with broker.\n");
            break;
          case MOSQ_ERR_PAYLOAD_SIZE:
            fprintf(stderr, "Error: Message payload is too large.\n");
            break;
        }
      }
      mosquitto_disconnect(mosq);
    }
  }
  else
  {
    if (result && !quiet)
    {
      fprintf(stderr, "%s\n", mosquitto_connack_string(result));
    }
  }
}

static void
my_disconnect_callback (struct mosquitto *mosq, void *obj, int rc)
{
  connected = false;
}

static void
my_publish_callback (struct mosquitto *mosq, void *obj, int mid)
{
  std::cout << __FUNCTION__ << '\n';
  last_mid_sent = mid;
  if (mode == MSGMODE_STDIN_LINE)
  {
    if (mid == last_mid)
    {
      mosquitto_disconnect(mosq);
      disconnect_sent = true;
    }
  }
  else if (disconnect_sent == false)
  {
    mosquitto_disconnect(mosq);
    disconnect_sent = true;
  }
}

static void
my_log_callback (struct mosquitto *mosq, void *obj, int level, const char *str)
{
  printf("%s\n", str);
}

static int
load_stdin (void)
{
  long pos = 0, rlen;
  char buf[1024];
  char *aux_message = NULL;

  mode = MSGMODE_STDIN_FILE;

  while (!feof(stdin))
  {
    rlen = fread(buf, 1, 1024, stdin);
    aux_message = (char *) realloc(message, pos + rlen);
    if (!aux_message)
    {
      if (!quiet)
        fprintf(stderr, "Error: Out of memory.\n");
      free(message);
      return 1;
    }
    else
    {
      message = aux_message;
    }
    memcpy(&(message[pos]), buf, rlen);
    pos += rlen;
  }
  msglen = pos;

  if (!msglen)
  {
    if (!quiet)
      fprintf(stderr, "Error: Zero length input.\n");
    return 1;
  }

  return 0;
}

static int
load_file (const char *filename)
{
  long pos, rlen;
  FILE *fptr = NULL;

  fptr = fopen(filename, "rb");
  if (!fptr)
  {
    if (!quiet)
      fprintf(stderr, "Error: Unable to open file \"%s\".\n", filename);
    return 1;
  }
  mode = MSGMODE_FILE;
  fseek(fptr, 0, SEEK_END);
  msglen = ftell(fptr);
  if (msglen > 268435455)
  {
    fclose(fptr);
    if (!quiet)
      fprintf(stderr, "Error: File \"%s\" is too large (>268,435,455 bytes).\n", filename);
    return 1;
  }
  else if (msglen == 0)
  {
    fclose(fptr);
    if (!quiet)
      fprintf(stderr, "Error: File \"%s\" is empty.\n", filename);
    return 1;
  }
  else if (msglen < 0)
  {
    fclose(fptr);
    if (!quiet)
      fprintf(stderr, "Error: Unable to determine size of file \"%s\".\n", filename);
    return 1;
  }
  fseek(fptr, 0, SEEK_SET);
  message = (char *) malloc(msglen);
  if (!message)
  {
    fclose(fptr);
    if (!quiet)
      fprintf(stderr, "Error: Out of memory.\n");
    return 1;
  }
  pos = 0;
  while (pos < msglen)
  {
    rlen = fread(&(message[pos]), sizeof(char), msglen - pos, fptr);
    pos += rlen;
  }
  fclose(fptr);
  return 0;
}

static void
print_usage (void)
{
#define VERSION "1.4.7"
  int major, minor, revision;

  mosquitto_lib_version(&major, &minor, &revision);
  printf(
      "mosquitto_pub is a simple mqtt client that will publish a message on a single topic and exit.\n");
  printf("mosquitto_pub version %s running on libmosquitto %d.%d.%d.\n\n",
  VERSION,
         major, minor, revision);
  printf(
      "Usage: mosquitto_pub [-h host] [-k keepalive] [-p port] [-q qos] [-r] {-f file | -l | -n | -m message} -t topic\n");
#ifdef WITH_SRV
  printf("                     [-A bind_address] [-S]\n");
#else
  printf("                     [-A bind_address]\n");
#endif
  printf("                     [-i id] [-I id_prefix]\n");
  printf("                     [-d] [--quiet]\n");
  printf("                     [-M max_inflight]\n");
  printf("                     [-u username [-P password]]\n");
  printf(
      "                     [--will-topic [--will-payload payload] [--will-qos qos] [--will-retain]]\n");
#ifdef WITH_TLS
  printf("                     [{--cafile file | --capath dir} [--cert file] [--key file]\n");
  printf("                      [--ciphers ciphers] [--insecure]]\n");
#ifdef WITH_TLS_PSK
  printf("                     [--psk hex-key --psk-identity identity [--ciphers ciphers]]\n");
#endif
#endif
#ifdef WITH_SOCKS
  printf("                     [--proxy socks-url]\n");
#endif
  printf("       mosquitto_pub --help\n\n");
  printf(
      " -A : bind the outgoing socket to this host/ip address. Use to control which interface\n");
  printf("      the client communicates over.\n");
  printf(" -d : enable debug messages.\n");
  printf(" -f : send the contents of a file as the message.\n");
  printf(" -h : mqtt host to connect to. Defaults to localhost.\n");
  printf(
      " -i : id to use for this client. Defaults to mosquitto_pub_ appended with the process id.\n");
  printf(
      " -I : define the client id as id_prefix appended with the process id. Useful for when the\n");
  printf("      broker is using the clientid_prefixes option.\n");
  printf(" -k : keep alive in seconds for this client. Defaults to 60.\n");
  printf(" -l : read messages from stdin, sending a separate message for each line.\n");
  printf(" -m : message payload to send.\n");
  printf(" -M : the maximum inflight messages for QoS 1/2..\n");
  printf(" -n : send a null (zero length) message.\n");
  printf(" -p : network port to connect to. Defaults to 1883.\n");
  printf(" -P : provide a password (requires MQTT 3.1 broker)\n");
  printf(" -q : quality of service level to use for all messages. Defaults to 0.\n");
  printf(" -r : message should be retained.\n");
  printf(" -s : read message from stdin, sending the entire input as a message.\n");
#ifdef WITH_SRV
  printf(" -S : use SRV lookups to determine which host to connect to.\n");
#endif
  printf(" -t : mqtt topic to publish to.\n");
  printf(" -u : provide a username (requires MQTT 3.1 broker)\n");
  printf(" -V : specify the version of the MQTT protocol to use when connecting.\n");
  printf("      Can be mqttv31 or mqttv311. Defaults to mqttv31.\n");
  printf(" --help : display this message.\n");
  printf(" --quiet : don't print error messages.\n");
  printf(" --will-payload : payload for the client Will, which is sent by the broker in case of\n");
  printf(
      "                  unexpected disconnection. If not given and will-topic is set, a zero\n");
  printf("                  length message will be sent.\n");
  printf(" --will-qos : QoS level for the client Will.\n");
  printf(" --will-retain : if given, make the client Will retained.\n");
  printf(" --will-topic : the topic on which to publish the client Will.\n");
#ifdef WITH_TLS
  printf(" --cafile : path to a file containing trusted CA certificates to enable encrypted\n");
  printf("            communication.\n");
  printf(" --capath : path to a directory containing trusted CA certificates to enable encrypted\n");
  printf("            communication.\n");
  printf(" --cert : client certificate for authentication, if required by server.\n");
  printf(" --key : client private key for authentication, if required by server.\n");
  printf(" --ciphers : openssl compatible list of TLS ciphers to support.\n");
  printf(" --tls-version : TLS protocol version, can be one of tlsv1.2 tlsv1.1 or tlsv1.\n");
  printf("                 Defaults to tlsv1.2 if available.\n");
  printf(" --insecure : do not check that the server certificate hostname matches the remote\n");
  printf("              hostname. Using this option means that you cannot be sure that the\n");
  printf("              remote host is the server you wish to connect to and so is insecure.\n");
  printf("              Do not use this option in a production environment.\n");
#  ifdef WITH_TLS_PSK
  printf(" --psk : pre-shared-key in hexadecimal (no leading 0x) to enable TLS-PSK mode.\n");
  printf(" --psk-identity : client identity string for TLS-PSK mode.\n");
#  endif
#endif
#ifdef WITH_SOCKS
  printf(" --proxy : SOCKS5 proxy URL of the form:\n");
  printf("           socks5h://[username[:password]@]hostname[:port]\n");
  printf("           Only \"none\" and \"username\" authentication is supported.\n");
#endif
  printf("\nSee http://mosquitto.org/ for more information.\n\n");
}

static void
init_config (struct mosq_config *cfg)
{
  memset(cfg, 0, sizeof(*cfg));
  cfg->port = 1883;
  cfg->max_inflight = 20;
  cfg->keepalive = 60;
  cfg->clean_session = true;
  cfg->eol = true;
  cfg->protocol_version = MQTT_PROTOCOL_V31;
}

static int
client_config_load (struct mosq_config *cfg, int pub_or_sub, int argc, char *argv[])
{
  int rc;
  FILE *fptr;
  char line[1024];
  int count;
  char *loc = NULL;
  int len;
  char *args[3];

#ifndef WIN32
  char *env;
#else
  char env[1024];
#endif
  args[0] = NULL;

  init_config(cfg);

  /* Default config file */
#ifndef WIN32
  env = getenv("XDG_CONFIG_HOME");
  if (env)
  {
    len = strlen(env) + strlen("/mosquitto_pub") + 1;
    loc = (char *) malloc(len);
    if (pub_or_sub == CLIENT_PUB)
    {
      snprintf(loc, len, "%s/mosquitto_pub", env);
    }
    else
    {
      snprintf(loc, len, "%s/mosquitto_sub", env);
    }
    loc[len - 1] = '\0';
  }
  else
  {
    env = getenv("HOME");
    if (env)
    {
      len = strlen(env) + strlen("/.config/mosquitto_pub") + 1;
      loc = (char *) malloc(len);
      if (pub_or_sub == CLIENT_PUB)
      {
        snprintf(loc, len, "%s/.config/mosquitto_pub", env);
      }
      else
      {
        snprintf(loc, len, "%s/.config/mosquitto_sub", env);
      }
      loc[len - 1] = '\0';
    }
    else
    {
      fprintf(
      stderr,
              "Warning: Unable to locate configuration directory, default config not loaded.\n");
    }
  }

#else
  rc = GetEnvironmentVariable("USERPROFILE", env, 1024);
  if(rc > 0 && rc < 1024)
  {
    len = strlen(env) + strlen("\\mosquitto_pub.conf") + 1;
    loc = malloc(len);
    if(pub_or_sub == CLIENT_PUB)
    {
      snprintf(loc, len, "%s\\mosquitto_pub.conf", env);
    }
    else
    {
      snprintf(loc, len, "%s\\mosquitto_sub.conf", env);
    }
    loc[len-1] = '\0';
  }
  else
  {
    fprintf(stderr, "Warning: Unable to locate configuration directory, default config not loaded.\n");
  }
#endif

  if (loc)
  {
    fptr = fopen(loc, "rt");
    if (fptr)
    {
      while (fgets(line, 1024, fptr))
      {
        if (line[0] == '#')
          continue; /* Comments */

        while (line[strlen(line) - 1] == 10 || line[strlen(line) - 1] == 13)
        {
          line[strlen(line) - 1] = 0;
        }
        /* All offset by one "args" here, because real argc/argv has
         * program name as the first entry. */
        args[1] = strtok(line, " ");
        if (args[1])
        {
          args[2] = strtok(NULL, " ");
          if (args[2])
          {
            count = 3;
          }
          else
          {
            count = 2;
          }
          rc = client_config_line_proc(cfg, pub_or_sub, count, args);
          if (rc)
          {
            fclose(fptr);
            free(loc);
            return rc;
          }
        }
      }
      fclose(fptr);
    }
    free(loc);
  }

  /* Deal with real argc/argv */
  rc = client_config_line_proc(cfg, pub_or_sub, argc, argv);
  if (rc)
    return rc;

  if (cfg->will_payload && !cfg->will_topic)
  {
    fprintf(stderr, "Error: Will payload given, but no will topic given.\n");
    return 1;
  }
  if (cfg->will_retain && !cfg->will_topic)
  {
    fprintf(stderr, "Error: Will retain given, but no will topic given.\n");
    return 1;
  }
  if (cfg->password && !cfg->username)
  {
    if (!cfg->quiet)
      fprintf(stderr, "Warning: Not using password since username not set.\n");
  }
#ifdef WITH_TLS
  if((cfg->certfile && !cfg->keyfile) || (cfg->keyfile && !cfg->certfile))
  {
    fprintf(stderr, "Error: Both certfile and keyfile must be provided if one of them is.\n");
    return 1;
  }
#endif
#ifdef WITH_TLS_PSK
  if((cfg->cafile || cfg->capath) && cfg->psk)
  {
    if(!cfg->quiet) fprintf(stderr, "Error: Only one of --psk or --cafile/--capath may be used at once.\n");
    return 1;
  }
  if(cfg->psk && !cfg->psk_identity)
  {
    if(!cfg->quiet) fprintf(stderr, "Error: --psk-identity required if --psk used.\n");
    return 1;
  }
#endif

  if (pub_or_sub == CLIENT_SUB)
  {
    if (cfg->clean_session == false && (cfg->id_prefix || !cfg->id))
    {
      if (!cfg->quiet)
        fprintf(
        stderr,
                "Error: You must provide a client id if you are using the -c option.\n");
      return 1;
    }
    if (cfg->topic_count == 0)
    {
      if (!cfg->quiet)
        fprintf(stderr, "Error: You must specify a topic to subscribe to.\n");
      return 1;
    }
  }

  if (!cfg->host)
  {
    char localhost[] = "localhost";
    cfg->host = localhost;
  }
  return MOSQ_ERR_SUCCESS;
}

int
main (int argc, char *argv[])
{
  int buf_len = 1024;
  int buf_len_actual;
  int read_len;
  int pos;

  char *buf = (char *) malloc(buf_len);
  if (!buf)
  {
    fprintf(stderr, "Error: Out of memory.\n");
    return 1;
  }

  struct mosq_config cfg;
  memset(&cfg, 0, sizeof(struct mosq_config));

  {
    int rc = client_config_load(&cfg, CLIENT_PUB, argc, argv);
    if (rc)
    {
      client_config_cleanup(&cfg);
      if (rc == 2)
      {
        /* --help */
        print_usage();
      }
      else
      {
        fprintf(stderr, "\nUse 'mosquitto_pub --help' to see usage.\n");
      }
      return 1;
    }
  }

  topic = cfg.topic;
  message = cfg.message;
  msglen = cfg.msglen;
  qos = cfg.qos;
  retain = cfg.retain;
  mode = cfg.pub_mode;
  username = cfg.username;
  password = cfg.password;
  quiet = cfg.quiet;
  std::cout << STR(cfg) << '\n';

  if (cfg.pub_mode == MSGMODE_STDIN_FILE)
  {
    if (load_stdin())
    {
      fprintf(stderr, "Error loading input from stdin.\n");
      return 1;
    }
  }
  else if (cfg.file_input)
  {
    if (load_file(cfg.file_input))
    {
      fprintf(stderr, "Error loading input file \"%s\".\n", cfg.file_input);
      return 1;
    }
  }

  if (!topic || mode == MSGMODE_NONE)
  {
    fprintf(stderr, "Error: Both topic and message must be supplied.\n");
    print_usage();
    return 1;
  }

  mosquitto_lib_init();

  if (client_id_generate(&cfg, "mosqpub"))
  {
    return 1;
  }

  struct mosquitto *mosq = mosquitto_new(cfg.id, true, NULL);
  if (!mosq)
  {
    switch (errno)
    {
      case ENOMEM:
        if (!quiet)
          fprintf(stderr, "Error: Out of memory.\n");
        break;
      case EINVAL:
        if (!quiet)
          fprintf(stderr, "Error: Invalid id.\n");
        break;
    }
    mosquitto_lib_cleanup();
    return 1;
  }
  if (cfg.debug)
  {
    mosquitto_log_callback_set(mosq, my_log_callback);
  }
  mosquitto_connect_callback_set(mosq, my_connect_callback);
  mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
  mosquitto_publish_callback_set(mosq, my_publish_callback);

  if (client_opts_set(mosq, &cfg))
  {
    return 1;
  }
  int rc = client_connect(mosq, &cfg);
  if (rc)
    return rc;

  if (mode == MSGMODE_STDIN_LINE)
  {
    mosquitto_loop_start(mosq);
  }

  do
  {
    if (mode == MSGMODE_STDIN_LINE)
    {
      if (status == STATUS_CONNACK_RECVD)
      {
        pos = 0;
        read_len = buf_len;
        while (fgets(&buf[pos], read_len, stdin))
        {
          buf_len_actual = strlen(buf);
          if (buf[buf_len_actual - 1] == '\n')
          {
            buf[buf_len_actual - 1] = '\0';
            int rc2 = mosquitto_publish(mosq, &mid_sent, topic, buf_len_actual - 1, buf, qos,
                                        retain);
            if (rc2)
            {
              if (!quiet)
                fprintf(
                stderr,
                        "Error: Publish returned %d, disconnecting.\n", rc2);
              mosquitto_disconnect(mosq);
            }
            break;
          }
          else
          {
            buf_len += 1024;
            pos += 1023;
            read_len = 1024;
            buf = (char *) realloc(buf, buf_len);
            if (!buf)
            {
              fprintf(stderr, "Error: Out of memory.\n");
              return 1;
            }
          }
        }
        if (feof(stdin))
        {
          last_mid = mid_sent;
          status = STATUS_WAITING;
        }
      }
      else if (status == STATUS_WAITING)
      {
        if (last_mid_sent == last_mid && disconnect_sent == false)
        {
          mosquitto_disconnect(mosq);
          disconnect_sent = true;
        }
#ifdef WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
      }
      rc = MOSQ_ERR_SUCCESS;
    }
    else
    {
      rc = mosquitto_loop(mosq, -1, 1);
    }
  }
  while (rc == MOSQ_ERR_SUCCESS && connected);

  if (mode == MSGMODE_STDIN_LINE)
  {
    mosquitto_loop_stop(mosq, false);
  }

  if (message && mode == MSGMODE_FILE)
  {
    free(message);
  }
  mosquitto_destroy(mosq);
  mosquitto_lib_cleanup();

  if (rc)
  {
    fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
  }
  return rc;
}
