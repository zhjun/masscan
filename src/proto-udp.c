#include "proto-udp.h"
#include "proto-dns.h"
#include "proto-netbios.h"
#include "proto-snmp.h"
#include "proto-preprocess.h"
#include "syn-cookie.h"
#include "logger.h"
#include "output.h"
#include "masscan.h"
#include "unusedparm.h"



void handle_udp(struct Output *out, time_t timestamp, const unsigned char *px, unsigned length, struct PreprocessedInfo *parsed)
{
    unsigned ip_them;
    unsigned port_them = parsed->port_src;
    unsigned status = 0;

    ip_them = parsed->ip_src[0]<<24 | parsed->ip_src[1]<<16
            | parsed->ip_src[2]<< 8 | parsed->ip_src[3]<<0;



    switch (port_them) {
    case 53:
        status = handle_dns(out, timestamp, px, length, parsed);
        break;
    case 137:
        status = handle_nbtstat(out, timestamp, px, length, parsed);
        break;
    case 161:
        status = handle_snmp(out, timestamp, px, length, parsed);
        break;
    }

    if (status == 0)
        output_report_status(
                        out,
                        timestamp,
                        Port_UdpOpen,
                        ip_them,
                        port_them,
                        0,
                        0);

}
