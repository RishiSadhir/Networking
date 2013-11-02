#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#include <iostream>

#include "Minet.h"
//#include "tcpstate.h"

using namespace std;

class Closed{

};

class Listen{

};

class Syn_Rcvd{

};

class Syn_Sent{

};

class Established{

};

class Fin_Wait_1{

};

class Fin_Wait_2{

};

class Closing{

};

class Time_Wait{
	
};

class Close_Wait{

};

class Last_Ack{

};

class Closed{

};

class TCPState{

public:
	TCPState();
	void send();
	void recv();

};



class TCPConnection{
	unsigned int seqnum;
	unsigned int acknum;
	//int last_acked, last_sent, rwnd;
	//int last_recvd;
	//int buffer_size;
	//int N_size;

	TCPState *state;

	SockRequestResponse r;

public:
	TCPConnection();
	void handle_packet(Packet &p);

};

TCPConnection::TCPConnection(){
	seqnum = 0;
	acknum = 0;
}

void TCPConnection::handle_packet(Packet &p){
	//code here
}

void send_Sean(bool ack, bool syn, bool fin, unsigned short port1, unsigned short port2, unsigned int ourseqnum, 
	unsigned int seqnum, IPAddress ips, MinetHandle mux);

void sock_reply(int error, MinetHandle sock, SockRequestResponse req);

void sock_add();

ConnectionList<TCPConnection> clist;


int main(int argc, char * argv[]) {
    MinetHandle mux;
    MinetHandle sock;

    MinetInit(MINET_TCP_MODULE);

    mux = MinetIsModuleInConfig(MINET_IP_MUX) ?  
	MinetConnect(MINET_IP_MUX) : 
	MINET_NOHANDLE;
    
    sock = MinetIsModuleInConfig(MINET_SOCK_MODULE) ? 
	MinetAccept(MINET_SOCK_MODULE) : 
	MINET_NOHANDLE;

    if ( (mux == MINET_NOHANDLE) && (MinetIsModuleInConfig(MINET_IP_MUX)) ) {
		MinetSendToMonitor(MinetMonitoringEvent("Can't connect to ip_mux"));
		return -1;
    }

    if ( (sock == MINET_NOHANDLE) && (MinetIsModuleInConfig(MINET_SOCK_MODULE)) ) {
		MinetSendToMonitor(MinetMonitoringEvent("Can't accept from sock_module"));
		return -1;
    }
    
    cerr << "tcp_module STUB VERSION handling tcp traffic.......\n";

    MinetSendToMonitor(MinetMonitoringEvent("tcp_module STUB VERSION handling tcp traffic........"));

    MinetEvent event;
    double timeout = 1;

    /*Packet p;
    send_Sean(false, true, false, 1502, 1503, 0, 0, IPAddress("192.168.42.2"), mux);
    send_Sean(false, true, false, 1502, 1503, 0, 0, IPAddress("192.168.42.2"), mux);*/

    while (MinetGetNextEvent(event, timeout) == 0) {

		if ((event.eventtype == MinetEvent::Dataflow) && (event.direction == MinetEvent::IN)) {

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MUX HANDLING IS HERE.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		    if (event.handle == mux) {
		    	Packet p;
		    	MinetReceive(mux, p);
		    	unsigned short len;
		    	unsigned char flags;

		    	len = TCPHeader::EstimateTCPHeaderLength(p);
		    	p.ExtractHeaderFromPayload<TCPHeader>(len);

		    	TCPHeader tcph;
		    	tcph = p.FindHeader(Headers::TCPHeader);

		    	tcph.GetFlags(flags);

		    	IPHeader iph;
		    	iph = p.FindHeader(Headers::IPHeader);

		    	printf("Before the Syn Flag\n");

		    	/*if(IS_SYN(flags) && IS_ACK(flags)){
		    		IPAddress ips;
		    		unsigned short port1;
		    		unsigned short port2;
		    		unsigned int seqnum;
		    		iph.GetSourceIP(ips);
		    		tcph.GetDestPort(port1);
		    		tcph.GetSourcePort(port2);
		    		tcph.GetSeqNum(seqnum);
		    		send_Sean(true, false, false, port1, port2, 1, seqnum, ips, mux);
		    	}*/

		    	if(IS_SYN(flags)){
		    		IPAddress ips;
		    		unsigned short port1;
		    		unsigned short port2;
		    		unsigned int seqnum;
		    		iph.GetSourceIP(ips);
		    		tcph.GetDestPort(port1);
		    		tcph.GetSourcePort(port2);
		    		tcph.GetSeqNum(seqnum);
		    		send_Sean(true, true, false, port1, port2, 0, seqnum, ips, mux);
		    	}
			// ip packet has arrived!
		    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SOCKET HANDLING IS HERE.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		    if (event.handle == sock) {		    	
				SockRequestResponse req;
				MinetReceive(sock, req);

				switch(req.type){
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONNECT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case CONNECT: {



						break;
					}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ACCEPT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case ACCEPT: {

						// Have to use an iterator to find something in the list.
						// Line 00142 constate_8h_source.html
						ConnectionList<TCPState>::iterator find = clist.FindMatching(req.connection);

						// If the accept half socket isn't in the list, add it to the list.
						if(find == clist.end()){



						}

						// If the accept socket is in the list, then accept this new connection.
						else{



						}

						break;
					}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WRITE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case WRITE:{

						break;
					}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORWARD
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case FORWARD:{

						break;
					}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLOSE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case CLOSE:{

						break;
					}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STATUS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					case STATUS:{

						break;
					}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFAULT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					default:{

						break;
					}

				}
		    }
		}

		if (event.eventtype == MinetEvent::Timeout) {
		    // timeout ! probably need to resend some packets
		}

    }

    MinetDeinit();

    return 0;
}

void send_Sean(bool ack, bool syn, bool fin, unsigned short port1, unsigned short port2, 
	unsigned int our_seqnum, unsigned int seqnum, IPAddress ips, MinetHandle mux){

	Packet p;
	TCPHeader send_tcp;
	IPHeader send_ip;
	unsigned char flags = 0;

	printf("In Sean Function\n");

	if(ack){
		SET_ACK(flags);
	}
	if(syn){
		SET_SYN(flags);
	}
	if(fin){
		SET_FIN(flags);
	}

	send_ip.SetProtocol(IP_PROTO_TCP);

	send_ip.SetSourceIP(MyIPAddr());
	send_ip.SetDestIP(ips);

	send_ip.SetTotalLength(IP_HEADER_BASE_LENGTH + TCP_HEADER_BASE_LENGTH);

	p.PushFrontHeader(send_ip);

	send_tcp.SetSourcePort(port1, p);
	send_tcp.SetDestPort(port2, p);
	send_tcp.SetHeaderLen(TCP_HEADER_BASE_LENGTH, p);
	send_tcp.SetSeqNum(our_seqnum, p);
	send_tcp.SetAckNum(seqnum+1, p);
	send_tcp.SetFlags(flags, p);
	send_tcp.SetWinSize(5000, p);

	printf("TCP is set\n");

	p.PushBackHeader(send_tcp);

	printf("Pushed on packet\n");

	MinetSend(mux, p);

	printf("After minet send\n");

}

void sock_reply(int error, MinetHandle sock, SockRequestResponse req){

	SockRequestResponse reply;

	MinetSend(sock, reply);

}

void sock_add(){

}
