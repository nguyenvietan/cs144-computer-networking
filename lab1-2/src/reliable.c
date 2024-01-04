
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>

#include "rlib.h"



struct reliable_state {
  rel_t *next;			/* Linked list for traversing all connections */
  rel_t **prev;

  conn_t *c;			/* This is the connection object */

  /* Add your own data fields below this */

  // uint32_t seqno; // Sequence number of the next byte to send
  // uint32_t ackno; // Next sequence number expected to receive (acknowledgment number)
  // size_t snd_wnd; // Sender window size
  // size_t rcv_wnd; // Receiver window size
  // char *send_buffer; // Buffer for data to be sent
  // char *receive_buffer; // Buffer for received data
  // int state; // Current state of the connection (e.g., LISTEN, SYN_SENT, ESTABLISHED)
  // int retransmission_count; // Counter for retransmissions
  // bool is_fin_sent; // Flag to indicate if FIN has been sent
  // bool is_fin_received; // Flag to indicate if FIN has been received
};
rel_t *rel_list;





/* Creates a new reliable protocol session, returns NULL on failure.
 * Exactly one of c and ss should be NULL.  (ss is NULL when called
 * from rlib.c, while c is NULL when this function is called from
 * rel_demux.) */
rel_t *
rel_create (conn_t *c, const struct sockaddr_storage *ss,
	    const struct config_common *cc)
{
  rel_t *r;

  r = xmalloc (sizeof (*r));
  memset (r, 0, sizeof (*r));

  if (!c) {
    c = conn_create (r, ss);
    if (!c) {
      free (r);
      return NULL;
    }
  }

  r->c = c;
  r->next = rel_list;
  r->prev = &rel_list;
  if (rel_list)
    rel_list->prev = &r->next;
  rel_list = r;

  /* Do any other initialization you need here */

  // r->seqno = INITIAL_SEQNO; // Initialize to a constant or a dynamic value
  // r->ackno = INITIAL_ACKNO; // Similar for acknowledgment number
  // r->snd_wnd = cc->window; // Sender window size
  // r->rcv_wnd = cc->window; // Receiver window size
  // r->timer = INITIAL_TIMER_VALUE;
  // r->timeout = cc->timeout; // You might get this from the config_common
  // r->send_buffer = initialize_buffer(); // Function to initialize your buffer
  // r->receive_buffer = initialize_buffer();
  // r->state = INITIAL_STATE; // e.g., CLOSED, LISTENING, etc., based on your FSM

  return r;
}

void
rel_destroy (rel_t *r)
{
  if (r->next)
    r->next->prev = r->prev;
  *r->prev = r->next;
  conn_destroy (r->c);

  /* Free any other allocated memory here */
  // if (r->send_buffer) {
  //   free(r->send_buffer);
  //   r->send_buffer = NULL;
  // }
  // if (r->receive_buffer) {
  //     free(r->receive_buffer);
  //     r->receive_buffer = NULL;
  // }
  // // Add cleanup for any other dynamically allocated resources or state
  // free(r);
}


/* This function only gets called when the process is running as a
 * server and must handle connections from multiple clients.  You have
 * to look up the rel_t structure based on the address in the
 * sockaddr_storage passed in.  If this is a new connection (sequence
 * number 1), you will need to allocate a new conn_t using rel_create
 * ().  (Pass rel_create NULL for the conn_t, so it will know to
 * allocate a new connection.)
 */
void
rel_demux (const struct config_common *cc,
	   const struct sockaddr_storage *ss,
	   packet_t *pkt, size_t len)
{
}

void
rel_recvpkt (rel_t *r, packet_t *pkt, size_t n)
{
/*
    // Check for packet validity (e.g., size, checksum)
    if (!is_valid_packet(pkt, n)) {
        return; // Ignore invalid packets
    }

    // Process packet based on its type (e.g., ACK, SYN, FIN)
    switch (pkt->type) {
        case PKT_ACK:
            handle_ack_packet(r, pkt);
            break;
        case PKT_SYN:
            handle_syn_packet(r, pkt);
            break;
        case PKT_FIN:
            handle_fin_packet(r, pkt);
            break;
        case PKT_DATA:
            handle_data_packet(r, pkt, n);
            break;
        default:
            // Handle other packet types or ignore unknown types
            break;
    }

    // Additional processing as needed
    // E.g., updating timers, retransmission logic, etc.

    // After processing, send out any necessary responses
    // E.g., ACKs for received data packets
*/
}



void
rel_read (rel_t *s)
{
/*
  // Check if there is buffer space available for reading new data
  if (!can_read_data(s)) {
      return; // If no buffer space, return and wait to be called again later
  }

  // Buffer to store the data read from the application
  char buffer[MAX_DATA_SIZE];
  memset(buffer, 0, MAX_DATA_SIZE);

  // Read data from the application
  ssize_t bytes_read = conn_input(s->c, buffer, MAX_DATA_SIZE);

  // Check if the read was successful
  if (bytes_read > 0) {
      // Process the read data (e.g., create a packet and enqueue it for sending)
      process_application_data(s, buffer, bytes_read);
  } else if (bytes_read == 0) {
      // No data available to read, return and wait for more data
  } else {
      // An error occurred, handle it (e.g., connection error)
      handle_read_error(s);
  }

  // Additional logic as needed (e.g., managing window size, sending packets)
*/
}

void
rel_output (rel_t *r)
{
/*
  // Check if there is data received that needs to be delivered
  if (!has_data_to_output(r)) {
      return; // If no data to deliver, return
  }

  // Buffer to store the data to be output
  char buffer[MAX_DATA_SIZE];
  memset(buffer, 0, MAX_DATA_SIZE);

  // Retrieve the data that needs to be delivered
  ssize_t bytes_to_output = get_data_for_output(r, buffer, MAX_DATA_SIZE);

  // Check if there is data to output
  if (bytes_to_output > 0) {
      // Deliver the data to the application
      conn_output(r->c, buffer, bytes_to_output);
  } else {
      // Handle cases with no data or errors
      // This could be a normal situation or an error state depending on your protocol
  }

  // Additional logic as needed (e.g., updating state, acknowledging receipt)
*/
}

void
rel_timer ()
{
  /* Retransmit any packets that need to be retransmitted */
/*
  // Iterate through all connections
  for (rel_t *r = rel_list; r != NULL; r = r->next) {
      // Check for packets that need retransmission
      packet_t *pkt_to_retransmit = get_packet_to_retransmit(r);
      while (pkt_to_retransmit != NULL) {
          // Retransmit the packet
          send_packet(r, pkt_to_retransmit);

          // Update retransmission related state, e.g., increase retransmission counter
          update_retransmission_state(r, pkt_to_retransmit);

          // Get the next packet to retransmit, if any
          pkt_to_retransmit = get_packet_to_retransmit(r);
      }

      // Additional timer-based actions
      // E.g., Checking for timeouts, connection state updates, etc.
      perform_additional_timer_actions(r);
  }
*/
}
