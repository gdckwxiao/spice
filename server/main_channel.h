/*
   Copyright (C) 2009 Red Hat, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MAIN_CHANNEL_H__
#define __MAIN_CHANNEL_H__

#include <stdint.h>
#include <spice/vd_agent.h>
#include "common/marshaller.h"
#include "red_channel.h"

/* This is a temporary measure for reds/main split - should not be in a header,
 * but private (although only reds.c includes main_channel.h) */
struct MainMigrateData {
    uint32_t version;
    uint32_t serial;
    uint32_t ping_id;

    uint32_t agent_connected;
    uint32_t client_agent_started;
    uint32_t num_client_tokens;
    uint32_t send_tokens;

    uint32_t read_state;
    VDIChunkHeader vdi_chunk_header;
    uint32_t recive_len;
    uint32_t message_recive_len;
    uint32_t read_buf_len;

    uint32_t write_queue_size;
};

typedef struct MainChannel MainChannel;

Channel *main_channel_init(void);
/* This is a 'clone' from the reds.h Channel.link callback */
MainChannelClient *main_channel_link(struct Channel *,
                 RedsStream *stream, int migration, int num_common_caps,
                 uint32_t *common_caps, int num_caps, uint32_t *caps);
void main_channel_close(MainChannel *main_chan); // not destroy, just socket close
int main_channel_push_ping(MainChannel *main_chan, int size);
void main_channel_push_mouse_mode(MainChannel *main_chan, int current_mode, int is_client_mouse_allowed);
void main_channel_push_agent_connected(MainChannel *main_chan);
void main_channel_push_agent_disconnected(MainChannel *main_chan);
void main_channel_push_tokens(MainChannel *main_chan, uint32_t num_tokens);
void main_channel_push_agent_data(MainChannel *main_chan, uint8_t* data, size_t len,
           spice_marshaller_item_free_func free_data, void *opaque);
void main_channel_start_net_test(MainChannelClient *mcc);
// TODO: huge. Consider making a reds_* interface for these functions
// and calling from main.
void main_channel_push_init(MainChannelClient *mcc, int connection_id, int display_channels_hint,
    int current_mouse_mode, int is_client_mouse_allowed, int multi_media_time,
    int ram_hint);
void main_channel_push_notify(MainChannel *main_chan, uint8_t *mess, const int mess_len);
// TODO: consider exporting RedsMigSpice from reds.c
void main_channel_push_migrate_begin(MainChannel *main_chan, int port, int sport, char *host,
    uint16_t cert_pub_key_type, uint32_t cert_pub_key_len, uint8_t *cert_pub_key);
void main_channel_push_migrate(MainChannel *main_chan);
void main_channel_push_migrate_switch(MainChannel *main_chan);
void main_channel_push_migrate_cancel(MainChannel *main_chan);
void main_channel_push_multi_media_time(MainChannel *main_chan, int time);
int main_channel_getsockname(MainChannel *main_chan, struct sockaddr *sa, socklen_t *salen);
int main_channel_getpeername(MainChannel *main_chan, struct sockaddr *sa, socklen_t *salen);

// TODO: Defines used to calculate receive buffer size, and also by reds.c
// other options: is to make a reds_main_consts.h, to duplicate defines.
#define REDS_AGENT_WINDOW_SIZE 10
#define REDS_NUM_INTERNAL_AGENT_MESSAGES 1

#endif
